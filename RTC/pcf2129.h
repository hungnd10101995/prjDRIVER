/*===========================================================================*/
/*
SOURCE_FILE:		pcf2129.h
APPLICATION:		RTC
MODULE:				RTC
ORIGINAL AUTHOR:	thaodtn
VERSION:			2.0
DATE:				18/10/2016
*/
#ifndef _PCF2129_H_
#define _PCF2129_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Header prjEMETERS
#include "library.h"
#include "main.h"
#include "hardware.h"


#ifdef PCF2129_SUPPORT
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
//typedef struct
//{
//    unsigned char second;
//    unsigned char minute;
//    unsigned char hour;
//    unsigned char day;
//    unsigned char weekday;
//    unsigned char month;
//    unsigned char year;
//    unsigned long utc;
//    struct 
//    {
//        unsigned char new_hour:1;
//        unsigned char new_day:1;
//        unsigned char new_month:1;
//        unsigned char new_year:1;
//        unsigned char set_time:1;
//        unsigned char set_date:1;
//        unsigned char same_year:1;
//        unsigned char same_month:1;
//#ifdef RTC_DISPLAY_SUPPORT
//        unsigned char display:1;
//#endif
//        //unsigned char update_time:1;
//        //unsigned char set_time_date:1;
//        //unsigned char check_time:1;
//    }flags;
//} RTC_STRUCT;
//
//typedef struct
//{
//    unsigned char second;
//    unsigned char minute;
//    unsigned char hour;
//    unsigned char day;
//    unsigned char weekday;
//    unsigned char month;
//    unsigned char year;
//    unsigned long utc;
//    unsigned char month_reverse;
//    unsigned char day_reverse; // so ngay bi nguoc thoi gian so voi thoi gian hien tai cua meter
//#ifdef BILLING_DAY
//    unsigned char billing_hour;
//    unsigned char billing_minute;
//    unsigned char billing_second;
//#endif
////#ifdef BILLING_REQUEST
////    unsigned char billing_request_hour;
////    unsigned char billing_request_minute;
////    unsigned char billing_request_second;
////    unsigned char billing_request_weekday;
////    unsigned char billing_request_day;
////    unsigned char billing_request_month;
////    unsigned char billing_request_year;
////#endif
//    struct 
//    {
//        unsigned char new_hour:1;
//        unsigned char new_day:1;
//        unsigned char new_month:1;
//        unsigned char new_year:1;
//        unsigned char set_time:1;
//        unsigned char set_date:1;
//        unsigned char same_year:1; // co nay bat khi dong bo lai thoi gian co nam = nam cua meter
//        unsigned char same_month:1; // co nay bat khi dong bo lai thoi gian co thang = thang cua meter
//#ifdef RTC_DISPLAY_SUPPORT
//        unsigned char display:1;
//#endif
//        //unsigned char update_time:1;
//        //unsigned char set_time_date:1;
//        //unsigned char check_time:1;
//    }flags;
//}RTC_STRUCT;
//extern RTC_STRUCT rtc_2129;

