#include "nolibc.h"
#include "stio.h"
#include "../ProtoGL/protogl.h"
#include <stdbool.h>


void _start(void)
{

   if(!init_protogl()) {
    sys_write("ProtoGL initialization Failed.");
    sys_exit();
   }

   ProtoWindow* Window = create_proto_window(112, 84, 800, 600);

   ProtoColor white = {255, 255, 255, 255};
   ProtoColor red = {255, 0, 0, 255};
   ProtoColor green = {0, 255, 0, 255};
   ProtoColor blue = {0, 0, 255, 255};
   ProtoColor color = red;

   




   bool exit = false;

   int counter = 3;

   ProtoVertex A = {100, 100, 100, red};
   ProtoVertex B = {100, 200, 100, red};
   ProtoVertex C = {200, 100, 100, red};
   ProtoTriangle T = {A, B, C};

   uint32_t fps = 60;
   float frame_ms = 1000.0 / fps;

   uint32_t next = get_proto_time() + frame_ms;
   while (!exit)
   {

    uint32_t now = get_proto_time();
    if(now < next) continue;

     
      
           clear_proto_window(white);
           proto_draw_triangles(&T, 1);
           update_proto_window(Window);

           next += frame_ms;



      


   }
   

   


    sys_exit();



}
