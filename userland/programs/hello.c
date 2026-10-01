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

   ProtoColor red = {255, 255, 255, 255};
   ProtoColor blue = {0, 255, 0, 255};
   ProtoColor green = {0, 0, 255, 255};
   ProtoColor color = red;




   bool exit = false;

   int counter = 3;

   uint32_t time = get_proto_time();
   while (!exit)
   {

    clear_proto_window(color);


    if(get_proto_time() - time  > 1000) {
      update_proto_window(Window);
      counter++;
      int rem = counter % 3;
      if(rem == 0) {
        color = red;
      }
      if(rem == 1) {
        color = blue;
      }

      if(rem == 2) {
        color = green;
      }

      time = get_proto_time();
    
    }


   }
   

   


    sys_exit();



}
