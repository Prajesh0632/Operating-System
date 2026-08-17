[bits 16]

MEMORY_MAP_BUFFER equ 0x8000   ; scratch area: past the boot sector (0x7C00-0x7DFF), well below the stack (0x9FC00)
MEMORY_MAP_COUNT equ 0x77FC    ; location to store the count 4 bytes below the buffer


detect_memory:
    mov word [MEMORY_MAP_COUNT], 0  ; initialized to zero 
    xor ebx, ebx             ; EBX = 0 starts the detection
    mov di, MEMORY_MAP_BUFFER ; es:di = destination for each SMAP entry

.loop:
    mov eax, 0x0000E820     ; Reset EAX to E820 on every loop
    mov edx, 0x534D4150     ; Reset EDX to "SMAP" on every loop
    mov ecx, 20             ; Tell BIOS to write a basic 20-byte entry
    
    int 0x15
    jc .error               ; Carry flag = end of list or error

    cmp ebx, 0              ; EBX = 0 means this was the last entry
    je .done
    
    inc word [MEMORY_MAP_COUNT]
    add di, 20              ; Move buffer pointer forward 20 bytes
    jmp .loop

.done:
   ret 

.error:
   hlt



   


   
        