[bits 32]

global user_entry 
extern user_prog 


USER_CODE_SEG equ 0x18
USER_DATA_SEG equ 0x20

user_entry:

   mov eax, [esp + 4] ; arg1 : user_stack_top
   mov ebx, [esp + 8] ; arg2 : user_entry_point

   ; iret to ring 3 nulls any of DS/ES/FS/GS whose descriptor is not
   ; reachable at CPL 3, so load them with the user data selector first --
   ; otherwise the first access through DS in ring 3 raises #GP.
   mov cx, USER_DATA_SEG | 3
   mov ds, cx
   mov es, cx
   mov fs, cx
   mov gs, cx

   push USER_DATA_SEG | 3  ;SS - Stack Segment
   push eax                ;ESP - Stack pointer to use in ring 3  
   push 0x202              ;EFLAGS - Status flags (with interrupts enabled)
   push USER_CODE_SEG | 3  ;CS - Code Segment
   push ebx                ;EIP - Instruction Pointer
   iret  