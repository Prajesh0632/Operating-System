#pragma once
#include<stdint.h>



typedef struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;


static const Color COLOR_RED   = {255, 0, 0};
static const Color COLOR_GREEN = {0, 255, 0};
static const Color COLOR_BLUE  = {0, 0, 255};
static const Color COLOR_WHITE = {255, 255, 255};
static const Color COLOR_BLACK = {0, 0, 0};