#pragma once
#include <stdint.h>

/* ---- ELF32 file header (offset 0) ---- */
typedef struct {
    uint8_t  e_ident[16];   /* [0..3] = 0x7F 'E' 'L' 'F', [4] = class, [5] = data */
    uint16_t e_type;        /* 2 = ET_EXEC */
    uint16_t e_machine;     /* 3 = EM_386  */
    uint32_t e_version;
    uint32_t e_entry;       /* virtual address to jump to */
    uint32_t e_phoff;       /* program header table: file offset */
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;   /* size of one Elf32_Phdr (32) */
    uint16_t e_phnum;       /* number of program headers */
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed)) Elf32_Ehdr;

/* ---- ELF32 program header (one per segment) ---- */
typedef struct {
    uint32_t p_type;        /* 1 = PT_LOAD */
    uint32_t p_offset;      /* file offset of the segment bytes */
    uint32_t p_vaddr;       /* virtual address it must load at */
    uint32_t p_paddr;
    uint32_t p_filesz;      /* bytes present in the file */
    uint32_t p_memsz;       /* bytes in memory (>= p_filesz; tail is .bss) */
    uint32_t p_flags;       /* PF_R=4 PF_W=2 PF_X=1 */
    uint32_t p_align;
} __attribute__((packed)) Elf32_Phdr;

#define ELFCLASS32  1
#define ET_EXEC     2
#define EM_386      3

#define PT_LOAD     1
#define PF_X        1
#define PF_W        2
#define PF_R        4
