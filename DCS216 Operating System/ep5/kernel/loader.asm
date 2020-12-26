; a basic loader that prints hint then jump to the monitor;

delay equ 25000

SECTION loaderCode align=16 vstart=0x7c00
;========================================

start:
.set_dataSeg:
    xor ax, ax 
    mov ds, ax
    mov es, ax      ; set all basic seg pointer to 0
.set_stackSeg:
    cli 
    mov ax, 0x7c00  ; stack goes down from 0x7c00 while code goes up
    mov sp, ax
    mov bp, ax
    xor ax, ax 
    mov ss, ax      ; some bios may have bugs on dealing with interupt when setting ss:sp
    sti             ; disable interupt brfore setting them

.set_codeSeg:
    jmp set_cs_ip
; cs:ip initial value diffs between bios. Set unique values anyway
set_displaySeg:
    cld             
    mov si, message
    call print_hint ; call function to print message

.wait_for_return:
    dec word[count]
    jnz .wait_for_return
    mov word[count], delay
    dec word[count + 2]
    jnz .wait_for_return
    mov word[count], delay
    mov word[count + 2], delay

.handle_control:
    mov ax, 0x800
    mov ds, ax
    mov es, ax      ; set cs = ds = es = 0x800

.load_monitor:
    mov bl, 2       ; load the monitor into memory
    call read_floppy

    push ds
    push 0x100
    retf            ; handle control to the monitor program

set_cs_ip:
    push 0x00    ; cs
    mov ax, set_displaySeg
    push ax      ; ip
    retf

print_hint:
    mov ah, 0x0e    ; set bios function
.print_loop:
    lodsb           ; load chars to al one at a time
    cmp al, 0x00
    je .print_done

    int 0x10        ; bios call
    jmp .print_loop
.print_done:
    ret

read_floppy:
    mov ah, 0x02   
    mov al, 0x02    ; only read two sectors
    mov ch, 0x00    ; cylinder zero
    mov cl, bl      ; get sector index from reg bx, assuming in one sector
    mov dh, 0x00    ; head zero
    mov dl, 0x00    ; drive number, 0 and 1 for floppy
                    ; es is set 0x800 before
    mov bx, 0x100   ; set offset of the target memory address

    int 0x13        ; call interupt
    ret

data:
    count       dw  25000, 25000
    message     db  "Basic loader by Jing Lan is loading...", 0xD, 0xA, 0xD, 0xA, 0x00

times 510-($-$$) db 0         ; fill zeros
                 dw 0xaa55    ; bootable sector signature