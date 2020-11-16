delay   equ 5_000

SECTION main align=16 vstart=0
; vstart and org only change shift addresses of tags
; cannot place spaces between arguments and their parameters.

initial:
    mov ax, cs
    mov ds, ax
    mov es, ax
    cli
    mov ss, ax      ; cs = ds = es
    xor ax, ax 
    mov sp, ax      ; sp = 0
    sti

    mov ax, 0xb800  ; Jump instructions and other segment registers stay still.
    mov gs, ax      ; point es to the display memory segment
begin:
    jmp showName    ; print out author information
printLoop:
    dec word[count]
    jnz printLoop
    mov word[count], delay
    dec word[count + 2]
    jnz printLoop
    mov word[count], delay
    mov word[count + 2], delay

    inc word[count + 4]
    mov ax, 100
    sub ax, [count + 4]
    jz  clearScreen ; only print 100 times

    mov ax, 24      ; a samller region to display
    sub ax, [y]     ; check boundary for all four directions.
    jnz BottomNotReached
    mov word[movey], -1
BottomNotReached:
    mov ax, [y]
    sub ax, 12
    jnz CeilNotReached
    mov word[movey], 1
CeilNotReached:
    mov ax, 39      ; left half of the screen;
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
    sub ax, [y + 2]
    jg  checkPassed1 ; check y 
    mov ax, [x]
    sub ax, [infoLen]
    jge checkPassed1 ; check x
    jmp printLoop   ; Skip if area of personal information is touched
checkPassed1:
    call showChar
    jmp printLoop

showName:
    xor di, di      ; set the intial index register
printNameLoop:
    mov ax, [y + 2]
    imul ax, 80
    add ax, [x + 2] ; set the offset to fit a smaller region
    add ax, di      ; set the first postion
    imul ax, 2         
    mov bx, ax      ; double the index

    cmp byte[char], ' '
    jne .firstTime
    jmp .secondTime
.firstTime:
    mov al, [AuthorInfo + di]
    mov ah, 0x0C    ; set the character and its RGB color
    jmp .disp
.secondTime:
    mov al, ' '     ; clear the information 
    mov ah, 0x0F
.disp:
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
    cmp byte[char], ' '
    jne .bright
    mov ah, 0x07
.bright:
    mov [gs:bx], ax
    ret             ; return to the caller

clearScreen:
    cmp byte[char], ' '
    je  return 

    mov word[x], 2
    mov word[y], 12
    mov byte[char], ' '
    mov word[count + 4], 0

    jmp begin

return:
    xor ax, ax
    push ax
    mov ax, 0x7c00
    push ax
    retf           ;end the program and return control

data:
    AuthorInfo  db  "Jing Lan, 18340085"
    infoLen     dw  $-AuthorInfo

    char        db  'A'
    x           dw  2, 0
    y           dw  12, 12
    movex       dw  1
    movey       dw  1
    count       dw  delay, delay, 0