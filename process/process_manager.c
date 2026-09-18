#include "process_manager.h"
#include "../memory/pmm.h"
#include "../memory/paging.h"
#include "../user_space/switch_user.h"
#include "../memory/heap.h"
#include "../memory/elf_loader.h"
#include "../memory/vmm.h"
#include <stdbool.h>

Process_32* running_process = NULL;
uint32_t process_id = 0;




Process_32* create_proc() {

    // if(!current_process) return NULL;

     
    Process_32* process = (Process_32*)halloc(sizeof(Process_32));
    process->pid = 100;
    process->page_directory = (uint32_t*)fralloc_kernel(1024 * 4);

    // fralloc'd memory isn't zeroed -- entries we don't explicitly set below
    // must be marked not-present ourselves, or they hold whatever garbage
    // was left in that frame (which could easily have PDE_PRESENT set by
    // coincidence, mapping user space into unrelated physical memory).
    for(uint32_t i = 0; i < 1024; i++) process->page_directory[i] = 0x00000002;

    // Only the shared kernel window -- isolation: low/user addresses stay
    // unmapped in this directory until this process's own code faults them
    // in via map_page/vmm_map_page.
    //
    // page_directory[i] itself keeps PDE_USER set (the shell needs that,
    // running at CPL=3 under the kernel's OWN directory -- see paging.c).
    // But x86 ANDs the PDE and PTE permission bits together, so clearing
    // PDE_USER on just THIS process's copy is enough to make the whole
    // window supervisor-only when reached through process->page_directory,
    // without touching the kernel's own mapping (or the shell) at all.
    for(uint32_t i = 768; i < 1024; i++) process->page_directory[i] = page_directory[i] & ~PDE_USER;

    // Exception: the GDT and TSS (bootloader-placed near 0x7000-0x7FFF,
    // pd_idx 0) aren't part of the kernel's high-linked image, so they
    // aren't covered by the mirror above -- but the CPU reads them itself
    // on every segment load and ring transition, regardless of which
    // process is running, so this entry must stay present everywhere too.
    // Descriptor-table accesses are always treated as supervisor by the
    // CPU regardless of current CPL, so masking USER off here doesn't
    // break the ring transition -- and it means this process can't read
    // the GDT/TSS as ordinary data either.
    process->page_directory[0] = page_directory[0] & ~PDE_USER;

    loadPageDirectory((uint32_t*)virt_to_phys(process->page_directory));


    process->ready = false;
    return process;

}


void load_user_process(char* filename, uint16_t cluster) {

     Process_32* process = create_proc();
     if(process == NULL) return;
    load_program(filename, cluster, process);

    if(process->ready){

        uint32_t i = 0;
        for(i; filename[i] != '\0'; i++) process->filename[i] = filename[i];
        process->filename[i] = '\0';
        process->dir_cluster = 0;
        current_process = process;
        start_user_process(process);

    } 

}

void start_user_process(Process_32* process) {
   
  
    uint32_t user_stack_top = process->sp;
    switch_user_mode(user_stack_top, process->ip);

}








void exit_process() {
     
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