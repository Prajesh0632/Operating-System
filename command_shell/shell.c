#include "shell.h"
#include "../screen_driver/screen.h"


int strcmp(const char* s1, const char* s2) {
   
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    
    return (unsigned char)*s1 - (unsigned char)*s2;


}


void execute_command(char* command) {

    if(strcmp(command, "clear") == 0) cls();

}