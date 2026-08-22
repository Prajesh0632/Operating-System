[bits 32]

global user_entry 
extern user_prog 


USER_CODE_SEG equ 0x18
USER_DATA_SEG equ 0x20

user_entry:

   mov eax, [esp + 4] ; arg1 : user_stack_top
   mov ebx, [esp + 8] ; arg2 : user_entry_point
   
   push USER_DATA_SEG | 3  ;SS - Stack Segment
   push eax                ;ESP - Stack pointer to use in ring 3  
   push 0x202              ;EFLAGS - Status flags (with interrupts enabled)
   push USER_CODE_SEG | 3  ;CS - Code Segment
   push ebx                ;EIP - Instruction Pointer
   iret  