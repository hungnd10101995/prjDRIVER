/*===========================================================================*/
/*
SOURCE_FILE:		pcf2129.c
APPLICATION:		RTC
MODULE:				RTC
ORIGINAL AUTHOR:	thaodtn
VERSION:			2.0
DATE:				18/10/2016
*/
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
// Header prjEMETERS
#include "devices.h"

// Header prjLIB
#include "delay.h"
#include "utc.h"
#include "convert.h"

// Header prjDRVIER
#include "spi.h"
#include "pcf2129.h"

#ifdef PCF2129_SUPPORT //endif to end of file
/*===========================================================================*/

/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/

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


//===========================================================================
//FUNCTION NAME:    pcf2129_set_time_12h
//PROTOTYPE:        void pcf2129_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
//INPUT:            mode(AM hhoac PM), hour(1-12), minute(0-59), second(0-59)
//OUTPUT:
//DESCRIPTION:      ghi thoi gian(gio, phut, giay) theo mode 12h vao pcf2129
//NOTE:
//============================================================================
void pcf2129_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CONTROL_1); // command: ghi du lieu vao bat dau tu dia chi CONTROL_1
    spi_send_byte(MODE_12H);
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+SECONDS);
    spi_send_byte(dec2bcd(second));
    spi_send_byte(dec2bcd(minute));
    if(mode == PM)
    {
        spi_send_byte(dec2bcd(hour)+PM);
    }
    else
    {
        spi_send_byte(dec2bcd(hour));
    }
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_second_alarm
//PROTOTYPE:        void pcf2129_read_time(void)
//INPUT:            second(0-59)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua giay ma khi thanh ghi SECONDS
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_second_alarm(unsigned char second)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+SECOND_ALARM);
    spi_send_byte(dec2bcd(second));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_minute_alarm
//PROTOTYPE:        void pcf2129_set_minute_alarm(unsigned char minute)
//INPUT:            minute(0-59)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua phut ma khi thanh ghi MINUTES
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_minute_alarm(unsigned char minute)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+MINUTE_ALARM);
    spi_send_byte(dec2bcd(minute));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_hour_12h_alarm
//PROTOTYPE:        void pcf2129_set_hour_12h_alarm(unsigned char hour, unsigned char mode)
//INPUT:            hour(1-12), mode(AM hoac PM)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua gio ma khi thanh ghi HOURS
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_hour_12h_alarm(unsigned char hour, unsigned char mode)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE + HOUR_ALARM);
    if(mode == PM)
    {
        spi_send_byte(dec2bcd(hour) + PM);
    }
    else
    {
        spi_send_byte(dec2bcd(hour));
    }
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_hour_24h_alarm
//PROTOTYPE:        void pcf2129_set_hour_24h_alarm(unsigned char hour)
//INPUT:            hour(0-23)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua gio ma khi thanh ghi HOURS
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_hour_24h_alarm(unsigned char hour)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+HOUR_ALARM);
    spi_send_byte(dec2bcd(hour));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_day_alarm
//PROTOTYPE:        void pcf2129_set_day_alarm(unsigned char day)
//INPUT:            day(0-31)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua ngay ma khi thanh ghi DAYS
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_day_alarm(unsigned char day)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+DAY_ALARM);
    spi_send_byte(dec2bcd(day));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_weekday_alarm
//PROTOTYPE:        void pcf2129_set_weekday_alarm(unsigned char weekday)
//INPUT:            weekday(SUNDAY....SATURDAY)
//OUTPUT:
//DESCRIPTION:      ghi vao pcf2129 gia tri cua thu trong tuan ma khi thanh ghi WEEKDAYS
//                  cua pcf2129 dat den se phat ra tin hieu canh bao(keo chan INT cua RTC xuong 0)
//NOTE:
//============================================================================
void pcf2129_set_weekday_alarm(unsigned char weekday)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+WEEKDAY_ALARM);
    spi_send_byte(weekday);
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_enable_mi
//PROTOTYPE:        void pcf2129_enable_mi(void)
//INPUT:            weekday(SUNDAY....SATURDAY)
//OUTPUT:
//DESCRIPTION:      khoi dong pcf2129 cho phep ngat moi phut(cu sau moi phut chan INT keo xuong logic 0)
//NOTE:
//============================================================================
void pcf2129_enable_mi(void)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CONTROL_1); // command: ghi du lieu vao bat dau tu dia chi CONTROL_1
    spi_send_byte(MODE_24H+MI); //  mode 24h, enable minute interrupt
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

