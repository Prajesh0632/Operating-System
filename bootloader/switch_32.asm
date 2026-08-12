
[bits 16]
switch_to_32_bit_protected:

    
    xor ax, ax 
    mov ds, ax 
    lgdt [gdt_descriptor]

   

  
    
    cli ; disable all interrupts 

    
    mov eax, cr0 
    or eax, 0x1 
    mov cr0, eax 


   
    

    jmp CODE_SEG:init_protected_mode 


[bits 32]

init_protected_mode:

   

    mov ax, DATA_SEG ; move everything to the flat memory since segments are no longer required 
    mov ds, ax 
    mov ss, ax 
    mov es, ax 
    mov fs, ax 
    mov gs, ax 



    mov ebp, 0x90000
    mov esp, ebp 
    
    
    

    call START_PROTECTED_MODE

%include "protected.asm"  
%include "gdt.asm" 

success : db "Huccess", 0