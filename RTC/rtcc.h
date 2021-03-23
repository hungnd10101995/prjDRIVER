/* 
 * File:   rtcc.h
 * Author: tuantm
 *
 * Created on March 5, 2016, 1:28 PM
 */

#ifndef RTCC_H
#define	RTCC_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void rtcc_init();
    extern void rtcc_set_time(uint8_t h, uint8_t m, uint8_t s);
    extern void rtcc_set_date(uint8_t w, uint8_t d, uint8_t mt, uint8_t y);
    //extern void rtcc_read_time();
    extern void rtcc_read_time(uint8_t *sec, uint8_t *min, uint8_t *hour, uint8_t *day, uint8_t *wday, uint8_t *mon, uint8_t *year);
    void rtcc_unlock();
    void rtcc_lock();
    extern unsigned char rtc_check_time(uint8_t hour, uint8_t minute, uint8_t second);
    extern unsigned char rtc_check_date_time(uint8_t day, uint8_t month, uint32_t year);
#ifdef	__cplusplus
}
#endif

#define rtcc_interrupt_enable()         (IEC3bits.RTCIE = 1)
#define rtcc_interrupt_disable()        (IEC3bits.RTCIE = 0)
#define rtcc_interrupt_flag_check()     (IFS3bits.RTCIF)
#define rtcc_interrupt_flag_clear()     (IFS3bits.RTCIF = 0)

#endif	/* RTCC_H */

