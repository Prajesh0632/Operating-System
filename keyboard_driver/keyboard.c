#include "keyboard.h"
#include "../port_io/io.h"
#include "../screen_driver/screen.h"
#include "../command_shell/shell.h"



bool CAPS_LOCK;
bool SHIFT_PRESS;
bool input = false;

  
static volatile char ring_queue[KEY_MAX];
static volatile int head = 0;//only keyboard isr writes to it
static volatile int tail = 0;//only syscall for write can write to it


void keyboard_isr() {
    char c = get_pressed_char();
    if(c == 0) return;

    int next_head = (head + 1) % KEY_MAX;

    if (next_head != tail) {
        ring_queue[head] = c;
        head = next_head;
    }

    if(input) {

     char s[2] = {c, '\0'};
     sprint(s, -1, -1);


    }
   
    
  
    
}



void clear_ring() {
    __asm__ volatile("cli");
    head = 0;
    tail = 0;
    __asm__ volatile("sti");

}


char key_dequeue() {
    
    if(tail == head) return '\0';
    char c = ring_queue[tail];
    tail = (tail + 1) % KEY_MAX;

    return c;

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



                char key = !press ? lower_keyboard_map[key_code] : upper_keyboard_map[key_code];
                return key;
            
            

            }


            else {
                if(key_code == 0xAA|| key_code == 0xB6) {
                    SHIFT_PRESS = false;
                }

                return 0;
            }
}