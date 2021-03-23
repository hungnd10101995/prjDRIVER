//#include <msp430x471x7.h>
//#include <emeter-toolkit.h>
//#include "emeter-structs.h"
//#include <msp430f6731.h>
#include <stdint.h>
#include <stdlib.h>
#include <io.h>
#include "lcd-segment-b157m0742.h"
#include "../../LIBRARY/framework/delay.h"
#include "..\framework\macro.h"
#include "..\library.h"
#ifdef _DT01P_RF_
#include "..\..\dt01p80-rf\fw\emeter\emeter-main.h"
#include "..\..\dt01p80-rf\fw\emeter\emeter-hardware.h"
#endif

#ifdef _PREPAID_METER_
#include "..\..\prepaid-meter\fw\emeter\emeter-main.h"
#include "..\..\prepaid-meter\fw\emeter\emeter-hardware.h"
#endif

#ifndef _IHD_
#include "../../LIBRARY/1-phase-measurement/measure-struct.h"
#include "../../LIBRARY/1-phase-measurement/measure-foreground.h"
#else
#include "..\..\ihd\fw\ihd-main.h"
#include "..\..\ihd\fw\ihd-hardware.h"
#endif
/*
__a___
|	   |
f      b
|__g___|
|      |
e      c
|__d___|
*/
#if defined(PCB_EXT_V0)
#define SEG_A	0x10
#define SEG_B	0x01
#define SEG_C	0x04
#define SEG_D	0x80
#define SEG_E	0x40
#define SEG_F	0x20
#define SEG_G	0x02
#elif defined(PCB_EXT_V1)
#define SEG_A	0x10
#define SEG_B	0x01
#define SEG_C	0x04
#define SEG_D	0x80
#define SEG_E	0x40
#define SEG_F	0x20
#define SEG_G	0x02
#elif defined(PCB_DT01P80_2014) //|| defined (_IHD_)
#define SEG_A	1
#define SEG_B	16
#define SEG_C	64
#define SEG_D	8
#define SEG_E	4
#define SEG_F	2
#define SEG_G	32
#elif defined (_IHD_)
#define SEG_A	BIT3
#define SEG_B	BIT2
#define SEG_C	BIT1
#define SEG_D	BIT0

#define SEG_E	BIT5
#define SEG_F	BIT7
#define SEG_G	BIT6
#endif
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
#define CHAR_Y      ((DIGIT_8^SEG_A)^SEG_E)
#define CHAR_N      ((DIGIT_8^SEG_G)^SEG_D)
#define CHAR_L      (SEG_F|SEG_E|SEG_D)
#define CHAR_r      (SEG_G|SEG_E)
#define CHAR_t      (SEG_F|SEG_G|SEG_E|SEG_D)

#define CHAR_c      (CHAR_o^SEG_C)
#define CHAR_h      (SEG_F|SEG_E|SEG_G|SEG_C)
#define CHAR_o      (SEG_C|SEG_D|SEG_E|SEG_G)
#define CHAR_i      (SEG_E)
#define CHAR_b      ((DIGIT_8^SEG_A)^SEG_B)
#define CHAR_ll     (SEG_B | SEG_C | SEG_F | SEG_E)

#define V_CHAR  0
#define E_CHAR  1
#define R_CHAR  2
#define A_CHAR  3
#define B_CHAR  4
#define C_CHAR  5
#define H_CHAR  6
#define DASH_CHAR       7
#define P_CHAR  8
#define O_CHAR  9
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
const unsigned char lcd_seg_alphabet[] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,
                                        CHAR_C,CHAR_H,CHAR_DASH,CHAR_P,CHAR_O,
                                        CHAR_Y, CHAR_N, CHAR_L, CHAR_r, CHAR_t,
                                        CHAR_F, CHAR_c, CHAR_h, CHAR_o, CHAR_i, 
                                        CHAR_b, CHAR_ll}; 

void lcd_clear_all(void)
{
    LCDCMEMCTL set LCDCLRM;
    // unsigned char i;
    //  for (i=0;i<20;i++) *(&LCDM1+i)=0x00;
}

