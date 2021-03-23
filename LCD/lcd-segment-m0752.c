//#include <msp430x471x7.h>
//#include <emeter-toolkit.h>
//#include "emeter-structs.h"
//#include <msp430f6731.h>
#include <stdint.h>
#include <stdlib.h>
#include <io.h>
#include "..\library.h"

#ifdef _LCD_M0752_
//------------------------------------------------------------------------------------------
#include "lcd-segment-m0752.h"
#include "../../LIBRARY/framework/delay.h"
#include "..\framework\macro.h"

#ifdef _DT01P_RF_
#include "..\..\dt01p80-rf\fw\emeter-main.h"
#include "..\..\dt01p80-rf\fw\emeter-hardware.h"
#endif

#include "../../LIBRARY/1-phase-measurement/measure-struct.h"
#include "../../LIBRARY/1-phase-measurement/measure-foreground.h"
/*
 __a___
|	   |
f      b
|__g___|
|      |
e      c
|__d___|
*/
    #define SEG_A	0x40
    #define SEG_B	0x10
    #define SEG_C	0x02
    #define SEG_D	0x01
    #define SEG_E	0x04
    #define SEG_F	0x20
    #define SEG_G	0x08

////////////////////////////////////////////////////////////////////////////////
/* so */
#define DIGIT_8  (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DIGIT_0  (DIGIT_8 ^ SEG_G)
#define DIGIT_1  (SEG_B|SEG_C)
#define DIGIT_2  (SEG_A|SEG_B|SEG_G|SEG_E|SEG_D)
#define DIGIT_3  (SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define DIGIT_4  (SEG_F|SEG_G|SEG_B|SEG_C)
#define DIGIT_5  (SEG_A|SEG_F|SEG_G|SEG_C|SEG_D)
#define DIGIT_6  (DIGIT_8 ^ SEG_B)
#define DIGIT_7  (SEG_A|SEG_B|SEG_C)
#define DIGIT_9  (DIGIT_8 ^ SEG_E)
/* ky tu */
#define CHAR_A		(DIGIT_8 ^ SEG_D)
#define CHAR_B		((DIGIT_8 ^ SEG_A) ^ SEG_B)
#define CHAR_C		(SEG_A|SEG_F|SEG_E|SEG_D)
#define CHAR_D		((DIGIT_8 ^ SEG_F) ^ SEG_A)
#define CHAR_E		((DIGIT_8 ^ SEG_B) ^ SEG_C)
#define CHAR_F      CHAR_E^SEG_D
#define CHAR_V      DIGIT_8^SEG_A^SEG_G
#define CHAR_R      SEG_A|SEG_F|SEG_E
#define CHAR_DASH   SEG_G //dau gach ngang cua ngay thang nam
#define CHAR_H      (SEG_B|SEG_C|SEG_E|SEG_F|SEG_G)
#define CHAR_P      CHAR_F^SEG_B  //vinh
#define CHAR_O      DIGIT_8^SEG_G // vinh 26/06/15

///////////////////////////////////////////////////////////////////////////////////
//const char lcd_seg_data[10] = {0x5F,0x50,0x3D,0x79,0x72,0x6B,0x6F,0x51,0x7F,0x7B};//{0xF5,0x05,0xD3,0x97,0x27,0xB6,0xF6,0x15,0xF7,0xB7};//
const char lcd_seg_data[MAX_SEG_DATA] = {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9,CHAR_C};
//cac so tuong ung:             0    1    2     3    4    5   6    7    8    9
								//V      E     R   C    U    -     H
//const char lcd_seg_alphabet[7] = {0x5E, 0x2F, 0x07,0x0F,0x4C,0x20,0x76};
const char lcd_seg_data2[MAX_SEG_DATA_2] = {swap4bit(DIGIT_0),swap4bit(DIGIT_1),swap4bit(DIGIT_2),swap4bit(DIGIT_3),
								swap4bit(DIGIT_4),swap4bit(DIGIT_5),swap4bit(DIGIT_6),
								swap4bit(DIGIT_7),swap4bit(DIGIT_8),swap4bit(DIGIT_9),swap4bit(CHAR_C)};
   //cac so tuong ung:            0    1    2    3    4    5    6    7    8    9   C
