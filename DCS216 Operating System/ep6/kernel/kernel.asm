; a kernel(monitor now) that provides program options;
BITS 16

prefixCount equ 4
kernelSeg   equ 0x800
pcbSize     equ 41

global set_seg      ; start point of the whole program

extern _pcbList     ; pcb list
extern _proCount    ; number of active processes
extern _pcbPtr      ; pointer to the current process
extern _nowPid      ; id of the current process
extern _ready       ; signal of completed loading

extern _schedule    ; schedule function 

SECTION .text align=16
;=========================================
; main body of the kernel
;=========================================
set_seg:            ; set segment registers
    mov ax, kernelSeg
    mov ds, ax
    mov es, ax      ; ds = es = ss = cs

    mov ss, ax
    mov sp, [kernelSp]
    mov bp, sp      ; sp = bp = kernelSp
    
    mov ax, 0xb800
    mov gs, ax      ; gs point to display memory 
set_interupt_table:
    xor ax, ax 
    mov es, ax      ; set segment to zero
    mov ax, cs
    mov ebx, dword[es:0x08 * 4]
    mov dword[INT08HAddr], ebx 
                    ; set system clock interrupt
    mov word[es:0x08 * 4], timer
    mov word[es:0x08 * 4 + 2], ax
                    ; set return interrupt
    mov word[es:0x20 * 4], INT20H
    mov word[es:0x20 * 4 + 2], ax
                    ; set syscall
    mov word[es:0x21 * 4], INT21H
    mov word[es:0x21 * 4 + 2], ax
                    ; now unused
    mov word[es:0x22 * 4], INT22H
    mov word[es:0x22 * 4 + 2], ax
    mov ax, ds
    mov es, ax      ; recover es

kernel_restart:     ; back here when all processes ended
    mov word[_proCount], 1  ; ds:0x582
    mov byte[_ready], 0     ; ds:0x580
    mov word[_nowPid], 0    ; ds:0x584
    mov word[_pcbPtr], _pcbList ; ds:0x588 / ds:0x5a0
    mov word[program_segPos + 2], 0
                    ; roll back the target segment address
print_hint:
; set cursor to the upper left then display hint
    call clear_screen
    mov byte[coord], 0x00 
    mov byte[coord + 1], 0x00
    call set_cursor
    mov si, message 
    call print_string
; User starts at most 4 different programs
load_loop:
    call keyboard_input_blocked
    cmp al, 0x30
    jle .handle_control
    cmp al, 0x35    ; check kerboard input
    jge .handle_control
    call cursor_display
; select the target program
    mov bl, 0x30
.select_program:
    inc bl
    cmp al, bl
    jnz .select_program
; load program into the target address, then move the address
.load_program:
    add word[program_segPos + 2], 0x1000
    sub bl, 0x30    
    add bl, prefixCount
    call read_floppy
; set pcb information
.set_proc_info:
    mov bx, word[_proCount]
    imul bx, pcbSize
    add bx, _pcbList
    add bx, pcbSize-1               ; move to the end of the pcb
    mov byte[bx], 1                 ; pstate = 1
    mov sp, bx                      
    sub sp, 10                      ; sp pointed to pid, skipping the name
    push word[_proCount]            ; pid = nowPid
    pushf                           ; flags
    push word[program_segPos + 2]   ; cs
    push word[program_segPos]       ; ip
    push 0xfffb                     ; sp, sub 4 bytes for ret address
    push word[program_segPos + 2]   ; ss
    push word[program_segPos + 2]   ; ds
    push word[program_segPos + 2]   ; es
    push 0xffff                     ; bp
    mov sp, word[kernelSp]          ; restore sp to prevent error
    mov es, word[program_segPos + 2]
    mov word[es:0x0], 0x20CD        ; int 0x20
    mov dword[es:0xfffb], 0x00      ; address of "int 0x20"

    inc word[_proCount]
    cmp word[_proCount], 5          ; full pcb list
    je  .handle_control             
    jmp load_loop                   ; load another process
