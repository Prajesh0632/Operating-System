#include "inputs.h"
#include "../keyboard_driver/keyboard.h"
#include <stdint.h>
#include <stdbool.h>

char is_pressed(Scancode code) {
       
    return key_down[code];
    

}
