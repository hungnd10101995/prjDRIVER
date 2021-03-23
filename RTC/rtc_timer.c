#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <io.h>
#ifdef __XC8
#include <xc.h>
#endif

// HI-TECH PICC-18 compiler 
#ifdef __PICC18__
#include <htc.h>
#endif
#ifdef _RMR_
#include "../../RMR/FW/main.h"
#endif
#include "rtc_timer.h"
#include "../framework/utc.h"
//#include "../../../LIBRARY/framework/delay.h"
#ifdef BILLING_REQUEST
#include "../../../LIBRARY/billing/billing_period.h"
#endif
#include "../meter/meterpc.h"

//////////////////////////////////////////////////////////////////////////
//	Function :	date2jdn
//	Comments :
//	Input	 :	None
//	Output	 :      None
//////////////////////////////////////////////////////////////////////////
// unsigned long date2jdn(unsigned char day, unsigned char month,unsigned int year)
// {
// unsigned char a;
// unsigned char m;
// unsigned int y ;
// unsigned long jdn;//,aaa;
   // a = (unsigned char) ((14 - month)/12);
   // y = (year + 4800 - a);
   // m = month + 12*a - 3 ;
   // //jdn = day + (153m +2)\5 + 365*y + y\4 - y\100 + y\400 - 32045;
       // jdn = day + ((unsigned long)m*153 + 2)/5;
       // //jdn +=  (unsigned long)y*365 + (unsigned long)y*97/400; // y/4 - y/100 + y/400 =  97*y/400;
       // jdn +=  (unsigned long)y*365 + (unsigned long)y/4 - (unsigned long)y/100 + (unsigned long)y/400;
       // jdn -= 32045;
   // /*
   // //jdn = 367*y - 7*(y + (m+9)/12)/4 + 275*m/9 + d -730530;
     // jdn = (unsigned long)367*y;
     // jdn += (unsigned long)275*m/9;
     // aaa = ((unsigned long)m+9);
     // aaa = (unsigned long)y + aaa/12;
     // aaa = 7*aaa/4;
     // //jdn -= (unsigned long)((unsigned long)(7*(y + (unsigned long)(m+9)/12)) /4);
     // jdn -= aaa;
     // jdn += (unsigned long)(day - 730530);
   // */
     // //DJD
     // jdn -= 2415020;
// return(jdn);
// }
//////////////////////////////////////////////////////////////////////////
//======================================================================//
/*
struct tm {
   int tm_sec;         // seconds,  range 0 to 59          
   int tm_min;         // minutes, range 0 to 59           
   int tm_hour;        // hours, range 0 to 23             
   int tm_mday;        // day of the month, range 1 to 31  
   int tm_mon;         // month, range 0 to 11             
   int tm_year;        // The number of years since 1900   
   int tm_wday;        // day of the week, range 0 to 6    
   int tm_yday;        // day in the year, range 0 to 365  
   int tm_isdst;       // daylight saving time             
};

utc2date: chuyen tu utc sang kieu date time.
ham nay chi chuyen duoc den 23:59:59 31/12/2035
*/
//unsigned char utc2date(RTC_STRUCT *time, uint32_t utc)
//{    
//    //kiem tra dieu kien utc sau ngay 23:59:59 31/12/2035 thi return FALSE
//    if (utc > 757382399) return FALSE;
//    time_t      stamp; //00:00:00 1/1/2012: stamp = 1328050800//1325372400
//    struct tm   *temp;
//    stamp = (time_t)utc + 1325376000;
//    temp = gmtime(&(stamp));
//    time->year = temp->tm_year + 1900 - 2000;
//    time->month = temp->tm_mon + 1;
//    time->day = temp->tm_mday;
//    time->weekday = temp->tm_wday;
//    time->hour = temp->tm_hour;
//    time->minute = temp->tm_min;
//    time->second = temp->tm_sec;
//    time->utc = utc;
//    return TRUE;
//}
//////////////////////////////////////////////////////////////////////////
//	Function :	date2utc
//	Comments :
//	Input	 :	None
//	Output	 :      None
//////////////////////////////////////////////////////////////////////////
//Dublin Julian Day (DJD)         12:00 December 31, 1899, Sunday         DJD = JD - 2415020
// unsigned long date2utc(rtc_def time)  // Thuc hien ham nay het 388us
// {
    // unsigned long jd,utc,h,m;
     // //jd = jdn + hour-12/24 + minute/1440 + second/86400;
     // // Neu thuc hien trong dinh nghia 0->23 gio thi khong can - 12
     // //utc = jd - 2440587.5 * 86400;
     // //utc = ((float)jdn - 2440587.5) * 86400;  // form G
     // //jd =  date2jdn(ds1307.hex.date,ds1307.hex.month & 0x1F,ds1307.hex.year + 2000);

     // jd =  date2jdn(time.day,time.month,time.year + 2000);
      // //0h00m00s1,1,1970 = 25568
     // // utc = (jdn(now) - jdn(0h00m00s,1,1,1970)) * 86400 = (jd(now) - 25568) 86400
     // utc = (jd - 25568-15340) * 86400;  // form DJD  //15340: tinh tu 0h0m0s,1/1/2012
