; a basic loader that outputs text
; provides option to run different programs

SECTION loaderCode align=16 vstart=0x7c00
;====================================

start:
.set_dataSeg:
    xor ax, ax 
    mov ds, ax
    mov es, ax      ; set all basic seg pointer to 0
.set_stackSeg:
    cli 
    mov ax, 0x7c00   ; stack goes down from 0x7c00 while code goes up
    mov sp, ax
    xor ax, ax 
    mov ss, ax      ; some bios may have bugs on dealing with interupt when setting ss:sp
    sti             ; disable interupt brfore setting them
.set_codeSeg:
    jmp set_cs_ip
; cs:ip initial value diffs between bios. Set unique values anyway
set_displaySeg:
    mov ax, 0xb800
    mov gs, ax      ; set display segment for potential use

    cld             
    mov si, message
    call print_hint ; call function to print message

.get_loadeeSeg:
    mov ax, [program_absPos]
    mov dx, [program_absPos + 2]
    mov bx, 16
    div bx          ; shift 4 bits right to get segment index;

    mov word[program_segPos], dx
    mov word[program_segPos + 2], ax
.get_input:    
    call keyboard_input
                    ; get keyboard input and place it in al
.handle_input:
    cmp al, 0x30
    jz  .get_input
    cmp al, 0x35    ; only 1-4 valid
    jge .get_input  ; get back to the beginning 

    mov bl, 0x30
.select_program:
    inc bl
    cmp al, bl
    jnz .select_program
    inc bl          ; offset the bootloader itself
    sub bl, 0x30
    call read_floppy
    
    mov ax, 0x0700  ; scoll the whole window
    mov bh, 0x07    ; white on black
    mov cx, 0x0000  ;
    mov dx, 0x184f  ; corner coordinates of the window
    int 0x10        ; clear screen for user program
    jmp far [program_segPos]
                    ; handle control to the user program

; SECTION functions align=16 vstart=0
;===================================

set_cs_ip:
    push 0x00    ; cs
    mov ax, set_displaySeg
    push ax     ; ip
    retf

print_hint:
.set_cursor:
    mov ah, 0x00
    mov al, 0x03
    int 0x10        ; clear the screen

    mov ah, 0x02
    mov bh, 0x00
    mov dh, 0x00
    mov dl, 0x00
    int 0x10        ; set cursor position

    mov ah, 0x0e    ; set bios function
.print_loop:
    lodsb           ; load chars to al one at a time
    cmp al, 0x00
    je .print_done

    int 0x10        ; bios call
    jmp .print_loop
.print_done:
    ret

keyboard_input:
    mov ah, 0x00
    int 0x16        ; read a char from keyboard press
    ret             

read_floppy:
    mov ah, 0x02   
    mov al, 0x01    ; only read one sector
    mov ch, 0x00    ; cylinder zero
    mov cl, bl      ; get sector index from reg bx, assuming in one sector
    mov dh, 0x00    ; head zero
    mov dl, 0x00    ; drive number, 0 and 1 for floppy
    mov bx, [program_segPos + 2]
    mov es, bx      ; set es = future ds
    xor bx, bx      ; set offset of the target memory address

    int 0x13        ; call interupt

    ret

; SECTION loaderData align=16 vstart=0
; be careful! $$ points to the origin of the section, not the code
;====================================

data:
    authorInfo  db  "Jing Lan, 18340085"
    infoLen     dw  $-authorInfo

    message     db  "A basic loader", 0xD, 0xA 
                db  "Press keyboard to run corresponding program", 0xD, 0xA, 
                db  "    output string and length 1", 0xD, 0xA,
                db  "    stoneUR.com  2", 0xD, 0xA, 
                db  "    stoneDL.com  3", 0xD, 0xA, "    stoneDR.com  4", 0XD, 0xA, 0x00

    program_absPos dd  0x10000  ; preset
    program_segPos dw  0x0, 0x0 ; computed later

times 510-($-$$) db 0         ; fill zeros
                 dw 0xaa55    ; bootable sector signature

