/*===========================================================================*/
/*
SOURCE_FILE:		rs512.h
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
#ifndef _RS512_H_
#define _RS512_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include "..\library.h"
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
//#warning "check meter type define in aa1024.h"
#endif

#define FRAM_1    1
#define FRAM_2    2
#define FRAM_3    3

//------------------- Defination for FRAM Address ----------------------------
#if defined(FRAM_RS1024)
//#define FRAM_PAGE_SIZE                  256
#define FRAM_SIZE                       131072 //byte
#elif defined(FRAM_RS512)
//#define FRAM_PAGE_SIZE                  256
#define FRAM_SIZE                       65536 //byte
#elif defined(EEPROM_RS16)
//#define EEPROM_PAGE_SIZE                32
#define EEPROM_SIZE                     2048 //byte
#endif

// AA512 commands
#define RS512_READ          0x03  //FRAM
//#define SECTOR_ERASE        0xD8
//#define PAGE_ERASE          0x42
//#define CHIP_ERASE          0xC7
#define RS512_PROGRAM       0x02  //FRAM-WRITE
//#define AAI_PROGRAM         0xAF
#define RS512_RDSR          0x05   //READ_STATUS_REGISTER//FRAM
//#define RS512_EWSR          0x50   //ENABLE_WRITE_STATUS_REGISTER
#define RS512_WRSR          0x01   //WRITE_STATUS_REGISTER//FRAM
#define RS512_WREN          0x06   //WRITE_ENABLE//FRAM
#define RS512_WRDI          0x04   //WRITE_DISABLE//FRAM
#define RS512_RDID          0x9F   //0xAB//FRAM
#define RS512_SLEEP			0xB9   //FRAM
#define RS512_FSTRD			0x0B   //FRAM
//#define RS512_WAKEUP        0xAB
/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/
extern int rs512_chip_enable(unsigned char eepromIndex);
extern int rs512_chip_disable(unsigned char eepromIndex);
//extern unsigned char checksum(void *buf, int len);
extern unsigned char rs512_read_status(unsigned char eepromIndex);
extern int rs512_write_status(char status, unsigned char eeprom);
extern int rs512_erase(unsigned char eepromIndex);
extern void rs512_check_write_complete(unsigned char eepromIndex);
extern int rs512_write_byte(uint32_t addr, uint8_t value, uint8_t eepromIndex);
extern unsigned char rs512_read_byte(uint32_t addr,uint8_t eepromIndex);
extern int rs512_write_2byte(uint32_t addr, uint16_t value, uint8_t eepromIndex);
extern uint16_t rs512_read_2byte(uint32_t addr, uint8_t eepromIndex);
extern int rs512_write_3byte(uint32_t addr, uint32_t value,uint8_t eepromIndex);
extern uint32_t rs512_read_3byte(uint32_t addr, uint8_t eepromIndex);
extern int rs512_write_4byte(uint32_t addr, uint32_t value, uint8_t eepromIndex);
extern uint32_t rs512_read_4byte(uint32_t addr, uint8_t eepromIndex);
extern int rs512_write_8byte(uint32_t addr, uint64_t value, uint8_t eepromIndex);
extern uint64_t rs512_read_8byte(uint32_t addr, uint8_t eepromIndex);
extern int rs512_write_burst_pages(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern int rs512_write_burst_pages_nocrc(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern int rs512_read_burst(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex);
extern void rs512_sleep_mode(unsigned char eepromIndex);
extern void rs512_wake_up(uint8_t eeprom);
/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
		-
		-
*/

#endif /* _AA1024_H_ */





