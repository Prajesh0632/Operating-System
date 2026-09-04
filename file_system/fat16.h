#pragma once 

#include <stddef.h>
#include <stdint.h>

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20
#define ATTR_LFN        0x0F


typedef struct {
    // --- Standard DOS 2.0 BPB (Offsets 0x00 - 0x17) ---
    uint8_t  jump_instruction[3]; // 0x00: Jump to boot code
    uint8_t  oem[8];              // 0x03: OEM / Manufacturer name
    uint16_t bytes_per_sector;    // 0x0B: Bytes per sector (512)
    uint8_t  sectors_per_cluster; // 0x0D: Sectors per cluster
    uint16_t reserved_sectors;    // 0x0E: Reserved sectors (usually 1)
    uint8_t  fat_n;               // 0x10: Number of FAT tables (2)
    uint16_t directories_n;       // 0x11: Max root directory entries (512)
    uint16_t total_sectors_short; // 0x13: 16-bit total sector count (0 if > 32MB)
    uint8_t  media_type;          // 0x15: Media descriptor byte
    uint16_t sectors_per_fat;     // 0x16: Sectors per FAT table

    // --- DOS 3.31 & Extended FAT16 BPB (Offsets 0x18 - 0x3E) ---
    uint16_t sectors_per_track;   // 0x18: Sectors per track (CHS geometry)
    uint16_t head_count;          // 0x1A: Head count (CHS geometry)
    uint32_t hidden_sectors;      // 0x1C: Hidden sectors before partition
    uint32_t total_sectors_large; // 0x20: 32-bit total sector count (used if total_sectors_short == 0)

    // Extended Boot Record
    uint8_t  drive_number;        // 0x24: BIOS drive number (0x80 for hard disk)
    uint8_t  reserved_flags;      // 0x25: Reserved / Windows NT flags
    uint8_t  signature;           // 0x26: Extended boot signature (0x29)
    uint32_t volume_id;           // 0x27: Volume serial number
    char     volume_label[11];    // 0x2B: Volume label string
    char     system_id[8];        // 0x36: File system type string ("FAT16   ")
} __attribute__((packed)) Bpb;


typedef struct {

    uint8_t name[11];
    uint8_t attribute; // 0x20 for archive, 0x10 for directory 
    uint8_t reserved;
    uint8_t created_at_seconds;
    uint16_t created_at_time;
    uint16_t created_at_date;
    uint16_t last_accessed_date;
    uint16_t high_cluster; // higher 16 bits of the first cluster number(0 for fat 12 and fat 16)
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t low_cluster; // lower 16 bits of the first cluster number (this is used to find the next cluster)
    uint32_t size;


}__attribute__((packed)) DirEntry;


_Static_assert(sizeof(DirEntry) == 32, "FAT16 dir entry must be 32 bytes");


void init_fat16();
void list_dir(uint32_t);
void print_file(char*, uint16_t);
void create_file(char*, uint16_t);
void delete_file(char*, uint16_t);
uint16_t change_dir(char*, uint16_t);
void make_dir(char*, uint16_t);