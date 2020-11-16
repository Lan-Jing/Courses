delay   equ 5_000

org 0x0100           ; the 256-bytes prefix is used by DOS;

SECTION MBR align=16 
; vstart and org only change shift addresses of tags
; cannot place spaces between arguments and their parameters.

initial:
    mov ax, 0xb800  ; Jump instructions and other segment registers stay still.
    mov gs, ax      ; point es to the display memory segment

    ; for a .com program on DOS, no need to set cs/ds/es/ss and sp(if no stack specifed)
    ; cs = ds = es = ss = PSP, sp = first byte avaliable in the tail of the 64K segment
    ; ip = 0x0100

    mov byte[char], 'A'
    mov word[x], 2
    mov word[y], 0
    mov word[movex], 1
    mov word[movey], 1
    mov word[count + 4], 0

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

    mov ax, 11      ; a samller region to display
    sub ax, [y]     ; check boundary for all four directions.
    jnz BottomNotReached
    mov word[movey], -1
BottomNotReached:
    mov ax, [y]
    sub ax, 0
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
    sub ax, 0
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
    ret             ; return to the caller

clearScreen:
    mov word[x], 0
    mov word[y], 0
    mov byte[char], ' '
printSpaceLoop:
    mov ax, 12
    sub ax, [y]
    jnz checkPassed2

    push ds
    xor ax, ax 
    push ax
    retf           ; return control to DOS
checkPassed2:
    call showChar
    mov ax, 39
    sub ax, [x]
    jz  nextLine
    inc word[x]
    jmp printSpaceLoop
nextLine:
    mov word[x], 0
    inc word[y]
    jmp printSpaceLoop

data:
    AuthorInfo  db  "Jing Lan, 18340085"
    infoLen     dw  $-AuthorInfo

    char        db  'A'
    x           dw  2, 0
    y           dw  0, 0
    movex       dw  1
    movey       dw  1
    count       dw  delay, delay, 0