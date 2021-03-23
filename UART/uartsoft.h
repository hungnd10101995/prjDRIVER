/* 
 * File:   uartsoft.h
 * Author: tuantm
 *
 * Created on June 25, 2016, 3:03 PM
 */

#ifndef UARTSOFT_H
#define	UARTSOFT_H

#ifdef	__cplusplus
extern "C" {
#endif
    extern unsigned char send_bitno;
    extern void init_uartsoft(void);
    extern void putch(char c);
    void uartsoft_interrupt_procees(void);
    
    volatile extern unsigned char uart_soft_timer, uart_soft_enable;
#ifdef	__cplusplus
}
#endif

#endif	/* UARTSOFT_H */

