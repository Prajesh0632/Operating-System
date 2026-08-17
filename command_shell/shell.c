#include "shell.h"
#include "../screen_driver/screen.h"
#include "../memory/memory.h"

bool SHELL_ACTIVE = true;
volatile bool executed = true;



int strcmp(const char* s1, const char* s2) {
   
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    
    return (unsigned char)*s1 - (unsigned char)*s2;


}




void execute_command(char* command) {

    if(strcmp(command, "clear") == 0) 
    {
        cls();
    }

    else if(strcmp(command, "avail") == 0) {

        sprint("Bitmap Size : ", -1, -1);
        iprint(frames);
        sprint("\n", -1, -1);

    }

    else {

        sprint("No command Found for ", -1, -1);
        sprint(command, -1, -1);
        sprint("\n", -1,-1);
    }

}