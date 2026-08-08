# Bootloader

The bootloader is the first program that gets executed when the computer boots.
BIOS loads it from a bootable device (hard disk, floppy, USB, etc.).

## The boot sector

BIOS reads exactly **one sector — 512 bytes** — from the start of the device
(LBA sector 0). In the older CHS scheme BIOS calls this cylinder 0, head 0,
**sector 1** — CHS sectors are 1-indexed while LBA starts at 0.

For BIOS to consider a device bootable, that sector must end with the signature
`0xAA55` at offset 510. Without it, BIOS skips the device entirely.

    offset 0    ┌──────────────┐
                │ boot code    │   510 bytes usable
    offset 510  ├──────────────┤
                │  55 AA       │   signature (little-endian 0xAA55)
    offset 512  └──────────────┘

This is why the padding line works out:

    times 510-($-$$) db 0   ; fill up to offset 510
    dw 0xaa55               ; signature in the last 2 bytes

`$` is the current address, `$$` is the section start, so `$-$$` is how many
bytes have been emitted so far.

## 512 bytes is the sector, not the bootloader

Only the *first stage* has to fit in 512 bytes. Real bootloaders are much
bigger — GRUB is megabytes. The first sector contains just enough code to read
**more sectors** off the disk and jump to them. This is called staging.

So the real constraint is "stage 1 must fit in 510 bytes". When it doesn't,
NASM fails with `TIMES value -N is negative` — that is the signal to move code
into a second stage.

## Where it loads

BIOS copies the sector to physical address **0x7C00** and jumps there.

That address is why every bootloader starts with `[org 0x7c00]` — it tells the
assembler to compute label addresses as if the code sits at 0x7C00. There is no
loader to relocate anything afterward, unlike an `.exe` which the OS fixes up at
load time.

Disk and memory are separate things here: sector 0 *of the disk* is loaded to
address 0x7C00 *in memory*.

## CPU state on entry

- **16-bit real mode** — 1 MB addressable, no memory protection, no paging
- **DL = boot drive number** — 0x00 for floppy, 0x80+ for hard disk / USB.
  Save it immediately; it is needed for any `int 0x13` disk read.
- Segment registers and SP are **not** in a guaranteed state — set up DS, ES and
  a stack before relying on them.

## BIOS services

While in real mode BIOS provides interrupt routines:

- `int 0x10` — video. AH=0x0E is teletype output: prints AL, advances the
  cursor, wraps at column 80, scrolls at the bottom.
- `int 0x13` — disk. Read/write sectors, needed to load stage 2.
- `int 0x16` — keyboard input.

**These stop working in protected mode.** Real-mode interrupt vectors do not
exist there, so all disk loading must happen *before* the mode switch —
afterwards the kernel needs its own drivers, and screen output means writing
video memory at 0xB8000 directly.

## MBR vs floppy boot sector

On a partitioned hard disk this sector is the **MBR**, which also holds the
partition table at offset 446. That leaves only **446 bytes** for code, not 510.
A floppy (or an unpartitioned image) has no partition table, so the full 510
bytes are available.

## Build and run

    nasm -f bin boot.asm -o boot.bin              ; raw 512-byte sector
    nasm -f bin -d FLOPPY boot.asm -o boot.img    ; padded to 1.44MB floppy
    qemu-system-i386 -fda boot.img

`-f bin` emits raw bytes with no headers — no entry point, no sections, no
relocations. Byte 0 of the file is the first instruction executed. QEMU pads
short images itself, so `-fda boot.bin` also works; the full 1.44MB image is
only needed for VirtualBox or real hardware.
