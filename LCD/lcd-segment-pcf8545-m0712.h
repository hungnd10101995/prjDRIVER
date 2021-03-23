#include <sg8v1.h>
#include "define.h"
#include "spi.h"

extern void lcd_disable();
extern void lcd_enable();
extern void load_data_pointer(unsigned char start_segment);
extern void clear_lcd();
extern void display_character(unsigned char data_pointer, unsigned char character);
extern unsigned char swap_byte(unsigned char in_byte);
extern unsigned char encode_lcd(unsigned char in_byte);
extern void lcd_anten(char status);
extern void lcd_warning(char status);
extern void lcd_lowbat(char status);
extern void lcd_reverse(char status);
extern void lcd_Er(char status);
extern void lcd_ACB(char status);
extern void lcd_MD(char status);
extern void lcd_COL2(char status);
extern void lcd_L1(char status);
extern void lcd_L2(char status);
extern void lcd_L3(char status);
extern void lcd_GND(char status);
extern void lcd_magnetic(char status);
extern void lcd_unlock(char status);
extern void lcd_powerfail(char status);
extern void lcd_display_value(unsigned long value);
extern void lcd_display_digit(unsigned char value,unsigned char  pos);
