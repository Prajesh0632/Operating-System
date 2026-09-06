[bits 32]

; -----------------------------------------------------------------------------
; Higher-half bootstrap.
;
; The bootloader copied kernel.bin to physical 0x10000 and did `call 0x10000`,
; so we enter here in 32-bit protected mode with paging OFF and EIP low.
;
; This file is linked LOW (see the .boot section in linker.ld: VMA == LMA ==
; physical), so references to its own symbols already resolve to real physical
; addresses.  The rest of the kernel is linked at 0xC0000000+, so we build a
; bootstrap page directory, turn paging on, then call main up in the higher
; half.
;
; Bootstrap window: physical 0..8 MiB is mapped both identity (virtual
; 0..8 MiB) and higher half (virtual 0xC0000000..0xC0800000).  That covers the
; kernel image plus the 2 MiB boot stack.  init_paging() replaces all of this
; with the real page tables later.
; -----------------------------------------------------------------------------

PAGE_PRESENT_RW equ 0x3          ; present | writable

extern main
extern _kernel_end

global _start

section .text
_start:
    cli

    ; --- boot_pt_lo: physical 0 .. 4 MiB --------------------------------
    xor ecx, ecx
.fill_lo:
    mov eax, ecx
    shl eax, 12                  ; frame address = i * 4096
    or  eax, PAGE_PRESENT_RW
    mov [boot_pt_lo + ecx*4], eax
    inc ecx
    cmp ecx, 1024
    jb  .fill_lo

    ; --- boot_pt_hi: physical 4 MiB .. 8 MiB --------------------------
    xor ecx, ecx
.fill_hi:
    mov eax, ecx
    add eax, 1024                ; frame index 1024.. -> physical 4 MiB..
    shl eax, 12
    or  eax, PAGE_PRESENT_RW
    mov [boot_pt_hi + ecx*4], eax
    inc ecx
    cmp ecx, 1024
    jb  .fill_hi

    ; --- zero the page directory -------------------------------------
    mov edi, boot_pd
    xor eax, eax
    mov ecx, 1024
    rep stosd

    ; --- directory entries -----------------------------------------
    ; identity : virtual 0..8 MiB              -> physical 0..8 MiB
    mov dword [boot_pd + 0*4],   boot_pt_lo + PAGE_PRESENT_RW
    mov dword [boot_pd + 1*4],   boot_pt_hi + PAGE_PRESENT_RW
    ; higher half : virtual 0xC0000000..       -> physical 0..8 MiB
    ; (0xC0000000 >> 22 == 768)
    mov dword [boot_pd + 768*4], boot_pt_lo + PAGE_PRESENT_RW
    mov dword [boot_pd + 769*4], boot_pt_hi + PAGE_PRESENT_RW

    ; --- enable paging -------------------------------------------
    mov eax, boot_pd
    mov cr3, eax
    mov eax, cr0
    or  eax, 0x80000000         ; CR0.PG
    mov cr0, eax

    ; --- into the higher half ---------------------------------
    ; EIP is still low here (identity-mapped); the stack and main are high.
    mov esp, _kernel_end
    add esp, 0x200000           ; 2 MiB boot stack (matches pmm.c reservation)
    mov ebp, esp

    call main                   ; relative call resolves to 0xC00xxxxx

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 4096
boot_pd:    resb 4096
boot_pt_lo: resb 4096
boot_pt_hi: resb 4096
