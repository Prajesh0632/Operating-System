#pragma once
#include <stdint.h>
#include "../process/process32.h"



/* Standard ELF Program Header Types (p_type) */
#define PT_NULL         0           /* Unused / array element */
#define PT_LOAD         1           /* Loadable segment */
#define PT_DYNAMIC      2           /* Dynamic linking information */
#define PT_INTERP       3           /* Path to dynamic linker / interpreter */
#define PT_NOTE         4           /* Auxiliary information */
#define PT_SHLIB        5           /* Reserved (unused) */
#define PT_PHDR         6           /* Location and size of Program Header Table */
#define PT_TLS          7           /* Thread-Local Storage segment */

/* GNU / OS-Specific Extensions */
#define PT_LOOS         0x60000000  /* Environment-specific range start */
#define PT_GNU_EH_FRAME 0x6474e550  /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK    0x6474e551  /* Stack execution permissions flag */
#define PT_GNU_RELRO    0x6474e552  /* Read-only after relocation */
#define PT_GNU_PROPERTY 0x6474e553  /* GNU property note segment */
#define PT_HIOS         0x6fffffff  /* Environment-specific range end */

#define PT_LOPROC       0x70000000  /* Processor-specific range start */
#define PT_HIPROC       0x7fffffff  /* Processor-specific range end */



/* Segment Permission Flags (p_flags) */
#define PF_X            0x1         /* Executable */
#define PF_W            0x2         /* Writable */
#define PF_R            0x4         /* Readable */

#define PF_MASKOS       0x0ff00000  /* OS-specific flag mask */
#define PF_MASKPROC     0xf0000000  /* Processor-specific flag mask */


/*
 * Read an ELF32 file from the FAT16 disk and print its header + program
 * headers. Inspection only - nothing is mapped or executed.
 *
 *   name        8.3 file name (e.g. "HELLO.ELF")
 *   dir_cluster start cluster of the directory (0 = root)
 */

 
void load_program(const char *name, uint16_t dir_cluster, Process_32*);
