#include "protogl.h"
#define PROTO_BPP 3 
#define PROTO_PITCH 3072




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



void put_pixel(int x, int y, ProtoColor color) {

if((x < 0 || x >= renderer->width) || (y < 0 || y >= renderer->height))return;

   
    uint8_t *pixel = renderer->framebuffer + y * (renderer->width * PROTO_BPP) + x *PROTO_BPP;

    pixel[0] = (pixel[0] * (255 - color.alpha) + color.blue * color.alpha) / 255;
    pixel[1] = (pixel[1] * (255 - color.alpha) + color.green * color.alpha) / 255;
    pixel[2] = (pixel[2] * (255 - color.alpha) + color.red * color.alpha) / 255;

}








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


PointLocation is_point_inside(int x, int y, ProtoTriangle* t, float* out_a, float* out_b, float* out_c) {

int x1 = t->A.x, x2 = t->B.x, x3 = t->C.x;
int y1 = t->A.y, y2 = t->B.y, y3 = t->C.y;


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




void scanline(ProtoTriangle* t, int min_x, int min_y, int max_x, int max_y) {
           
 
    for(int i = min_y; i <= max_y; i++) {
        for(int j = min_x; j <= max_x; j++) {

            float a, b, c; // weights 
            PointLocation where = is_point_inside(j, i, t, &a, &b, &c);

            if(where == OUTSIDE) continue;

            ProtoColor col;
            col.red   = (uint8_t)(t->A.color.red   * a + t->B.color.red   * b + t->C.color.red   * c);
            col.green = (uint8_t)(t->A.color.green * a + t->B.color.green * b + t->C.color.green * c);
            col.blue  = (uint8_t)(t->A.color.blue  * a + t->B.color.blue  * b + t->C.color.blue  * c);
            col.alpha = (uint8_t)(t->A.color.alpha * a + t->B.color.alpha * b + t->C.color.alpha * c);

            put_pixel(j, i, col);



        }
    }


}







void proto_draw_triangles(ProtoTriangle *triangles, uint32_t n) {


    

      

    for(uint32_t i = 0; i < n; i++) {
            
        ProtoTriangle* t = &triangles[i];

        //setup the max and min coordinates for the rectangle boundary for the scanline
        int min_x = min(t->A.x, t->B.x);
        min_x = min(min_x, t->C.x);
        int max_x = max(t->A.x, t->B.x);
        max_x = max(max_x, t->C.x);

        int min_y = min(t->A.y, t->B.y);
        min_y = min(min_y, t->C.y);
        int max_y = max(t->A.y, t->B.y);
        max_y = max(max_y, t->C.y);

        scanline(t, min_x, min_y, max_x, max_y);

        
        




    }


}