.handle_control:
    cmp word[_proCount], 1          ; nothing is loaded, back to input
    jle load_loop
.clear:
    call clear_screen               ; clear the screen
    mov byte[coord], 0x00 
    mov byte[coord + 1], 0x00
    call set_cursor                 
.set_ready:
    mov byte[_ready], 1
    jmp $                           ; wait for schedule

;=========================================
; helper functions
;=========================================
keyboard_input:
    mov ah, 0x01
    int 0x16        ; try to get a keystroke from keyboard
    jz keyboard_input
keyboard_input_blocked:
    mov ah, 0x00    ; AH = 0x01 won't remove the stroke from buffer
    int 0x16        ; call again to read it
    ret             

read_floppy:
    mov ah, 0x02   
    mov al, 0x01    ; only read one sector
    mov ch, 0x00    ; cylinder zero
    mov cl, bl      ; get sector index from reg bx, assuming in one sector
    mov dh, 0x00    ; head zero
    mov dl, 0x00    ; drive number, 0 and 1 for floppy
    mov es, word[program_segPos + 2]
                    ; set es = future ds
    mov bx, word[program_segPos]   
                    ; set offset of the target memory address
    int 0x13        ; call interupt
    ret

clear_screen:    
    mov ax, 0x0700  ; scoll the whole window
    mov bh, 0x07    ; white on black
    mov cx, 0x0000  ;
    mov dx, 0x184f  ; corner coordinates of the window
    int 0x10        ; clear screen for user program
    ret

set_cursor:
    mov ah, 0x02
    mov bh, 0x00
    mov dh, [coord]
    mov dl, [coord + 1]
    int 0x10        ; set cursor position
    ret

print_string:
    mov ah, 0x0e    ; set bios function
.print_loop:
    lodsb           ; load chars to al one at a time
    cmp al, 0x00
    je .print_done

    int 0x10        ; bios call
    jmp .print_loop
.print_done:
    ret

; directly display char according to coord data
direct_display:
    xor ax, ax 
    mov al, byte[coord]
    imul ax, 80
    xor bx, bx
    mov bl, byte[coord + 1]
    add ax, bx
    imul ax, 2
    mov bx, ax      ; compute index for coordinate (y,x)

    mov al, byte[coord + 2]
    mov ah, 0x0f
    mov [gs:bx], ax ; display
    ret             ; return to caller
; display at the cursor position, then move the cursor
; al now has the output char
cursor_display:
    cmp al, 0x0D
    je .disp
    cmp al, 0x0A    ; characters for new line
    je .disp
    cmp al, 0x20    ; skip any control character
    jl .end
.disp:
    mov ah, 0x0e
    mov bh, 0x00
    int 0x10
.end:
    ret

;==========================================
; interrupt service routines
;==========================================
save:
    push ds         ; flags/cs/ip/ret/ds
    push cs
    pop ds          ; move ds first
    pop word[dsSave]
    pop word[retSave]
    mov word[siSave], si
    mov si, word[_pcbPtr]
    add si, 22
    pop word[si]    ; ip
    pop word[si+2]  ; cs
    pop word[si+4]  ; flags
    mov word[si-2], sp
    mov word[si-4], ss
    mov si, ds
    mov ss, si      ; ss <- ds
    mov sp, word[_pcbPtr]
    add sp, 18      ; sp <- &ss
    push word[dsSave]
    push es         ; directly push other registers
    push bp
    push di
    push word[siSave]
    push dx
    push cx
    push bx
    push ax         
    mov bp, word[interrSp]
    mov sp, word[interrSp]
                    ; move to interrupt stack
    mov si, word[retSave]
    jmp si          ; return to the main body 

restart_keepAX:
    mov word[interrSp], sp
    mov sp, word[_pcbPtr]
    pop bx
    jmp continue    ; keep ax as it was 
restart:
    mov word[interrSp], sp
    mov sp, word[_pcbPtr]
    pop ax          ; restore it if caller doesn't need return value
