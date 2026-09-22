[org 0x7c00]

KERNEL_LOC equ 0x10000 

    xor ax, ax 
    mov es, ax 
    mov ds, ax 


    mov [BOOT_DRIVE_NUM], dl 

    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFC00

    

    call get_vesa_info

    call load_kernel 

    call detect_memory


    in  al, 0x92 ; Enable the A20 line 
    or  al, 0x02
    out 0x92, al

    call switch_to_32_bit_protected



    jmp $


[bits 16]
load_kernel:
    
  


    mov ax, KERNEL_LOC >> 4  ; es:bx = physical KERNEL_LOC (bx alone can't hold a 20-bit address)
    mov es, ax
    xor bx, bx
    mov dl, [BOOT_DRIVE_NUM]
    mov al, 120



    call disk_load

    xor ax, ax               ; restore es=0 -- detect_memory's es:di addressing assumes it
    mov es, ax

    ret




get_vesa_info:
   push es
   mov ax, 0
   mov es, ax
   mov di, 0x7E00        ; free RAM right after the boot sector (0x7C00-0x7DFF)
   mov word [es:di], 'VB'    ; write "VBE2" into the buffer at runtime --
   mov word [es:di+2], 'E2'
   mov ax, 0x4F00
   int 0x10

   cmp ax, 0x4F
   jne .error

   mov cx, 0x0118        ; 1024x768x24 -- reuse the same buffer for ModeInfoBlock
   mov di, 0x7E00
   mov ax, 0x4F01
   int 0x10

   cmp ax, 0x4F
   jne .error

   mov bx, 0x4118         ; mode 0x118 with bit 14 set = use linear framebuffer
   mov ax, 0x4F02
   int 0x10

   cmp ax, 0x4F
   jne .error

   pop es
   ret

.error:
   pop es
   jmp $


%include "disk.asm"
%include "switch_32.asm"
%include "../low level/detect_ram.asm"


; newline: db 0x0D,0x0A, 0 

BOOT_DRIVE_NUM: db 0


times 510 - ($ - $$) db 0 
dw 0xaa55    

