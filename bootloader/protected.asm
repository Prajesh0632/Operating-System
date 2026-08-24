[bits 32]
START_PROTECTED_MODE:

    call clear_screen
    
    call KERNEL_LOC

    jmp $



clear_screen:
    pusha
    cld
    mov edi, 0xb8000
    mov ecx, 80 * 25
    mov ax, 0x0f20          ; White space on black background
    rep stosw
    popa
    ret


%include "print_pm.asm"


