[bits 32]
extern main
extern _kernel_end

global _start
_start:
    call main
    jmp $
