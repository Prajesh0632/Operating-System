nasm -f bin -d FLOPPY boot.asm -o boot.img 
qemu-system-i386 -fda boot.img
