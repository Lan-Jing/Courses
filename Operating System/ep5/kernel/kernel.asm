; a kernel(monitor now) that provides program options;
BITS 16

prefixCount equ 3
kernelSeg   equ 0x800

global set_seg      ; start point of the whole program

extern _pcbPtr

SECTION .text align=16
;=========================================
; main body of the kernel
;=========================================
set_seg:
    mov ax, kernelSeg
    mov ds, ax
    mov es, ax      ; ds = es = ss = cs

    mov ss, ax
    mov sp, [kernelSp]
    mov bp, sp
    
    mov ax, 0xb800
    mov gs, ax      ; gs point to display memory 
set_interupt_table:
    xor ax, ax 
    mov es, ax      ; set segment to zero
    mov ax, cs
    mov bx, word[es:0x08 * 4]
    mov word[INT08HAddr], bx 
    mov word[es:0x08 * 4], display_wheel
    mov word[es:0x08 * 4 + 2], ax
                    ; set system clock interrupt
    mov word[es:0x20 * 4], INT20H
    mov word[es:0x20 * 4 + 2], ax
                    ; set return interrupt
    mov word[es:0x21 * 4], INT21H
    mov word[es:0x21 * 4 + 2], ax
                    ; set syscall
    mov word[es:0x22 * 4], INT22H
    mov word[es:0x22 * 4 + 2], ax
                    ; now unused
    mov ax, ds
    mov es, ax      ; recover es

print_hint:
    call clear_screen
    mov byte[coord], 0x00 
    mov byte[coord + 1], 0x00
                    ; set cursor to the upper left
    call set_cursor
    mov si, message
    call print_string
                    ; display hint
get_input:    
    call keyboard_input
                    ; get keyboard input and place it in al  

.handle_input:
    cmp al, 0x30
    jz  get_input
    cmp al, 0x35    ; only 1-4 valid
    jge get_input  ; get back to the beginning 

    mov bl, 0x30
select_program:
    inc bl
    cmp al, bl
    jnz select_program
    
    sub bl, 0x30
    add bl, prefixCount     ; offset the loader and kernel
    call read_floppy        ; program now locates at 0x1000:0x100
    call clear_screen
handle_control: 
    mov bx, word[program_segPos+2]
    mov ds, bx
    mov es, bx
    mov ss, bx              ; cs = ds = es = ss = 0x1000
    mov sp, 0xffff          
    push dword 0x00         ; push the ret address into the user stack
    xor bx, bx              ; use 'dword' to catch c-generated code
    mov word[bx], 0x20CD    ; put "int 0x20" into the psp head
    jmp far [cs:program_segPos]
                    ; handle control to the user program
    jmp print_hint
                    ; go back

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
cursor_display:
; al now has the output char
    mov byte[coord+2], al   
    mov ah, 0x03    ; save the output char
    mov bh, 0x00
    int 0x10        ; get cursor position to dx
    mov byte[coord], dh
    mov byte[coord+1], dl
    call direct_display
.decode_char:       ; determine the next position of the cursor
    cmp byte[coord+2], 0x08
    je .backspace
    cmp byte[coord+2], 0x0D
    je .newline
.nextspace:
    cmp byte[coord+1], 0x79
    jge .newline
    inc byte[coord+1]
    jmp .set
.backspace:
    cmp byte[coord+1], 0x00
    jle .preline
    dec byte[coord+1]
    jmp .set

.newline:
    cmp byte[coord], 0x39
    jge .bottom
    inc byte[coord]
    mov byte[coord+1], 0x00
    jmp .set        ; newline
.bottom:            ; to the lower-right corner
    mov byte[coord], 0x39
    mov byte[coord+1], 0x79
    jmp .set 

.preline:
    cmp byte[coord], 0x00
    jle .top
    dec byte[coord]
    mov byte[coord+1], 0x79
    jmp .set
.top:
    mov byte[coord], 0x00
    mov byte[coord+1], 0x00
    jmp .set

.set:               
    call set_cursor
    ret

;==========================================
; interrupt service routines
;==========================================
save:
    push ds         ; flags/cs/ip/ret/ds
    push cs
    pop ds
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
    push es
    push bp
    push di
    push word[siSave]
    push dx
    push cx
    push bx
    push ax
    mov sp, word[interrSp]      
    mov si, word[retSave]
    jmp si

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

display_wheel:
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
;================ OS return ================
INT20H:
    call save       ; after the save, cs = ds = ss = kernelSs
INT20H_main:        ; and sp = interrSp
    mov bx, ds
    mov es, bx      ; cs = ds = es = ss
    mov sp, word[kernelSp]
    jmp print_hint
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
    cmp ah, 0x00
    jg ._getchar
._getch:            ; get input from the keyboard
    call keyboard_input
    ret
._getchar:          ; get input from the keyboard then output it
    cmp ah, 0x01
    jg ._putchar
    call keyboard_input
._putchar:          ; call the cursor display function
    call cursor_display
    ret

SECTION .data align=16 
;=========================================
; data section of the kernel
;=========================================
    kernelSp    dw  0x1000      ; kernel stack sp
    interrSp    dw  0x2000      ; interrupt stack sp
    coord       db  0, 0, 0     ; y x and char used for helper functions
    cursorCoord db  0, 0        ; cursor position
    wheel       db  0, 0, "|/-\"
    message     db  "A basic monitor", 0xD, 0xA 
                db  "Press keyboard to run corresponding programs", 0xD, 0xA, 
                db  "output string  1", 0xD, 0xA,
                db  "test IO syscall  2", 0xD, 0xA,
                db  "output stones  3, 4", 0xD, 0xA, 0x00
    INT22Mes    db  "INT22H", 0x00

    program_segPos dw  0x100, 0x1000 ; computed later
.tmpReg:
    siSave      dw  0
    dsSave      dw  0
    retSave     dw  0
.IVT_info:
    INT08HAddr  dw  0