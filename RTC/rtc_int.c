#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <io.h>
#include "..\library.h"

#ifdef INT_RTC_SUPPORT
//-----------------------------------
#ifndef _IHD_
#include <emeter-toolkit.h>
#endif

#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter\emeter-main.h"
#include "..\lcd\lcd-segment-ost11295.h"
#include "..\3-phase-measurement\measure-struct.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter\emeter-main.h"
#include "..\lcd\lcd-segment-ost11295.h"
#include "..\3-phase-measurement\measure-struct.h"
#endif
#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter\emeter-main.h"
#include "..\lcd\lcd-segment-ost11295.h"
#include "..\1-phase-measurement\measure-foreground.h"
#include "..\1-phase-measurement\measure-struct.h"
#endif

#ifdef _DT01P_RF_
//#include "..\..\dt01p80-rf\fw\emeter\emeter-main.h"
//#include "..\lcd\lcd-segment-ost11248.h"
//#include "..\1-phase-measurement\measure-foreground.h"
//#include "..\1-phase-measurement\measure-struct.h"
#endif

#ifdef _IHD_
#include "..\..\ihd\fw\ihd-main.h"
#include "..\..\ihd\fw\ihd-hardware.h"
#include "..\lcd\lcd-segment-b157m0742.h"
#include "..\framework\delay.h"  
#include "..\lcd\ihd-display.h"
#include "..\rf\hu.h"
#endif

#include "rtc_int.h"
#include "../framework/convert.h"

#ifdef _IHD_
#include "../eeprom/ic24aa08.h" 
#else
//#include "../eeprom/24aa08.h"
#endif

#ifdef LOG_RECORD_SUPPORT
#include "../eeprom/eeprom.h" //vinh
#endif

//#ifdef RF_SUPPORT
//#include "../rf/rf.h"
//#include "../rf/rfmesh.h"
//#include "../rf/cc1101.h"  //vinh
//#endif

#include "../framework/utc.h"  //vinh
#include "../framework/crc.h"  //vinh

#include "../framework/process.h"

//#ifdef DTP_SUPPORT
//#include "../communication/dtp.h"
//#endif

//RTC_STRUCT rtc_int;

