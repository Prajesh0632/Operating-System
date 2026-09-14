#include "vmm.h"
#include "pmm.h"
#include "paging.h"
#include "../process/process32.h"
#include "../file_system/fat16.h"
#include "elf_loader.h"
#include "elf.h"
#include "memory.h"
#include "../screen_driver/screen.h"



void vmm_map_page(uint32_t vaddr, uint32_t* page_directory) {



    uint16_t pd_idx = (vaddr >> 22) & 0x03FF;
    uint16_t pt_idx = (vaddr >> 12) & 0x03FF;

    uint32_t* page_table;
    
    if(!(page_directory[pd_idx] & PDE_PRESENT)) {

       uint32_t* new_page = (uint32_t*)fralloc(PAGE_SIZE);

       page_directory[pd_idx] = ((uint32_t)new_page & 0xFFFFF000) | PDE_PRESENT | PDE_WRITABLE | PDE_USER;

       page_table = new_page;
    }

    else {
        
        page_table = (uint32_t*)(page_directory[pd_idx] & 0xFFFFF000);

    }

    page_table[pt_idx] = ((uint32_t)0x00000000 & 0xFFFFF000) | PTE_WRITABLE | PTE_USER;
    



}


void vmm_handle_pagefault(uint32_t vaddr) {

    Process_32* process = current_process;
    

    Vma* temp_vma = process->vma_list;

    while(temp_vma != NULL) {

        if(vaddr >= temp_vma->v_start && vaddr <= temp_vma->v_end) break;

        temp_vma = temp_vma->next;
        

    }

    if(!temp_vma) return;

    for(uint32_t p = 0; p < temp_vma->pages_required; p++) {
          
        uint32_t cur_vaddr = temp_vma->v_start + p * PAGE_SIZE;
        map_page(cur_vaddr, process->page_directory);

    }

    if(temp_vma->inFile) {

        DirEntry De;

    if (!fat_find(process->filename, process->dir_cluster, &De))
    {
        sprint("Cannot find the file specified.\n", -1, -1);
        return;
    }

    uint8_t *content = (uint8_t *)(uintptr_t)fralloc(De.size);
    memset(content, 0, De.size);

    if (!fat_read(&De, content, De.size))
    {
        sprint("Cannot read file.\n", -1, -1);
        return;
    }

    Elf32_Ehdr *hddr = (Elf32_Ehdr *)content;
    void* dst = (void*)temp_vma->vaddr;
    void* src = (void*)((uint8_t*)hddr + temp_vma->segment_offset);

    memcpy(dst, src, temp_vma->segment_size);
    memset((void*)temp_vma->bss_offset, 0, temp_vma->bss_size);



  

}

invlpg(vaddr);

}