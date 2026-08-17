[org 0x7c00]

KERNEL_LOC equ 0x10000 

    xor ax, ax 
    mov es, ax 
    mov ds, ax 


    mov [BOOT_DRIVE_NUM], dl 

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFC00

    mov bx, msg_rm 
    call print_string 

    call load_kernel 

    call detect_memory

    call switch_to_32_bit_protected



    jmp $


[bits 16]
load_kernel:
    
    mov bx, newline
    call print_string

    mov bx, msg_pm
    call print_string


    mov ax, KERNEL_LOC >> 4  ; es:bx = physical KERNEL_LOC (bx alone can't hold a 20-bit address)
    mov es, ax
    xor bx, bx
    mov dl, [BOOT_DRIVE_NUM]
    mov al, 50



    call disk_load

    xor ax, ax               ; restore es=0 -- detect_memory's es:di addressing assumes it
    mov es, ax

    ret








%include "print.asm"
%include "disk.asm"
%include "switch_32.asm"
%include "../low level/detect_ram.asm"


msg_rm : db "Booted into Real 16 bit-mode", 0
msg_pm : db "Reading Disk..............", 0 

newline: db 0x0D,0x0A, 0 

BOOT_DRIVE_NUM: db 0

times 510 - ($ - $$) db 0 
dw 0xaa55    

