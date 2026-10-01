#include "timer.h"
#include "../port_io/io.h"
#include <stdint.h>


uint32_t kernel_time;

void init_time() {
    kernel_time = 0;
}

void update_clock() {

    kernel_time++;
}

uint32_t get_time_now() {

    

    return kernel_time;
}

