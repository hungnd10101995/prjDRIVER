/*===========================================================================*/
/*
SOURCE_FILE:		datetimec
APPLICATION:		RTC
MODULE:				RTC
ORIGINAL AUTHOR:	thaodtn
VERSION:			2.0
DATE:				21/10/2016
 */
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Header prjEMETERS
#include "library.h"
#include "devices.h"
#include "main.h"

#if defined(RTC_SUPPORT) || defined(INT_RTC_SUPPORT) || defined(RTCC_SUPPORT)//endif to end of file
/*===========================================================================*/
#ifdef LOAD_PROFILE_SUPPORT
#include "load-profile.h"
#endif
#ifdef NEW_TARIFF_SUPPORT
#include "multi-tariff.h"
#endif
#ifdef OLD_TARIFF_SUPPORT
#include "emec-tariff.h"
#endif
#ifdef BILLING_SUPPORT
#include "billing.h"
#endif
#ifdef EVENT_SUPPORT
#include "events.h"
#endif
#ifdef EVENTS_LOG_SUPPORT
#include "..\events\event-log.h"
#endif
#ifdef BILLING_PERIOD_SUPPORT
#include "..\billing\billing_period.h"
#endif

#include "uart.h"
#include "spi.h"
#include "rtc.h"

#if defined(PCF2129_SUPPORT) 
#include "pcf2129.h"
#elif defined(INT_RTC_SUPPORT)
#include "rtc_int.h"
#elif defined(RTCC_SUPPORT)
#include "rtcc.h"
#elif defined(RTC_TIMER_SUPPORT)
#include "rtc_timer.h"
#endif

#include "delay.h"
#include "utc.h"
#include "process.h"

#ifdef DISPLAY_SUPPORT
#if defined(_LCD_OST11295_)  || defined(_LCD_L0712_)
#include "../lcd/lcd-segment-ost11295.h"
#elif defined(_LCD_OST11248_)
#include "../lcd/lcd-segment-ost11248.h"
#elif defined(_LCD_M0754v2_PCF8545_)
#include "../lcd/lcd-segment-pcf8545-m0754v2.h"
#elif defined(_LCD_M0754v3_PCF8545_)
#include "../lcd/lcd-segment-pcf8545-m0754v3.h"
#elif defined(_LCD_1602_)
#include "../lcd/lcd16x2.h"
#elif defined(_LCD_M0754_)
#include "../lcd/lcd-segment-m0754.h"
#elif defined(_LCD_M0752_)
#include "../lcd/lcd-segment-m0752.h"
#elif defined(_LCD_SEQ0460_)
#include "../lcd/lcd-segment-seq0460.h"
#elif defined(_LCD_M0753_)
#include "../lcd/lcd-segment-m0753.h"
#endif

#include "emeter-display.h"
#endif


////////////////////////////////////////////////////////////////////////////
/*   debug cac chuc nang ko duoc define    */
////////////////////////////////////////////////////////////////////////////
#ifndef TRANSFORMER_SUPPORT
#warning "undef TRANSFORMER_SUPPORT in pcf2129.c"
#endif
#ifndef RTC_SUPPORT
#warning "undef RTC_SUPPORT in pcf2129.c"
#endif
#ifndef IEC62056_21_SUPPORT
#warning "undef IEC62056_21_SUPPORT in pcf2129.c"
#endif
#ifndef TARIFF_SUPPORT
#warning "undef TARIFF_SUPPORT in pcf2129.c"
#endif
#ifndef DEFERRED_TARIFF_SUPPORT
#warning "undef DEFERRED_TARIFF_SUPPORT in pcf2129.c"
#endif
#ifndef MAX_DEMAND_SUPPORT
#warning "undef MAX_DEMAND in pcf2129.c"
#endif
#ifndef EVENT_SUPPORT
#warning "undef EVENT in pcf2129.c"
#endif
#ifndef LOAD_PROFILE_SUPPORT
#warning "undef LOAD_PROFILE in pcf2129.c"
#endif
#ifndef BILLING_SUPPORT
#warning "undef BILLING in pcf2129.c"
#endif
#ifndef CUSTOMER_DEFINED_REGS_SUPPORT
#warning "undef CUSTOMER_DEFINED_REGS in pcf2129.c"
#endif
#ifndef ALERT_SUPPORT
#warning "undef ALERT in pcf2129.c"
#endif
#ifndef CONFIG_UART
#warning "undef CONFIG_UART in pcf2129.c"
#endif
#ifndef RF_SUPPORT
#warning "undef RF in pcf2129.c"
#endif
#ifndef WOR_SUPPORT
#warning "undef WOR in pcf2129.c"
#endif
#ifndef LUU_XOAY_VONG_THANH_GHI_SUPPORT
#warning "undef LUU_XOAY_VONG_THANH_GHI in pcf2129.c"
#endif

