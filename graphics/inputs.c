#include "inputs.h"
#include "../keyboard_driver/keyboard.h"
#include <stdint.h>

// char keyboard_buffer_grpcs[64];
// uint32_t head_g = 0;
// uint32_t tail_g = 0;

char read_keystroke() {
       
    char c = get_pressed_unpressed();
    return c;

}
