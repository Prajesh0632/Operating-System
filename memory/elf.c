/*
 * Freestanding user-space test program.
 *
 * This is NOT linked into the kernel. It is built as a standalone ELF32
 * executable (see user.ld / build_user.sh) so it can be dropped on the
 * FAT16 disk and loaded by the kernel's ELF loader.
 *
 * It talks to the kernel only through int 0x80 (see system/system_header.h
 * for the ids, interrupts/interrupt.asm for the register ABI:
 *   eax = syscall id, ebx = arg1, ecx = arg2, edx = arg3).
 */

#define SYS_WRITE 0          /* first entry of SyscallId in system_header.h */

static void sys_write(const char *s)
{
    __asm__ volatile ("int $0x80" :: "a"(SYS_WRITE), "b"(s) : "memory");
}

__attribute__((noreturn))
void _start(void)
{
    sys_write("hello from an ELF program\n");

    for (;;)
        __asm__ volatile ("pause");   /* no SYS_EXIT yet - just spin */
}
