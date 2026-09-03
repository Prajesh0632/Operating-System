#include "../system/system_calls.h"
#include "../headers/string/str.h"
#include "shell.h"
#include <stddef.h>
#include <stdint.h>


char shell_buffer[128];
int shell_index = 0;
int shell_input_count = 0;



char* current_directory = "root>";
uint16_t current_cluster = 0;

void shell_main() {

    sys_write(current_directory);

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

    char* args = NULL;
    
    for(int i = 0; command[i] != '\0'; i++) {

        if(command[i] == ' ') {
            command[i] = '\0';
            
            for(int j = 0; command[i + j + 1] != '\0'; j++) {
                if(command[i+j+1] != ' ') {
                     args = &command[i+j+1];
                     break;
                }
            }

            break;
        }
          
    }


   

    if(strcmp(command, "clear") == 0) 
    {
        sys_sclear();
    }

    else if(strcmp(command, "-help") == 0) {

        sys_write("ls : lists all files and directories within the current directory\n");
        sys_write("cat <filename> : print a file within the current directory\n");
        sys_write("touch <filename> : create an empty file within the current directory\n");
        sys_write("delete <filename> : delete the file within the current directory\n");




        sys_write("clear : clears the screen\n");
        sys_write("exit  : exit the command line\n");

    }


    else if(strcmp(command, "ls") == 0) {
         
        sys_flist(current_cluster);

    }

    else if(strcmp(command, "exit") == 0) {
        sys_write("Shell Successfully exited\n");
        return true;

    }

    else if(strcmp(command, "cat") == 0) {

        if(args == NULL){
         sys_write("No filename provided\n");
         sys_write(current_directory);
         return false;


        }

        sys_fprint(current_cluster, args);
    }


    else if(strcmp(command, "touch") == 0) {

        if(args == NULL){
         sys_write("Provide a filename to create\n");
         sys_write(current_directory);
         return false;


        }

        sys_fcreate(current_cluster, args);



    }

    else if(strcmp(command, "delete") == 0) {

        if(args == NULL){
         sys_write("Provide a filename to delete\n");
         sys_write(current_directory);
         return false;


        }

        sys_fdelete(current_cluster, args);

    }




    else {
         

          


        sys_write("No command Found for ");
        sys_write(command);
        sys_write("\n");
        sys_write("Use -help command for more information.\n");
    }

    sys_write(current_directory);

    return false;

}