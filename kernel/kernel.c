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

void start_user_program(uint32_t entry_point) {
   
    uint32_t user_stack = fralloc(PAGE_SIZE * 2);
    uint32_t frame = user_stack - PAGE_SIZE;
    guard_frame(frame / PAGE_SIZE);

    uint32_t user_stack_top = user_stack + (2 * PAGE_SIZE);
    switch_user_mode(user_stack_top, entry_point);


}



void main() {


    init_screen();
    init_mem();
    init_heap();
    init_idt();
    init_paging();
    init_tss((uint32_t)(_kernel_end + 0x200000));
    init_fat16();
    Process_32* process = (Process_32*)halloc(sizeof(Process_32));
    process->pid = 100;
    process->page_directory = (uint32_t*)fralloc(PAGE_SIZE);
    elf_inspect("HELLO.ELF", 0, process);

     
   




    // start_user_program((uint32_t)user_prog);

    


    
}