/*===========================================================================*/
/* G L O B A L   F U N C T I O N S   P R O T O T Y P E S                     */
/*===========================================================================*/


//void rtc_interrupt_enable(void)
//{
//    //cau hinh ngat
//#if defined(__PIC24FJxxxGA406__)
//    pcf2129_interrupt_select_pin();
//#endif    
//    pcf2129_interrupt_dir_input();
//    pcf2129_interrupt_pullup_disable();
//    pcf2129_interrupt_high_to_low();
//    pcf2129_interrupt_flag_clear();
//    pcf2129_interrupt_enable();
//}
//
//void rtc_interrupt_disable(void)
//{
//    pcf2129_interrupt_flag_clear();
//    pcf2129_interrupt_disable();
//}

//===========================================================================
//FUNCTION NAME:    pcf2129_init
//PROTOTYPE:        unsigned char dec2bcd(unsigned char dec)
//INPUT:            none
//OUTPUT:           none
//DESCRIPTION:      khoi dong va config cho pcf2129
//NOTE:
//============================================================================
void pcf2129_init(void)
{
    //cau hinh I/O cho chan select chip
    pcf2129_cs_dir_output();
    //cau hinh ngat
#ifdef PCF2129_POLL_INTA
    pcf2129_interrupt_dir_input();
#else
    pcf2129_interrupt_dir_input(); //thaodtn
    pcf2129_interrupt_enable();
#endif
    //cau hinh thanh ghi
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CONTROL_1); // command: ghi du lieu vao bat dau tu dia chi CONTROL_1
    spi_send_byte(MODE_24H+MI);  // Minute interrupt
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    delay_us(5);
    //CONTROL_2
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CONTROL_2); // command: ghi du lieu vao bat dau tu dia chi CONTROL_1
    spi_send_byte(0);  // Minute interrupt
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    delay_us(5);
    //CONTROL_3
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CONTROL_3); // command: ghi du lieu vao bat dau tu dia chi CONTROL_1
    spi_send_byte(0);  // Minute interrupt
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    delay_us(5);
    //WATCHDG_CTL
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+WATCHDG_CTL);
    spi_send_byte(0);//spi_send_byte(0x20);//(BIT5);  //TI_TP
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    //
    delay_us(5);
    //////////TEST pcf OFSET 8PPM
    #ifdef  OFFSET_PCF2129_SUPPORT
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+Aging_offset);
    //spi_send_byte(0x00); //offset +8ppm
    spi_send_byte(0x0F); //offset -7ppm
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    #endif
    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+CLKOUT_ctl);
    //spi_send_byte(0xC6); //COF[2:0] = 110,fCLKOUT =1Hz, TCR[1:0] = 11, a temperature measurement period equal to 30 seconds
    #ifdef CLKOUT_1HZ_PCF2129_SUPPORT
        //spi_send_byte(0x00); //32.768khz output
        spi_send_byte(0xC6); //1Hz output
    #else
        spi_send_byte(0x07); //CLKOUT = high-Z, a temperature measurement period equal to 4 minute  //thaodtn
    #endif
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI

//#ifdef RTC_CORRECTION
//    delay_us(5);
//    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
//    spi_send_byte(WRITE+ 0x19);  //thanh ghi bu thach anh
//    spi_send_byte(0x08);  //bu them +8ppm
//        pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
//#endif
//    pcf2129_select(); // pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    //spi_send_byte(WRITE+CONTROL_2);//send command:cho phep ghi du lieu vao pcf2129, tai thanh ghi co dia chi bat dau la control 2
    //spi_send_byte(AIE); // enable interrtupt alarm
    //spi_send_byte(PWRMNG0+BIE+BLIE);// enable interrupt when battery switch-over, low battery occur
    //    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    delay_ms(10);//phai co lenh nay
//    pcf2129_read_time();  // da co o rtc_init
//    rtc_2129.utc = date2utc(&rtc_2129); // da co o rtc_init
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_time
//PROTOTYPE:        void pcf2129_set_time(unsigned char hour, unsigned char minute, unsigned char second)
//INPUT:            'hour'(0-23),minute(0-59),second(0-59)
//OUTPUT:           none
//DESCRIPTION:      ghi thoi gian(gio, phut,giay) theo mode 24h vao pcf2129
//NOTE:
//============================================================================
void pcf2129_set_time(unsigned char hour, unsigned char minute, unsigned char second)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+SECONDS);
    spi_send_byte(dec2bcd(second));
    spi_send_byte(dec2bcd(minute));
    spi_send_byte(dec2bcd(hour));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_set_date
