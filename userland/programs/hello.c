#include "nolibc.h"
#include "stio.h"


void _start(void)
{
    char name[20];
    print("Enter your name: ");
    scan("%s", name);

    print("\nYour name is %s", name);


    sys_exit();
}
