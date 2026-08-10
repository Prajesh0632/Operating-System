[org 0x7c00]

mov bp, 0x9000
mov sp, bp 

mov bx, msg 
call print_string 

mov bx, newline
call print_string

call switch_to_32_bit_protected

jmp $




%include "print.asm" 
%include "switch_32.asm"
msg : db "Booted into Real 16 bit-mode", 0

newline: db 0x0D,0x0A, 0 

times 510 - ($ - $$) db 0 
dw 0xaa55    

times 1474560-($-$$) db 0
