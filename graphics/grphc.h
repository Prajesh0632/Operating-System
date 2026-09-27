#pragma once 
#include <stdint.h>
#include "polygon.h"

typedef enum {
    OUTSIDE,
    INSIDE,
    VERTEX_A,
    VERTEX_B,
    VERTEX_C,
    EDGE_AB,
    EDGE_BC,
    EDGE_AC,
} PointLocation;

void draw_line(int, int, int, int, Color);
void draw_rect(int, int, int, int, Color);
void draw_rect_fill(int, int, int, int, Color);
void draw_circle(int x, int y, int, Color);
void draw_circle_fill(int x, int y, int, Color);


void draw_triangle(Triangle*, uint32_t);
void scanline(Triangle*, int, int, int, int);



PointLocation is_point_inside(int, int, Triangle*, float*, float*, float*);