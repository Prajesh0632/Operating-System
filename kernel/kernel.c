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
#include "../process/process_manager.h"
#include "../graphics/vbe.h"
#include "../graphics/font_renderer.h"
#include "../graphics/grphc.h"



uint32_t init_user_stack() {

     uint32_t user_stack = (uint32_t)fralloc_kernel(PAGE_SIZE * 2);
    uint32_t user_stack_top = user_stack + (2 * PAGE_SIZE);


    return user_stack_top;
}


void main() {


    init_screen();
    init_mem();
    init_heap();
    init_idt();
    init_paging();
    init_tss((uint32_t)(_kernel_end + 0x200000));
    init_fat16();
    init_graphics();

    draw_line(5, 5, 100, 100, COLOR_RED, 1);
    draw_rect_fill(50, 50, 100, 100, COLOR_GREEN, 1);
    draw_circle(500, 500, 100, COLOR_BLUE, 1);
    draw_circle_fill(200, 200, 100, COLOR_RED, 1);

    
   
    
     
    
    // Process_32* process = (Process_32*)halloc(sizeof(Process_32));

    // process->ip = (uint32_t)user_prog;
    // process->page_directory = page_directory;
    // process->sp = init_user_stack();
    // process->ready = true;

    // start_user_process(process, 0, 0);





   



    


    
}

