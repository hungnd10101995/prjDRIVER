#ifndef _LCD16X2_H_
#define _LCD16X2_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/


/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
//#define LCD8BIT

#define DISPLAY_OBIS_CHARACTER_C    0xFF
#define DISPLAY_DASH                0xFE

#define DISPLAY_UNIT_POSITION       12 //vi tri bat dau cua don vi, tru kvarh bat dau 11
#define DISPLAY_POINT1_POSITION     9
#define DISPLAY_POINT2_POSITION     8
#define DISPLAY_POINT3_POSITION     7

/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/
extern void LCD_init (void);
extern void LCD_gotoxy (unsigned char x, unsigned char y);
extern void LCD_cls (void);
extern void LCD_cur_off (void);
extern void LCD_cur_blink (void);
extern void LCD_putc (unsigned char c);
extern void LCD_print (unsigned char x, unsigned char y, unsigned char *string);
extern void LCD_bargraph (unsigned char x, unsigned char y, unsigned char size, unsigned char val);
extern void lcd_display_value(long long value, int point);
extern void lcd_display_tariff(unsigned char source, unsigned char index);
extern void lcd_display_maxdemand(unsigned char source, unsigned char index);
extern void lcd_display_digit(unsigned char value,unsigned char  pos);


/******************************************************************************/
extern void lcd_anten(char status);
extern void lcd_reverse(char status);
extern void lcd_magnetic(char status);
extern void lcd_lowbat(char status);
extern void lcd_powerfail(char status);
extern void lcd_L1(char status);
extern void lcd_L2(char status);
extern void lcd_L3(char status);
extern void lcd_COL2(unsigned char status);
extern void lcd_Er(char status);
extern void lcd_unlock(char status);
extern void lcd_phA(char status);
extern void lcd_phB(char status);
extern void lcd_phC(char status);
extern void lcd_phase(int ph, int x, int y);
extern void lcd_V(char status);
extern void lcd_A(char status);
extern void lcd_point1(char status);
extern void lcd_point2(char status);
extern void lcd_point3(char status);
extern void lcd_point4(char status);
extern void lcd_point6(char status);
extern void lcd_point7(char status);
extern void lcd_point8(char status);
extern void lcd_point9(char status);
extern void lcd_warning(char status);
extern void lcd_W(char status);
extern void lcd_kW(char status);
extern void lcd_VAr(char status);
extern void lcd_kVAr(char status);
extern void lcd_VA(char status);
extern void lcd_kVA(char status);
extern void lcd_Hz(char status);
extern void lcd_kWh(char status);
extern void lcd_Wh(char status);
extern void lcd_kVArh(char status);
extern void lcd_VArh(char status);
extern void lcd_kVAh(char status);
extern void lcd_VAh(char status);
extern void lcd_P_active(char status);
extern void lcd_P_reverse(char status);
extern void lcd_Q_active(char status);
extern void lcd_Q_reverse(char status);
extern void lcd_GND(char status);
extern void lcd_MD(char status);
extern void lcd_ACB(char status);
extern void lcd_clear_all(void);
extern void lcd_hide_digit(unsigned char  pos);
extern void lcd_display_alphabet(unsigned char value,unsigned char  pos);
extern void lcd_over(char status);
extern void lcd_lo(char status);
#endif