// //     utc += (3600*time.hour) + (60*time.minute) + time.second;
// //     h = time.hour;
// //     m = time.minute;
// //     utc += 3600*h+ 60*m + time.second;
     // utc += 3600*(unsigned long)time.hour + 60*(unsigned long)time.minute + time.second;
     // //delay_ms(10);
    // return(utc);
// }
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	Function :	rtc_init
//	Comments :
//	Input	 :	None
//	Output	 :      None
//////////////////////////////////////////////////////////////////////////
void rtc_timer_init(void)
{
    rtc_timer.timer.start = FALSE;
    rtc_timer.timer.action = FALSE;
    rtc_timer.timer.counter = 0;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	Function :	rtc_read_timer
//	Comments :
//	Input	 :	None
//	Output	 :      None
//////////////////////////////////////////////////////////////////////////
void rtc_timer_read(void)
{
    if(rtc_timer.timer.start == FALSE) return;
    if (++rtc_timer.timer.counter < RTC_1S) return; //GPRS_TIMER_COUTER_STEP;
    rtc_timer.timer.action = TRUE;
    rtc_timer.timer.counter = 0;
}
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Function :	rtc_task
//	Comments :
//	Input	 :	None
//	Output	 :      None
//////////////////////////////////////////////////////////////////////////
void rtc_timer_task(void)
{
	if (rtc_timer.timer.action == TRUE)
        {
            rtc_timer.timer.action = FALSE;
            rtc_timer.utc++;
            utc2date(&datetime,rtc_timer.utc);
        }
    if(((rtc_timer.utc%RTC_READ_CYCLE)==0)&&(rtc_timer.utc !=0)&&(meterpc.flags.readoffline == FALSE))
    {
        meterpc.flags.readoffline = TRUE;
        meterpc.timeout = 0;
        meterpc.timer   = 0;
    }
    if(((rtc_timer.utc%RTC_SAVE_CYCLE)==0)&&(rtc_timer.utc !=0)&&(meterpc.flags.save_record== FALSE))
    {
        meterpc.flags.save_record = TRUE;
    }
    if(((rtc_timer.utc%RTC_HISTORICAL_CYCLE)==120)&&(rtc_timer.utc !=0)&&(meterpc.flags.readoffline == FALSE)&&(meterpc.flags.read_historical == FALSE))
    {
        meterpc.flags.read_historical = TRUE;
        meterpc.flags.read_historical_success = FALSE;
        meterpc.timeout = 0;
        meterpc.timer   = 0;
//        meterpc_reset_register();
    }
#ifdef BILLING_REQUEST
    if ((billing_period.flags.waitting == TRUE)&&(rtc_timer.utc>= billing_period.utc_data))
{
        billing_period.flags.waitting = FALSE;
        billing_period.flags.instant = TRUE;
        meterpc.flags.read_request = TRUE;
        if (meterpc.flags.readoffline == FALSE) {
            meterpc.timeout = 0;
            meterpc.timer = 0;
        }
    }
#endif
}

//////////////////////////////////////////////////////////////////////////
//======================================================================//