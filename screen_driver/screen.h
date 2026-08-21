#pragma once

#include <stdint.h>


#define MAX_COL 80
#define MAX_ROW 25 
#define VGA_ADDRESS 0xb8000
#define WHITE 0x0F
#define BLACK 0x00
#define INDEX_PORT 0x3D4
#define DATA_PORT 0x3D5 





int get_offset(int, int);
void handle_scroll(volatile char*);
void cls();
void sprint(char*, int, int);
void iprint(int);
void cprint(char);
void hprint(uint32_t);
void write_char(volatile char*, int, char, int);
void set_cursor_offset(int, int);
int get_cursor_offset();
void init_screen(void);


