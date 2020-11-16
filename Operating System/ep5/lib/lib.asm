BITS 16

global _getchar
global _getch
global _putchar
global _puts
global _gets

extern _strlen

SECTION .text align=16
_getch:
    mov ah, 0x00    ; function number 0x01
    int 0x21
    ret
    
_getchar:
    mov ah, 0x01    ; no parameters, no local variables
    int 0x21
    ret             ; return value stays in ax

_putchar:
    push ebp
    mov ebp, esp
    push eax
    mov al, byte[bp+8]
    mov ah, 0x02    ; al holds the output char
    int 0x21
    pop eax
    mov esp, ebp
    pop ebp
    ret

_puts:              ; iteratively call _putchar
    push ebp
    mov ebp, esp
    push bx         ; save bx for string location
    push di         ; save di for iteration
    push ax         ; save ax for string length
.get_length:    
    mov ebx, dword[bp+8]; get address of the string
    push ebx            ; then push it as a parameter
    call dword _strlen
    add sp, 4           ; ax now holds the length

    xor di, di
.put_loop:
    cmp ax, di
    je  .put_end        ; check string length
    push dword[bx+di]
    call dword _putchar ; call _putchar to print single char
    add sp, 4
    inc di
    jmp .put_loop
.put_end:
    pop ax
    pop di
    pop bx
    mov esp, ebp
    pop ebp
    ret

_gets:
    push ebp
    mov ebp, esp
    push bx
    push di
    push ax
    xor di, di
    mov bx, word[bp+8]
.gets_loop:
    call dword _getchar
    cmp ax, 0xD
    je .gets_end
    mov byte[bx+di], al
    inc di
    jmp .gets_loop
.gets_end:
    pop ax
    pop di
    pop bx
    mov esp, ebp
    pop ebp
    ret