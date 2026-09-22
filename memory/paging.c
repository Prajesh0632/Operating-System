#include <stdint.h>
#include "paging.h"
#include "pmm.h"


uint32_t* page_directory;
uint32_t* page_table;

extern char _kernel_phys_end[];

void guard_frame(uint32_t* table, uint32_t frame) {
    table[frame] &= ~1;
}

void init_paging() {

    // High-half aliases, not raw physical: guard_frame(), map_page() and
    // anything else that dereferences these globals needs to keep working
    // regardless of which process's directory is currently loaded in CR3.
    page_directory = (uint32_t*)fralloc_kernel(1024 * 4);
    page_table = (uint32_t*)fralloc_kernel(((frames + 1023) / 1024) * 1024 * 4);

    


    

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
    // USER stays set here: the shell runs at CPL=3 under this SAME
    // directory (not a separate one), sharing this exact code/data, so it
    // needs these pages to stay user-accessible. Isolation for a loaded
    // process's OWN directory is enforced separately, by masking this bit
    // off the copy create_process() installs there -- see process32.c.
    page_table[i * 1024 + j] = ((i * 1024 + j) * PAGE_SIZE) | PTE_PRESENT | PTE_WRITABLE | PTE_USER;
}

// attributes: supervisor level, read/write, present
// A PDE holds a physical address (hardware-defined), but page_table is
// now the high-half alias -- convert back at the boundary.
page_directory[i] = virt_to_phys(&page_table[i * 1024]) | PDE_PRESENT | PDE_WRITABLE | PDE_USER;


}


// Higher half: the kernel is linked at 0xC0000000+, and fralloc_kernel()
// hands out phys+KERNEL_VMA pointers for anything the kernel keeps
// dereferencing across a CR3 switch. Mirror ALL identity-mapped RAM up
// there (not just the kernel image) so both promises hold everywhere,
// not just in the first 8 MiB.
//   0xC0000000 >> 22 == 768
for (int i = 0; i < page_directory_size && (768 + i) < 1024; i++) {
    page_directory[768 + i] = page_directory[i];
}

// Trap a stack underflow past the end of the kernel image.
guard_frame(page_table, (uint32_t)_kernel_phys_end / PAGE_SIZE);




loadPageDirectory((uint32_t*)virt_to_phys(page_directory));
enablePaging();


}



void invlpg(uint32_t vaddr)
{
    __asm__ volatile ("invlpg (%0)" :: "r"(vaddr) : "memory");
}

void map_page(uint32_t vaddr, uint32_t* page_directory) {



    uint16_t pd_idx = (vaddr >> 22) & 0x03FF;
    uint16_t pt_idx = (vaddr >> 12) & 0x03FF;

    // Local, not the shared `page_table` global -- that global gets
    // reassigned by every caller (including this one used to), so reusing
    // it here would leave it pointing at whichever table was touched last.
    uint32_t* table;

    if (!(page_directory[pd_idx] & PDE_PRESENT)) {
        uint32_t new_table = fralloc(PAGE_SIZE);
        page_directory[pd_idx] = (new_table & 0xFFFFF000) | PDE_PRESENT | PDE_WRITABLE | PDE_USER;
        table = (uint32_t*)phys_to_virt(new_table & 0xFFFFF000);
    } else {
        table = (uint32_t*)phys_to_virt(page_directory[pd_idx] & 0xFFFFF000);
    }

    uint32_t frame = fralloc(PAGE_SIZE);
    table[pt_idx] = (frame & 0xFFFFF000) | PTE_PRESENT | PTE_WRITABLE | PTE_USER;




}

void map_page_to(uint32_t vaddr, uint32_t paddr, uint32_t* page_directory) {



    uint16_t pd_idx = (vaddr >> 22) & 0x03FF;
    uint16_t pt_idx = (vaddr >> 12) & 0x03FF;

    // Local, not the shared `page_table` global -- that global gets
    // reassigned by every caller (including this one used to), so reusing
    // it here would leave it pointing at whichever table was touched last.
    uint32_t* table;

    if (!(page_directory[pd_idx] & PDE_PRESENT)) {
        uint32_t new_table = fralloc(PAGE_SIZE);
        page_directory[pd_idx] = (new_table & 0xFFFFF000) | PDE_PRESENT | PDE_WRITABLE | PDE_USER;
        table = (uint32_t*)phys_to_virt(new_table & 0xFFFFF000);
    } else {
        table = (uint32_t*)phys_to_virt(page_directory[pd_idx] & 0xFFFFF000);
    }

   
    table[pt_idx] = (paddr & 0xFFFFF000) | PTE_PRESENT | PTE_WRITABLE | PTE_USER;




}
