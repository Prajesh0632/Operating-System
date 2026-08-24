#include "system_calls.h"

void sys_write(char* string) {
    asm volatile("int $0x80" :: "a"(0), "b"(string));
}