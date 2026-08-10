[bits 32]
START_PROTECTED_MODE:

    call clear_screen

    mov edx, success_msg
    call print_string_pm

    jmp $


clear_screen:
    pusha
    cld
    mov edi, 0xb8000
    mov ecx, 80*25
    mov ax, 0x0f20          ; space, white on black
    rep stosw
    popa
    ret


%include "print_pm.asm"

success_msg: db "Now entered Protected mode", 0    

