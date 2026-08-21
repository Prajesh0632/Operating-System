#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"
#include "../command_shell/shell.h"
#include "../memory/pmm.h"
#include "../memory/heap.h"
#include "../memory/paging.h"
#include "../descriptors/tss.h"



void main() {


    init_screen();
    init_mem();
    init_mem();
    init_heap();
    init_idt();
    init_paging();
    init_tss((uint32_t)(_kernel_end + 0x100000));

    sprint("Paging Enabled\n", -1, -1);
  

    char* c = (char*)halloc(sizeof(char));
      
    iprint((int)c);
    sprint("\n", -1, -1);
    *c = 'A';

    cprint(*c);
    sprint("\n", -1, -1);


   


    hfree(c);

    

    char* a = (char*)halloc(sizeof(char));

    cprint(*a);
    sprint("\n", -1, -1);

    *a = 'b';

     cprint(*c);
    sprint("\n", -1, -1);



   


    





    

    
    
    

    
    sprint("kernel>", -1, -1);
    while(true) {
        asm volatile("hlt"); // sleep until next interrupt
    }

 

   



    
    
    
}

