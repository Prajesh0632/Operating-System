#pragma once

#include <stdint.h>

void port_byte_out(uint16_t, uint8_t);
uint8_t port_byte_in(uint16_t);
void port_word_out(uint16_t, uint16_t);
uint16_t port_word_in(uint16_t);
void io_wait(void);


