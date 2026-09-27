#include "grphc.h"
#include "vbe.h"
#include "font_renderer.h"

int abs(int x) {
    if(x < 0) return -x;
    return x;
}

float fabs(float x) {
    if(x < 0) return -x;
    return x;
}

int min(int x, int y) {
    return (x > y) ? y : x;
}

int max(int x, int y) {
    return (x > y) ? x : y;
}

void draw_line(int x_start, int y_start, int x_end, int y_end, Color color) {
         
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





void draw_rect(int x_start, int y_start, int width, int height, Color color) {
    int x_end = x_start + width;
    int y_end = y_start + height;

    draw_line(x_start, y_start, x_end, y_start, color);
    draw_line(x_start, y_end, x_end, y_end, color);
    draw_line(x_start, y_start, x_start, y_end, color);
    draw_line(x_end, y_start, x_end, y_end, color);
}



//filled rectangles
void draw_rect_fill(int x_start, int y_start, int width, int height, Color color) {
    int x_end = x_start + width;
    int y_end = y_start + height;

    draw_line(x_start, y_end, x_end, y_end, color);
    draw_line(x_start, y_start, x_start, y_end, color);

    for(int h = 0; h < height; h++) {

        draw_line(x_start, y_start + h, x_end, y_start + h, color);

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

void draw_circle(int x_centre, int y_centre, int radius, Color color) {
         
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




void draw_circle_fill(int x_centre, int y_centre, int radius, Color color) {
          
    int x0 = 0, y0 = radius;
    int p0 = 1 - radius;
    int cx = x_centre, cy = y_centre;
    while(x0 <= y0) {

        draw_line(cx - x0, cy + y0, cx + x0, cy + y0, color);
        draw_line(cx - x0, cy - y0, cx + x0, cy - y0, color);
        draw_line(cx - y0, cy + x0, cx + y0, cy + x0, color);
        draw_line(cx - y0, cy - x0, cx + y0, cy - x0, color);
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





/*
Setup the barycentric coordinates
For a triangle with vertices A(x1, y1, z1), B(x2, y2, z2), C(x3, y3, z3) and a point P(x, y, z).
aA + bB + cC = P 
For each coordinates: 
ax1 + bx2 + cx3 = x ------- (1)
ay1 + by2 + cy3 = y ------- (2)
az1 + bz2 + cz3 = z ------- (3)
a + b + c = 1 ------------- (4)

For a xy-plane z = 0 so only considering eqn 1, 2 and 4
ax1 + bx2 + cx3 = x ------- (1)
ay1 + by2 + cy3 = y ------- (2)
a + b + c = 1 ------------- (4)

Using cramer's rule 
D = (y2 - y3)(x1 - x3) + (x3 - x2)(y1 - y3)

a = ((y2 - y3)(x - x3) + (x3 - x2)(y - y3)) / D 
b = ((y3 - y1)(x - x3) + (x1 - x3)(y - y3)) / D 
c = 1 - b - a



It checks whether a point is inside a triangle or not
Also gives the weight for colors interpolation from each vertex

*/


void draw_triangle(Triangle *triangles, uint32_t n) {


    

      

    for(uint32_t i = 0; i < n; i++) {
            
        Triangle* t = &triangles[i];

        //setup the max and min coordinates for the rectangle boundary for the scanline
        int min_x = min(t->x1, t->x2);
        min_x = min(min_x, t->x3);
        int max_x = max(t->x1, t->x2);
        max_x = max(max_x, t->x3);

        int min_y = min(t->y1, t->y2);
        min_y = min(min_y, t->y3);
        int max_y = max(t->y1, t->y2);
        max_y = max(max_y, t->y3);

        scanline(t, min_x, min_y, max_x, max_y);

        
        




    }


}





PointLocation is_point_inside(int x, int y, Triangle* t, float* out_a, float* out_b, float* out_c) {

int x1 = t->x1, x2 = t->x2, x3 = t->x3;
int y1 = t->y1, y2 = t->y2, y3 = t->y3;


float D = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);

float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / D ;
float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / D ;
float c = 1 - b - a;

*out_a = a;
*out_b = b;
*out_c = c;

float e = 0.001;
if(b == 0 && c == 0) return VERTEX_A;
if(a == 0 && c == 0) return VERTEX_B;
if(a == 0 && b == 0) return VERTEX_C;

if(fabs(a) <= e && b > 0 && c > 0) return EDGE_BC;
if(fabs(b) <= e && a > 0 && c > 0) return EDGE_AC;
if(fabs(c) <= e && a > 0 && b > 0) return EDGE_AB;

if(a > 0 && b > 0 && c > 0) return INSIDE;

return OUTSIDE;

}



void scanline(Triangle* t, int min_x, int min_y, int max_x, int max_y) {
           
 
    for(int i = min_y; i <= max_y; i++) {
        for(int j = min_x; j <= max_x; j++) {

            float a, b, c; // weights 
            PointLocation where = is_point_inside(j, i, t, &a, &b, &c);

            if(where == OUTSIDE) continue;

            Color col;
            col.red   = (uint8_t)(t->c1.red   * a + t->c2.red   * b + t->c3.red   * c);
            col.green = (uint8_t)(t->c1.green * a + t->c2.green * b + t->c3.green * c);
            col.blue  = (uint8_t)(t->c1.blue  * a + t->c2.blue  * b + t->c3.blue  * c);
            col.alpha = (uint8_t)(t->c1.alpha * a + t->c2.alpha * b + t->c3.alpha * c);

            put_pixel(j, i, col);



        }
    }


}