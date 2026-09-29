#include "nolibc.h"
#include "stio.h"


void _start(void)
{

    char name[20];
    print("Enter your name: ");
    scan("%s", name);

    print("\nYour name is %s\n", name);
    
    uint32_t* addr =  (uint32_t*)sys_brk(32);
    *addr = 100;
    print("\n%d", *addr);
    
    // sys_exit();
}
