#include <stdint.h>
#include "paging.h"
#include "pmm.h"


uint32_t* page_directory;
uint32_t* page_table;



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
    page_table[i * 1024 + j] = ((i * 1024 + j) * 0x1000) | 7; // attributes: supervisor level, read/write, present.
}


// attributes: supervisor level, read/write, present
page_directory[i] = ((unsigned int)&page_table[i * 1024]) | 3;


}





loadPageDirectory(page_directory);
enablePaging();


}


