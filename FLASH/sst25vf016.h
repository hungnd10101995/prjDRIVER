/* 
 * File:   sst25vf016.h
 * Author: THAODTN
 *
 * Created on September 23, 2014, 11:18 PM
 */

#ifndef SST25VF016_H
#define	SST25VF016_H
#include "stdint.h"

//Header prjLIB
#include "library.h"
#include "main.h"
#include "hardware.h"


#define FLASH_1     1
#define FLASH_2     2
#define SECTOR_LENGTH                   4096
//---------------------- SST25 Instruction Set -----------------------
#define SST25_WREN                      0x06    // Write Enable
#define SST25_WRDI                      0x04    // Write Disable
#define SST25_WRSR                      0x01    // Write Status Register
#define SST25_CHIP_ERASE                0x60    // Full Chip Erase
#define SST25_SECTOR_ERASE              0x20    // 4 KByte Sector-Erase
#define SST25_BLOCK32_ERASE             0x52    // 32 KByte Block-Erase
#define SST25_BLOCK64_ERASE             0xD8    // 64 KByte Block-Erase
#define SST25_RDSR                      0x05    // Read Status Register
#define SST25_READ                      0x03    // Read memory up to 25MHz
#define SST25_HSREAD                    0x0B    // High Speed read up to 80MHz
#define SST25_WRITE                     0x02    // Write memory
#define SST25_AAIP                      0xAD    // Auto Address Increment Programming
#define SST25_EWSR                      0x50    // Enable Write Status Register
#define SST25_EBSY                      0x70    // enable SO to output RY/BY# status during AAI
#define SST25_DBSY                      0x80    // disable SO to output RY/BY# status during AAI
#define SST25_UNPROTECT                 0x00    // Byte that must be transfered to the Status register to unprotect the memory
#define SST25_BUSY                      0x01    // Mask for the Busy Bit on the status register
#define SST25_RDID                      0x90    // Read ID
#define SST25_JEDECID                   0x9F    // Jecdec ID read


//------------------------------ MACROS -------------------------------

#define SST25_ADDRESS_START_HEXRECORD       SST25_UPDATE_FW_DATA
//------------------------------ chia thanh 3 vung -----------------------------
#define UPDATE_FW_PARTITION         0x01
#define UPDATE_FW_START_ADDRESS     0x000000

#define SPIDER_PATH_PARTITION       0x02
#define SPIDER_PATH_START_ADDRESS   0x0B0000

#define METER_INFO_PARTITION        0x03
#define METER_INFO_START_ADDRESS    0x100001

#define ERASE_ALL_PARTITION         0x04

extern unsigned char flag_flash_error;

/* Function Prototypes */
void sst25_wait_busy(char flashIndex);
extern void sst25_init(void);
extern void sst25_unblock_write_protection(char flashIndex);
extern void sst25_block_write_protection(char flashIndex);
extern void sst25_write_byte(uint32_t address, uint8_t value, char flashIndex);
extern void sst25_write_burst(uint32_t address, uint8_t *buffer, uint32_t length, uint8_t writeCRC, char flashIndex);
extern void sst25_write_burst_pages(uint32_t addr, uint8_t *buffer, uint32_t len, uint8_t writeCRC, char flashIndex);
extern unsigned char sst25_read_byte(uint32_t address, char flashIndex);
extern void sst25_read_burst(uint32_t address, uint8_t *buffer, uint32_t no_bytes, uint8_t checkCRC, char flashIndex);
extern void sst25_highspeed_read_burst(uint32_t address, uint8_t *buffer, uint32_t no_bytes, char flashIndex);
extern void sst25_chip_erase(char flashIndex);
extern void sst25_chip_erase_partition(uint8_t partition, char flashIndex);
void sst25_wait_busy_AAI(char flashIndex);
extern void sst25_block_erase_64K(uint32_t address, char flashIndex);
extern void sst25_sector_erase(uint32_t address, char flashIndex);
extern void sst25_block_erase_32K(uint32_t address, char flashIndex);
extern void sst25_write_4byte(uint32_t address, uint32_t value,char flashIndex);
extern uint32_t sst25_read_4byte(uint32_t address,char flashIndex);
#endif	/* SST25VF016_H */

