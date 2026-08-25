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


void sys_echo_on() {

 asm volatile("int $0x80" :: "a"(SYS_ECHO_ON), "b"(0));


}

void sys_echo_off() {

 asm volatile("int $0x80" :: "a"(SYS_ECHO_OFF), "b"(0));


}