/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
RTC_STRUCT datetime;
RTC_STRUCT lasttime; //thoi gian cu, truoc khi dong bo lai RTC
#ifdef BILLING_REQUEST
RTC_STRUCT rtc_billing_request;
#endif
/*===========================================================================*/
/*  G L O B A L   D A T A   D E F I N I T I O N S                            */
/*===========================================================================*/


/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/


/*===========================================================================*/
/* L O C A L   D A T A   D E F I N I T I O N S                               */
/*===========================================================================*/


/*===========================================================================*/
/* L O C A L   F U N C T I O N   P R O T O T Y P E S                         */

/*===========================================================================*/
unsigned char is_leap_year(uint32_t year)
{
    if (year % 400 == 0) return TRUE;
    else if (year % 100 == 0) return FALSE;
    else if (year % 4 == 0) return TRUE;
    else return FALSE;
}

unsigned char month2day(unsigned char month, unsigned char year)
{
    switch (month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            if (is_leap_year(2000 + year)) //tuantm sua lai ngay 25/8/2017
                return 29;
            else
                return 28;
        default:
            return 30;
    }
}

unsigned long time_to_second(uint8_t hour, uint8_t minute, uint8_t second)
{
    unsigned long s;
    if (rtc_check_time(hour, minute, second) == FALSE) return 0;
    s = hour * 3600 + minute * 60 + second;
    return s;
}
//===========================================================================
//FUNCTION NAME:    rtc_set_time_12h
//PROTOTYPE:        void rtc_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
//INPUT:            mode(AM hhoac PM), hour(1-12), minute(0-59), second(0-59)
//OUTPUT:
//DESCRIPTION:      ghi thoi gian(gio, phut, giay) theo mode 12h vao rtc
//NOTE:
//============================================================================

void rtc_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
{
#if defined(PCF2129_SUPPORT)
    pcf2129_set_time_12h(hour, minute, second, mode);
#elif defined(RTCC_SUPPORT)
    rtcc_set_time(hour, minute, second);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_second_alarm
//PROTOTYPE:        void rtc_read_time(void)
//INPUT:            second(0-59)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua giay ma khi thanh ghi SECONDS
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_second_alarm(unsigned char second)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_second_alarm(second);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_minute_alarm
//PROTOTYPE:        void rtc_set_minute_alarm(unsigned char minute)
//INPUT:            minute(0-59)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua phut ma khi thanh ghi MINUTES
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_minute_alarm(unsigned char minute)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_minute_alarm(minute);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_hour_12h_alarm
//PROTOTYPE:        void rtc_set_hour_12h_alarm(unsigned char hour, unsigned char mode)
//INPUT:            hour(1-12), mode(AM hoac PM)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua gio ma khi thanh ghi HOURS
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_hour_12h_alarm(unsigned char hour, unsigned char mode)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_hour_12h_alarm(hour, mode);
#endif    
}

