#pragma once

/*
 * nolibc - the minimal syscall interface for standalone user-space programs.
 *
 * Bundles the syscall ids and wrapper declarations that kernel-internal
 * code gets from system/system_header.h + system/system_calls.h. Kept as
 * its own copy here (not shared with those) so user-space programs never
 * reach into kernel-only headers, and this can stay a small, stable on
 * its own - the kernel-linked shell keeps using system/system_calls.* as
 * before.
 *
 * See interrupts/interrupt.asm for the registers:
 *   eax = syscall id, ebx = arg1, ecx = arg2, edx = arg3.
 */

#include <stdint.h>
#include <stdbool.h>

typedef enum {
       // SCREEN SYSTEM CALLS
    SYS_WRITE,
    SYS_READ,
    SYS_CLEAR,
    SYS_SCLEAR,

    // FILE SYSTEM CALLS
    SYS_FLIST,
    SYS_FFIND,
    SYS_FPRINT,
    SYS_FCREATE,
    SYS_FDELETE,
    SYS_FCD,
    SYS_FMKDIR,
    SYS_FWRITE,
    SYS_LOAD_PROGRAM,


    //Process SYSTEM CALLS
    SYS_FORK,
    SYS_EXEC,
    SYS_EXIT,

    //MEMORY
    SYS_BRK,
    SYS_SBRK,


    //Graphics 
    SYS_CREATE_WINDOW,

} SyscallId;

void     sys_write(char *string);
char     sys_read(void);
void     sys_clear(void);
void     sys_sclear(void);

void     sys_flist(uint16_t cluster);
bool     sys_ffind(uint16_t cluster, char *filename);
void     sys_fprint(uint16_t cluster, char *filename);
void     sys_fcreate(uint16_t cluster, char *filename);
void     sys_fdelete(uint16_t cluster, char *filename);
uint16_t sys_fcd(uint16_t cluster, char *path);
void     sys_fmkdir(uint16_t cluster, char *dirname);
void     sys_fwrite(uint16_t cluster, char *filename, char *content);
void     sys_load_program(uint16_t cluster, char *filename);

void sys_exit();

void* sys_brk(uint32_t);




typedef struct Window
{
    uint32_t width;
    uint32_t height;
    int win_x_pos, win_y_pos;
    int x_pos, y_pos;
    uint32_t x_offset, y_offset;
} Window __attribute__((packed));


bool sys_create_window(Window);