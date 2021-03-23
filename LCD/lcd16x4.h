/* 
 * File:   lcd20x4.h
 * Author: tuantm
 *
 * Created on November 19, 2016, 9:18 AM
 */

#ifndef LCD20X4_H
#define	LCD20X4_H

#ifdef	__cplusplus
extern "C" {
#endif
extern void lcd16x4_io_test(void);
extern void lcd16x4_enable(void);
extern void lcd16x4_clear(void);
extern void lcd16x4_init(void);
extern void lcd16x4_send4bit(uint8_t data);
extern void lcd16x4_sendcmd(uint8_t cmd);
extern void lcd16x4_gotoXY(unsigned char x, unsigned char y);
extern void lcd16x4_write_char(char ch);
extern void lcd16x4_put_string(uint8_t x, uint8_t y, char *pData);
    
#ifdef	__cplusplus
}
#endif

#endif	/* LCD20X4_H */

