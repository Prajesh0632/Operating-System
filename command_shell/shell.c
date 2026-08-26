#include "../system/system_calls.h"
#include "shell.h"

char shell_buffer[128];
int shell_index = 0;
int shell_input_count = 0;


int strcmp(const char* s1, const char* s2) {
   
    while(*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    
    return (unsigned char)*s1 - (unsigned char)*s2;


}


void shell_main() {

    sys_write("User> ");

    bool exit = false;
    while(!exit) {

    char c = sys_read();
    if(c == '\0') continue;
    
    if(c == '\b') {
        if(shell_input_count == 0) continue;
        shell_input_count--;
    }

    char s[2] = {c, '\0'};
    sys_write(s);
    

    if(c == '\n') {
        shell_buffer[shell_index] = '\0';
        exit =  execute_command(shell_buffer);

        shell_index = 0;
        shell_input_count = 0;
        if(exit) break;
        
    }

    else if(c == '\b') {
        
        if(shell_index != 0) shell_index--;
    }

    else {
        shell_buffer[shell_index++] = c;
        shell_input_count++;
    }
    


    }
    
   


}



bool execute_command(char* command) {

    if(strcmp(command, "clear") == 0) 
    {
        sys_sclear();
    }

    else if(strcmp(command, "-help") == 0) {

        sys_write("clear : clears the screen\n");
        sys_write("exit  : exit the command line\n");
    }

    else if(strcmp(command, "exit") == 0) {
        sys_write("Shell Successfully exited\n");
        return true;

    }

  

    else {

        sys_write("No command Found for ");
        sys_write(command);
        sys_write("\n");
        sys_write("Use -help command for more information.\n");
    }

    sys_write("User> ");

    return false;

}