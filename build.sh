
#!/usr/bin/env bash
set -euo pipefail

# Build kernel, write into boot.img (sector 1), and run QEMU with serial
BOOT_FILE="bootloader/boot_kernel.asm"
KERNEL_ENTRY_FILE="kernel/kernel_entry.asm"
INTERRUPT="interrupts/interrupt.asm"

KERNEL_FILE="kernel/kernel.c"

IDT_DESCRIPTOR="interrupts/idt.c"
SCREEN_DRIVER="screen_driver/screen.c"
KEYBOARD_DRIVER="keyboard_driver/keyboard.c"
PORT_IO="port_io/io.c"
COMMAND_SHELL="command_shell/shell.c"

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

echo "Compiling shell.c -> shell.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $COMMAND_SHELL -o shell.o





echo "Assembling kernel_entry.asm -> kernel_entry.o"
nasm -f elf32 $KERNEL_ENTRY_FILE -o kernel_entry.o

echo "Assembling interrupt.asm -> interrupt.o"
nasm -f elf32 $INTERRUPT -o interrupt.o

echo "Linking kernel with interrupts at 0x1000 -> kernel.bin"
ld -m elf_i386 -Ttext 0x1000 --oformat binary kernel_entry.o kernel.o idt.o interrupt.o screen.o io.o keyboard.o shell.o -o kernel.bin



echo "Assembling bootloader -> boot_sector.img"
nasm -f bin -d FLOPPY -Ibootloader/ $BOOT_FILE -o boot.img


echo "Writing kernel.bin into boot.img (sector 1)"
dd if=kernel.bin of=boot.img bs=512 seek=1 conv=notrunc

truncate -s 1474560 boot.img

echo "Launching QEMU (serial to stdio)"
env -i HOME="$HOME" DISPLAY="$DISPLAY" XAUTHORITY="${XAUTHORITY:-$HOME/.Xauthority}" /usr/bin/qemu-system-i386 -drive file=boot.img,format=raw,index=0,if=floppy