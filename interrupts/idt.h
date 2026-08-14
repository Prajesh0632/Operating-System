#include <stdint.h>
#include <stdbool.h>
#define MAX_INTR 256

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
 
    uint32_t base; //start address of IDT 
    uint16_t limit; // size of IDT - 1 

}__attribute__((packed)) idtr_t; // pointer to idt


void init_idt();

void idt_set_descriptor(int, void*, uint8_t);

void handle_interrupt(int, int);