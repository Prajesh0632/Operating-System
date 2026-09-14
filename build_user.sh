#!/usr/bin/env bash
set -euo pipefail

# Build a standalone user-space program as an ELF32 executable, linked
# against the nolibc syscall layer (userland/nolibc).
#   ./build_user.sh userland/programs/hello.c hello.elf

SRC="${1:-userland/programs/hello.c}"
OUT="${2:-hello.elf}"

NOLIBC_DIR="userland/nolibc"
STIO_DIR="userland/headers/io"
STR_DIR="userland/headers/string"
LD_SCRIPT="userland/user.ld"
CFLAGS="-m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -nostdlib -O2 -Wall -I$NOLIBC_DIR -I$STIO_DIR -I$STR_DIR"

echo "Compiling $SRC"
gcc $CFLAGS -c "$SRC" -o "${OUT%.elf}.o"

echo "Compiling nolibc (syscalls.c, stio.c, str.c)"
gcc $CFLAGS -c "$NOLIBC_DIR/syscalls.c" -o "${OUT%.elf}_syscalls.o"
gcc $CFLAGS -c "$STIO_DIR/stio.c" -o "${OUT%.elf}_stio.o"
gcc $CFLAGS -c "$STR_DIR/str.c" -o "${OUT%.elf}_str.o"

echo "Linking $OUT (ELF32, entry _start, base 0x400000)"
ld -m elf_i386 -T "$LD_SCRIPT" "${OUT%.elf}.o" "${OUT%.elf}_syscalls.o" "${OUT%.elf}_stio.o" "${OUT%.elf}_str.o" -o "$OUT"

echo
readelf -h "$OUT" | grep -E 'Type|Entry|Machine'
echo
echo "Program headers (what the kernel loader will map):"
readelf -l "$OUT" | sed -n '/Program Headers/,/Section to Segment/p'

# Drop the built ELF onto the disk image the kernel actually boots from, as
# an 8.3 FAT16 name (e.g. hello.elf -> HELLO.ELF) so fat_find() can see it.
HDD_FILE="fat16_disk.img"
DISK_NAME="$(basename "${OUT%.elf}" | tr '[:lower:]' '[:upper:]').ELF"

echo
if [ -f "$HDD_FILE" ] && command -v mcopy >/dev/null 2>&1; then
    echo "Copying $OUT -> $HDD_FILE as $DISK_NAME"
    mcopy -o -i "$HDD_FILE" "$OUT" "::$DISK_NAME"
    mdir -i "$HDD_FILE" ::/
elif [ ! -f "$HDD_FILE" ]; then
    echo "warning: $HDD_FILE not found - built $OUT locally but didn't copy it to disk" >&2
else
    echo "warning: mcopy not found (mtools) - built $OUT locally but didn't copy it to disk" >&2
fi
