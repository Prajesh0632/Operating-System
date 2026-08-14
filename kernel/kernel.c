#include "screen.h"
#include "../interrupts/idt.h"



void main() {


    init_screen();
    init_idt();

    asm volatile("int $2");
 

    sprint("Success", 1, 0);
   



    
    
    
}

