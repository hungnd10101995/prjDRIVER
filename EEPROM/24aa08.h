#ifndef _24AA08_H_
#define _24AA08_H_

#include <stdint.h>

#if defined(_DT01P_RF_) && defined(__MSP430__)
#include "..\..\dt01p80-rf\fw\emeter-main.h"
#include "..\..\library\1-phase-measurement\measure-struct.h"
#endif
#if defined(_DT01P_RF_) && defined(_18F86J94)
#include "..\..\dt01p80-rf\fw\main.h"
#endif
#if defined(_DT01P_RF_) && defined(_PIC16XX1919Y_) //namhv 01/11/2019
#include "../../dt01p-rf/fw/emeter-main.h"
#endif

#ifdef _PREPAID_METER_
#include "..\..\prepaid-meter\fw\emeter\emeter-main.h"
#include "..\..\library\1-phase-measurement\measure-struct.h"
#endif

/*===========================================================================*/
#define T_wc        3

#define EEPROM_1    1
#define EEPROM_2    2
#define EEPROM_3    3

#define  KWH_COMPONENT            0xA8
#define  PARA_COMPONENT           0xAA
#define  RFMESH_COMPONENT         0xAE //8
#define  EMETERID_ADDR            0x00
#define  PROGPASS_ADDR            0x10
#define  KWHWRAP_ADDR             0x20
#define  PROGTIMES_ADDR           0x30
#define  CURRPAGE_ADDR            0x40
#define  PAGE_MAX                 0x10
#define  KWHPULSE_COUNTER_ADDR    0x50 //0x148
//#define  BACKUP_OFFSET            0x60 //8
#define  EEPROM_PAGE_MAX       0x06 // 0x10 //namhv 6 hay 5?
// #define  FLAG_SAVE_DATA_FORWARD        0x0A
// #define  FLAG_SAVE_DATA_REVERSE        0x0C

// #ifdef NEUTRAL_ENEGRY_SUPPORT
	// #define  FLAG_SAVE_NEUTRAL_DATA_FORWARD        0x0D
	// #define  FLAG_SAVE_NEUTRAL_DATA_REVERSE        0x0E
// #endif

#define  RFMESH_STT               0x00 //8
#define  RFMESH_MASTERID          0x10 //8
#define  RFMESH_PATH              0x20 //8

///#define EX_EEPROM
#ifndef EX_EEPROM
#define IN_EEPROM
#endif


//#define EEPROM_Power_L   (P6OUT &= ~BIT2)
//#define EEPROM_Power_H   (P6OUT |=  BIT2)

//#define EEPROM_METER_ID                 1   tuantm 15/12/2014

#define EEPROM_ENERGY_TOTAL_ACTIVE      20
//#define EEPROM_ENERGY_TOTAL_REVERSE_ACTIVE    25 tuantm 15/12/2014


#define EEPROM_ENERGY_TOTAL_ACTIVE_COUNT      45
//#define EEPROM_ENERGY_TOTAL_REVERSE_ACTIVE_COUNT    47 tuantm 15/12/2014

/*******************RFMESH*******************/
#define RFMESH_STATUS_ADDR        0x080
#define RFMESH_MASTERID_ADDR      0x081 //4 byte
//write
//------------------- Defination for EEPROM Address ---------------------------
#define MONTH_1                 1
#define MONTH_2                 2
#define MONTH_3                 3
#define MONTH_4                 4
#define MONTH_5                 5       
#define MONTH_6                 6
#define MONTH_7                 7
#define MONTH_8                 8
#define MONTH_9                 9
#define MONTH_10                10
#define MONTH_11                11
#define MONTH_12                12
#define MONTH_MAINTAIN_1        13
#define MONTH_MAINTAIN_2        14
#define MONTH_MAINTAIN_3        15
#define MONTH_MAINTAIN_4        16

#define NO_DAY  NULL
#define DAY_1           1
#define DAY_2           2
#define DAY_3           3
#define DAY_4           4
#define DAY_5           5
#define DAY_6           6
#define DAY_7           7
#define DAY_8           8
#define DAY_9           9
#define DAY_10          10
#define DAY_11          11
#define DAY_12          12
#define DAY_13          13
#define DAY_14          14
#define DAY_15          15
#define DAY_16          16
#define DAY_17          17
#define DAY_18          18
#define DAY_19          19
#define DAY_20          20
#define DAY_21          21
#define DAY_22          22
#define DAY_23          23
#define DAY_24          24
#define DAY_25          25
#define DAY_26          26
#define DAY_27          27
#define DAY_28          28
#define DAY_29          29
#define DAY_30          30
#define DAY_31          31
#define DAY_MAINTAIN    32

