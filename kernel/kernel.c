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
#include "../graphics/inputs.h"
#include "../keyboard_driver/scancodes.h"
#include "../graphics/bitmap.h"



uint32_t init_user_stack() {

     uint32_t user_stack = (uint32_t)fralloc_kernel(PAGE_SIZE *4);
    uint32_t user_stack_top = user_stack + (4 * PAGE_SIZE);


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
    init_pit(1000);



    
//     int x = 50, y = 50;
//     int dir = 0;

//     while(true) {

//     int speed = 3;
//     if(is_pressed(SC_A) && x > 50) x -= speed;
//     if(is_pressed(SC_D) && x < 800) x += speed;
//     if(is_pressed(SC_W) && y > 50) y -= speed;
//     if(is_pressed(SC_S) && y < 600) y += speed;


//    clear_scr();
//     // draw_line(5, 5, 100, 100, COLOR_RED);
//     draw_rect_fill(x, y, 100, 100, COLOR_GREEN);
//     // draw_circle(500, 500, 100, COLOR_BLUE);
//     // draw_circle_fill(200, 200, 100, COLOR_RED);

    
//    if(update_screen) {present(); update_screen = false;}
    
   

//     }

//     write_text("Program Exited\n");


init_image("IMAGE.BMP", 0);
present();

   
    
     
    
    // Process_32* process = (Process_32*)halloc(sizeof(Process_32));

    // process->ip = (uint32_t)user_prog;
    // process->page_directory = page_directory;
    // process->sp = init_user_stack();
    // process->ready = true;

    // start_user_process(process, 0, 0);





   



    


    
}

