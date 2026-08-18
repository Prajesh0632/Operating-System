ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    _kernel_start = .;

    .text : {
        *kernel_entry.o(.text) /* Forces kernel entry code to be first at 0x10000 */
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    . = ALIGN(4096);
    _kernel_end = .;
}