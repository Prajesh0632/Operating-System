[bits 32]

extern handle_interrupt
extern handle_syscall

global isr128
isr128:
    pusha
    push ebx 
    push eax 
    call handle_syscall
    add esp, 8
    popa
    iret

; Macro for interrupts WITHOUT a CPU error code
%macro isr_no_err_stub 1
isr_stub_%+%1:
    
    pusha                ; Save all general registers
    push dword 0         ; Push dummy error code (for stack consistency)
    push dword %1        ; Push interrupt number
    call handle_interrupt
    add esp, 4           ; Remove interrupt number
    add esp, 4           ; Remove dummy error code

    popa                 ; Restore registers
    iret                 ; Return from interrupt
%endmacro


; Macro for interrupts WITH a CPU error code
%macro isr_err_stub 1
isr_stub_%+%1:
    
    pusha                ; Save all general registers
    push dword %1        ; Push interrupt number
    call handle_interrupt
    add esp, 4           ; Remove interrupt number
    popa                 ; Restore registers
    add esp, 4           ; Remove CPU-pushed error code
    iret                 ; Return from interrupt
%endmacro


; CPU exceptions that push error codes: 8, 10, 11, 12, 13, 14, 17, 21, 29, 30
isr_no_err_stub 0
isr_no_err_stub 1

isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8   ; Double Fault (pushes err code)
isr_no_err_stub 9
isr_err_stub    10  ; Invalid TSS (pushes err code)
isr_err_stub    11  ; Segment Not Present (pushes err code)
isr_err_stub    12  ; Stack-Segment Fault (pushes err code)
isr_err_stub    13  ; General Protection Fault (pushes err code)
isr_err_stub    14  ; Page Fault (pushes err code)
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17  ; Alignment Check (pushes err code)
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_err_stub    21  ; Control Protection Exception
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_err_stub    29  ; VMM Communication Exception
isr_err_stub    30  ; Security Exception
isr_no_err_stub 31

isr_no_err_stub 32
isr_no_err_stub 33
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47



;setting up ISR address pointers 
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    48
    dd isr_stub_%+i 
%assign i i+1 
%endrep




