#pragma once 
#include <stdint.h>
#include "colors.h"


void draw_line(int, int, int, int, Color, uint32_t);
void draw_rect(int, int, int, int, Color, uint32_t);
void draw_rect_fill(int, int, int, int, Color, uint32_t);
void draw_circle(int x, int y, int, Color, uint32_t);
void draw_circle_fill(int x, int y, int, Color, uint32_t);