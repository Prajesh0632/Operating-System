[org 0x7c00]


mov [BOOT_DRIVE], dl ; disk driver identifier (BIOS first loads it into the dl register)


mov bp, 0x8000
mov sp, bp ; moving stack out of the way of the bootloader 

init_read_write:
        mov ah, 0x02 ; BIOS read sector function


 ; Here are the addresses we wish the BIOS to read
 ; Defined by the ES:BX (ES segment with BX offset)

init_memory:
        mov bx, 0x0000
        mov es, bx 
        mov bx, 0x1000
        mov al, 4 ; no of sectors to be read 
        mov dl, [BOOT_DRIVE]
        call disK_load

        mov bx, newline
        call print_string

        mov ax, [0x1000]
        call print_hex


    


  


    




    
END:
    jmp $   
    

%include "print.asm"
%include "disk.asm"

newline: db 0x0D,0x0A, 0 

   
BOOT_DRIVE : db 0
    
    
    


times 510 - ($ - $$) db 0
dw 0xaa55

dw 0xFACE


times 1474560-($-$$) db 0


