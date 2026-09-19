# Operating-System

A small x86 operating system written from scratch in C and assembly — no Linux kernel, no existing OS underneath. It boots on real (emulated) hardware, sets up its own memory management, runs multiple programs with real isolation between them, and gives you a little command-line shell to try it out.

This is a learning project: the goal is to understand how an OS actually works by building the pieces yourself — the bootloader, paging, processes, a filesystem, and a shell — rather than to compete with a real OS.

## What it can do

- **Boots itself** — a custom bootloader (no GRUB) loads the kernel from disk and switches the CPU into 32-bit protected mode.
- **Manages memory with paging** — the kernel runs in its own "higher half" of memory, and pages physical RAM in on demand instead of loading everything up front.
- **Runs real programs in isolation** — each program gets its own address space and can't see or touch another program's memory (or the kernel's), enforced by the CPU's own protection hardware, not just convention.
- **Loads real ELF executables** — programs are compiled normally and loaded from disk, the same file format Linux uses.
- **Switches between programs** — a process can start another program, suspend itself, and correctly resume exactly where it left off when that program exits.
- **Has a simple filesystem** — a FAT16 disk driver so files actually persist across reboots.
- **Has a shell** — a small command-line interface with `cd`, `ls`, `cat`, `touch`, `write`, `delete`, `mkdir`, `run`, and `clear`.

## How it's put together

| Piece | What it does |
|---|---|
| `bootloader/` | The very first code that runs — loads the kernel off disk |
| `kernel/` | Boot handoff into 32-bit protected mode and the kernel's entry point |
| `memory/` | Physical memory allocation, paging, the kernel heap, and the ELF loader |
| `process/` | Creating, running, switching between, and cleaning up processes |
| `interrupts/` | The interrupt table, exception handling, and system calls |
| `descriptors/` | GDT/TSS setup needed for switching CPU privilege levels |
| `file_system/` | The FAT16 driver and the disk (ATA) driver underneath it |
| `command_shell/` | The interactive shell |
| `screen_driver/`, `keyboard_driver/`, `port_io/` | Basic drivers for text output, keyboard input, and hardware I/O |
| `user_space/` | The code that actually runs user programs, and the switch into user mode |
| `system/` | The system call interface programs use to talk to the kernel |

## Running it

You'll need `gcc` (with 32-bit support), `nasm`, `ld`, and `qemu-system-i386` installed.

```bash
./build.sh
```

This compiles everything, builds a bootable floppy image, and launches it in QEMU. The shell needs a FAT16 disk image to store files on — if you don't have one yet, create it first:

```bash
dd if=/dev/zero of=fat16_disk.img bs=1M count=32
mkfs.fat -F 16 -n MYOS_DISK fat16_disk.img
```

Once it boots, you'll land in the shell. Type `help` to see the available commands, or `run <file>` to launch a program from the disk.

## Where this is at

This is a working, actively-developed hobby OS, not a finished product. Things like scheduling, fork/exec as separate operations, and a broader syscall surface are still growing. Expect rough edges — that's part of the fun of building it yourself.
