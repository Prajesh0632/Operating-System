#pragma once

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
    

} SyscallId;
