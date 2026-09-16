#include "system_header.h"
#include "system_calls.h"


void sys_write(char* string) {

    asm volatile("int $0x80" :: "a"(SYS_WRITE), "b"(string));

}


char sys_read() {

    char c;
    asm volatile("int $0x80" :: "a"(SYS_READ), "b"(&c) : "memory");
    return c;


}

void sys_clear() {

        asm volatile("int $0x80" :: "a"(SYS_CLEAR), "b"(0));

}


void sys_sclear() {
   
        asm volatile("int $0x80" :: "a"(SYS_SCLEAR), "b"(0));


}



void sys_flist(uint16_t cluster) {

    asm volatile("int $0x80" :: "a"(SYS_FLIST), "b"(cluster));


}


bool sys_ffind(uint16_t cluster, char* filename) {

    bool found = false;
    asm volatile("int $0x80" :: "a"(SYS_FFIND), "b"(filename), "c"(cluster), "d"(&found) : "memory");

    return found;


}


void sys_fprint(uint16_t cluster, char* filename) {

    asm volatile("int $0x80" :: "a"(SYS_FPRINT), "b"(filename), "c"(cluster));


}


void sys_fcreate(uint16_t cluster, char* filename) {

        asm volatile("int $0x80" :: "a"(SYS_FCREATE), "b"(filename), "c"(cluster));


}



void sys_fdelete(uint16_t cluster, char* filename) {

        asm volatile("int $0x80" :: "a"(SYS_FDELETE), "b"(filename), "c"(cluster));


}


 uint16_t sys_fcd(uint16_t cluster, char* path) {
        
        uint16_t new_cluster = cluster;
        asm volatile("int $0x80" :: "a"(SYS_FCD), "b"(path), "c"(&new_cluster) : "memory");
        return new_cluster;



 }




void sys_fmkdir(uint16_t cluster, char* dirname) {

        asm volatile("int $0x80" :: "a"(SYS_FMKDIR), "b"(dirname), "c"(cluster));


}

void sys_fwrite(uint16_t cluster, char* filename, char* content) {

        asm volatile("int $0x80" :: "a"(SYS_FWRITE), "b"(filename), "c"(cluster), "d"(content));


}


void sys_load_program(uint16_t cluster, char* filename) {

    asm volatile("int $0x80" :: "a"(SYS_LOAD_PROGRAM), "b"(filename), "c"(cluster));


}



void sys_fork() {

            asm volatile("int $0x80" :: "a"(SYS_FORK));


}

