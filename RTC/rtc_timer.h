#ifndef RTC_H
#define RTC_H

#include "rtc.h"
#ifdef RTC_SUPPORT
    //#include "..\rtc\pcf2129.h"
#elif   INT_RTC_SUPPORT
    //#include "..\rtc\rtc.h"
#endif
// typedef struct RTC_DEF {
	// unsigned char second;
	// unsigned char minute;
	// unsigned char hour;
	// unsigned char day;
	// unsigned char weekday;
	// unsigned char month;
	// unsigned char year;
    // unsigned long utc;
    // struct {
        // unsigned action : 1;
        // unsigned start : 1;
        // unsigned long counter;
	// }timer;
// }rtc_def;

RTC_STRUCT rtc_timer;

#define UTC_DEFINE                  1
#define RTC_1S                      50
#define RTC_READ_CYCLE              300
#define RTC_SAVE_CYCLE              3600
#define RTC_HISTORICAL_CYCLE        86400
// extern unsigned long date2jdn(unsigned char day, unsigned char month,unsigned int year);
// extern unsigned long date2utc(rtc_def time);
//extern unsigned char utc2date(RTC_STRUCT *time, uint32_t utc);
extern void rtc_timer_init(void);
extern void rtc_timer_read(void);
extern void rtc_timer_task(void);
#endif
