#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"



void main() {


    init_screen();
    init_idt();

    sprint("Hello, World!", -1, -1);

 

   



    
    
    
}

