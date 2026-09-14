#include "program.h"
#include "../memory/pmm.h"
#include "../memory/paging.h"
#include "../memory/elf_loader.h"
#include "../process/process32.h"
#include "../user_space/switch_user.h"

void start_user_program(uint32_t entry_point) {
   
    uint32_t user_stack = fralloc(PAGE_SIZE * 2);
    uint32_t frame = user_stack - PAGE_SIZE;
    guard_frame(frame / PAGE_SIZE);

    uint32_t user_stack_top = user_stack + (2 * PAGE_SIZE);
    switch_user_mode(user_stack_top, entry_point);


}


void load_user_program(char* filename, uint16_t cluster) {

     Process_32* process = create_process();
    load_program(filename, cluster, process);

    if(process->ready){

        process->filename = "HELLO.ELF";
        process->dir_cluster = 0;
        current_process = process;
        start_user_program(process->ip);

    } 

}