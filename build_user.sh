#!/usr/bin/env bash
set -euo pipefail

# Build a standalone user-space program as an ELF32 executable.
#   ./build_user.sh memory/elf.c hello.elf

SRC="${1:-memory/elf.c}"
OUT="${2:-hello.elf}"

CFLAGS="-m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -nostdlib -O2 -Wall"

echo "Compiling $SRC"
gcc $CFLAGS -c "$SRC" -o "${OUT%.elf}.o"

echo "Linking $OUT (ELF32, entry _start, base 0x400000)"
ld -m elf_i386 -T user.ld "${OUT%.elf}.o" -o "$OUT"

echo
readelf -h "$OUT" | grep -E 'Type|Entry|Machine'
echo
echo "Program headers (what the kernel loader will map):"
readelf -l "$OUT" | sed -n '/Program Headers/,/Section to Segment/p'
