#include "grphc.h"
#include "vbe.h"
#include "font_renderer.h"

int abs(int x) {
    if(x < 0) return -x;
    return x;
}

void draw_line(int x_start, int y_start, int x_end, int y_end, Color color, uint32_t width) {
         
      int dx = abs(x_end - x_start);
    int dy = abs(y_end - y_start);
    int x_dir = (x_end >= x_start) ? 1 : -1;
    int y_dir = (y_end >= y_start) ? 1 : -1;

    int steep = dy > dx;
    if (steep) { int t = dx; dx = dy; dy = t; }

    int p = 2 * dy - dx;
    int x = x_start, y = y_start;

    for (int i = 0; i <= dx; i++) {
        put_pixel(x, y, color);

        if (p >= 0) {
            if (steep) x += x_dir; else y += y_dir;
            p -= 2 * dx;
        }
        if (steep) y += y_dir; else x += x_dir;
        p += 2 * dy;
    }
}





void draw_rect(int x_start, int y_start, int width, int height, Color color, uint32_t thickness) {
    int x_end = x_start + width;
    int y_end = y_start + height;

    draw_line(x_start, y_start, x_end, y_start, color, thickness);
    draw_line(x_start, y_end, x_end, y_end, color, thickness);
    draw_line(x_start, y_start, x_start, y_end, color, thickness);
    draw_line(x_end, y_start, x_end, y_end, color, thickness);
}



//filled rectangles
void draw_rect_fill(int x_start, int y_start, int width, int height, Color color, uint32_t thickness) {
    int x_end = x_start + width;
    int y_end = y_start + height;

    draw_line(x_start, y_end, x_end, y_end, color, thickness);
    draw_line(x_start, y_start, x_start, y_end, color, thickness);

    for(int h = 0; h < height; h++) {

        draw_line(x_start, y_start + h, x_end, y_start + h, color, thickness);

    }


}

void put_octant(int x_centre, int y_centre, int x0, int y0, Color color)  {

        put_pixel(x_centre + x0, y_centre + y0, color);
        put_pixel(x_centre - x0, y_centre - y0, color);
        put_pixel(x_centre + x0, y_centre - y0, color);
        put_pixel(x_centre - x0, y_centre + y0, color);
        put_pixel(x_centre + y0, y_centre + x0, color);
        put_pixel(x_centre - y0, y_centre - x0, color);
        put_pixel(x_centre + y0, y_centre - x0, color);
        put_pixel(x_centre - y0, y_centre + x0, color);

}

void draw_circle(int x_centre, int y_centre, int radius, Color color, uint32_t thickness) {
         
    int x0 = 0, y0 = radius;

    int p0 = 1 - radius;



    while(x0 <= y0) {

       put_octant(x_centre, y_centre, x0, y0, color);

       if(p0 < 0) {
        x0++;
        p0 = p0 + 2*x0 + 1;

       }

       else {
        x0++;
        y0--;
        p0 = p0 + 2*x0 - 2*y0 + 1;
       }

        

    }

}




void draw_circle_fill(int x_centre, int y_centre, int radius, Color color, uint32_t thickness) {
          
    int x0 = 0, y0 = radius;
    int p0 = 1 - radius;
    int cx = x_centre, cy = y_centre;
    while(x0 <= y0) {

        draw_line(cx - x0, cy + y0, cx + x0, cy + y0, color, 1);
        draw_line(cx - x0, cy - y0, cx + x0, cy - y0, color, 1);
        draw_line(cx - y0, cy + x0, cx + y0, cy + x0, color, 1);
        draw_line(cx - y0, cy - x0, cx + y0, cy - x0, color, 1);
       if(p0 < 0) {
        x0++;
        p0 = p0 + 2*x0 + 1;

       }

       else {
        x0++;
        y0--;
        p0 = p0 + 2*x0 - 2*y0 + 1;
       }

        

    }

}