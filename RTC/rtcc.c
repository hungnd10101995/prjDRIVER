/*
 * File:   rtcc.c
 * Author: tuantm
 *
 * Created on March 5, 2016, 1:27 PM
 */

#include "../library.h"
#include "xc.h"
#include <stdio.h>
#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#include "../../ROUTER/FW/hardware.h"
#endif
#if defined(_DT03M_RF_) && defined(__PIC24FJxxxGA410__)
#include "..\..\dt03m-rf\fw\main.h"
#include "..\..\dt03m-rf\fw\hardware.h"
#endif
#include "rtc.h"
#include "rtcc.h"
#ifdef RTCC_SUPPORT

void rtcc_unlock() //theo code mau cua datasheet
{
    asm volatile ("DISI #6");
    asm volatile ("MOV #NVMKEY, W1");
    asm volatile ("MOV #0x55, W2");
    asm volatile ("MOV W2, [W1]");
    asm volatile ("MOV #0xAA, W3");
    asm volatile ("MOV W3, [W1]");
    RTCCON1Lbits.WRLOCK = 0;//asm volatile ("BCLR RTCCON1L, #WRLOCK");
}
void rtcc_lock() //theo code mau cua datasheet
{
    asm volatile ("DISI #6");
    asm volatile ("MOV #NVMKEY, W1");
    asm volatile ("MOV #0x55, W2");
    asm volatile ("MOV W2, [W1]");
    asm volatile ("MOV #0xAA, W3");
    asm volatile ("MOV W3, [W1]");
    RTCCON1Lbits.WRLOCK = 1;//asm volatile ("BSET RTCCON1L, #WRLOCK");
}
void rtcc_init()
{
    rtcc_unlock();  
    //RTCCON1Lbits.RTCEN = 0; //disable RTC
    RTCCON1Lbits.RTCEN = 1; //enable RTC
#ifdef USE_LPRC
    RTCCON2Lbits.CLKSEL = 0x01;//internal clock LPRC (31.25kHz)
#else
    RTCCON2Lbits.CLKSEL = 0x00; //32.768khz
    OSCCONbits.SOSCEN = 1;
#endif
    RTCCON2Lbits.PS = 0; //1:1
    RTCCON2Lbits.FDIV = 0;//24;//0;
    RTCCON2Lbits.PWCPS = 0; //1:1
    RTCCON2Lbits.PS = 0; //1:1
#ifdef USE_LPRC
    RTCCON2H = 15624;  //31250/2 - 1
#else
    RTCCON2H = 16371;//16370;//16395;//0x3FFF;  //32768/2 - 1 
#endif
    //alarm (interrupt)
    RTCCON1Hbits.ALRMEN = 1;
    RTCCON1Hbits.AMASK = 3; //every minute
    RTCCON1Hbits.ALMRPT = 0xFF;
    RTCCON1Hbits.CHIME = 1; //repeat alarm forever
    RTCCON1Lbits.OUTSEL = 0; //alarm event
    //power control => output xung dung` de can chinh
//    RTCCON1Lbits.PWCEN = 1;
//    RTCCON1Lbits.RTCOE = 1;
//    RTCCON1Lbits.OUTSEL = 0b011;
//    RTCCON1Lbits.PWCPOL = 0;
    //interrupt
    rtcc_interrupt_flag_clear();
    rtcc_interrupt_enable();
    //RTCCON1Lbits.RTCEN = 1; //enable RTC
    rtcc_lock();
}
void rtcc_set_time(uint8_t h, uint8_t m, uint8_t s)
{
    rtcc_unlock();
    //__RTCWREN() = 1;
    //RTCCON1Lbits.RTCEN = 0;
    TIMEHbits.HRTEN = h / 10;
    TIMEHbits.HRONE = h % 10;
    TIMEHbits.MINTEN = m / 10;
    TIMEHbits.MINONE = m % 10;
    TIMELbits.SECTEN = s / 10;
    TIMELbits.SECONE = s % 10;
    //RTCCON1Lbits.RTCEN = 1;
    rtcc_lock();
}
void rtcc_set_date(uint8_t w, uint8_t d, uint8_t mt, uint8_t y)
{
    rtcc_unlock();
    //RTCCON1Lbits.RTCEN = 0;
    DATELbits.WDAY = w;
    DATELbits.DAYTEN = d / 10;
    DATELbits.DAYONE = d % 10;
    DATEHbits.MTHTEN = mt / 10;
    DATEHbits.MTHONE = mt % 10;
    DATEHbits.YRTEN = y / 10;
    DATEHbits.YRONE = y % 10; 
    //RTCCON1Lbits.RTCEN = 1;
    rtcc_lock();
}
//void rtcc_read_time()
void rtcc_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year)
{
    *hour = TIMEHbits.HRTEN*10 + TIMEHbits.HRONE;
    *min = TIMEHbits.MINTEN*10 + TIMEHbits.MINONE;
    *sec = TIMELbits.SECTEN*10 + TIMELbits.SECONE;
    
    *wday = DATELbits.WDAY;
    
    *day = DATELbits.DAYTEN*10 + DATELbits.DAYONE;
    *mon = DATEHbits.MTHTEN*10 + DATEHbits.MTHONE;
    *year = DATEHbits.YRTEN*10 + DATEHbits.YRONE;
}
#endif
