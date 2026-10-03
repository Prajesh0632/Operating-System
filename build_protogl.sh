#!/usr/bin/env bash
set -euo pipefail

# Build ProtoGL as a single relocatable object file (protogl.o) for a
# user-space program to link against - see build_user.sh.
#   ./build_protogl.sh

PROTOGL_DIR="userland/ProtoGL"
NOLIBC_DIR="userland/nolibc"
OUT="protogl.o"

CFLAGS="-m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector -nostdlib -O2 -Wall -I$PROTOGL_DIR -I$NOLIBC_DIR"

echo "Compiling protogl.c"
gcc $CFLAGS -c "$PROTOGL_DIR/protogl.c" -o protogl_core.o

echo "Compiling proto_window.c"
gcc $CFLAGS -c "$PROTOGL_DIR/proto_window.c" -o proto_window.o

echo "Compiling proto_draw.c"
gcc $CFLAGS -c "$PROTOGL_DIR/proto_draw.c" -o proto_draw.o

echo "Combining into $OUT"
ld -m elf_i386 -r protogl_core.o proto_window.o proto_draw.o -o "$OUT"

echo
echo "Built $OUT"