//===========================================================================
//FUNCTION NAME:    rtc_set_hour_24h_alarm
//PROTOTYPE:        void rtc_set_hour_24h_alarm(unsigned char hour)
//INPUT:            hour(0-23)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua gio ma khi thanh ghi HOURS
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_hour_24h_alarm(unsigned char hour)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_hour_24h_alarm(hour);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_day_alarm
//PROTOTYPE:        void rtc_set_day_alarm(unsigned char day)
//INPUT:            day(0-31)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua ngay ma khi thanh ghi DAYS
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_day_alarm(unsigned char day)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_day_alarm(day);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_weekday_alarm
//PROTOTYPE:        void rtc_set_weekday_alarm(unsigned char weekday)
//INPUT:            weekday(SUNDAY....SATURDAY)
//OUTPUT:
//DESCRIPTION:      ghi vao rtc gia tri cua thu trong tuan ma khi thanh ghi WEEKDAYS
//                  cua rtc dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================

void rtc_set_weekday_alarm(unsigned char weekday)
{
#ifdef PCF2129_SUPPORT
    pcf2129_set_weekday_alarm(weekday);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_enable_mi
//PROTOTYPE:        void rtc_enable_mi(void)
//INPUT:            weekday(SUNDAY....SATURDAY)
//OUTPUT:
//DESCRIPTION:      khoi dong rtc cho phep ngat moi phut(cu sau moi phut chan INT keo xuong logic 0)
//NOTE:
//============================================================================

void rtc_enable_mi(void)
{
#ifdef PCF2129_SUPPORT
    pcf2129_enable_mi();
#endif
}

/*===========================================================================*/
/* G L O B A L   F U N C T I O N S   P R O T O T Y P E S                     */

/*===========================================================================*/


void rtc_interrupt_enable(void)
{
#if defined(PCF2129_SUPPORT)
    //cau hinh ngat
#if defined(__PIC24FJxxxGA406__) || defined(__PIC24FJxxxGA410__)
    pcf2129_interrupt_select_pin();
#endif    
    pcf2129_interrupt_dir_input();
    pcf2129_interrupt_pullup_disable();
    pcf2129_interrupt_high_to_low();
    pcf2129_interrupt_flag_clear();
    pcf2129_interrupt_enable();
#elif defined(RTCC_SUPPORT)
    rtcc_interrupt_flag_clear();
    rtcc_interrupt_enable();
#endif
}

void rtc_interrupt_disable(void)
{
#ifdef PCF2129_SUPPORT
    pcf2129_interrupt_flag_clear();
    pcf2129_interrupt_disable();
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_init
//PROTOTYPE:        unsigned char dec_to_bcd(unsigned char dec)
//INPUT:            none
//OUTPUT:           none
//DESCRIPTION:      khoi dong va config cho rtc
//NOTE:
//============================================================================

void rtc_init(void)
{
#if defined(PCF2129_SUPPORT)
    pcf2129_init();
    pcf2129_read_flag(); //doc flag de kiem tra het pin pcf2129 chua
#elif defined(INT_RTC_SUPPORT)
    rtc_int_init();
#elif defined(RTCC_SUPPORT)
    rtcc_init();
#elif defined(RTC_TIMER_SUPPORT)
	rtc_timer_init();
#endif
    rtc_read_time();
    datetime.utc = date2utc(&datetime);    
}

//===========================================================================
//FUNCTION NAME:    rtc_set_time
//PROTOTYPE:        void rtc_set_time(unsigned char hour, unsigned char minute, unsigned char second)
//INPUT:            'hour'(0-23),minute(0-59),second(0-59)
//OUTPUT:           none
//DESCRIPTION:      ghi thoi gian(gio, phut,giay) theo mode 24h vao rtc
//NOTE:
//============================================================================

void rtc_set_time(unsigned char hour, unsigned char minute, unsigned char second)
{
#if defined(PCF2129_SUPPORT)
    pcf2129_set_time(hour, minute, second);
#elif defined(RTCC_SUPPORT)
    rtcc_set_time(hour, minute, second);
#elif defined(INT_RTC_SUPPORT)
    rtc_int_set_time(hour, minute, second);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_set_date
//PROTOTYPE:        void rtc_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
//INPUT:            weekday(sunday...saturday), day(1-31), month(1-12), year(0-99)
//OUTPUT:
//DESCRIPTION:      ghi thoi gian(cac thu trong tuan(weekday), ngay(day), thang(month),nam(year)) vao rtc
//NOTE:
//============================================================================

void rtc_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year)
{
#if defined(PCF2129_SUPPORT)
    pcf2129_set_date(weekday, day, month, year);
#elif defined(RTCC_SUPPORT)
    rtcc_set_date(weekday, day, month, year);
#elif defined(INT_RTC_SUPPORT)
    rtc_int_set_date(weekday, day, month, year);
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_read_time
//PROTOTYPE:        void rtc_read_time(void)
//INPUT:            weekday(sunday...saturday), day(1-31), month(1-12), year(0-99)
//OUTPUT:
//DESCRIPTION:      doc thoi gian hien tai cua rtc(gio,phut, giay,...,nam),
//                  cac du lieu doc ve duoi dang bcd nen ta chuyen ve dang thap phan
//                  roi luu vao cac bien thoi gian.
//NOTE:
//============================================================================

void rtc_read_time(void)
{
    unsigned char cnt_tmp = 0;
    unsigned char cnt_check = 0;
    for(cnt_tmp = 0; cnt_tmp < 10; cnt_tmp++)
    {  
        nop();
        if(power_fail_detect_input()==0)
            cnt_check++;
        if(emeter.flags.low_voltage_detect == TRUE)
        {
            cnt_check = 10;
            cnt_tmp = 10;
        }
    }
    if((cnt_check > 4) || (emeter.flags.low_voltage_detect == TRUE)) return;
#if defined(PCF2129_SUPPORT)
    pcf2129_read_time(&datetime.second,
            &datetime.minute,
            &datetime.hour,
            &datetime.day,
            &datetime.weekday,
            &datetime.month,
            &datetime.year);    
#elif defined(RTCC_SUPPORT)
    //rtcc_read_time();
    rtcc_read_time(&datetime.second,
            &datetime.minute,
            &datetime.hour,
            &datetime.day,
            &datetime.weekday,
            &datetime.month,
            &datetime.year);
#elif defined(INT_RTC_SUPPORT)
    rtc_int_read_time(&datetime.second,
            &datetime.minute,
            &datetime.hour,
            &datetime.day,
            &datetime.weekday,
            &datetime.month,
            &datetime.year);
#endif
    datetime.year_long = 2000 + datetime.year;
}

//===========================================================================
//FUNCTION NAME:    rtc_read_flag
//PROTOTYPE:        void rtc_read_flag(void)
//INPUT:
//OUTPUT:
//DESCRIPTION:      doc cac thanh ghi control cua RTC, kiem tra co nao duoc set
//                  de nhan biet ngat(chan INT(cua RTC) = low)la do nguyen nhan nao cua RTC
//NOTE:
//============================================================================

void rtc_read_flag(void)
{
#ifdef PCF2129_SUPPORT
    pcf2129_read_flag();
#endif
}

//===========================================================================
//FUNCTION NAME:    rtc_clr_flag
//PROTOTYPE:        void rtc_clr_flag(unsigned char flag)
//INPUT:            flag(AF hoac MSF)
//OUTPUT:
//DESCRIPTION:      xoa cac co de xoa ngat(set chan INT cua rtc len 1)
//NOTE:
//============================================================================

void rtc_clr_flag(unsigned char flag)
{
#ifdef PCF2129_SUPPORT
    pcf2129_clr_flag(flag);
#endif
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_scan(void)
//  Created     : 15/05/2016, by thaodtn
//  Description : scan the state of intA if not 
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void rtc_scan(void)
{
#ifdef PCF2129_SUPPORT
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        if ((!rtc_interrupt_input())&&(emeter.flags.low_voltage_detect == FALSE))
        {
            restart_wdt();
            delay_us(10);
            emeter.flags.rtc_interrupt = TRUE;
        }
        else
        {
            i = 20;
            emeter.flags.rtc_interrupt = FALSE;
        }
    }
#endif
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_task(void)
//  Created     : 11/28/2014, by thaodtn
//  Description : ham xu ly cac cong viec khi co ngat 1 phut
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void rtc_task(void)
{
    #ifdef RTC_TIMER_SUPPORT
    rtc_timer_task();
    #endif
    if (emeter.task.update_time == 1)
    {
        emeter.task.update_time = 0;
        rtc_read_time();
        #ifdef DISPLAY_SUPPORT
        if (flag_display_TIME == TRUE)//if (emeter.task.display_time == 1)
        {
            lcd_hide_value();
            display_current_time();
            if (count_0_5s % 2)
            #if defined(_LCD_OST11248_) || defined(_LCD_M0752_)
                lcd_col(HIDE);
            #else
                lcd_COL2(0);
            #endif
        }
        #endif
#ifdef BILLING_PERIOD_SUPPORT
        #ifdef BILLING_DAY
        ///// kiem tra co phai sang ngay moi hay khong
        if ((datetime.hour == datetime.billing_hour) && (datetime.minute == datetime.billing_minute) && (datetime.second == datetime.billing_second))
        {
            if (datetime.flags.set_time == 0)
                billing_period.flags.day_action = TRUE;
        }
        else
        {
            if ((datetime.flags.set_time) || (datetime.flags.set_date))
            {
                datetime.flags.set_time = 0;
                datetime.flags.set_date = 0;
            }
            billing_period.flags.save_everyday = FALSE; // xoa co nay de thuc hien ky chot tiep theo  
        }
        #endif

        #ifdef BILLING_MONTH
        //////// kiem tra co phai sang thang moi hay khong
        if ((datetime.day == 1)&&(datetime.minute == 0)&&(datetime.second == 0)&&(datetime.hour == 0))
        {
            if ((datetime.flags.set_date == 0) || (datetime.flags.set_time == 0))
                billing_period.flags.month_action = TRUE;
        }
        else
        {
            if ((datetime.flags.set_time) || (datetime.flags.set_date))
            {
                datetime.flags.set_date = 0;
            }
            billing_period.flags.save_everymonth = FALSE; // xoa co nay de thuc hien ky chot tiep theo  
        }
        #endif

        #ifdef BILLING_REQUEST
        if (billing_period.flags.waitting == TRUE)
        {
            if (rtc_billing_request.utc <= date2utc(&datetime))
                billing_period.flags.waitting_action = TRUE;
        }
        #endif
#endif
    }
    //ngat RTC
    if (emeter.flags.rtc_interrupt == TRUE) // moi lan sang phut moi se set co ngat nay
    {
        emeter.flags.rtc_interrupt = FALSE;
        #ifdef PCF2129_SUPPORT
        pcf2129_read_flag(); //doc flag de kiem tra het pin pcf2129 chua
        pcf2129_clr_flag(MSF); // xoa co ngat minute
        #endif
        rtc_read_time();
        #ifdef BILLING_DAY //=======================================================================
        ///// kiem tra co phai sang ngay moi hay khong
        if ((datetime.hour == datetime.billing_hour) && (datetime.minute == datetime.billing_minute) && (datetime.second == datetime.billing_second))
        {
            if (datetime.flags.set_time == 0)
                billing_period.flags.day_action = TRUE;
        }
        else
        {
            if ((datetime.flags.set_time) || (datetime.flags.set_date))
            {
                datetime.flags.set_time = 0;
                datetime.flags.set_date = 0;
            }
            billing_period.flags.save_everyday = FALSE; // xoa co nay de thuc hien ky chot tiep theo  
        }
        #endif
        
        #ifdef BILLING_MONTH //=====================================================================
        //////// kiem tra co phai sang thang moi hay khong
        if ((datetime.day == 1)&&(datetime.minute == 0)&&(datetime.second == 0)&&(datetime.hour == 0))
        {
            if ((datetime.flags.set_date == 0) || (datetime.flags.set_time == 0))
                billing_period.flags.month_action = TRUE;
        }
        else
        {
            if ((datetime.flags.set_time) || (datetime.flags.set_date))
            {
                datetime.flags.set_date = 0;
            }
            billing_period.flags.save_everymonth = FALSE; // xoa co nay de thuc hien ky chot tiep theo  
        }
        #endif
        
        datetime.utc = date2utc(&datetime);
        
        #ifdef MAX_DEMAND_SUPPORT //================================================================
        //kiem tra khoang thoi gian max demand
        #ifdef MAX_DEMAND_PERIOD
        if (datetime.utc % tariff.max_demand.period == 0)
        #endif
        {
            //tariff.max_demand.flag = TRUE; thaodtn 18-09-2015: luu maxdemand truoc khi update lai bieu gia
            max_demand_proccess(datetime.utc);
            #ifdef MAX_DEMAND_PERIOD		
            if (power_fail_detect_input() &&(tariff.max_demand.update_flag == TRUE))
            {
                tariff.max_demand.update_flag = FALSE;
                //max_demand_init();
            }
            #else
            if (power_fail_detect_input())
            {
                max_demand_init(); // hoi lai Chi Thao 30/10/2018
            }
            #endif	
            #ifdef BILLING_DAY_MAX_DEMAND_SUPPORT
            billing_max_demand_proccess(datetime.utc);
            #endif
        }
        #endif

        #ifdef LOAD_PROFILE_SUPPORT //==============================================================
        //kiem tra thoi gian load profile
//        if (datetime.utc % load_profile.period == 0)
        if (load_profile.period_minute != 0)
        {
            if (datetime.minute % load_profile.period_minute == 0)
            {
                load_profile.proccess = TRUE;
                load_profile.stop_time = datetime.utc;
                #ifdef TEST_ERROR_LOADPROFILE
                loadprofile_test_error(1);
                #endif
            }
        }
        #endif
        
        #ifdef TARIFF_SUPPORT //====================================================================
        //tariff_update_active_rate(); // update cac thanh ghi co thay doi
    #ifdef DEFERRED_TARIFF_SUPPORT
        tariff_deferred_check(datetime.utc); //ham nay fai nam truoc ham tariff_update_switching_time();
    #endif
        //neu sang nam moi thi update lich am
        if ((datetime.day == 1)&&(datetime.month == 1))
        {
            tariff_update_lunar_holiday();
        }
        #endif
        //==========================================================================================
        //sang ngay moi: kiem tra update thoi diem chuyen mach bieu gia, ngay thuong/ngay le
        if ((datetime.hour == 0) && (datetime.minute == 0))
        {
            #ifdef NEW_TARIFF_SUPPORT            
            multi_tariffs_update_status();
            #endif
            
            #ifdef OLD_TARIFF_SUPPORT
            #ifdef DEFERRED_TARIFF_SUPPORT
            tariff_update_switching_time();
            #endif
            #ifdef SEASON_TARIFF_SUPPORT            
            tariff_season_update_switching_time();            
            #endif      
            #endif
            
            #ifdef BILLING_SUPPORT
            billing_check(&datetime);
            #endif
            
            #ifdef BILLING_DAY
            datetime.flags.new_day = TRUE;
            #endif
            
            #ifdef LOAD_PROFILE_SUPPORT
            if ((load_profile.config != 0)&&(load_profile.period != 0))
            {
                load_profile.flag = TRUE;
                #ifdef LOAD_PROFILE_LOG_SUPPORT
                lp_log.flags.new_date = TRUE;
                #endif
            }
            #endif
            
            #ifdef EVENTS_LOG_SUPPORT
            eventsLog.save_sector = TRUE;
            #endif
            
            #ifdef TEST_ERROR_LOADPROFILE
            loadprofile_test_error(2);
            #endif
        }
        restart_wdt();
        #ifdef TARIFF_SUPPORT
            //ham nay thuc hien sau de cap nhat lai bieu gia kich hoat sau khi doi switching time
            tariff_update_active_rate(); // update cac thanh ghi co thay doi
        #endif
    }

}

unsigned char rtc_check_time(uint8_t hour, uint8_t minute, uint8_t second)
{
    if (hour > 23) return FALSE;
    if (minute > 59) return FALSE;
    if (second > 59) return FALSE;
    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////
unsigned char rtc_check_date(uint8_t day, uint8_t month, uint32_t year)
{
    if ((day == 0) || (month == 0) || (year == 0)) return FALSE;
    if (month > 12) return FALSE;
    switch (month)
    {
        case 2:
            if (is_leap_year(year + 2000) == TRUE)//if ((year % 4) == 0) //nam nhuan
            {
                if (day > 29) return FALSE;
            }
            else
            {
                if (day > 28) return FALSE;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30) return FALSE;
            break;
        default:
            if (day > 31) return FALSE;
            break;
    }
    return TRUE;
}

unsigned char rtc_check_date_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day, uint8_t month, uint32_t year)
{
    if (rtc_check_time(hour, minute, second) == FALSE) return FALSE;
    if (rtc_check_date(day, month, year) == FALSE) return FALSE;
    return TRUE;
}
#ifdef BILLING_PERIOD_SUPPORT
////////////////////////////////////////////////////////////////////////////////
//  Function    : void check_sync_time(uint8_t hour_sync, uint8_t minute_sync, uint8_t second_sync)
//  Created     : 19/05/2015, by vinhld
//  Description : 
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void rtc_check_sync_time(unsigned char hour_sync, unsigned char minute_sync, unsigned char second_sync)
{
    unsigned char flag_set_time = 0;
    if (datetime.hour != hour_sync)
        flag_set_time = 1;
    if (datetime.minute != minute_sync)
        flag_set_time = 1;
    if (datetime.second != second_sync)
        flag_set_time = 1;
    if (flag_set_time == 1)
    {
        flag_set_time = 0;
        rtc_set_time(hour_sync, minute_sync, second_sync);
        datetime.hour = hour_sync;
        datetime.minute = minute_sync;
        datetime.second = second_sync;
    }
    if ((hour_sync == 0)&&(minute_sync == 0)&&(second_sync == 0))
        datetime.flags.set_time = 1;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : void rtc_check_sync_date(unsigned char weekday_sync, unsigned char day_sync, unsigned char month_sync, unsigned char year_sync)
//  Created     : 19/05/2015, by vinhld
//  Description : 
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void rtc_check_sync_date(unsigned char weekday_sync, unsigned char day_sync, unsigned char month_sync, unsigned char year_sync)
{
    //  if(rtc_check_date_time(day_sync, month_sync, year_sync) == TRUE)
    //  {
    //    led_status_off();
    if (datetime.day != day_sync)// ngay dong bo khac ngay hien tai cua meter
    {
        billing_period.flags.day_save_data = 1;
    }
    //////////////////////////////////////
    if (datetime.month != month_sync) // thang dong bo khac thang hien tai cua meter
    {
        billing_period.flags.day_save_data = 1;
        billing_period.flags.month_save_data = 1;
        datetime.flags.same_month = 0;
    }
    /////////////////////////////////////////
    if (datetime.year != year_sync) // nam dong bo khac voi nam cua meter
    {
        billing_period.flags.day_save_data = 1;
        billing_period.flags.month_save_data = 1;
        datetime.flags.same_year = 0;
    #ifdef BILLING_DAY
        datetime.month_reverse = 0;
        datetime.day_reverse = 0;
    #endif
    }
    else // datetimeyear = year_sync // nam dong bo = nam cua meter
    {
        datetime.flags.same_year = 1; // bat co nam dong bo = nam cua meter
#ifdef BILLING_MONTH
        /////////////////// kiem tra truong hop chot thang
        if (datetime.month > month_sync) // thang dong bo nho hon thang cua meter
        {
            datetime.month_reverse = datetime.month - month_sync;
        }
        else
            datetime.month_reverse = 0;
#endif
#ifdef BILLING_DAY
        /////////// kiem tra truong hop chot ngay/////
        if (datetime.month >= month_sync) // thang dong bo nho hon thang cua meter
        {
            switch (datetime.month - month_sync)
            {
                case 0: // cung thang
                    datetime.flags.same_month = 1; // bat co thang dong bo = thang cua meter
                    if (datetime.day > day_sync) // neu ngay dong bo nho hon ngay cua meter tuc la dang dong bo nguoc thoi gian
                        datetime.day_reverse = datetime.day - day_sync; // so ngay bi nguoc thoi gian tinh tu ngay dong bo den ngay cua meter
                        // vd :    -----------day_sync----------------------------------datetimeday------------------  
                    else // ngay dong bo lon hon ngay cua meter
                        datetime.day_reverse = 0; // ko co ngay bi nguoc thoi gian
                    // vd :    -----------rtcday--------------------------------------day_sync--------------          
                    break;
                case 1: // thang dong bo nho hon thang cua meter la 1 thang
                    datetime.day_reverse = 31 - day_sync + datetime.day;
                    // vd :    ----------------day_sync--------------1-----------------datetimeday--------------
                    //                    thang 12            ngay chuyen thang       thang 1
                    break;
                case 2: // thang dong bo nho hon thang cua meter la 2 thang
                    datetime.day_reverse = 31 - day_sync + 31 + datetime.day;
                    // vd :    ----------------day_sync----------------1------------------------1-----------------datetimeday--------------
                    //                    thang 11            ngay chuyen thang    12      ngay chuyen thang           thang 1
                    break;
                default: // thang cua meter lon hon thang dong bo 3 thang tro len
                    datetime.day_reverse = 62; // gan 1 gia tri nao do >= 62 la duoc, vi thang co 31 ngay, 2 thang lien tiep nhieu nhat co 62 ngay
                    break;
            }
        }
        else // thang dong bo lon hon thang cua meter
            datetime.day_reverse = 0; // khong co ngay bi thoi gian
#endif
    }
    /////////////////////////////////////////////
    if (year_sync < datetime.year) // nam dong bo nho nam cua meter
    {
        ///////////// thang chot//////////////////////////////
        if ((datetime.year - year_sync) > 1) // nam cua meter lon hon nam dong bo 2 nam tro len
        {
#ifdef BILLING_MONTH
            datetime.month_reverse = 12; // gan 1 gia tri nao >= 12 la duoc
#endif
#ifdef BILLING_DAY
            datetime.day_reverse = 62; // gan 1 gia tri nao do >= 62 la duoc, vi lon hon 60 ngay (2thang)
#endif
        }
        else // nam cua meter lon hon nam dong bo 1 nam
        {
    #ifdef BILLING_MONTH
            datetime.month_reverse = 12 - month_sync + datetime.month;
    #endif
    #ifdef BILLING_DAY            
            // vd :    ----------------month_sync--------------1-----------------datetimemonth--------------
            //                         nam 2015                                     nam 2016
            switch (datetime.month)
            {
                case 1: // thang hien tai cua meter la thang 1
                    switch (month_sync)
                    {
                        case 11: // thang dong bo la thang 11
                            datetime.day_reverse = 31 - day_sync + 31 + datetime.day;
                            break;
                        case 12: // thang dong bo la thang 12
                            datetime.day_reverse = 31 - day_sync + datetime.day;
                            break;
                        default: // thang dong bo la thang 10,9,.....,1
                            datetime.day_reverse = 62;
                            break;
                    }
                    break;
                case 2: // thang hien tai cua meter la thang 2
                    if (month_sync == 12) // thang dong bo la thang 12
                        datetime.day_reverse = 31 - day_sync + 31 + datetime.day;
                    else // 
                        datetime.day_reverse = 62; // gan 1 gia tri nao do >= 62 la duoc, vi lon hon 60 ngay (2thang)
                    break;
                default:
                    datetime.day_reverse = 62; // gan 1 gia tri nao do >= 62 la duoc, vi lon hon 60 ngay (2thang)
                    break;
            }
    #endif
        }
    }
    //////////////////////////////////////////////
    if (billing_period.flags.day_save_data) // neu ngay thang nam khac thoi gian hien tai cua meter thi moi dong bo lai thoi gian
    {
        rtc_set_date(weekday_sync, day_sync, month_sync, year_sync);
        datetime.weekday = weekday_sync;
        datetime.day = day_sync;
        datetime.month = month_sync;
        datetime.year = year_sync;
    }
    ////////////////////////////////////////////////
    if (day_sync == 1)
        datetime.flags.set_date = 1; // set co nay de phan biet ngay nay la do dong bo lai chu khong phai sang thang moi hay ngay moi
    //  }
}
#endif // end cua BILLING_DAY
#endif // end cua RTC_SUPPORT
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
    Rev 1.0: thaodtn - 24/10/2013
        -
        -
 */

































