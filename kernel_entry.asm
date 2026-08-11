[ bits 32] ; We ’re in protected mode by now , so use 32 - bit instructions.
[ extern main ]


call main 
jmp $