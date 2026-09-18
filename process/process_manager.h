#pragma once 
#include "process32.h"

extern Process_32* running_process;


typedef struct ProcessQueue
{

    Process_32* front;
    Process_32* rear;
    

    
}ProcessQueue;


Process_32* create_proc();
void load_user_process(char*, uint16_t);
void start_user_process(Process_32*);
void exit_proc();



