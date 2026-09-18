#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"
#include "../command_shell/shell.h"
#include "../memory/pmm.h"
#include "../memory/heap.h"
#include "../memory/paging.h"
#include "../descriptors/tss.h"
#include "../user_space/user_program.h"
#include "../user_space/switch_user.h"
#include "../file_system/fat16.h"
#include "../memory/elf_loader.h"
#include "../process/process32.h"
#include "../process/program.h"



void init_user(uint32_t user_entry) {

     uint32_t user_stack = (uint32_t)fralloc_kernel(PAGE_SIZE * 2);
    uint32_t user_stack_top = user_stack + (2 * PAGE_SIZE);

    switch_user_mode(user_stack_top, user_entry);

}


void main() {


    init_screen();
    init_mem();
    init_heap();
    init_idt();
    init_paging();
    init_tss((uint32_t)(_kernel_end + 0x200000));
    init_fat16();
   
    
     
   



    init_user((uint32_t)user_prog);


   



    


    
}

