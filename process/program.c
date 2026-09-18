#include "program.h"
#include "../memory/pmm.h"
#include "../memory/paging.h"
#include "../memory/elf_loader.h"
#include "../user_space/switch_user.h"
#include "../memory/vmm.h"

void start_user_program(Process_32* process) {
   
    vmm_map_page(process->sp, process->page_directory);
    uint32_t user_stack_top = process->sp;
    switch_user_mode(user_stack_top, process->ip);


}


void load_user_program(char* filename, uint16_t cluster) {

     Process_32* process = create_process();
    load_program(filename, cluster, process);

    if(process->ready){

        process->filename = "HELLO.ELF";
        process->dir_cluster = 0;
        current_process = process;
        start_user_program(process);

    } 

}