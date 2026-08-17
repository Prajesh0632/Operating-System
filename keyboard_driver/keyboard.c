#include "keyboard.h"
#include "../port_io/io.h"
#include "../screen_driver/screen.h"
#include "../command_shell/shell.h"



bool CAPS_LOCK;
bool SHIFT_PRESS;
char input_buffer[256];
int buffer_index;



bool handle_keyboard() {

    char c = get_pressed_char();
    if(c == 0) return false;

    if(c == '\b') {
       
        if(buffer_index > 0) {
            buffer_index--;
            input_buffer[buffer_index] = '\0';
            
            sprint("\b\0", -1, -1);
        }

        


    }

    else if(c == '\n') {
          input_buffer[buffer_index] = '\0';
          buffer_index = 0;

          sprint("\n\0", -1, -1);


          
          execute_command(input_buffer);
          sprint("kernel>",-1,-1);



         return true;

    }
    
    else if(buffer_index < BUFFER_SIZE - 1) {
        input_buffer[buffer_index++] = c;
        
        char s[2] = {c, '\0'};
        sprint(s, -1, -1);
    }


    return false;
    

    

}

char get_pressed_char() {
    uint8_t key_code = port_byte_in(0x60);

            if(!(key_code & 0x80)) {

             
                if(key_code == 0x2A || key_code == 0x36) {
                    SHIFT_PRESS = true;
                    return 0;
                }

                if(key_code == 0x3A) {
                    CAPS_LOCK = !CAPS_LOCK;
                    return 0;
                }

                bool press = SHIFT_PRESS ^ CAPS_LOCK;



             
                return !press ? lower_keyboard_map[key_code] : upper_keyboard_map[key_code];
            
            

            }


            else {
                if(key_code == 0xAA|| key_code == 0xB6) {
                    SHIFT_PRESS = false;
                }

                return 0;
            }
}