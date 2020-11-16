BITS 16

GLOBAL _start
extern _countA

delay equ 5000

SECTION .text align=16
_start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    cli
    mov ss, ax      ; cs = ds = es
    mov eax, 0x1000 
    mov esp, eax      ; sp = bp
    mov ebp, eax
    sti

    mov ah, 0x02
    mov bh, 0x00
    mov dh, 0x00
    mov dl, 0x00
    int 0x10        ; set cursor position

    cld
    mov si, message     ; load the first location
print_message:    
    lodsb               ; load to register al
    cmp al, 0x00        
    je .count_message

    mov ah, 0x0e
    mov bl, 0x07        ; set function and color
    int 0x10            ; bios call
    jmp print_message
.count_message:
    push dword [messageLen]
    push dword message
    call dword _countA  ; call the C function
    add sp, 8          ; restore the stack 

    push eax
    call dword div_and_disp   ; a recursive function
    add sp, 4

.wait_for_return:
    dec word [count]
    jnz .wait_for_return
    mov word [count], delay
    dec word [count + 2]
    jnz .wait_for_return

    xor eax, eax 
    push eax
    mov eax, 0x7c00
    push eax
    retf                ; return to the loader

div_and_disp:
    push ebp
    mov ebp, esp
    sub sp, 4           ; set function stack
    mov dword [ebp-4], 0
    
    xor ax, ax
    mov al, [ebp + 8]
    mov bl, 10
    div bl              ; divide the number

    mov dword [ebp-4], eax
    cmp al, 0x00
    je .return_and_disp

    push eax            ; push quotient and reminder
    call dword div_and_disp
    add sp, 4           ; unwind the parameter
.return_and_disp:
    mov ax, [ebp-4]
    mov al, ah          ; print the reminder
    add al, 0x30        ; ascii offset
    mov ah, 0x0e
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
.no_disp:
    mov esp, ebp        ; unwind the local variable
    pop ebp             ; unwind the callee stack frame
    ret

SECTION .data align=16
count       dw  65000, 65000
message     db  "Jing Lan is a AAAAAAAAAAAA student :", 0x00 
messageLen  dw  $-message