//PROTOTYPE:        void pcf2129_set_time_12h(unsigned char hour, unsigned char minute, unsigned char second, unsigned char mode)
//INPUT:            weekday(sunday...saturday), day(1-31), month(1-12), year(0-99)
//OUTPUT:
//DESCRIPTION:      ghi thoi gian(cac thu trong tuan(weekday), ngay(day), thang(month),nam(year)) vao pcf2129
//NOTE:
//============================================================================
void pcf2129_set_date(unsigned char weekday, unsigned char day, unsigned char month, unsigned char year)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE+DAYS); // command: ghi du lieu vao bat dau tudia chi DAYS
    spi_send_byte(dec2bcd(day));
    spi_send_byte(dec2bcd(weekday));
    spi_send_byte(dec2bcd(month));
    spi_send_byte(dec2bcd(year));
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_read_time
//PROTOTYPE:        void pcf2129_read_time(void)
//INPUT:            weekday(sunday...saturday), day(1-31), month(1-12), year(0-99)
//OUTPUT:
//DESCRIPTION:      doc thoi gian hien tai cua pcf2129(gio,phut, giay,...,nam),
//                  cac du lieu doc ve duoi dang bcd nen ta chuyen ve dang thap phan
//                  roi luu vao cac bien thoi gian.
//NOTE:
//============================================================================
void pcf2129_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(READ+SECONDS); //+0x02);//

    //control_1 = spi_get_byte();
    //control_2 = spi_get_byte();
    //control_3 = spi_get_byte();
    *sec = bcd2dec(spi_get_byte() & 0x7F);
    *min = bcd2dec(spi_get_byte());
    *hour = bcd2dec(spi_get_byte());
    *day = bcd2dec(spi_get_byte());
    *wday = bcd2dec(spi_get_byte());
    *mon = bcd2dec(spi_get_byte());
    *year = bcd2dec(spi_get_byte());
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
}

//===========================================================================
//FUNCTION NAME:    pcf2129_read_flag
//PROTOTYPE:        void pcf2129_read_flag(void)
//INPUT:
//OUTPUT:
//DESCRIPTION:      doc cac thanh ghi control cua RTC, kiem tra co nao duoc set
//                  de nhan biet ngat(chan INT(cua RTC) = low)la do nguyen nhan nao cua RTC
//NOTE:
//============================================================================
void pcf2129_read_flag(void)
{
    unsigned char  control_2, control_3; //control_1,
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(READ+CONTROL_1);
    control_2 = spi_get_byte();
    control_2 = spi_get_byte();
    control_3 = spi_get_byte();
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    if ((control_2&MSF) != 0)
    {
        // co MSF duoc set
        // code
    }
    if ((control_2&WDTF) != 0)
    {
        // co WDTF duoc set
        // code
    }
    if ((control_2&AF) != 0)
    {
        // co AF duoc set
        // code
    }
    if ((control_3&BF) != 0)
    {
        // co BF duoc set
        // code
    }
    if ((control_3&BLF) != 0) // co BLF duoc set
    {
        emeter.flags.low_batt_rtc = 1;
        //emeter.flags.low_batt = 1;
    }
    else
    {
        emeter.flags.low_batt_rtc = 0;
        //emeter.flags.low_batt = 0;
    }
}

//===========================================================================
//FUNCTION NAME:    pcf2129_clr_flag
//PROTOTYPE:        void pcf2129_clr_flag(unsigned char flag)
//INPUT:            flag(AF hoac MSF)
//OUTPUT:
//DESCRIPTION:      xoa cac co de xoa ngat(set chan INT cua pcf2129 len 1)
//NOTE:
//============================================================================
void pcf2129_clr_flag(unsigned char flag)
{
    pcf2129_select(); // spi_disable();    NOP();    pcf2129_cs_low(); Vuonglg Quy hoach MODE SPI
    spi_send_byte(WRITE + CONTROL_2);
    if (flag == AF)
    {
        spi_send_byte(CLR_AF);
    }
    if (flag == MSF)
    {
        spi_send_byte(0);//CLR_MSF
    }
    pcf2129_de_select();// pcf2129_cs_high(); Vuonglg Quy hoach MODE SPI
    delay_us(5);
}

#endif
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/

































