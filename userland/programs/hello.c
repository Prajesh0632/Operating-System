
#include "stio.h"


__attribute__((noreturn))
void _start(void)
{
    int a = 3, b = 4;
    print("Prajesh Subedi %d %d \n", a, b);

    for (;;)
        __asm__ volatile ("pause");   /* no SYS_EXIT yet - just spin */
}
