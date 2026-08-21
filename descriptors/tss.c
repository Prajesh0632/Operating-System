#include "tss.h"

void init_tss(uint32_t kernel_stack_top) {
    tss->ss0 = DATA_SEG;
    tss->esp0 = kernel_stack_top;
    tss->iomap_base = sizeof(tss_t);
    __asm__ volatile ("ltr %%ax" :: "a"((uint16_t)TSS_SEG));
}