continue:
    pop bx
    pop cx
    pop dx
    pop si
    pop di
    pop bp
    pop es
    mov word[siSave], bx
    pop word[dsSave]
    mov bx, sp
    mov bx, word[bx]
    mov ss, bx
    mov bx, sp
    add bx, 2
    mov sp, word[bx]
    push word[bx+6]
    push word[bx+4]
    push word[bx+2]
    lds bx, [siSave]
    push ax
	mov al, 0x20		
	out 0x20, al		
	out 0xA0, al
	pop ax          ; send EOI signal back to hardware
    iret

display_wheel:      ; will be skipped for timer
    call save
    call display_wheel_main
    jmp restart
display_wheel_main: 
    xor ax, ax      
    mov al, byte[wheel]
    mov di, ax      ; get the current character

    mov byte[coord], 24
    mov byte[coord + 1], 79
    mov al, byte[wheel + di + 2]
    mov byte[coord + 2], al
    call direct_display
                    ; call function to display char
    inc byte[wheel]
    cmp byte[wheel], 4
    jnz .dw_end
    mov byte[wheel], 0
.dw_end:
    ret

;================ timer ====================
; each time the timer is called, the spinning wheel rotates
; then another process will continue (if at least one app is running)
; otherwise only the kernel will continue
timer:
    call save
    call display_wheel_main
    call dword _schedule
    jmp restart

;================ OS return ================
INT20H:
    cli
    call save       ; after the save, cs = ds = ss = kernelSs
INT20H_main:        ; and bp = sp = interrSp
.set_proc:
    mov bx, word[_pcbPtr]
    add bx, pcbSize-1       
    mov word[bx], 0         ; set process as idle
    dec word[_proCount]
    cmp word[_proCount], 1
    sti
    jle  .return_kernel     ; no active process, return to kernel
.wait_for_schedule:
    jmp $                   ; wait for the timer
.return_kernel:     
    mov word[_ready], 0     ; disable scheduling
    mov bx, ds
    mov es, bx              ; cs = ds = es = ss
    mov bp, word[kernelSp]
    mov sp, word[kernelSp]  ; bp = sp = kernelSp
    jmp kernel_restart
;=============== ????????? ================
INT22H:
    call save
    call INT22H_main
    jmp restart
INT22H_main:
    mov byte[coord], 39
    mov byte[coord+1], 0
    call set_cursor
    mov si, INT22Mes
    call print_string
    ret

;============== OS syscall ===============
INT21H:
    call save
    call INT21H_main
    jmp restart_keepAX
INT21H_main:        ; ah determines the function called
    xor bx, bx
    mov es, bx
    mov ebx, dword[INT08HAddr]
    mov dword[es:0x08 * 4], ebx
    cmp ah, 0x00
    jg ._getchar
._getch:            ; get input from the keyboard
    call keyboard_input_blocked
    jmp .return
._getchar:          ; get input from the keyboard then output it
    cmp ah, 0x01
    jg ._putchar
    call keyboard_input_blocked
._putchar:          ; call the cursor display function
    call cursor_display
.return:
    mov word[es:0x08 * 4], timer
    mov word[es:0x08 * 4+2], ds
    ret             ; restore clock interrupt

SECTION .data align=16 
;=========================================
; data section of the kernel
;=========================================
    kernelSp    dw  0x1000      ; kernel stack sp
    interrSp    dw  0x2000      ; interrupt stack sp
    coord       db  0, 0, 0     ; y x and char used for helper functions
    wheel       db  0, 0, "|/-\"
    message     db  "A basic monitor", 0xD, 0xA 
                db  "Press keyboard to run corresponding programs", 0xD, 0xA, 
                db  "output string  1", 0xD, 0xA,
                db  "test IO syscall  2", 0xD, 0xA,
                db  "output stones  3, 4", 0xD, 0xA, 0x00
    INT22Mes    db  "INT22H", 0x00

    program_segPos dw  0x100, 0x0 ; computed later
.tmpReg:
    siSave      dw  0
    dsSave      dw  0
    retSave     dw  0
.IVT_info:
    INT08HAddr  dw  0, 0