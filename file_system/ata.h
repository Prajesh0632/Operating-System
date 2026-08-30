#include <stdint.h>

#define ATA_PRIMARY_DATA         0x1F0
#define ATA_PRIMARY_ERR          0x1F1
#define ATA_PRIMARY_SECCOUNT     0x1F2
#define ATA_PRIMARY_LBA_LOW      0x1F3
#define ATA_PRIMARY_LBA_MID      0x1F4
#define ATA_PRIMARY_LBA_HIGH     0x1F5
#define ATA_PRIMARY_DRIVE_SEL    0x1F6
#define ATA_PRIMARY_STATUS       0x1F7
#define ATA_PRIMARY_COMMAND      0x1F7

// ATA Commands
#define ATA_CMD_READ_PIO         0x20
#define ATA_CMD_WRITE_PIO        0x30
#define ATA_CMD_IDENTIFY         0xEC

// Status Register Bits
#define ATA_STATUS_ERR           0x01  // Error occurred
#define ATA_STATUS_DRQ           0x08  // Data Request Ready
#define ATA_STATUS_SRV           0x10  // Service
#define ATA_STATUS_DF            0x20  // Drive Fault
#define ATA_STATUS_RDY           0x40  // Drive Ready
#define ATA_STATUS_BSY           0x80  // Drive Busy



void ata_read_sector(uint32_t, uint16_t*);
void ata_write_sector(uint32_t, uint16_t*);