//bang ma hien thi chu cai (ver)
//const char lcd_seg_alphabet[6] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C};
//const unsigned char lcd_seg_alphabet[8] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C,CHAR_H,CHAR_DASH};
//const unsigned char lcd_seg_alphabet[9] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C,CHAR_H,CHAR_DASH,CHAR_P};  //vinh
const unsigned char lcd_seg_alphabet[10] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C,CHAR_H,CHAR_DASH,CHAR_P,CHAR_O};  //vinh 26/06/15

unsigned char flag_display_all = 0;
uint8_t pos_display;					

void lcd_clear_all(void)
{
    LCDCMEMCTL set LCDCLRM;
    // unsigned char i;
    //  for (i=0;i<20;i++) *(&LCDM1+i)=0x00;
}

void lcd_clear_8digit(void)
{
    unsigned char i;
    for (i=0;i<10;i++) *(&LCDM20+i)=0x00;
}

/*************************************************************************************************************
* Function: lcd_display_digit
* Desciption: hien thi cac so(0...9) tai cac vi tri 1...7
* Input: 'value':gia tri ta muon hien thi(0...9), 'pos': vi tri hien thi gia tri(1...7)
* Output: none
*************************************************************************************************************/

void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    switch (pos)
    {
    case 0:
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x80)|(lcd_seg_data[value]);  //& 0xF0
        break;

    case 1:
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 2:
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 3:
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 4:
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 5:
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 6:
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 7:
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 8:
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x80)|lcd_seg_data[value];
        break;
    case 9:
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x80)|lcd_seg_data[value];
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_display_alphabet
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi ky tu len LCD
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_display_alphabet(unsigned char value,unsigned char pos)
{
    switch (pos)
    {
    case 0:
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x80)|(lcd_seg_alphabet[value]);  //& 0xF0        
        break;

    case 1:
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 2:
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 3:
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 4:
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 5:
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 6:
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 7:
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    case 8:
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x80)|lcd_seg_alphabet[value];
        break;
    }
}
void lcd_display_value(unsigned long value)
{
    unsigned long value1;
    unsigned char temp;
    int i;
    //clear_LCD_panel();
    value1=value;
    for(i=0;i<8;i++)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        temp = value1%10;
        lcd_display_digit(temp,i);
        value1 /= 10;
        if (value1 == 0) i = 8;
    }
    //_NOP();
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_hide_digit
//  Created     : 
//  Description : 
//  Input	: 
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_hide_digit(unsigned char  pos)
{
    switch (pos)
    {
        case 0:
            LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM&0x80);
            break;
        case 1:
            LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM&0x80);
            break;
        case 2:
            LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM&0x80);
            break;
        case 3:
            LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM&0x80);
            break;
        case 4:
            LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM&0x80);
            break;
        case 5:
            LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM&0x80);
            break;
        case 6:
            LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM&0x80);
            break;
        case 7:
            LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM&0x80);
            break;
        case 8:
            LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM&0x80);
            break;
        case 9:
            LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM&0x80);
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_process_hide_digits
//  Created     : 
//  Description : 
//  Input	: 
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_process_hide_digits(uint8_t position)
{
    int idx;//, idx_tmp ;
    for (idx = position; idx < MAX_DIGITS; idx++)        
    {
        //idx_tmp = idx;
        lcd_hide_digit(idx);
    }
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_display_error_value
//  Created     : 27/06/2015, by vinhld
//  Description : hien thi ma loi len LCD
//  Input	: 0< value < 100
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_display_error_value(unsigned char value)
{
    unsigned char value1;
    unsigned char temp;
    char i;
    //clear_LCD_panel();
    value1=value;
    for(i=8;i<10;i++)        //la`n lu.ot hie?n thi cac digit tu` 8 -> 9
    {
        temp = value1%10;
        lcd_display_digit(temp,i);
        value1 /= 10;
        if (value1 == 0) i = 10;
    }
}
/////////////////vinh//////////////////
void lcd_anten(char status) // S1
{
    if (status == 1)
        LCD_ANTEN_MEM set LCD_ANTEN_BIT;
    else if(flag_display_all == 0)//||(emeter.flags.calib_enable==0))	
        LCD_ANTEN_MEM clr LCD_ANTEN_BIT;
}

void lcd_lowbat(char status) // S4
{
    if (status == 1)
	LCD_BATT_MEM set LCD_BATT_BIT; //LCDM11 set 0x80;
    else if(flag_display_all == 0)//||(emeter.flags.calib_enable==0))
	LCD_BATT_MEM clr LCD_BATT_BIT;
}

void lcd_Er(char status)
{
    if (status == 1)
	LCD_ER_MEM set LCD_ER_BIT;
    else if(flag_display_all == 0)
	LCD_ER_MEM clr LCD_ER_BIT;
}
void lcd_point1(char status)
{
    if(status == 1)
	LCD_POINT1_MEM set LCD_POINT1_BIT;
    else
	LCD_POINT1_MEM clr LCD_POINT1_BIT;
}

void lcd_point2(char status)
{
    if(status == 1)
	LCD_POINT2_MEM set LCD_POINT2_BIT;
    else
	LCD_POINT2_MEM clr LCD_POINT2_BIT;
}
void lcd_point3(char status)
{
    if(status == 1)
	LCD_POINT3_MEM set LCD_POINT3_BIT;
    else
	LCD_POINT3_MEM clr LCD_POINT3_BIT;
}
void lcd_kWh(char status)
{
    if(status == 1)
	LCD_KWH_MEM set LCD_KWH_BIT;
    else
	LCD_KWH_MEM clr LCD_KWH_BIT;
}

void lcd_over_vol(char status)
{
    if(status == 1)
	LCD_OVER_VOL_MEM set LCD_OVER_VOL_BIT;
    else if(flag_display_all == 0)
	LCD_OVER_VOL_MEM clr LCD_OVER_VOL_BIT;
}
void lcd_over_current(char status)
{
    if(status == 1)
	LCD_OVER_CURRENT_MEM set LCD_OVER_CURRENT_BIT;
    else if(flag_display_all == 0)
	LCD_OVER_CURRENT_MEM clr LCD_OVER_CURRENT_BIT;
}

void lcd_A(char status)
{
    if (status == 1)
	LCD_A_MEM set LCD_A_BIT;
    else if(flag_display_all == 0)
	LCD_A_MEM clr LCD_A_BIT;
}

void lcd_V(char status)
{
    if (status == 1)
	LCD_V_MEM set LCD_V_BIT;
    else if(flag_display_all == 0)
	LCD_V_MEM clr LCD_V_BIT;
}

void lcd_unlock(char status)
{
    if(status == 1)
	LCD_UNLOCK_MEM set LCD_UNLOCK_BIT;
    else if(flag_display_all == 0)
	LCD_UNLOCK_MEM clr LCD_UNLOCK_BIT;
}

void lcd_powerfail(char status)
{
    if(status == 1)
	LCD_POWER_FAIL_MEM set LCD_POWER_FAIL_BIT;
    else
	LCD_POWER_FAIL_MEM clr LCD_POWER_FAIL_BIT;
}

void lcd_low_vol(char status)
{
    if(status == 1)
	LCD_LOW_VOL_MEM set LCD_LOW_VOL_BIT;
    else if(flag_display_all == 0)
	LCD_LOW_VOL_MEM clr LCD_LOW_VOL_BIT;
}

void lcd_col(char status)
{
    if(status == 1)
	LCD_COL_MEM set LCD_COL_BIT;
    else
	LCD_COL_MEM clr LCD_COL_BIT;
}

void lcd_reverse(char status)
{
    if(status == 1)
	LCD_REVERSE_MEM set LCD_REVERSE_BIT;
    else if(flag_display_all == 0)
	LCD_REVERSE_MEM clr LCD_REVERSE_BIT;
}

void lcd_GND(char status)
{
    if(status == 1)
	LCD_GND_MEM set LCD_GND_BIT;
    else if(flag_display_all == 0)
	LCD_GND_MEM clr LCD_GND_BIT;
}
void lcd_magnetic(char status)
{
    if(status == 1)
        LCD_MAGNETIC_MEM set LCD_MAGNETIC_BIT;
    else
        LCD_MAGNETIC_MEM clr LCD_MAGNETIC_BIT;
}

void lcd_display_kwh(unsigned long value)
{
    lcd_display_value(value);
    lcd_point3(SHOW);
    lcd_kWh(SHOW);
}
/*
void display_Error(void)
{
delay_us(1);
}
*/

void lcd_display_Vrms(void)
{
    int32_t x;

    // Display RMS voltage in 0.1V increments
    x =  current_rms_voltage(LIVE)/10; //current_rms_neutral_current()/10;//
    if (x<100)
    {
        lcd_display_digit(0,5);
        lcd_display_digit(0,6);
    }
    lcd_display_value(x);
    //LCD_display_value(2213); //fake Vol: tuantm, dis Low vol display

    lcd_point3(SHOW);lcd_point2(HIDE);
    lcd_V(SHOW);
}

void lcd_display_Irms(void)
{
    int32_t x;
#if defined(USE_CT_FOR_I1)
	x = current_rms_neutral_current()/10;
#else
    x = current_rms_current(LIVE)/10;
#endif
    //if (x<5) x=0;  //tuantm 06/11/2014 //I <50 mA: noise
    if (x<100)
    {
        lcd_display_digit(0,5);
        lcd_display_digit(0,6);
    }
    lcd_display_value(x);
    lcd_point2(SHOW);lcd_point3(HIDE);
    lcd_A(SHOW);
}

void lcd_display_Neutral_Irms(void)
{
    int32_t x;

    x = current_rms_neutral_current()/10;//
    if (x<5) x=0;  //I <50 mA: noise
    if (x<100)
    {
        lcd_display_digit(0,5);
        lcd_display_digit(0,6);
    }
    lcd_display_value(x);
    //LCD_A(1);
    lcd_point2(SHOW); lcd_point3(HIDE);
    lcd_A(SHOW);
}

void lcd_display_consumed_active_energy(void)
{
    int32_t x;

    //Display per phase or total consumed energy in 0.01kWh increments
    x =  emeter.active.forward.value/10;  //12345678;//
    //x /= 10;
    lcd_display_value(x);
    if (x<10) lcd_display_digit(0,6);
    lcd_kWh(SHOW);
    lcd_point3(SHOW);lcd_point2(HIDE);
}

void lcd_display_consumed_reverse_active_energy(void)
{
    int32_t x;

    //Display per phase or total consumed energy in 0.01kWh increments
    x =  emeter.active.reverse.value/10; //98765432;//
    lcd_display_value(x);
    if (x<10) lcd_display_digit(0,6);
    lcd_kWh(SHOW);
    lcd_point3(SHOW);lcd_point2(HIDE);
    lcd_reverse(SHOW);
}

/*
void display_consumed_reactive_energy(int ph)
{
int32_t x;

//Display per phase or total consumed energy in 0.01kWh increments
x = current_consumed_reactive_energy(ph);
//x /= 10;
LCD_display_value(x);
LCD_kVArh(1);
display_phase_icon(ph);
LCD_Point1(1);
//display_obis_code(REACTIVE_ENERGY_CODE,ph);
}*/
/*
void display_consumed_apparent_energy(void)  //int ph
{
int32_t x;

//Display per phase or total consumed energy in 0.01kWh increments
x = total_consumed_apparent_energy; //current_consumed_apparent_energy(ph);
//x /= 10;
LCD_display_value(x);
LCD_kVAh(1);
//display_phase_icon(ph);
LCD_Point2(1);
display_obis_code(APPARENT_ENERGY_CODE,3);
}
*/

//-----------------------------------------------------------
#endif // _LCD_M0752_