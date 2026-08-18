#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"
#include "../command_shell/shell.h"
#include "../memory/pmm.h"
#include "../memory/heap.h"



void main() {


    init_screen();
    init_mem();
    init_heap();
    init_idt();
    

    uint64_t addr = halloc(0, 2);
    iprint((int)addr);
    sprint("\n", -1, -1);

    uint64_t addr1 = halloc(0, 2);
    iprint((int)addr1);
    sprint("\n", -1, -1);
    

    
    
    

    
    sprint("kernel>", -1, -1);
    while(true) {
        asm volatile("hlt"); // sleep until next interrupt
    }

 

   



    
    
    
}

