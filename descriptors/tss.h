#include <stdint.h>

#define TSS_ADDR 0x7800
#define TSS_SEG  0x28   // gdt_null(0) + code(8) + data(8) + user_code(8) + user_data(8) = 0x28
#define DATA_SEG 0x10   // mirror gdt.asm's segment offsets so C code doesn't hardcode magic numbers

typedef struct __attribute__((packed)) {
    uint32_t link;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3;
    uint32_t eip, eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp;
    uint32_t esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} tss_t;

#define tss ((tss_t*)TSS_ADDR)

void init_tss(uint32_t);