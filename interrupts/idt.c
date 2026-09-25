#include "idt.h"
#include "../port_io/io.h"
#include "../keyboard_driver/keyboard.h"
#include "../screen_driver/screen.h"
#include "../memory/paging.h"
#include "../system/system_header.h"
#include "../file_system/fat16.h"
#include "../memory/vmm.h"
#include "../process/process_manager.h"
#include "../graphics/font_renderer.h"
#include "../graphics/colors.h"

bool clear = false;
int counter = 4;

idt_t interrupts[MAX_INTR];
static idtr_t idtr;

static bool vectors[MAX_INTR];


extern void isr128();
extern void* isr_stub_table[];


static inline uint16_t get_code_segment(void) {
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs;
}

void init_idt() {
    idtr.base = (uintptr_t)(&interrupts[0]);
    idtr.limit = (uint16_t)(sizeof(idt_t) * MAX_INTR - 1);


    
    for (int vector = 0; vector < CURR_INTR; vector++) {
            idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;

        
    }

    idt_set_descriptor(0x80, isr128, 0xEE);
    

    PIC_remap(0x20, 0x28);   // remap IRQ0-7 -> 32-39, IRQ8-15 -> 40-47


    __asm__ volatile ("lidt %0" : : "m"(idtr));
    __asm__ volatile ("sti");

}


void idt_set_descriptor(int vector, void* isr, uint8_t flags) {
 
    idt_t* interrupt = &interrupts[vector];

    interrupt->offset_1 = (uint32_t)isr & 0xFFFF;
    interrupt->selector = get_code_segment();
    interrupt->zero = 0;
    interrupt->type_attributes = flags;
    interrupt->offset_2 = (uint32_t)isr >> 16;


}


void handle_interrupt(int vector, int error_code) {
    
        if(vector == 0) sprint("Division by zero occured", -1, -1);

       else if(vector == 14) {
       
        uint32_t pagefault_addr =  (uint32_t)readCR2();
        vmm_handle_pagefault(pagefault_addr);
    }

        if(vector == 33) {

            
           keyboard_isr();
            


           
        } 

        if(vector == 32) {
          
          clear = true;
          
        }



        if(vector >= 32 && vector <= 47) send_EOI(vector - 32);


}


void init_pit(uint32_t frequency) {
     // 1. Calculate the 16-bit divisor
    uint32_t divisor = 1193182 / frequency;
    
    // Clamp the divisor if it exceeds 16-bit bounds
    if (divisor > 0xFFFF) divisor = 0xFFFF;
    if (divisor < 1)      divisor = 1;

    // 2. Send the Command Byte to Port 0x43
    // Bits 7-6: 00  (Select Channel 0)
    // Bits 5-4: 11  (Access mode: Lobyte/Hibyte)
    // Bits 3-1: 011 (Mode 3: Square Wave Generator)
    // Bit 0   : 0   (Binary counter)
    // 00110110b = 0x36
    port_byte_out(0x43, 0x36);

    // 3. Send the Divisor to Port 0x40 (Low byte first, then High byte)
    port_byte_out(0x40, (uint8_t)(divisor & 0xFF));        // Low byte
    port_byte_out(0x40, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
}


void handle_syscall(int type, int value, int extra, int extra1, uint32_t* regs) {


   switch(type) {

   case SYS_WRITE:
       
        write_text((char*)value);
        break;

   case SYS_READ: {
        char *c = (char*)(value);
        *c = key_dequeue();
        break;
   }

   case SYS_CLEAR:
        clear_ring();
        break;

   case SYS_SCLEAR:
        clear_scr();
        break;

   case SYS_FLIST:
        list_dir(value);
        break;
    
   case SYS_FFIND:
        bool* found = (bool*)extra1;
        *found = ffind((char*)value, (uint16_t)extra);
        break;


   case SYS_FPRINT:
        print_file((char*)value, (uint16_t)extra);
        break;

   case SYS_FCREATE:
        create_file((char*)value, (uint16_t)extra);
        break;

   case SYS_FDELETE:
        delete_file((char*)value, (uint16_t)extra);
        break;

   case SYS_FCD: {
        uint16_t *new_extra = (uint16_t*)extra;
        *new_extra = change_dir((char*)value, *new_extra);
        break;
   }

   case SYS_FMKDIR:
        make_dir((char*)value, (uint16_t)extra);
        break;

    case SYS_FWRITE:
        write_file((char*)value, (char*)extra1, (uint16_t)extra);
        break;

     
     case SYS_LOAD_PROGRAM:
         
        uint32_t current_user_eip = regs[8];
        uint32_t current_user_esp = regs[11];
        load_user_process((char*)value, (uint16_t)extra, current_user_eip, current_user_esp);
        break;

     case SYS_EXIT:
        exit_proc();
        break;

         

    

   }

        


}


void send_EOI(uint8_t irq) {

     
 
    if(irq >= 8)
		port_byte_out(PIC2_COMMAND,PIC_EOI);
	
	port_byte_out(PIC1_COMMAND,PIC_EOI);

}



/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	port_byte_out(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	port_byte_out(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	port_byte_out(PIC1_DATA, 1 << CASCADE_IRQ);        // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	io_wait();
	port_byte_out(PIC2_DATA, CASCADE_IRQ);             // ICW3: tell Slave PIC its cascade identity
	io_wait();
	
	port_byte_out(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	port_byte_out(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	port_byte_out(PIC1_DATA, 0);
	port_byte_out(PIC2_DATA, 0);
}