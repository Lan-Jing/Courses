BITS 16

global _getchar
global _getch
global _putchar
global _puts
global _gets
global _newline

extern _strlen

SECTION .text align=16
_getch:
    mov ah, 0x00    ; function number 0x01
    int 0x21
    ret 2
    
_getchar:
    mov ah, 0x01    ; no parameters, no local variables
    int 0x21        ; return value stays in ax
    ret 2  

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
    ret 2

_puts:              ; iteratively call _putchar
    push ebp
    mov ebp, esp
    push bx         ; save bx for string location
    push di         ; save di for iteration
    push ax         ; save ax for string length
.get_length:    
    xor ebx, ebx 
    mov bx, word[bp+8]  ; get address of the string
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
    call dword _newline ; move to the next line
    pop ax
    pop di
    pop bx
    mov esp, ebp
    pop ebp
    ret 2

_gets:
    push ebp
    mov ebp, esp
    push bx
    push di
    push ax             ; callee-saved registers
    xor di, di
    mov bx, word[bp+8]  ; get address of the string
.gets_loop:
    call dword _getchar
    cmp al, 0x20
    jl .gets_end        ; stop on any control signal
    mov byte[bx+di], al
    inc di              ; store it otherwise
    jmp .gets_loop
.gets_end:
    mov byte[bx+di], 0  ; set the end char
    call dword _newline ; move to the next line
    pop ax
    pop di
    pop bx              ; restore the stack
    mov esp, ebp
    pop ebp
    ret 2

_newline:               ; move to the next line
    push ebp
    mov ebp, esp
    push dword 0xD
    call dword _putchar
    add sp, 4
    push dword 0xA
    call dword _putchar 
    add sp, 4         
    mov esp, ebp
    pop ebp  
    ret 2