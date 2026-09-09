#include <stdint.h>
#include "paging.h"
#include "pmm.h"


uint32_t* page_directory;
uint32_t* page_table;

extern char _kernel_phys_end[];

void guard_frame(uint32_t frame) {
    page_table[frame] &= ~1;
}

void init_paging() {

    page_directory = (uint32_t*)(uintptr_t)fralloc(1024 * 4);
    page_table = (uint32_t*)(uintptr_t)fralloc(((frames + 1023) / 1024) * 1024 * 4);

    


    

    //set each entry to not present
for(int i = 0; i < 1024; i++)
{
    // This sets the following flags to the pages:
    //   Supervisor: Only kernel-mode can access them
    //   Write Enabled: It can be both read from and written to
    //   Not Present: The page table is not present
    page_directory[i] = 0x00000002;
}


// holds the physical address where we want to start mapping these pages to.
// in this case, we want to map these pages to the very beginning of memory.

//we will fill all 1024 entries in the table, mapping 4 megabytes


int page_directory_size = (frames + 1023) / 1024;
for(int i = 0; i < page_directory_size ; i++) {

    for(int j = 0; j < 1024; j++)
{
    // As the address is page aligned, it will always leave 12 bits zeroed.
    // Those bits are used by the attributes ;)
    page_table[i * 1024 + j] = ((i * 1024 + j) * PAGE_SIZE) | 7; // attributes: supervisor level, read/write, present.
}

// attributes: supervisor level, read/write, present
page_directory[i] = ((unsigned int)&page_table[i * 1024]) | 7;


}


// Higher half: the kernel is linked at 0xC0000000+, so mirror the first 8 MiB
// of physical RAM there (kernel image + boot stack) or this directory would
// unmap the code that is running the moment CR3 is loaded.
//   0xC0000000 >> 22 == 768
page_directory[768] = page_directory[0];   // 0xC0000000..0xC03FFFFF -> phys 0..4 MiB
page_directory[769] = page_directory[1];   // 0xC0400000..0xC07FFFFF -> phys 4..8 MiB

// Trap a stack underflow past the end of the kernel image.
guard_frame((uint32_t)_kernel_phys_end / PAGE_SIZE);




loadPageDirectory(page_directory);
enablePaging();


}


void map_page(uint32_t vaddr, uint32_t* page_directory) {



    uint16_t pd_idx = (vaddr >> 22) & 0x03FF;
    uint16_t pt_idx = (vaddr >> 12) & 0x03FF;

    

   
}



