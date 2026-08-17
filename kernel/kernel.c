#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"
#include "../command_shell/shell.h"
#include "../memory/memory.h"



void main() {


    init_screen();
    init_idt();
    init_mem();

    uint64_t* addr = fralloc(4096);
    iprint((int)addr);
    sprint("\n",-1,-1);
    
    free(addr, 4096);


    uint64_t* addr1 = fralloc(4096);
    iprint((int)addr1);
    sprint("\n",-1,-1);
    
    

    
    sprint("kernel>", -1, -1);
    while(true) {
        asm volatile("hlt"); // sleep until next interrupt
    }

 

   



    
    
    
}