/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/
/////////////////////////////////////////////////////////////
// MCU <-> RTC
#define RTC_PxSEL         P3SEL
#define RTC_SDI_PIN       BIT1
#define RTC_SDO_PIN       BIT2
#define RTC_SCL_PIN       BIT3
#define RTC_CS_PxOUT      P5OUT
#define RTC_CS_PxDIR      P5DIR
#define RTC_CS_PIN        BIT6
/////////////////////////////////////////////////////////////
// cac thanh ghi cua PCF2129
#define CONTROL_1     0x00
#define CONTROL_2     0x01
#define CONTROL_3     0x02
#define SECONDS       0x03
#define MINUTES       0x04
#define HOURS         0x05
#define DAYS          0x06
#define WEEKDAYS      0x07
#define MONTHS        0x08
#define YEARS         0x09
#define AM            0x00
#define PM            0x20
#define SECOND_ALARM  0x0A
#define MINUTE_ALARM  0x0B
#define HOUR_ALARM    0x0C
#define DAY_ALARM     0x0D
#define WEEKDAY_ALARM 0x0E
#define WATCHDG_CTL   0x10
#define WATCHDG_VAL   0x11
#define CLKOUT_ctl    0x0F
#define Aging_offset  0x19
//////////////////////////////////////////////////////
// PCF2129 command
#define WRITE         0x20
#define READ          0xA0
/////////////////////////////////////////////////////
#define AEN_S         0x80 // disable second alarm(thanh ghi SECOND_ALARM)
#define AEN_M         0x80 // disable minute alarm(thanh ghi MINUTE_ALARM)
#define AEN_H         0x80 // disable hour alarm(thanh ghi HOUR_ALARM)
#define AEN_D         0x80 // disable day alarm(thanh ghi DAY_ALARM)
#define AEN_W         0x80 // diasble weekday alarm(thanh ghi WEEKDAY_ALARM)
////////////////////////////////////////////////////////////////////////////
// cac bit cua thanh ghi control_1
#define EXT_TEST      0x80 //external clock test mode
#define PCF2129_STOP          0x20 //stop RTC clock
#define TSF1          0x10 //flag set when TS input is driven to an intermediate level between the power and ground
#define POR_OVRD      0x08 //power on reset override is enabled
#define MI            0x02 //enable minute interrupt
#define SECOND_INT    0x01 //enable second interrupt
#define MODE_12H      0x04
#define MODE_24H      0x00
///////////////////////////////////////////////////////////////////////////
// cac bit cua thanh ghi control_2
#define MSF           0x80 //minute or second interrupt flag
#define WDTF          0x40 //watchdog timer interrupt or reset flag
#define TSF2          0x20 //flag set when TS input is LOW
#define AF            0x10 //alarm trigger flag
#define TSIE          0x04 //enable interrupt generated when timestamp flag set
#define AIE           0x02 //enable interrupt generated when alarm flag set
////////////////////////////////////////////////////////////////////////////
// cac bit cua thanh ghi control_3
#define PWRMNG0       0x00 //battery switch-over function enable in standard mode, battery low,extra power fail detection enable
#define PWRMNG1       0x02 //battery switch-over function enable in standard mode, battery low,extra power fail detection disable
#define PWRMNG2       0x03 //battery switch-over function enable in direct switching mode, battery low,extra power fail detection disable
#define PWRMNG3       0x05 //battery switch-over function enable in direct switching mode, battery low,extra power fail detection disable
#define PWRMNG4       0x07 //battery switch-over function, battery low,extra power fail detection disable, only use VDD
#define BTSE          0x10 //time-stamped when the batterry switch-over occurs
#define BF            0x08 //battery switch-over flag
#define BLF           0x04 //battery status low flag
#define BIE           0x02 //enable interrupt generated when BF is set
#define BLIE          0x01 //enable interrupt generated when BLF is set
/////////////////////////////////////////////////////////////
// xoa co trong cac thanh ghi control cua RTC
#define CLR_MSF       0x30
#define CLR_AF        0xA0

/////////////////////////////////////////////////////////////////////////////
// cac ngay trong tuan(thanh ghi WEEKDAYS)
#define SUNDAY        0x00
#define MONDAY        0x01
#define TUESDAY       0x02
#define WEDNESDAY     0x03
#define THURSDAY      0x04
#define FRIDAY        0x05
#define SATURDAY      0x06

//CS
// cac define IO dinh nghia trong hardware.h
//#define pcf2129_cs_dir_output()             _pcf2129_cs_dir_output() 
//#define pcf2129_cs_low()                    _pcf2129_cs_low()
//#define pcf2129_cs_high()                   _pcf2129_cs_high()

//intA
#if defined(__PIC24FJxxxGA406__)
#define pcf2129_interrupt_select_pin()      _pcf2129_interrupt_select_pin()
#endif

#define pcf2129_select()       { spi_disable();spi_sclk_low(); pcf2129_cs_low();}
#define pcf2129_de_select()    { spi_sclk_low(); pcf2129_cs_high();}

/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/
//extern void rtc_interrupt_enable(void);
//extern void rtc_interrupt_disable(void);
extern void pcf2129_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode);
extern void pcf2129_set_second_alarm(unsigned char second);
extern void pcf2129_set_minute_alarm(unsigned char minute);
extern void pcf2129_set_hour_12h_alarm(unsigned char hour, unsigned char mode);
extern void pcf2129_set_hour_24h_alarm(unsigned char hour);
extern void pcf2129_set_day_alarm(unsigned char day);
extern void pcf2129_set_weekday_alarm(unsigned char weekday);
extern void pcf2129_enable_mi(void);
extern void pcf2129_init(void);
extern void pcf2129_set_time(unsigned char hour, unsigned char minute, unsigned char second);
extern void pcf2129_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year);
extern void pcf2129_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year);
extern void pcf2129_read_flag(void);
extern void pcf2129_clr_flag(unsigned char flag);
//extern void  pcf2129_task(void);


/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/


/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/
#endif /* _PCF2129_H_ */


#endif //PCF2129_SUPPORT