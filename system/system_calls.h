#pragma once

#include<stdint.h>

void sys_write(char* string);
char sys_read();
void sys_clear();
void sys_sclear();

void sys_flist(uint16_t);
void sys_fprint(uint16_t, char*);
void sys_fcreate(uint16_t, char*);
