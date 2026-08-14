#include "idt.h"



idt_t interrupts[MAX_INTR];
static idtr_t idtr;

static bool vectors[MAX_INTR];


extern void* isr_stub_table[];

static inline uint16_t get_code_segment(void) {
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs;
}

void init_idt() {
    idtr.base = (uintptr_t)(&interrupts[0]);
    idtr.limit = (uint16_t)(sizeof(idt_t) * MAX_INTR - 1);

    for (int vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
    // __asm__ volatile ("sti");
        sprint("IDT loaded\n", 3, 0);

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
  
    sprint("Exception", -1, -1);

  
}