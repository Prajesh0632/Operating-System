disk_load:

  

  
    mov ah, 0x02
    mov dh, al 
    push dx 

    mov dh, 0 ; head/surface of the disk drive (0-front, 1-back)
    mov ch, 0 ; cylinder/tracks (0-based index, from the outermost concentric circle)
    mov cl, 2 ; initial sector to start reading from (1-based index) 


   
    int 0x13 ; BIOS interrupt for read
    ; after read al register contains the number of sectors read 

    jc disk_error ; if carry is set disk read failed

    pop dx 
    cmp dh, al
    jne disk_error 

    mov bx, SUCCESS 
    call print_string
    ret 
   



disk_error:
    mov bx, DISK_ERROR
    call print_string
    ret 

SUCCESS : db "Disk read successful", 0    
DISK_ERROR : db "Error while reading", 0    
ENTERED : db "Entered", 0