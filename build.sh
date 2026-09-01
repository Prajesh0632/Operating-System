
#!/usr/bin/env bash
set -euo pipefail


# Build kernel, write into boot.img (sector 1), and run QEMU with serial
BOOT_FILE="bootloader/boot_kernel.asm"
KERNEL_ENTRY_FILE="kernel/kernel_entry.asm"
INTERRUPT="interrupts/interrupt.asm"
PAGING_ENABLE="memory/enable_paging.asm"
USER_ENTRY="user_space/user_space_entry.asm"


KERNEL_FILE="kernel/kernel.c"
USER_FILE="user_space/user_program.c"


IDT_DESCRIPTOR="interrupts/idt.c"
SCREEN_DRIVER="screen_driver/screen.c"
KEYBOARD_DRIVER="keyboard_driver/keyboard.c"
PORT_IO="port_io/io.c"
PMM="memory/pmm.c"
HEAP="memory/heap.c"
PAGING="memory/paging.c"
TSS="descriptors/tss.c"
USER_SWITCH="user_space/switch_user.c"
SYSTEM_CALLS="system/system_calls.c"
STIO="headers/io/stio.c"
STRING="headers/string/str.c"
COMMAND_SHELL="command_shell/shell.c"
DISK_RW="file_system/ata.c"
FAT16="file_system/fat16.c"



echo "Compiling kernel.c -> kernel.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $KERNEL_FILE -o kernel.o

echo "Compiling idt.c -> idt.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $IDT_DESCRIPTOR -o idt.o

echo "Compiling keyboard.c -> keyboard.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $KEYBOARD_DRIVER -o keyboard.o

echo "Compiling screen.c -> screen.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $SCREEN_DRIVER -o screen.o

echo "Compiling io.c -> io.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $PORT_IO -o io.o

echo "Compiling pmm.c -> pmm.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $PMM -o memory.o

echo "Compiling heap.c -> heap.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $HEAP -o heap.o

echo "Compiling paging.c -> paging.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $PAGING -o paging.o

echo "Compiling tss.c -> tss.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $TSS -o tss.o

echo "Compiling ata.c -> ata.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $DISK_RW -o ata.o

echo "Compiling fat16.c -> fat16.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $FAT16 -o fat16.o

echo "Compiling user.c -> user.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $USER_FILE -o user.o

echo "Compiling user_switch.c -> user_switch.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $USER_SWITCH -o user_switch.o

echo "Compiling shell.c -> shell.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $COMMAND_SHELL -o shell.o

echo "Compiling system_calls.c -> system_calls.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $SYSTEM_CALLS -o system_calls.o

echo "Compiling stio.c -> stio.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $STIO -o stio.o

echo "Compiling str.c -> str.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $STRING -o str.o





echo "Assembling kernel_entry.asm -> kernel_entry.o"
nasm -f elf32 $KERNEL_ENTRY_FILE -o kernel_entry.o

echo "Assembling interrupt.asm -> interrupt.o"
nasm -f elf32 $INTERRUPT -o interrupt.o

echo "Assembling enable_paging.asm -> e_paging.o"
nasm -f elf32 $PAGING_ENABLE -o e_paging.o

echo "Assembling user_entry.asm -> user_entry.o"
nasm -f elf32 $USER_ENTRY -o user_entry.o


echo "Linking kernel with interrupts at 0x10000 -> kernel.bin"
ld -m elf_i386 -T linker.ld --oformat binary kernel_entry.o kernel.o idt.o interrupt.o screen.o io.o keyboard.o shell.o memory.o heap.o e_paging.o paging.o tss.o user.o user_entry.o user_switch.o system_calls.o stio.o str.o ata.o fat16.o -o kernel.bin



echo "Assembling bootloader -> boot_sector.img"
nasm -f bin -d FLOPPY -Ibootloader/ $BOOT_FILE -o boot.img


echo "Writing kernel.bin into boot.img (sector 1)"
dd if=kernel.bin of=boot.img bs=512 seek=1 conv=notrunc

truncate -s 1474560 boot.img

# Hard disk image: primary IDE master (ports 0x1F0/0x3F6) for file_system/ata.c
# Pre-formatted FAT16 volume, kept across builds so its contents survive.
HDD_FILE="fat16_disk.img"

if [ ! -f "$HDD_FILE" ]; then
    echo "error: $HDD_FILE not found. Recreate it with:" >&2
    echo "    dd if=/dev/zero of=$HDD_FILE bs=1M count=32" >&2
    echo "    mkfs.fat -F 16 -n MYOS_DISK $HDD_FILE" >&2
    exit 1
fi

echo "Launching QEMU (serial to stdio)"
env -i HOME="$HOME" DISPLAY="$DISPLAY" XAUTHORITY="${XAUTHORITY:-$HOME/.Xauthority}" /usr/bin/qemu-system-i386 -m 1024 \
    -boot order=a \
    -drive file=boot.img,format=raw,index=0,if=floppy \
    -drive file="$HDD_FILE",format=raw,if=ide,index=0,media=disk \
    -no-reboot