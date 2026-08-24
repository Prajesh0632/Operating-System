[bits 16]

SECTORS_PER_TRACK equ 18
HEADS             equ 2

; in: al = sector count, es:bx = destination, dl = drive number
; INT 13h AH=02h cannot cross a track boundary, so read one sector per call
; and convert LBA -> CHS each time.
disk_load:
    mov [sector_count], al
    mov [boot_drive], dl
    mov word [current_lba], 1      ; kernel starts at LBA 1 (sector after boot sector)

.next_sector:
    cmp byte [sector_count], 0
    je .done

    mov ax, [current_lba]
    xor dx, dx
    mov cx, SECTORS_PER_TRACK
    div cx                         ; ax = LBA / 18, dx = LBA % 18
    inc dl
    mov [chs_sector], dl           ; sector = (LBA % 18) + 1, 1-based
    xor dx, dx
    mov cx, HEADS
    div cx                         ; ax = cylinder, dx = head
    mov [chs_head], dl
    mov [chs_cylinder], al

    mov di, 3                      ; retries -- first-attempt failures are normal
.attempt:
    mov ah, 0x02
    mov al, 1
    mov ch, [chs_cylinder]
    mov cl, [chs_sector]
    mov dh, [chs_head]
    mov dl, [boot_drive]
    int 0x13
    jnc .sector_ok

    xor ah, ah                     ; reset the disk controller before retrying
    mov dl, [boot_drive]
    int 0x13
    dec di
    jnz .attempt
    jmp disk_error

.sector_ok:
    add bx, 512
    jnc .no_wrap
    mov ax, es                     ; bx wrapped -- advance es by 64KB
    add ax, 0x1000
    mov es, ax
.no_wrap:
    inc word [current_lba]
    dec byte [sector_count]
    jmp .next_sector

.done:
    mov bx, SUCCESS
    call print_string
    ret

disk_error:
    mov bx, DISK_ERROR
    call print_string
    jmp $                          ; fatal: never fall through into an unloaded kernel

sector_count : db 0
boot_drive   : db 0
current_lba  : dw 0
chs_sector   : db 0
chs_head     : db 0
chs_cylinder : db 0

SUCCESS : db "Disk OK", 0
DISK_ERROR : db "Disk ERR", 0
