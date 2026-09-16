#include "process_manager.h"
#include "../memory/pmm.h"
#include "../memory/paging.h"
#include <stdbool.h>

Process_32* running_process = NULL;
uint32_t process_id = 0;


bool create_proc() {

    if(!current_process) return;

     
    Process_32* process = (Process_32*)halloc(sizeof(Process_32));
    process->pid = 100;
    for(uint32_t i = 0; i < 1024; i++) process->page_directory[i] = current_process->page_directory[i];

    // for(uint32_t i = 768; i < 1024; i++) process->page_directory[i] = page_directory[i];

    loadPageDirectory(process->page_directory);


    process->ready = false;
    return process;

}


void process_exit() {
     
    Process_32* process = running_process;
    uint32_t* pd = process->page_directory;

    for(uint32_t i = 0; i < 1024; i++) {

        if (!(pd[i] & PDE_PRESENT)) continue;
        uint32_t *page_table = (uint32_t*)(pd[i] & 0xFFFFF000);

        for(uint32_t j = 0; j < 1024; j++) {
           
             if (!(page_table[j] & PTE_PRESENT)) continue;
            uint32_t physical_addr = page_table[j] & 0xFFFFF000;
            free((uint64_t*)(uintptr_t)physical_addr, PAGE_SIZE);
        }
    }



}