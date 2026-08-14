[bits 32]

global port_byte_in
global port_byte_out 
global port_word_in 
global port_word_out 

section .text

port_byte_in:
    mov dx, [esp + 4]     ; 1st argument (port) is at ESP + 4
    in al, dx             ; Read byte into AL
    ret                   ; Return (result is in AL)

port_byte_out:
    mov dx, [esp + 4]     ; 1st argument (port) is at ESP + 4
    mov al, [esp + 8]     ; 2nd argument (data) is at ESP + 8
    out dx, al            ; Write byte from AL to port DX
    ret

port_word_in:
    mov dx, [esp + 4]     ; 1st argument (port) is at ESP + 4
    in ax, dx             ; Read 16-bit word into AX
    ret                   ; Return (result is in AX)

port_word_out:
    mov dx, [esp + 4]     ; 1st argument (port) is at ESP + 4
    mov ax, [esp + 8]     ; 2nd argument (data) is at ESP + 8
    out dx, ax            ; Write word from AX to port DX
    ret