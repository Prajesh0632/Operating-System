[org 0x7c00]

KERNEL_LOC equ 0x1000 

    xor ax, ax 
    mov es, ax 
    mov ds, ax 


    mov [BOOT_DRIVE_NUM], dl 

    mov bp, 0x9000
    mov sp, bp 

    mov bx, msg_rm 
    call print_string 

    call load_kernel 

    call switch_to_32_bit_protected



    jmp $


[bits 16]
load_kernel:
    
    mov bx, newline
    call print_string

    mov bx, msg_pm
    call print_string


    mov bx, KERNEL_LOC
    mov dl, [BOOT_DRIVE_NUM]
    mov al, 15 

    
    
    call disk_load
    ret 








%include "print.asm"
%include "disk.asm"
%include "switch_32.asm"


msg_rm : db "Booted into Real 16 bit-mode", 0
msg_pm : db "Reading Disk..............", 0 

newline: db 0x0D,0x0A, 0 

BOOT_DRIVE_NUM: db 0

times 510 - ($ - $$) db 0 
dw 0xaa55    

