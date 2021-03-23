/*===========================================================================*/
/*
SOURCE_FILE:		aa1024.h
APPLICATION:		DT03M-RF
MODULE:				eeprom
ORIGINAL AUTHOR:	thaodtn
VERSION:			1.0
DATE:				25/10/2013

(C) 2013: CPC IT

All rights are reserved. Reproduction in whole or in part is
prohibited without the prior written consent of the copyright
owner. The information presented in this document does not
form part of any quotation or contract, is believed to be
accurate and reliable and may be changed without notice.
No liability will be accepted by the publisher for any
consequence of its use. Publication thereof does not convey
nor imply any license under patent- or other industrial or
intellectual property rights.
*/
#ifndef _AA1024_H_
#define _AA1024_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
// Header prjEMETERS
#include "library.h"

#ifndef UINT64_SUPPORT
#include "../../Elster-RF/FW/FWELSTERRFX/driver/uint64.h"
#endif
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/


/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/
#define CHECK_WIP_BIT
#if defined(_DT03M_RF_) || defined(_DT01M_RF_)  || defined(_PREPAID_METER_) || defined(_ROUTER_)|| defined(_RF_SLOT_)
//chuyen sang function.h
//#define EEPROM_25AA512    0
//#define EEPROM_25AA1024   1
#if defined(_DT01M_RF_)
#define T_wc              6//6//10 // 6 ms - tuantm
#else
#define T_wc              10//6//10 // 6 ms - tuantm
#endif
#elif defined(_DT03P_RF_)
//chuyen sang function.h
//#define EEPROM_25AA512    1
//#define EEPROM_25AA1024   0
#define T_wc              5
#else
#warning "check meter type define in aa1024.h"
#endif

#define EEPROM_1    1
#define EEPROM_2    2
#define EEPROM_3    3

//------------------- Defination for EEPROM Address ----------------------------
#if defined(EEPROM_25AA1024)
#define EEPROM_PAGE_SIZE                256
#elif defined(EEPROM_25AA128)
#define EEPROM_PAGE_SIZE                64
#elif defined(EEPROM_25AA640)
#define EEPROM_PAGE_SIZE                32
#endif
#define EEPROM_SIZE                     131072 //byte

// AA512 commands
#define AA512_READ          0x03
#define SECTOR_ERASE        0xD8
#define PAGE_ERASE          0x42
#define CHIP_ERASE          0xC7
#define AA512_PROGRAM       0x02
#define AAI_PROGRAM         0xAF
#define AA512_RDSR          0x05   //READ_STATUS_REGISTER
#define AA512_EWSR          0x50   //ENABLE_WRITE_STATUS_REGISTER
#define AA512_WRSR          0x01   //WRITE_STATUS_REGISTER
#define AA512_WREN          0x06   //WRITE_ENABLE
#define AA512_WRDI          0x04   //WRITE_DISABLE
#define READ_ID             0x90   //0xAB
#define AA512_SLEEP			0xB9 //tuantm
#define AA512_WAKEUP        0xAB
/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/
extern int aa1024_chip_enable(unsigned char eepromIndex);
extern int aa1024_chip_disable(unsigned char eepromIndex);
extern unsigned char checksum(void *buf, int len);
extern unsigned char aa1024_read_status(unsigned char eepromIndex);
extern int aa1024_write_status(char status, unsigned char eeprom);
extern int aa1024_erase(unsigned char eepromIndex);
extern void aa1024_check_write_complete(unsigned char eepromIndex);
extern int aa1024_write_byte(uint32_t addr, uint8_t value, uint8_t eepromIndex);
extern unsigned char aa1024_read_byte(uint32_t addr,uint8_t eepromIndex);
extern int aa1024_write_2byte(uint32_t addr, uint16_t value, uint8_t eepromIndex);
extern uint16_t aa1024_read_2byte(uint32_t addr, uint8_t eepromIndex);
extern int aa1024_write_3byte(uint32_t addr, uint32_t value,uint8_t eepromIndex);
extern uint32_t aa1024_read_3byte(uint32_t addr, uint8_t eepromIndex);
extern int aa1024_write_4byte(uint32_t addr, uint32_t value, uint8_t eepromIndex);
extern uint32_t aa1024_read_4byte(uint32_t addr, uint8_t eepromIndex);
extern int aa1024_write_8byte(uint32_t addr, uint64_t value, uint8_t eepromIndex);
extern uint64_t aa1024_read_8byte(uint32_t addr, uint8_t eepromIndex);
extern int aa1024_write_burst_pages(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern int aa1024_write_burst_pages_nocrc(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern int aa1024_read_burst(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern void aa1024_sleep_mode(unsigned char eepromIndex);
extern void aa1024_wake_up(uint8_t eeprom);
/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
		-
		-
*/

#endif /* _AA1024_H_ */





