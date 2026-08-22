[bits 32]
extern main
extern _kernel_end

global _start
_start:
    
    mov esp, _kernel_end
    add esp, 0x200000    ; stack top = _kernel_end + 2MB, must match init_bitmap()'s reservation
    call main
    jmp $
