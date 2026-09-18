
#include "stio.h"


__attribute__((noreturn))
void _start(void)
{
    char name[20];
    print("Enter your name: ");
    scan("%s", name);

    print("\nYour name is %s", name);


    for (;;)
        __asm__ volatile ("pause");   /* no SYS_EXIT yet - just spin */
}