void lcd_clear_8digit(void) 
{
    LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F);
    LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x1F);
    
    LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F);
    LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x1F);
    
    LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F);
    LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x1F);
    
    LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F);
    LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x1F);
    
    LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F);
    LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x1F);
    
    LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F);
    LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x1F);
    
    LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F);
    LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x1F);
    
    LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F);
    LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x1F);
    lcd_p10(HIDE);
    lcd_p100(HIDE);
    lcd_p1000(HIDE);
    lcd_p1000000(HIDE);
    lcd_col1(HIDE);
    lcd_col2(HIDE);
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
        /*COM4 > COM7 => BIT4 > BIT7*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x1F)|(lcd_seg_data[value]&0xE0);
        break;
        /*COM0 > COM3 => BIT0 > BIT3*/
    case 9:
        LCD_DIGIT9_MEM0 = (LCD_DIGIT9_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT9_MEM1 = (LCD_DIGIT9_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 10:
        LCD_DIGIT10_MEM0 = (LCD_DIGIT10_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT10_MEM1 = (LCD_DIGIT10_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 11:
        LCD_DIGIT11_MEM0 = (LCD_DIGIT11_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT11_MEM1 = (LCD_DIGIT11_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 12:
        LCD_DIGIT12_MEM0 = (LCD_DIGIT12_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT12_MEM1 = (LCD_DIGIT12_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 13:
        LCD_DIGIT13_MEM0 = (LCD_DIGIT13_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT13_MEM1 = (LCD_DIGIT13_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 14:
        LCD_DIGIT14_MEM0 = (LCD_DIGIT14_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT14_MEM1 = (LCD_DIGIT14_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 15:
        LCD_DIGIT15_MEM0 = (LCD_DIGIT15_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT15_MEM1 = (LCD_DIGIT15_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 16:
        LCD_DIGIT16_MEM0 = (LCD_DIGIT16_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT16_MEM1 = (LCD_DIGIT16_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 17:
        LCD_DIGIT17_MEM0 = (LCD_DIGIT17_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT17_MEM1 = (LCD_DIGIT17_MEM1&0xF8)|(lcd_seg_data[value]>>5);
        break;
    case 18:
        LCD_DIGIT18_MEM0 = (LCD_DIGIT18_MEM0&0xF0)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT18_MEM1 = (LCD_DIGIT18_MEM1&0xF8)|(lcd_seg_data[value]>>5);
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
        /*COM4 > COM7 => BIT4 > BIT7*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x1F)|(lcd_seg_alphabet[value]&0xE0);
        break;
        /*COM0 > COM3 => BIT0 > BIT3*/
    case 9:
        LCD_DIGIT9_MEM0 = (LCD_DIGIT9_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT9_MEM1 = (LCD_DIGIT9_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 10:
        LCD_DIGIT10_MEM0 = (LCD_DIGIT10_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT10_MEM1 = (LCD_DIGIT10_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 11:
        LCD_DIGIT11_MEM0 = (LCD_DIGIT11_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT11_MEM1 = (LCD_DIGIT11_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 12:
        LCD_DIGIT12_MEM0 = (LCD_DIGIT12_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT12_MEM1 = (LCD_DIGIT12_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 13:
        LCD_DIGIT13_MEM0 = (LCD_DIGIT13_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT13_MEM1 = (LCD_DIGIT13_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 14:
        LCD_DIGIT14_MEM0 = (LCD_DIGIT14_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT14_MEM1 = (LCD_DIGIT14_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 15:
        LCD_DIGIT15_MEM0 = (LCD_DIGIT15_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT15_MEM1 = (LCD_DIGIT15_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 16:
        LCD_DIGIT16_MEM0 = (LCD_DIGIT16_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT16_MEM1 = (LCD_DIGIT16_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 17:
        LCD_DIGIT17_MEM0 = (LCD_DIGIT17_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT17_MEM1 = (LCD_DIGIT17_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    case 18:
        LCD_DIGIT18_MEM0 = (LCD_DIGIT18_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT18_MEM1 = (LCD_DIGIT18_MEM1&0xF8)|(lcd_seg_alphabet[value]>>5);
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_hide_digit(unsigned char  pos)
{
    switch (pos)
    {
        /*4 COM dau tien*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x1F);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x1F);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x1F);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x1F);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x1F);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x1F);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x1F);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x1F);
        break;
        /*4 COM tiep theo*/
    case 9:
        LCD_DIGIT9_MEM0 = (LCD_DIGIT9_MEM0&0xF0);
        LCD_DIGIT9_MEM1 = (LCD_DIGIT9_MEM1&0xF8);
        break;
    case 10:
        LCD_DIGIT10_MEM0 = (LCD_DIGIT10_MEM0&0xF0);
        LCD_DIGIT10_MEM1 = (LCD_DIGIT10_MEM1&0xF8);
        break;
    case 11:
        LCD_DIGIT11_MEM0 = (LCD_DIGIT11_MEM0&0xF0);
        LCD_DIGIT11_MEM1 = (LCD_DIGIT11_MEM1&0xF8);
        break;
    case 12:
        LCD_DIGIT12_MEM0 = (LCD_DIGIT12_MEM0&0xF0);
        LCD_DIGIT12_MEM1 = (LCD_DIGIT12_MEM1&0xF8);
        break;
    case 13:
        LCD_DIGIT13_MEM0 = (LCD_DIGIT13_MEM0&0xF0);
        LCD_DIGIT13_MEM1 = (LCD_DIGIT13_MEM1&0xF8);
        break;
    case 14:
        LCD_DIGIT14_MEM0 = (LCD_DIGIT14_MEM0&0xF0);
        LCD_DIGIT14_MEM1 = (LCD_DIGIT14_MEM1&0xF8);
        break;
    case 15:
        LCD_DIGIT15_MEM0 = (LCD_DIGIT15_MEM0&0xF0);
        LCD_DIGIT15_MEM1 = (LCD_DIGIT15_MEM1&0xF8);
        break;
    case 16:
        LCD_DIGIT16_MEM0 = (LCD_DIGIT16_MEM0&0xF0);
        LCD_DIGIT16_MEM1 = (LCD_DIGIT16_MEM1&0xF8);
        break;
    case 17:
        LCD_DIGIT17_MEM0 = (LCD_DIGIT17_MEM0&0xF0);
        LCD_DIGIT17_MEM1 = (LCD_DIGIT17_MEM1&0xF8);
        break;
    case 18:
        LCD_DIGIT18_MEM0 = (LCD_DIGIT18_MEM0&0xF0);
        LCD_DIGIT18_MEM1 = (LCD_DIGIT18_MEM1&0xF8);
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_display_value(unsigned long value)
{
    unsigned long value1;
    unsigned char temp;
    int i;
#ifdef _IHD_
    //clear_LCD_panel();
    value1=value;
    for(i=8;i>0;i--)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        temp = value1%10;
        lcd_display_digit(temp,i);
        value1 /= 10;
        if (value1 == 0) i = 0;
    }
    //_NOP();
#else
	//clear_LCD_panel();
    value1=value;
    for(i=8;i>0;i--)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        temp = value1%10;
        lcd_display_digit(temp,i-1);
        value1 /= 10;
        if (value1 == 0) i = 0;
    }
    //_NOP();
#endif 
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
    for(i=10;i>8;i--)        //la`n lu.ot hie?n thi cac digit tu` 8 -> 9
    {
        temp = value1%10;
        lcd_display_digit(temp,i-1);
        value1 /= 10;
        if (value1 == 0) i = 0;
    }
}
void lcd_logo(char level)
{
    LCD_LOGO_EVN_MEM clr LCD_LOGO_EVN_BIT;
    LCD_EVN_MEM clr LCD_EVN_BIT;
    LCD_CPC_MEM clr LCD_CPC_BIT;
    switch(level)
    {
    case 0:
        break;
    case 1:
        LCD_LOGO_EVN_MEM set LCD_LOGO_EVN_BIT;
        break;
    case 2:
        LCD_LOGO_EVN_MEM set LCD_LOGO_EVN_BIT;
        LCD_EVN_MEM set LCD_EVN_BIT;
        break;
    case 3:
        LCD_LOGO_EVN_MEM set LCD_LOGO_EVN_BIT;
        LCD_EVN_MEM set LCD_EVN_BIT; 
        LCD_CPC_MEM set LCD_CPC_BIT;
        break;
    }
}
void lcd_logoEVN(char status)
{
    if(status == 1)
        LCD_LOGO_EVN_MEM set LCD_LOGO_EVN_BIT;
    else
        LCD_LOGO_EVN_MEM clr LCD_LOGO_EVN_BIT;
}
void lcd_CPC(char status)
{
    if(status == 1)
        LCD_CPC_MEM set LCD_CPC_BIT;
    else
        LCD_CPC_MEM clr LCD_CPC_BIT;
}
void lcd_EVN(char status)
{
    if(status == 1)
        LCD_EVN_MEM set LCD_EVN_BIT;
    else
        LCD_EVN_MEM clr LCD_EVN_BIT;
}
void lcd_CPC_tgl()
{
    LCD_CPC_MEM tgl LCD_CPC_BIT;
}
void lcd_EVN_tgl()
{
    LCD_EVN_MEM tgl LCD_EVN_BIT;
}
void lcd_logoEVN_tgl()
{
    LCD_LOGO_EVN_MEM tgl LCD_LOGO_EVN_BIT;
}
void lcd_wh(char status)
{
    if(status == 1)
    {
        LCDM1 set (BIT3|BIT4|BIT6|BIT7);
        LCDM2 set (BIT5|BIT4);
    }
    else
    {
        LCDM1 clr (BIT3|BIT4|BIT6|BIT7);
        LCDM2 clr (BIT5|BIT4);
    }
}
void lcd_kwh(char status)
{
    if(status == 1)
    {
        LCDM1 set (BIT0|BIT1|BIT3|BIT4|BIT6|BIT7);
        LCDM2 set (BIT5|BIT4);
    }
    else
    {
        LCDM1 clr (BIT0|BIT1|BIT3|BIT4|BIT6|BIT7);
        LCDM2 clr (BIT5|BIT4);
    }
}
void lcd_mwh(char status)
{
    if(status == 1)
    {
        LCDM1 set (BIT0|BIT2|BIT3|BIT4|BIT6|BIT7);
        LCDM2 set (BIT5|BIT4);
    }
    else
    {
        LCDM1 clr (BIT0|BIT2|BIT3|BIT4|BIT6|BIT7);
        LCDM2 clr (BIT5|BIT4);
    }
}
void lcd_var(char status)
{
    if(status == 1)
    {
        LCDM1 set (BIT3|BIT5|BIT6);
        LCDM2 set (BIT4);
    }
    else
    {
        LCDM1 clr (BIT3|BIT5|BIT6);
        LCDM2 clr (BIT4);
    }
}
void lcd_kvarh(char status)
{
    if(status == 1)
    {
        LCDM1 set (BIT0|BIT1|BIT3|BIT5|BIT6);
        LCDM2 set (BIT4|BIT6);
    }
    else
    {
        LCDM1 clr (BIT0|BIT1|BIT3|BIT5|BIT6);
        LCDM2 clr (BIT4|BIT6);
    }
}
void lcd_A(char status)
{
    if (status == 1)
        LCDM1 set (BIT5+BIT6);
    else
        LCDM1 clr (BIT5+BIT6);
}
void lcd_V(char status)
{
    if (status == 1)
        LCDM1 set BIT3;
    else
        LCDM1 clr BIT3;
}
void lcd_p10(char status)
{
    if(status == 1)
        LCD_P7_MEM set LCD_P7_BIT;
    else
        LCD_P7_MEM clr LCD_P7_BIT;
}
void lcd_p100(char status)
{
    if(status == 1)
        LCD_P6_MEM set LCD_P6_BIT;
    else
        LCD_P6_MEM clr LCD_P6_BIT;
}
void lcd_p1000(char status)
{
    if(status == 1)
        LCD_P5_MEM set LCD_P5_BIT;
    else
        LCD_P5_MEM clr LCD_P5_BIT;
}
void lcd_p1000000(char status)
{
    if(status == 1)
        LCD_P2_MEM set LCD_P2_BIT;
    else
        LCD_P2_MEM clr LCD_P2_BIT;
}
void lcd_col1(char status)
{
    if(status == 1)
        LCD_COL1_MEM set LCD_COL1_BIT;
    else
        LCD_COL1_MEM clr LCD_COL1_BIT;
}
void lcd_col2(char status)
{
    if(status == 1)
        LCD_COL2_MEM set LCD_COL2_BIT;
    else
        LCD_COL2_MEM clr LCD_COL2_BIT;
}
void lcd_anten(char level) // S1
{
    //LCDM12 & LCDM14 & LCDM16
    //hide all
    LCDM12 clr (BIT4|BIT5|BIT6|BIT7);
    LCDM14 clr BIT3;
    LCDM16 clr BIT3;
    switch(level)
    {
    case 0: //hide all
        break;
    case 1:
        LCDM16 set BIT3;
        break;
    case 2:
        LCDM16 set BIT3;
        LCDM14 set BIT3;
        break;
    case 3:
        LCDM16 set BIT3;
        LCDM14 set BIT3;
        LCDM12 set BIT4;
        break;
    case 4:
        LCDM16 set BIT3;
        LCDM14 set BIT3;
        LCDM12 set (BIT4|BIT5);
        break;
    case 5:
        LCDM16 set BIT3;
        LCDM14 set BIT3;
        LCDM12 set (BIT4|BIT5|BIT6);
        break;
    case 6: //super signal :))
        LCDM16 set BIT3;
        LCDM14 set BIT3;
        LCDM12 set (BIT4|BIT5|BIT6|BIT7);
        break;
    }
}
void lcd_battery(char level)
{
    //LCDM12 & LCDM14 & LCDM16
    //hide all
    LCDM8 set BIT4; //alway show batt cover
    LCDM9 clr (BIT4|BIT5|BIT6|BIT7);
    LCDM16 clr BIT4;
    switch(level)
    {
    case 0: //hide all
        break;
    case 1:
        LCDM9 set BIT7;
        break;
    case 2:
        LCDM9 set (BIT7|BIT6);
        break;
    case 3:
        LCDM9 set (BIT7|BIT6|BIT5);
        break;
    case 4:
        LCDM9 set (BIT7|BIT6|BIT5|BIT4);
        break;
    case 5://full
        LCDM9 set (BIT7|BIT6|BIT5|BIT4);
        LCDM16 set BIT4;
        break;
    }
}
void lcd_reverse(char status)
{
    if(status == 1)
        LCD_REVERSE_MEM set LCD_REVERSE_BIT;
    else
        LCD_REVERSE_MEM clr LCD_REVERSE_BIT;
}
void lcd_history(char status)
{
    if(status == 1)
        LCD_HISTORY_MEM set LCD_HISTORY_BIT;
    else
        LCD_HISTORY_MEM clr LCD_HISTORY_BIT;
}
void lcd_id(char status)
{
    if(status == 1)
        LCD_ID_MEM set LCD_ID_BIT;
    else
        LCD_ID_MEM clr LCD_ID_BIT;
}
void lcd_warning(char status)
{
    if (status == 1)
        LCD_WARNING_MEM set LCD_WARNING_BIT;
    else
        LCD_WARNING_MEM clr LCD_WARNING_BIT;
}
void lcd_setting(char status)
{
    if (status == 1)
        LCD_SETTING_MEM set LCD_SETTING_BIT;
    else
        LCD_SETTING_MEM clr LCD_SETTING_BIT;
}
void lcd_dola(char status)
{
    if (status == 1)
        LCD_DOLA_MEM set LCD_DOLA_BIT;
    else
        LCD_DOLA_MEM clr LCD_DOLA_BIT;
}
void lcd_time(char status)
{
    if (status == 1)
        LCD_TIME_MEM set LCD_TIME_BIT;
    else
        LCD_TIME_MEM clr LCD_TIME_BIT;   
}
void lcd_date(char status)
{
    if (status == 1)
        LCD_DATE_MEM set LCD_DATE_BIT;
    else
        LCD_DATE_MEM clr LCD_DATE_BIT;   
}
void lcd_clock_dot(char status)
{
    if (status == 1)
        LCD_COL3_MEM set LCD_COL3_BIT;
    else
        LCD_COL3_MEM clr LCD_COL3_BIT;  
}
void lcd_tariff(char status)
{
    if (status == 1)
        LCD_T_MEM set LCD_T_BIT;
    else
        LCD_T_MEM clr LCD_T_BIT;  
}
void lcd_t1(char status)
{
    if (status == 1)
        LCD_T1_MEM set LCD_T1_BIT;
    else
        LCD_T1_MEM clr LCD_T1_BIT;  
}
void lcd_t(char status)
{
    if (status == 1)
        LCD_T_MEM set LCD_T_BIT;
    else
        LCD_T_MEM clr LCD_T_BIT;  
}
void lcd_split(char status)
{
    if (status == 1)
        LCD_SPLIT_MEM set LCD_SPLIT_BIT;
    else
        LCD_SPLIT_MEM clr LCD_SPLIT_BIT;  
}
void lcd_t3(char status)
{
    if (status == 1)
        LCD_T3_MEM set LCD_T3_BIT;
    else
        LCD_T3_MEM clr LCD_T3_BIT;  
}
void lcd_t4(char status)
{
    if (status == 1)
        LCD_T4_MEM set LCD_T4_BIT;
    else
        LCD_T4_MEM clr LCD_T4_BIT;  
}
void lcd_t5(char status)
{
    if (status == 1)
        LCD_T5_MEM set LCD_T5_BIT;
    else
        LCD_T5_MEM clr LCD_T5_BIT;  
}
void lcd_temp_icon(char type)
{
    if(type == 0) //hide
    {
        lcd_t3(HIDE);
        lcd_t4(HIDE);
        lcd_t5(HIDE);
    }
    if(type == 1) //do C
    {
        lcd_t3(SHOW);
        lcd_t4(HIDE);
        lcd_t5(SHOW);  
    }
    if(type == 2) //do F
    {
        lcd_t3(SHOW);
        lcd_t4(SHOW);
        lcd_t5(HIDE);  
    }
}
void clock_dot_tgl()
{
    LCD_COL3_MEM tgl LCD_COL3_BIT;
}
/*
void display_Error(void)
{
delay_us(1);
}
*/
////////////////////////////////////////////////////////////////////////////////
//  Function    : 
//  Created     : 18/09/2015, by tuantm
//  Description : kiem tra segment cua LCD
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void lcd_test()
{
    lcd_turn_on_all();
    lcd_clear_all();
    lcd_display_digit(0,9);
    lcd_display_digit(0,1);
    lcd_display_digit(1,2);
    lcd_display_digit(1,10);
    lcd_display_digit(2,3);
    lcd_display_digit(2,11);
    lcd_display_digit(3,4);
    lcd_display_digit(3,12);
    lcd_display_digit(4,5);
    lcd_display_digit(4,13);
    lcd_display_digit(5,6);
    lcd_display_digit(5,14);
    lcd_display_digit(6,7);
    lcd_display_digit(6,15);
    lcd_display_digit(7,8);
    lcd_display_digit(7,16);
    lcd_clear_all();
    lcd_display_value(87654321);
    //lcd_clear_all();
    
    lcd_display_digit(8,17);
    lcd_display_digit(9,18);
    
    lcd_logo(3);
    lcd_wh(SHOW);
    lcd_wh(HIDE);
    lcd_kwh(SHOW);
    lcd_kwh(HIDE);
    lcd_mwh(SHOW);
    lcd_mwh(HIDE);
    lcd_var(SHOW);
    lcd_var(HIDE);
    lcd_kvarh(SHOW);
    lcd_kvarh(HIDE);
    lcd_A(SHOW);
    lcd_A(HIDE);
    lcd_V(SHOW);
    lcd_p10(SHOW);
    lcd_p100(SHOW);
    lcd_p1000(SHOW);
    lcd_p1000000(SHOW);
    lcd_anten(3);
    lcd_battery(3);
    lcd_reverse(SHOW);
    lcd_history(SHOW);
    lcd_id(SHOW);
    lcd_warning(SHOW);
    lcd_setting(SHOW);
    lcd_dola(SHOW);
    lcd_time(SHOW);
    lcd_date(SHOW);
    lcd_clock_dot(SHOW);
    lcd_t1(SHOW);
    lcd_t(SHOW);
    lcd_split(SHOW);

}