#pragma once

#include<stdint.h>
#include <stdbool.h>

void sys_write(char* string);
char sys_read();
void sys_clear();
void sys_sclear();

void sys_flist(uint16_t);
bool sys_ffind(uint16_t, char*);
void sys_fprint(uint16_t, char*);
void sys_fcreate(uint16_t, char*);
void sys_fdelete(uint16_t, char*);
uint16_t sys_fcd(uint16_t, char*);
void sys_fmkdir(uint16_t, char*);
void sys_fwrite(uint16_t, char*, char*);

