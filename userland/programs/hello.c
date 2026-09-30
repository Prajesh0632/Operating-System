#include "nolibc.h"
#include "stio.h"


void _start(void)
{

   Window win;
   win.width = 1000;
   win.height = 700;
   win.win_x_pos = 0;
   win.win_y_pos = 0;

   if(sys_create_window(win)) {

   }
    // sys_exit();
}
