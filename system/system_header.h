#pragma once

typedef enum {

    // SCREEN SYSTEM CALLS
    SYS_WRITE,
    SYS_READ,
    SYS_CLEAR,
    SYS_SCLEAR,

    // FILE SYSTEM CALLS
    SYS_FLIST,
    SYS_FPRINT,
    SYS_FCREATE,
    SYS_FDELETE,
    SYS_FCD,
    SYS_FMKDIR

} SyscallId;
