#include "memory.h"


void memcpy(void* destination, void* source, uint32_t mem_size) {

    uint8_t* src = (uint8_t*)source;
    uint8_t* dst = (uint8_t*)destination;

    for(uint32_t i = 0; i < mem_size; i++) {
             
        dst[i] = src[i];
    }
    

}

void memset(void* destination, uint8_t value, uint32_t mem_size) {

    uint8_t* dst = (uint8_t*)destination;

    for(uint32_t i = 0; i < mem_size; i++) {
        dst[i] = value;
    }
}