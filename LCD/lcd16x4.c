#include "../../DCU/FW/main.h"
//#ifdef LCD_SUPPORT
#include "../../LIBRARY/framework/delay.h"
#include "../../DCU/FW/hardware.h"
#include "../../DCU/FW/display.h"
#include "lcd16x4.h"
#ifdef LCD_16x4

void lcd16x_io_test(void)
{
    lcd16x_rs_output_low();
    lcd16x_rs_output_high();
    lcd16x_rw_output_low();
    lcd16x_rw_output_high();
    lcd16x_en_output_low();
    lcd16x_en_output_high();

    lcd16x_d4_pin() = 1;
    lcd16x_d4_pin() = 0;
    lcd16x_d5_pin() = 1;
    lcd16x_d5_pin() = 0;
    lcd16x_d6_pin() = 1;
    lcd16x_d6_pin() = 0;
    lcd16x_d7_pin() = 1;
    lcd16x_d7_pin() = 0;
}

void lcd16x_enable(void)
{
    lcd16x_en_output_high();
    delay_us(30);
    lcd16x_en_output_low();
    delay_us(50);
}

void lcd16x_clear(void)
{
    lcd16x_sendcmd(0x01);
    delay_ms(10);
}

void lcd16x_init(void)
{
    lcd16x_rs_dir_output();
    lcd16x_rw_dir_output();
    lcd16x_en_dir_output();
    //AD1PCFG = 0xFFFF; //disable d4 analog function
    lcd16x_d4_output();
    lcd16x_d5_output();
    lcd16x_d6_output();
    lcd16x_d7_output();
    //for d0-d3    
    //
    //lcd16x_enable();
    //
    lcd16x_rw_output_low();
    lcd16x_send4bit(0x00);
    restart_watchdog();
    delay_ms(10);
    restart_watchdog();
    lcd16x_rs_output_low();
    lcd16x_send4bit(0x03);
    lcd16x_enable();
    restart_watchdog();
    delay_ms(10);
    restart_watchdog();
    lcd16x_enable();
    delay_us(500);
    lcd16x_enable();
    lcd16x_send4bit(0x02);
    lcd16x_enable();
    lcd16x_sendcmd(0x28);
    lcd16x_sendcmd(0x0c);
    lcd16x_sendcmd(0x06);
    lcd16x_sendcmd(0x01);
    //clear
    lcd16x_clear();
}

void lcd16x_disable_io(void)
{
    lcd16x_d4_output_low();
    lcd16x_d5_output_low();
    lcd16x_d6_output_low();
    lcd16x_d7_output_low();
    lcd16x_rw_output_low();
    lcd16x_rs_output_low();
    lcd16x_en_output_low();
}

void lcd16x_send4bit(uint8_t data)
{
    lcd16x_d4_pin() = (data & 0x01);
    lcd16x_d5_pin() = ((data >> 1)&0x01);
    lcd16x_d6_pin() = ((data >> 2)&0x01);
    lcd16x_d7_pin() = ((data >> 3)&0x01);
}

void lcd16x_sendcmd(uint8_t cmd)
{
    lcd16x_rw_output_low();
    lcd16x_send4bit(cmd >> 4);
    lcd16x_enable();
    lcd16x_send4bit(cmd);
    lcd16x_enable();
    lcd16x_rw_output_high();
}

void lcd16x_gotoXY(unsigned char x, unsigned char y)
{
#ifdef PCB_DCU1P_V10
    if (y > 1) return; //chi hien thi 2 hang
#endif
    x += DISPLAY_OFFSET;
    if (y == 0) lcd16x_sendcmd(0x80 + x);
    if (y == 1) lcd16x_sendcmd(0xc0 + x);
    if (y == 2) lcd16x_sendcmd(0x90 + x);
    if (y == 3) lcd16x_sendcmd(0xd0 + x);
    delay_ms(10);
}

void lcd16x_write_char(char ch)
{
    lcd16x_rs_output_high();
    lcd16x_sendcmd(ch);
    lcd16x_rs_output_low();
}

void lcd16x_put_string(uint8_t x, uint8_t y, char *pData)
{
    uint8_t i = 0;
    lcd16x_gotoXY(x, y);
    while ((*pData)&&(i < 16))
    {
        i++;
        lcd16x_write_char(*pData++);
    }
}
#endif