#ifdef _IHD_
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_check_load
//  Created     : 19/09/2015, by tuantm
//  Description : kiem tra khoi tao RTC ban dau
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void rtc_check_load()
{
    uint8_t tmp_idx;
    /*neu doc RTC bi loi*/
    if((rtc_check_time(rtc_int.hour,rtc_int.minute,rtc_int.second) == FALSE)
       ||(rtc_check_date_time(rtc_int.day,rtc_int.month,rtc_int.year) == FALSE))
        //tien hanh reset lai RTC
    {
        if(ihd.data.index < ihd.data.rf_record_max) //da co du lieu
        {
            if(ihd.data.full_his == FULL_DATA_HIS)
            {
                tmp_idx = ihd.data.rf_record_max - 1;
            }
            else
                tmp_idx = ihd.data.index;
            //lay RTC luu trong eeprom de nap lai
            rtc_int.second = 59;
            rtc_int.minute = ihd.data.rtc[ihd.data.index].min;
            rtc_int.hour = ihd.data.rtc[ihd.data.index].hour;
            //rtc_int.weekday = 7;
            rtc_int.day = ihd.data.rtc[ihd.data.index].day;
            rtc_int.month = ihd.data.rtc[ihd.data.index].month;
            rtc_int.year = ihd.data.rtc[ihd.data.index].year;
        }
        else
        {
            rtc_int.second = 0;
            rtc_int.minute = 30;
            rtc_int.hour = 15;
            
            rtc_int.weekday = 7;
            rtc_int.day = 19;
            rtc_int.month = 9;
            rtc_int.year = 15;
        }
        rtc_set_time(rtc_int.hour,rtc_int.minute,rtc_int.second);
        rtc_set_date(rtc_int.weekday,rtc_int.day,rtc_int.month,rtc_int.year);
    }
    else //neu RTC khong loi nhung load lai gia tri be hon
    {
        if(ihd.data.index < ihd.data.rf_record_max) //da co du lieu
        {
            if(ihd.data.full_his == FULL_DATA_HIS)
            {
                tmp_idx = ihd.data.rf_record_max - 1;
            }
            else
                tmp_idx = ihd.data.index;
            ihd.data.time_present = rtc_int.year*356*24*60 + rtc_int.month*30*24*60 + 
                rtc_int.day*24*60 + rtc_int.hour*60 + rtc_int.minute;
            ihd.data.time_past = ihd.data.rtc[tmp_idx].year*356*24*60 + 
                ihd.data.rtc[tmp_idx].month*30*24*60 +
                    ihd.data.rtc[tmp_idx].day*24*60 + 
                        ihd.data.rtc[tmp_idx].hour*60 +
                            ihd.data.rtc[tmp_idx].min;
            if(ihd.data.time_present < ihd.data.time_past)  
            {
                //lay RTC luu trong eeprom de nap lai
                rtc_int.second = 59;
                rtc_int.minute = ihd.data.rtc[ihd.data.index].min;
                rtc_int.hour = ihd.data.rtc[ihd.data.index].hour;
                //rtc_int.weekday = 7;
                rtc_int.day = ihd.data.rtc[ihd.data.index].day;
                rtc_int.month = ihd.data.rtc[ihd.data.index].month;
                rtc_int.year = ihd.data.rtc[ihd.data.index].year;      
            }
            rtc_set_time(rtc_int.hour,rtc_int.minute,rtc_int.second);
            rtc_set_date(rtc_int.weekday,rtc_int.day,rtc_int.month,rtc_int.year);
        }
    }
    
}
#endif
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_intit
//  Created     : 26/04/2015, by tuantm
//  Description : 
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void rtc_int_init(void)
{
    UCSCTL6 &= ~(XT1OFF);                   // Enable XT1
    UCSCTL6 |= XCAP_3;                      // Internal load cap
    RTCCTL0_H = RTCKEY_H;               // Unlock RTC_C module
    RTCCTL0_L |= RTCRDYIE;              // read ready interrupt , vinh     23/06/15                                      // 
    RTCCTL1 |= RTCBCD | RTCHOLD;        // BCD mode, RTC hold
    RTCCTL1 &= ~(RTCHOLD);              // Start RTC calendar mode
    RTCCTL0_H = 0;                      // Lock RTC_C module
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_read_time
//  Created     : 26/04/2015, by tuantm
//  Description : 
//  Input		:  
//  Output      :  
//  modified    : by vinhld
////////////////////////////////////////////////////////////////////////////////
void rtc_int_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year)
{
    *sec = bcd2dec(RTCSEC);
    *min = bcd2dec(RTCMIN);
    *hour = bcd2dec(RTCHOUR);
    
    *wday = bcd2dec(RTCDOW);
    *day = bcd2dec(RTCDAY);
    *mon = bcd2dec(RTCMON);
    *year = bcd2dec(RTCYEAR);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_set_time
//  Created     : 26/04/2015, by tuantm
//  Description : 
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void rtc_int_set_time(unsigned char hour, unsigned char minute, unsigned char second)
{
    //RTCCTL0_L &= ~(RTCRDYIE);// vinh 24/06/15 , disable interrupt RTC
    //rtc_int.flags.set_time_date = 1;
    RTCCTL0_H = RTCKEY_H; //unlock
    RTCSEC = dec2bcd(second);
    RTCMIN = dec2bcd(minute);
    RTCHOUR = dec2bcd(hour);
    RTCCTL0_H = 0;    //lock
    //rtc_int.flags.set_time_date = 0;
    //RTCCTL0_L |= RTCRDYIE;// vinh 24/06/15
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_set_date
//  Created     : 26/04/2015, by tuantm
//  Description : 
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void rtc_int_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year)
{
    //RTCCTL0_L &= ~(RTCRDYIE);// vinh 24/06/15 , disable interrupt RTC
    //rtc_int.flags.set_time_date = 1;
    RTCCTL0_H = RTCKEY_H; //unlock
    RTCDOW = dec2bcd(weekday);
    RTCDAY = dec2bcd(day);
    RTCMON = dec2bcd(month);
    RTCYEAR = dec2bcd(year);
    RTCCTL0_H = 0;    //lock   
    //rtc_int.flags.set_time_date = 0;
    //RTCCTL0_L |= RTCRDYIE;// vinh 24/06/15
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : rtc_task(void)
//  Created     : 08/05/2015, by tuantm
//  Description : ham xu ly cac cong viec khi co ngat 1 phut
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

#ifdef _IHD_

#endif
//-----------------------------------------
#endif // INT_RTC_SUPPORT