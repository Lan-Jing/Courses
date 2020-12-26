    delay   equ 5_000

SECTION MBR align=16 vstart=0x7c00 ; vstart and org only change shift addresses of tags
                                   ; cannot place spaces between arguments and their parameters.
initial:
    mov ax, 0xb800  ; Jump instructions and other segment registers stay still.
    mov gs, ax      ; point es to the display memory segment
    
    mov ax, cs      ; this statement can't be ignored, which means cs != 0
    mov ds, ax
    mov es, ax

    jmp showName    ; print out author information
printLoop:
    dec word[count]
    jnz printLoop
    mov word[count], delay
    dec word[count + 2]
    jnz printLoop
    mov word[count], delay
    mov word[count + 2], delay
                    ; a 16-bit word occupies 2 bytes
    mov ax, 24
    sub ax, [y]     ; check boundary for all four directions.
    jnz BottomNotReached
    mov word[movey], -1
BottomNotReached:
    mov ax, [y]
    sub ax, 0
    jnz CeilNotReached
    mov word[movey], 1
CeilNotReached:
    mov ax, 79
    sub ax, [x]
    jnz RightNotReached
    mov word[movex], -1
RightNotReached:
    mov ax, [x]
    sub ax, 0
    jnz LeftNotReached
    mov word[movex], 1
LeftNotReached:
    mov ax, [x]
    add ax, [movex]
    mov [x], ax     ; compute new coordinate x
    mov ax, [y]
    add ax, [movey]
    mov [y], ax     ; compute new coordinate y

    mov ax, [y]
    sub ax, 0
    jg  showChar    ; check y 
    mov ax, [x]
    sub ax, [infoLen]
    jge showChar    ; check x
    jmp printLoop   ; Skip if area of personal information is touched

showName:
    xor di, di      ; set the intial index register
printNameLoop:
    mov ax, di
    imul ax, 2         
    mov bx, ax      ; double the index

    mov al, [AuthorInfo + di]
    mov ah, 0x0c    ; set the character and its RGB color
    mov [gs:bx], ax ; move into display memory 

    inc di
    cmp [infoLen], di
    jnz printNameLoop
    jmp printLoop

showChar:
    mov ax, [y]
    imul ax, 80
    add ax, [x]
    imul ax, 2
    mov bx, ax
    mov al, [char]
    mov ah, 0x0F
    mov [gs:bx], ax
    jmp printLoop 

data:
    AuthorInfo  db  "Jing Lan, 18340085"
    infoLen     dw  $-AuthorInfo

    char        db  'A'
    x           dw  20
    y           dw  0
    movex       dw  1
    movey       dw  1
    count       dw  delay, delay