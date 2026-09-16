#pragma once 
#include "process32.h"

extern Process_32* running_process;


typedef struct ProcessQueue
{

    Process_32* front;
    Process_32* rear;
    

    
}ProcessQueue;


bool create_proc();



