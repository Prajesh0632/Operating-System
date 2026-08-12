
#!/usr/bin/env bash
set -euo pipefail

# Build kernel, write into boot.img (sector 1), and run QEMU with serial
BOOT_FILE="bootloader/boot_kernel.asm"
KERNEL_FILE="kernel/kernel.c"
KERNEL_ENTRY_FILE="kernel/kernel_entry.asm"

echo "Compiling kernel.c -> kernel.o"
gcc -m32 -ffreestanding -fno-builtin -fno-pie -fno-pic -O2 -c $KERNEL_FILE -o kernel.o

echo "Assembling kernel_entry.asm -> kernel_entry.o"
nasm -f elf32 $KERNEL_ENTRY_FILE -o kernel_entry.o

echo "Linking kernel at 0x1000 -> kernel.bin"
ld -m elf_i386 -Ttext 0x1000 --oformat binary kernel_entry.o kernel.o -o kernel.bin

echo "Assembling bootloader -> boot_sector.img"
nasm -f bin -d FLOPPY -Ibootloader/ $BOOT_FILE -o boot.img


echo "Writing kernel.bin into boot.img (sector 1)"
dd if=kernel.bin of=boot.img bs=512 seek=1 conv=notrunc

echo "Launching QEMU (serial to stdio)"
env -i HOME="$HOME" DISPLAY="$DISPLAY" XAUTHORITY="${XAUTHORITY:-$HOME/.Xauthority}" /usr/bin/qemu-system-i386 -drive file=boot.img,format=raw,index=0,if=floppy