#pragma once

#include <stdint.h>
#include <stdbool.h>


#define MAX_INTR 256
#define CURR_INTR 48


#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_IRQ 2







void sprint(char*, int, int);
void cls();


typedef struct  
{
    uint16_t offset_1; // offset address (0...15 bits)
    uint16_t selector; // code segment set in the gdt 
    uint8_t zero; // always zero (reserved)
    uint8_t type_attributes; 
    uint16_t offset_2; // offset address (16...31 bits)
    
}__attribute__((packed)) idt_t; // interrupt description


typedef struct {
    
    uint16_t limit; // size of IDT - 1 
    uint32_t base; //start address of IDT 

}__attribute__((packed)) idtr_t; // pointer to idt


void idt_set_descriptor(int, void*, uint8_t);
void init_idt();
void PIC_remap(int, int);


void handle_interrupt(int, int);

void handle_syscall(int, int);


void send_EOI(uint8_t);


