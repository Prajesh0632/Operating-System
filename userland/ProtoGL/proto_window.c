#include "protogl.h"
#include "nolibc.h"
#include <stddef.h>

ProtoWindow* renderer = NULL;
Window* main_window = NULL;
uint16_t pitch = 3072;
uint8_t bpp = 3;
uint8_t framebuffer[1024 * 768 * 3];

ProtoWindow* create_proto_window(int x_pos, int y_pos, uint32_t width, uint32_t height) {

   if(renderer != NULL) {
    
    sys_write("Window Already exists.");
    return renderer;

   } 

   main_window = (Window*)sys_brk(sizeof(Window));

   main_window->width = width;
   main_window->height = height;
   main_window->win_x_pos = x_pos;
   main_window->win_y_pos = y_pos;
   main_window->x_offset = 10;
   main_window->y_offset = 10;

   if(!sys_create_window(*main_window)) {
    sys_write("Window Creation Failed.");
   }



   renderer = (ProtoWindow*)sys_brk(sizeof(ProtoWindow));
//    renderer->framebuffer = (uint8_t*)sys_brk((width * height * bpp));
      renderer->framebuffer = framebuffer;

   return renderer;

}


void put_pixel(int x, int y, ProtoColor color) {

if((x < 0 || x >= main_window->width) || (y < 0 || y >= main_window->height))return;

   
    uint8_t *pixel = renderer->framebuffer + y * (main_window->width * bpp) + x * bpp;

    pixel[0] = (pixel[0] * (255 - color.alpha) + color.blue * color.alpha) / 255;
    pixel[1] = (pixel[1] * (255 - color.alpha) + color.green * color.alpha) / 255;
    pixel[2] = (pixel[2] * (255 - color.alpha) + color.red * color.alpha) / 255;

}

void clear_proto_window(ProtoColor color) {

    for(int i = 0; i < main_window->height; i++) {

        for(int j = 0; j < main_window->width; j++) {
            
            put_pixel(j, i, color);
        }
    }


}

void update_proto_window(ProtoWindow*) {

    sys_update_window(renderer->framebuffer);

}


