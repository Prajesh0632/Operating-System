#pragma once
#include <stdint.h>

/*
 * Read an ELF32 file from the FAT16 disk and print its header + program
 * headers. Inspection only - nothing is mapped or executed.
 *
 *   name        8.3 file name (e.g. "HELLO.ELF")
 *   dir_cluster start cluster of the directory (0 = root)
 */
void elf_inspect(const char *name, uint16_t dir_cluster);
