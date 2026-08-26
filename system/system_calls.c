#include "system_header.h"
#include "system_calls.h"


void sys_write(char* string) {

    asm volatile("int $0x80" :: "a"(SYS_WRITE), "b"(string));

}


char sys_read() {

    char c;
    asm volatile("int $0x80" :: "a"(SYS_READ), "b"(&c));
    return c;


}

void sys_clear() {

        asm volatile("int $0x80" :: "a"(SYS_CLEAR), "b"(0));

}


