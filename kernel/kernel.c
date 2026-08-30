#include "../screen_driver/screen.h"
#include "../interrupts/idt.h"
#include "../command_shell/shell.h"
#include "../memory/pmm.h"
#include "../memory/heap.h"
#include "../memory/paging.h"
#include "../descriptors/tss.h"
#include "../user_space/user_program.h"
#include "../user_space/switch_user.h"
#include "../file_system/ata.h"

void start_user_program(uint32_t entry_point) {
   
    uint32_t user_stack = fralloc(PAGE_SIZE);
    uint32_t frame = user_stack - PAGE_SIZE;
    guard_frame(frame / PAGE_SIZE);

    uint32_t user_stack_top = user_stack + PAGE_SIZE;
    switch_user_mode(user_stack_top, entry_point);


}



void main() {


    init_screen();
    init_mem();
    init_mem();
    init_heap();
    init_idt();
    init_paging();
    init_tss((uint32_t)(_kernel_end + 0x200000));
     
    uint32_t lba = 0;
    uint16_t* buffer = halloc(256 * 16);
    for(int i = 0; i < 256; i++) {
        buffer[i] = i;
    }
    // ata_write_sector(lba, buffer);

    ata_read_sector(lba, buffer);

    for(int i = 0; i < 256; i++) {

        iprint((int)buffer[i]);
        sprint(" ",-1,-1);
    }




    // start_user_program((uint32_t)user_prog);

    


    
}

