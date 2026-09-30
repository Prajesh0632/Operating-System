#pragma once 
#include <stdint.h>
#include <stdbool.h>


typedef struct Window
{
    uint32_t width;
    uint32_t height;
    int win_x_pos, win_y_pos;
    int x_pos, y_pos;
    uint32_t x_offset, y_offset;
} Window __attribute__((packed));

bool create_window(uint32_t, uint32_t, int, int);
void draw_win();



