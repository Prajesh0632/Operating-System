#pragma once 
#include <stdint.h>
#include "process32.h"
void start_user_program(Process_32*);
void load_user_program(char*, uint16_t);
