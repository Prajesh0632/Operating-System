;GDT
gdt_start:

gdt_null: ; necessary 8 bytes segment 
    dd 0x0
    dd 0x0     

gdt_code: ; code segment descriptor 
    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    
    ;Access flags 
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    db 10011010b 

    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    db 11001111b ; 2nd flags, Limit (bits 16-19)

    db 0x0 ; Base (bits 24-31)

gdt_data: ; data segment descriptor 

    dw 0xffff 
    dw 0x0 
    db 0x0 

    ; Access flags
    db 10010010b 

    db 11001111b ; 2nd flags, Limit (bits 16-19)

    db 0x0 ; Base (bits 24-31)


gdt_user_code:

    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    
    ;Access flags 
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    db 11111010b 

    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    db 11001111b ; 2nd flags, Limit (bits 16-19)

    db 0x0 ; Base (bits 24-31)


gdt_user_data:
   
    dw 0xffff 
    dw 0x0 
    db 0x0 

    ; Access flags
    db 11110010b 

    db 11001111b ; 2nd flags, Limit (bits 16-19)

    db 0x0 ; Base (bits 24-31)


gdt_tss:
    dw 0x0067        ; Limit = 103 (104-byte TSS, byte granularity)
    dw 0x7800        ; Base (bits 0-15)
    db 0x00          ; Base (bits 16-23)
    db 10001001b     ; Present=1, DPL=00, S=0(system), Type=1001 (32-bit TSS, available)
    db 00000000b     ; Granularity=0, Limit (bits 16-19)=0
    db 0x00          ; Base (bits 24-31)




gdt_end:


;gdt descriptor 
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    
    dd gdt_start



CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
USER_CODE_SEG equ gdt_user_code - gdt_start
USER_DATA_SEG equ gdt_user_data - gdt_start
TSS_SEG equ gdt_tss - gdt_start