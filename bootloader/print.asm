print_string:
    xor si, si 

print:
    cmp byte [bx + si], 0 
    je done
    mov al, [bx + si]
    inc si 
    call print_char
    jmp print

  
    
    
; print_hex:
    
;     pusha
;     mov bx, hex_prefix
;     call print_string
;     popa

;     xor si, si 
;     mov dx, ax 
;     mov cx, 4 


; next:    
;     cmp si, 4 
;     je done 
;     rol dx, cx ; rotate the hex number to get the first digit at the last and so on... for 4 times
;     mov bx, 000fh 
;     and bx, dx ; stores the last hex digit at bx 
;     mov al, [hex_chars + bx] ; gets the respective hex ascii char from the hex_chars with offset bx (bx == 0 is 0 and bx == 15 is F)
;     call print_char
;     inc si 
;     jmp next
    


print_char:
    mov ah, 0x0e 
    int 0x10 
    ret     


done:
    ret 


; hex_prefix:
;     db "0x", 0 

; hex_chars:
;     db "0123456789ABCDEF", 0    


    