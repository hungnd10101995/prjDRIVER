/*===========================================================================*/
/*
SOURCE_FILE:		rtc.c
APPLICATION:		RTC
MODULE:				RTC
ORIGINAL AUTHOR:	thaodtn
VERSION:			2.0
DATE:				21/10/2016
*/
#ifndef _RTC_H_
#define _RTC_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Header prjEMETERS
#include "library.h"
#include "hardware.h"

// Header prjLIB
#include "pcf2129.h"

#if defined(RTC_SUPPORT) || defined(PCF2129_SUPPORT) || defined(INT_RTC_SUPPORT)
//----------------------------------------------------------------------------//
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
#define rtc_cs_dir_output()             pcf2129_cs_dir_output()
#define rtc_cs_low()                    pcf2129_select() //pcf2129_cs_low() Vuonglq quy hoach goi PCF2129
#define rtc_cs_high()                   pcf2129_de_select()//pcf2129_cs_high() Vuonglq quy hoach goi PCF2129
//intA
//#define rtc_interrupt_select_pin()    pcf2129_interrupt_select_pin()
#define rtc_interrupt_dir_input()       pcf2129_interrupt_dir_input()
#define rtc_interrupt_input()           pcf2129_interrupt_input()
#define rtc_interrupt_pullup_enable()   pcf2129_interrupt_pullup_enable()
#define rtc_interrupt_pullup_disable()  pcf2129_interrupt_pullup_disable()
#define rtc_interrupt_pullup()          pcf2129_interrupt_pullup()
//#define rtc_interrupt_enable()          pcf2129_interrupt_enable()
//#define rtc_interrupt_disable()         pcf2129_interrupt_disable()
#define rtc_interrupt_low_to_high()     pcf2129_interrupt_low_to_high()
#define rtc_interrupt_high_to_low()     pcf2129_interrupt_high_to_low()
#define rtc_interrupt_flag_check()      pcf2129_interrupt_flag_check()
#define rtc_interrupt_flag_clear()      pcf2129_interrupt_flag_clear()

typedef struct
{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char weekday;
    unsigned char month;
    unsigned char year;
    unsigned long utc;
    unsigned int  year_long;
    #ifdef BILLING_DAY
    unsigned char month_reverse;
    unsigned char day_reverse; // so ngay bi nguoc thoi gian so voi thoi gian hien tai cua meter    
    unsigned char billing_hour;
    unsigned char billing_minute;
    unsigned char billing_second;
    #endif
    #ifdef RTC_DISPLAY_SUPPORT
    unsigned char count_display_date;
    unsigned char count_display_time;
    #endif
#ifdef RTC_TIMER_SUPPORT
     struct {
         unsigned action : 1;
         unsigned start : 1;
         unsigned long counter;
	 }timer;
#endif
    struct 
    {
        unsigned char new_hour:1;
        unsigned char new_day:1;
        unsigned char new_month:1;
        unsigned char new_year:1;
        unsigned char set_time:1;
        unsigned char set_date:1;
        unsigned char same_year:1;
        unsigned char same_month:1;
        #ifdef RTC_DISPLAY_SUPPORT
        unsigned char display:1;        
        #endif
        #ifdef _DCU_
        unsigned char ff:1;
        #endif
        //unsigned char update_time:1;
        //unsigned char set_time_date:1;
        //unsigned char check_time:1;
    }flags;
}RTC_STRUCT;
//#endif

extern RTC_STRUCT datetime;
extern RTC_STRUCT lasttime; //thoi gian cu, truoc khi dong bo lai RTC

#ifdef BILLING_REQUEST
extern RTC_STRUCT rtc_billing_request;
#endif
/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/



/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/
extern unsigned char is_leap_year(uint32_t year);
extern unsigned char month2day(unsigned char month, unsigned char year);
extern unsigned long time_to_second(uint8_t hour, uint8_t minute, uint8_t second);
extern void rtc_interrupt_enable(void);
extern void rtc_interrupt_disable(void);
extern void rtc_init(void);
extern void rtc_set_time(unsigned char hour, unsigned char minute, unsigned char second);
extern void rtc_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year);
extern void rtc_read_time(void);
extern void rtc_read_flag(void);
extern void rtc_clr_flag(unsigned char flag);
extern void rtc_scan(void);
extern void  rtc_task(void);
extern unsigned char rtc_check_time(uint8_t hour, uint8_t minute, uint8_t second);
extern unsigned char rtc_check_date(uint8_t day, uint8_t month, uint32_t year);
extern unsigned char rtc_check_date_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day, uint8_t month, uint32_t year);
#ifdef BILLING_PERIOD_SUPPORT//BILLING_DAY
extern void rtc_check_sync_time(unsigned char hour_sync, unsigned char minute_sync, unsigned char second_sync);
extern void rtc_check_sync_date(unsigned char weekday_sync, unsigned char day_sync, unsigned char month_sync, unsigned char year_sync);
#endif

/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/


/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
	
*/
//----------------------------
#endif // #if defined(RTC_SUPPORT) || defined(PCF2129_SUPPORT) || defined(INT_RTC_SUPPORT)
//---------------------------
#endif /* _RTC_H_ */