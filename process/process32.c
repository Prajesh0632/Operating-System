#include "process32.h"
#include "../memory/pmm.h"
#include "../memory/paging.h"
#include "../memory/heap.h"


Process_32* current_process = NULL;

Process_32* create_process() {

   
    Process_32* process = (Process_32*)halloc(sizeof(Process_32));
    process->pid = 100;
    // for(uint32_t i = 0; i < 1024; i++) process->page_directory[i] = page_directory[i];

    for(uint32_t i = 768; i < 1024; i++) process->page_directory[i] = page_directory[i];

    loadPageDirectory(process->page_directory);


    process->ready = false;
    return process;

    
}