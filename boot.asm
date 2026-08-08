[org 0x7c00]


PRINT:
    
    mov bx, newline
    call print_string 

    mov bx, my_string
    call print_string

    mov bx, newline
    call print_string 

    mov bx, my_name
    call print_string

    mov bx, newline
    call print_string 
    
   
    mov ax, [number] 
    call print_hex 


    

    
END:
    jmp $   
    

%include "print.asm"

my_string:
 db "Hello, World ", 0

my_name:
    db "Prajesh Subedi ", 0
    
newline:
    db 0x0D,0x0A, 0 

   

number:
    dw 0xAABB 
    
    
    


times 510 - ($ - $$) db 0
dw 0xaa55

times 1474560-($-$$) db 0


