
#include "vmm.h"
#include "pmm.h"
#include "paging.h"


void vmm_map(uint32_t vaddr, uint32_t flags) {

    uint32_t pd_index = (vaddr >> 22) & 0x3FF;
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;
    uint32_t offset =    vaddr        & 0xFFF;





}
