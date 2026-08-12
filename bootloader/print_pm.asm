[bits 32]

VIDEO_MEMORY equ 0xb8000
COLOR equ 0x0f 


print_string_pm:
    pusha 
    mov ebx, VIDEO_MEMORY 
    

print_pm:
    mov al, [edx]
    mov ah, COLOR 

    cmp al, 0
    je done_pm 

    mov [ebx], ax
    
    add ebx, 2
    add edx, 1 

    jmp print_pm 

    


    
done_pm:
    popa 
    ret     











