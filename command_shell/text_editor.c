#include "text_editor.h"
#include "../system/system_calls.h"
#include <stdbool.h>




int text_editor(char* text_buffer) {
    int txt_idx = 0;
    sys_write("                      ----------TEXT EDITOR----------\n                    After writing press ctrl + s to save.\n");
    bool exit = false;
    sys_clear();

    while(!exit) {

        char c = sys_read();

        if((int)c == 0x13) { // save file 

            text_buffer[txt_idx] = '\0';
            break;

        }

        if(c == '\0' || txt_idx >= 1024) continue;


       
        
        if(c == '\b') {
            
            if(txt_idx == 0) continue;
            txt_idx--;
            text_buffer[txt_idx] = ' ';
        }



       else {

        text_buffer[txt_idx++] = c;

       }
        char s[2] = {c, '\0'};
        sys_write(s);
        
        

    }


    return txt_idx - 1;
    

}