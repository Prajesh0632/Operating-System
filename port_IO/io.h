#include<stdint.h>

void port_byte_out(uint16_t, uint8_t);
uint8_t port_byte_in(uint16_t);
void io_wait(void);



void port_byte_out(uint16_t port, uint8_t data) {
    
    __asm__ __volatile__(
        "outb %0, %1"
        ://no output
        :"a"(data) , "d"(port));

}

uint8_t port_byte_in(uint16_t port) {


    uint8_t result;

    __asm__ __volatile__(
        "inb %1, %0"
        :"=a"(result)
        :"d"(port)

    );

    return result;
}


void io_wait() {
    port_byte_out(0x80, 0);
}
