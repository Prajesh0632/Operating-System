ENTRY(_start)

/* Kernel runs at the top 1 GiB of the virtual address space. */
KERNEL_VMA = 0xC0000000;

SECTIONS
{
    /*
     * The bootloader copies kernel.bin to physical 0x10000 and does
     * `call 0x10000`, so the first byte of the image must be _start.
     */
    . = 0x10000;

    _kernel_phys_start = .;

    /*
     * Boot stub. Executes with paging OFF, therefore it must be linked at
     * the physical address it actually runs at (VMA == LMA == 0x10000).
     * kernel_entry.o goes first so _start is the very first byte.
     * This code sets up the bootstrap page tables, enables paging and
     * far-jumps into the higher half before calling main.
     */
    .boot ALIGN(4) : {
        *kernel_entry.o(.text*)
        *kernel_entry.o(.rodata*)
        *kernel_entry.o(.data*)
        *kernel_entry.o(.bss*)
        *kernel_entry.o(COMMON)
    }

    /*
     * The real kernel is linked at the higher half (VMA = 0xC00xxxxx) but
     * its load address (LMA) stays right after .boot so kernel.bin remains
     * one contiguous blob on disk. AT() carries the VMA->LMA offset.
     */
    . = ALIGN(4096);
    _kernel_load = .;            /* physical address the kernel is loaded at */
    . += KERNEL_VMA;             /* switch the location counter to virtual   */

    _kernel_start = .;          /* virtual */

    .text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VMA) {
        *(.text*)
    }

    .rodata ALIGN(4096) : AT(ADDR(.rodata) - KERNEL_VMA) {
        *(.rodata*)
    }

    .data ALIGN(4096) : AT(ADDR(.data) - KERNEL_VMA) {
        *(.data*)
    }

    .bss ALIGN(4096) : AT(ADDR(.bss) - KERNEL_VMA) {
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(4096);
    _kernel_end = .;                              /* virtual */
    _kernel_phys_end = _kernel_end - KERNEL_VMA;  /* physical */
}
