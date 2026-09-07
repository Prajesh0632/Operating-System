/*
 * Linker script for standalone user-space programs (e.g. memory/elf.c).
 *
 * The kernel lives in the top 1 GiB (0xC0000000+), so user space is
 * 0x00000000 .. 0xBFFFFFFF. We link at 4 MiB - clear of the kernel's
 * low bootstrap region and easy to load while paging is still identity.
 *
 * Output is a real ELF32 executable (NOT --oformat binary): the kernel's
 * loader reads the program headers and e_entry.
 */

ENTRY(_start)

SECTIONS
{
    . = 0x00400000;

    .text : {
        *(.text._start)
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data*)   }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(4096);
    _end = .;

    /DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
