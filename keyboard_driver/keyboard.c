#include "keyboard.h"
#include "../port_IO/io.h"
#include "../screen_driver/screen.h"

bool CAPS_LOCK = false;
bool SHIFT_PRESS = false;

void handle_keyboard() {
    uint8_t key_code = port_byte_in(0x60);

            if(!(key_code & 0x80)) {

             
                if(key_code == 0x2A || key_code == 0x36) {
                    SHIFT_PRESS = true;
                    return;
                }

                if(key_code == 0x3A) {
                    CAPS_LOCK = !CAPS_LOCK;
                    return;
                }

                bool press = SHIFT_PRESS ^ CAPS_LOCK;



             
            char key;

            if(press){
                key = upper_keyboard_map[key_code];

            } 
            else  {
                key = lower_keyboard_map[key_code];
            }

           
            char c[2] = {key, '\0'};
            sprint(c, -1, -1);

            
   
            

            }


            else {
                if(key_code == 0xAA|| key_code == 0xB6) {
                    SHIFT_PRESS = false;
                    return;
                }
            }
}