#ifndef _RTC_INT_H_
#define _RTC_INT_H_

#include "..\library.h"

#ifdef INT_RTC_SUPPORT
//-----------------------------------
typedef struct
{
  unsigned char code_error;
  struct
  {
    unsigned save_data:1;
    unsigned day_save_data:1;
    unsigned month_save_data:1;
    unsigned read:1;
    unsigned save_everyday:1;
    unsigned save_everymonth:1;
    unsigned checked:1;
    unsigned request:1;
  }flags;
}BILLING_RF_STRUCT;

//extern RTC_STRUCT rtc_int;

extern void rtc_int_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year);
extern void rtc_int_init(void);
extern void rtc_int_set_time(unsigned char hour, unsigned char minute, unsigned char second);
extern void rtc_int_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year);

#ifdef _IHD_
extern void rtc_check_load();
#endif
//---------------------------
#endif // #ifdef INT_RTC_SUPPORT
//-----------------------------------
#endif //#ifndef _RTC_H_