//#define TOTAL_AREA_BILLING    80

//#ifdef LOG_RECORD_SUPPORT
//    // #define LOG_EVENT_COUNT_ADDR           0x7B0
//    // #define LOG_EVENT_ADDR                 0x7C0
//    // #define LOG_EVENT_OVERFLOW_COUNT_ADDR  0x7B8
//    #define ADDR_INDEX_TAMPER               0x7C0
//    #define ADDR_COUNT_TAMPER               (ADDR_INDEX_TAMPER + 2)
//    #define ADDR_INDEX_WARNING              (ADDR_COUNT_TAMPER + 3)
//    #define ADDR_COUNT_WARNING              (ADDR_INDEX_WARNING + 2)
//    #define ADDR_EVENT_TAMPER               0x7E0
//    #define ADDR_EVENT_WARNING              0x800
//    
//    #define ADDR_INDEX_TAMPER_BACKUP        0x820
//    #define ADDR_COUNT_TAMPER_BACKUP        (ADDR_INDEX_TAMPER_BACKUP + 2)
//    #define ADDR_INDEX_WARNING_BACKUP       (ADDR_COUNT_TAMPER_BACKUP + 3)
//    #define ADDR_COUNT_WARNING_BACKUP       (ADDR_INDEX_WARNING_BACKUP + 2)
//#endif
#ifdef DISPLAY_CONFIG_SUPPORT
#define AUTO_DISPLAY_LIST_ADDR             LOG_EVENT_ADDR + (MAX_EVENT_RECORD/2)*16      
#endif

//#ifndef BILLING_PERIOD_SUPPORT
//#warning "undef BILLING_PERIOD_SUPPORT in 24aa08.h"
//#endif

///////////// STRUCT ////////////////////////
//#ifdef BILLING_RF_SUPPORT
//typedef struct
//{
//	uint32_t         mUTC;
//	uint32_t         mImport;
//	uint32_t	        mExport;
//	uint8_t	        mMaintain;
//	uint8_t 	        mWarning;
//	uint8_t 	        mTemper;
//	uint8_t	        mCRC ;
//}LATCH_VALUE;
//
////////////////// VARIABLE /////////////////////////////
////extern LATCH_VALUE billing_data;
//#endif
///////////////// FUNCTON ////////////////////////////
extern void ic24aa08_write_2byte(unsigned int address, int data,uint8_t eepromIndex);
extern void ic24aa08_write_3byte(unsigned int address, unsigned long data,uint8_t eepromIndex);
extern void ic24aa08_write_4byte(unsigned int address, unsigned long data,uint8_t eepromIndex);
extern void ic24aa08_write_byte(unsigned int address, uint8_t data, uint8_t eepromIndex);//tuantm
extern void ic24aa08_write_buffer(unsigned int address, uint8_t *buffer, uint8_t length, uint8_t eepromIndex);
//read
extern unsigned char ic24aa08_read_byte(unsigned int address,uint8_t eepromIndex); //tuantm - phuc vu rfmesh
extern unsigned long ic24aa08_read_4byte(unsigned int address,uint8_t eepromIndex);
extern int ic24aa08_read_2byte(unsigned int address, uint8_t eepromIndex);
extern unsigned long ic24aa08_read_3byte(unsigned int address, uint8_t eepromIndex);
extern void ic24aa08_read_buffer(unsigned int address,uint8_t *buffer_data, uint8_t length, uint8_t eepromIndex);
//other
extern unsigned char eeprom_24aa08_write_byte(uint16_t addr, uint8_t value, uint8_t number_check);
extern unsigned char eeprom_24aa08_write_2byte(uint16_t addr, uint16_t value, uint8_t number_check);
extern unsigned char eeprom_24aa08_write_4byte(uint16_t addr, uint32_t value, uint8_t number_check);
extern void eeprom_data_save_task(void); //tuantm
#if defined(ENERGY_5_INT_1_DECIMAL_SUPPORT)
unsigned char eeprom_24aa08_get_index(uint32_t value_data);
#endif
extern void ic24aa08_load_parameters(void);
//extern unsigned char read_ext_eeprom(unsigned int address); //tuantm - phuc vu rfmesh

//them phan nay cho RFmesh version 2.0
extern void save_rfmesh_stt (unsigned char stt);
extern unsigned char load_rfmesh_stt();
extern unsigned long load_rfmesh_masterID (void);
extern void save_rfmesh_masterID (unsigned long id);
extern unsigned char save_path_eeprom();
extern unsigned char load_path_eeprom();

#endif //_24AA08_H_
