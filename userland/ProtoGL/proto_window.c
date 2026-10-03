#include "protogl.h"
#include "nolibc.h"
#include <stddef.h>
#include "proto_draw.h"

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
      renderer->width = main_window->width;
      renderer->height = main_window->height;
      renderer->x = main_window->x_pos;
      renderer->y = main_window->y_pos;


   return renderer;

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


