#include "ata.h"
#include "../port_io/io.h"




void ata_read_sector(uint32_t lba, uint16_t* buffer) {

    // 1. Send drive select & highest 4 bits of LBA
    // 0xE0 sets LBA mode and targets the Master drive
    port_byte_out(ATA_PRIMARY_DRIVE_SEL, 0xE0 | ((lba >> 24) & 0x0F));

     // 2. Set sector count to 1
    port_byte_out(ATA_PRIMARY_SECCOUNT, 1);

     // 3. Send the rest of the LBA address
    port_byte_out(ATA_PRIMARY_LBA_LOW, (uint8_t)lba);          // LBA bits 0-7
    port_byte_out(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));   // LBA bits 8-15
    port_byte_out(ATA_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));  // LBA bits 16-23

    // 4. Issue the Read Sectors command
    port_byte_out(ATA_PRIMARY_COMMAND, ATA_CMD_READ_PIO);

     // 5. Poll Status Register
    // Bit 7 (0x80) is BSY (Busy), Bit 3 (0x08) is DRQ (Data Request)
    while ((port_byte_in(ATA_PRIMARY_STATUS) & ATA_STATUS_BSY) != 0);      // Wait while drive is busy
    while ((port_byte_in(ATA_PRIMARY_STATUS) & ATA_STATUS_DRQ) == 0);      // Wait until data request is ready

    // 6. Read 256 words (512 bytes) from the data port
    for (int i = 0; i < 256; i++) {
        buffer[i] = port_word_in(ATA_PRIMARY_DATA);
    }
    

}


void ata_write_sector(uint32_t lba, uint16_t* buffer) {

     // 1. Send drive select & highest 4 bits of LBA
    // 0xE0 sets LBA mode and targets the Master drive
    port_byte_out(ATA_PRIMARY_DRIVE_SEL, 0xE0 | ((lba >> 24) & 0x0F));

     // 2. Set sector count to 1
    port_byte_out(ATA_PRIMARY_SECCOUNT, 1);

     // 3. Send the rest of the LBA address
    port_byte_out(ATA_PRIMARY_LBA_LOW, (uint8_t)lba);          // LBA bits 0-7
    port_byte_out(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));   // LBA bits 8-15
    port_byte_out(ATA_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));  // LBA bits 16-23

      // 4. Issue the Write Sectors command
    port_byte_out(ATA_PRIMARY_COMMAND, ATA_CMD_WRITE_PIO);


    //400ns hardware delay
    // Read the status register 4 times to let the drive update its internal state
    port_byte_in(ATA_PRIMARY_STATUS);
    port_byte_in(ATA_PRIMARY_STATUS);
    port_byte_in(ATA_PRIMARY_STATUS);
    port_byte_in(ATA_PRIMARY_STATUS);

      // 5. Poll Status Register
    // Bit 7 (0x80) is BSY (Busy), Bit 3 (0x08) is DRQ (Data Request)
    while ((port_byte_in(ATA_PRIMARY_STATUS) & ATA_STATUS_BSY) != 0);      // Wait while drive is busy
    while ((port_byte_in(ATA_PRIMARY_STATUS) & ATA_STATUS_DRQ) == 0);      // Wait until data request is ready

    // 6. Write 256 words (512 bytes) from the data port
    for (int i = 0; i < 256; i++) {
        port_word_out(ATA_PRIMARY_DATA, buffer[i]);
    }


     // Wait for the drive to finish physically committing the buffer to the disk plater
    while ((port_byte_in(ATA_PRIMARY_STATUS) & ATA_STATUS_BSY) != 0);



}
