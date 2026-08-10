
[bits 16]
switch_to_32_bit_protected:

    lgdt [gdt_descriptor]

    mov bx, switch_msg
    call print_string

    mov bx, newline
    call print_string
    
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

switch_msg : db "Switching to protected 32 bit mode............", 0    
init_msg : db "Initializing protected mode............", 0