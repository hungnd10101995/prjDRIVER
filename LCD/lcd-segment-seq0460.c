/*===========================================================================*/
/*
SOURCE_FILE:		lcd-segment-ost11295.c
APPLICATION:		DT03M-RF
MODULE:				lcd
ORIGINAL AUTHOR:	thaodtn
VERSION:			1.0
DATE:				25/10/2013

(C) 2013: CPC IT

All rights are reserved. Reproduction in whole or in part is
prohibited without the prior written consent of the copyright
owner. The information presented in this document does not
form part of any quotation or contract, is believed to be
accurate and reliable and may be changed without notice.
No liability will be accepted by the publisher for any
consequence of its use. Publication thereof does not convey
nor imply any license under patent- or other industrial or
intellectual property rights.
*/
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
//#include <msp430x471x7.h>
#include <stdint.h>
#include <stdlib.h>
#include "..\library.h"
#if defined(_LCD_SEQ0460_) //endif to end of file
/*===========================================================================*/
#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter-hardware.h"
#endif

#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-hardware.h"
#endif

#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter-hardware.h"
#endif

#ifdef _DT01PP_RF_
#include "..\..\dt01pp\fw\emeter-hardware.h"
#endif

#include "lcd-segment-seq0460.h"
#include "..\framework\macro.h"
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
#define CODE_SX_75_CONGTO

#ifdef CODE_SX_75_CONGTO
const char lcd_seg_data[10] = {0x5F,0x06,0x6B,0x2F,0x36,0x3D,0x7D,0x07,0x7F,0x3F}; //theo code cu
#else
// bang ma de hien thi so(0...9) o cac vi tri 1..8 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit thap, seg D,E,G,F nam o 4 bit cao cua LCDMx(x=0...20)
const char lcd_seg_data[MAX_SEG_DATA] = {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9,CHAR_C};
   //cac so tuong ung:           0    1    2    3    4    5    6    7    8    9   -
#endif
//=================================================================================
// bang ma de hien thi so(0...9) o cac vi tri 9..10 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit cao, seg D,E,G,F nam o 4 bit thap cua LCDMx(x=0...20)
#ifdef CODE_SX_75_CONGTO
const char lcd_seg_data2[10] = {0xF5,0x60,0xB6,0xF2,0x63,0xD3,0xD7,0x70,0xF7,0xF3};
#else
const char lcd_seg_data2[MAX_SEG_DATA_2] = {swap4bit(DIGIT_0),swap4bit(DIGIT_1),swap4bit(DIGIT_2),swap4bit(DIGIT_3),
								swap4bit(DIGIT_4),swap4bit(DIGIT_5),swap4bit(DIGIT_6),
								swap4bit(DIGIT_7),swap4bit(DIGIT_8),swap4bit(DIGIT_9),swap4bit(CHAR_C)};
   //cac so tuong ung:            0    1    2    3    4    5    6    7    8    9   C
#endif

//bang ma hien thi chu cai (ver)
//const char lcd_seg_alphabet[6] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C};
const unsigned char lcd_seg_alphabet[] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,
                                            CHAR_C,CHAR_H,CHAR_DASH,CHAR_P,CHAR_O,
                                            CHAR_Y, CHAR_N, CHAR_L, CHAR_r, CHAR_t, CHAR_I}; 
//cac chu cai tuong ung
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
//FUNCTION NAME:    lcd_display_digit
//PROTOTYPE:        void lcd_display_digit(unsigned char value,unsigned char  pos)
//INPUT:            'value':gia tri ta muon hien thi(0...9),
//                  'pos': vi tri hien thi gia tri(1...8,10...15)
//OUTPUT:
//DESCRIPTION:      hien thi cac so(0...9) tai cac vi tri 1...8,10...15 tren LCD
//NOTE:ok
//============================================================================
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
#ifdef CODE_SX_75_CONGTO
    uint8_t data;
    uint8_t unused_bit_uper; //bit ko su dung cua day chu so o duoi
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o tren
    unused_bit_uper = 0x08;
    unused_bit_lower = 0x80;
    if (value == DISPLAY_OBIS_CHARACTER_C)
    {
        if ((pos == 3)||(pos == 4)) data = swap4bit(CHAR_C);
        else data = CHAR_C;
    }
    else if (value == '-')//DISPLAY_DASH)
    {
        if (pos < 9) data = swap4bit(CHAR_DASH);
        else data = CHAR_DASH;
    }
    else if (value < 10) //hien thi so
    {
        if (pos > 9) data = lcd_seg_data2[value];
        else data = lcd_seg_data[value];
    }
    else
    {
        //do nothing
    }
    
    switch (pos)
    {
        // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
        LCDM3 = (LCDM3 & 0x80) | data;//lcd_seg_data[value];
        break;
        case 2:
        LCDM4 = (LCDM4 & 0x80) | data;//lcd_seg_data[value];
        break;
        case 3: // vi tri nay co seg A,B,C,D o 4 bit cao cua LCDM5, seg E,G,F o 4 bit thap cua LCDM6
        LCDM5 = (LCDM5 & 0x0F) | (data<<4);//(lcd_seg_data[value]<<4);
        LCDM6 = (LCDM6 & 0xF8) | (data>>4);//(lcd_seg_data[value]>>4);
        break;
        case 4: // vi tri nay co seg A,B,C,D o 4 bit cao cua LCDM6, seg E,G,F o 4 bit thap cua LCDM7
        LCDM6 = (LCDM6 & 0x0F) | (data<<4);//(lcd_seg_data[value]<<4);
        LCDM7 = (LCDM7 & 0xF8) | (data>>4);//(lcd_seg_data[value]>>4);
        break;
        case 5:
        LCDM8 = (LCDM8 & 0x80) | data;//lcd_seg_data[value];
        break;
        case 6:
        LCDM9 = (LCDM9 & 0x80) | data;//lcd_seg_data[value];
        break;
        case 7:
        LCDM10 = (LCDM10 & 0x80) | data;//lcd_seg_data[value];
        break;
        case 8:
        LCDM11 = (LCDM11 & 0x80) | data;//lcd_seg_data[value];
        break;
        // vi tri tu 10...15 su dung bang ma lcd_seg_data2
        case 10:
        LCDM17 = (LCDM17 & 0x08) | data;//lcd_seg_data2[value];
        break;
        case 11:
        LCDM16 = (LCDM16 & 0x08) | data;//lcd_seg_data2[value];
        break;
        case 12:
        LCDM15 = (LCDM15 & 0x08) | data;//lcd_seg_data2[value];
        break;
        case 13:
        LCDM14 = (LCDM14 & 0x08) | data;//lcd_seg_data2[value];
        break;
        case 14:
        LCDM13 = (LCDM13 & 0x08) | data;//lcd_seg_data2[value];
        break;
        case 15:
        LCDM12 = (LCDM12 & 0x08) | data;//lcd_seg_data2[value];
        break;
    }
    
    
#else
    uint8_t data;
    uint8_t unused_bit_uper; //bit ko su dung cua day chu so o duoi
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o tren

#if defined(_DT01M_RF_)
    unused_bit_uper = 0x80;
    unused_bit_lower = 0x08;
    if (value == DISPLAY_OBIS_CHARACTER_C)
    {
        if (pos < 9) data = CHAR_C;
        else data = swap4bit(CHAR_C);
    }
    else if (value == '-')//DISPLAY_DASH)
    {
        if (pos < 9) data = CHAR_DASH;
        else data = swap4bit(CHAR_DASH);
    }
    else if (value < 10) //hien thi so
    {
        if (pos < 9) data = lcd_seg_data[value];
        else data = lcd_seg_data2[value];
    }
    else
    {
        //do nothing
    }
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_) //dt03m nguoc voi dt01m
    unused_bit_uper = 0x08;
    unused_bit_lower = 0x80;
    if (value == DISPLAY_OBIS_CHARACTER_C)
    {
        if (pos < 9) data = swap4bit(CHAR_C);
        else data = CHAR_C;
    }
    else if (value == '-')//DISPLAY_DASH)
    {
        if (pos < 9) data = swap4bit(CHAR_DASH);
        else data = CHAR_DASH;
    }
    else if (value < 10) //hien thi so
    {
        if (pos < 9) data = lcd_seg_data2[value];
        else data = lcd_seg_data[value];
    }
    else
    {
        //do nothing
    }
#else
#warning "LCD: Check meter type define"
#endif

    switch (pos)
    {
        // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
            LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & unused_bit_lower) | data;
            break;
        case 2:
            LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & unused_bit_lower) | data;
            break;
        case 3:
            LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & unused_bit_lower) | data;
            break;
        case 4:
            LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & unused_bit_lower) | data;
            break;
        case 5:
            LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & unused_bit_lower) | data;
            break;
        case 6:
            LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & unused_bit_lower) | data;
            break;
        case 7:
            LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & unused_bit_lower) | data;
            break;
        case 8:
            LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & unused_bit_lower) | data;
            break;
        // vi tri tu 9...15 su dung bang ma lcd_seg_data2
        case 9:
            LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & unused_bit_uper) | data;
            break;
        case 10:
            LCD_DIGIT10_MEM = (LCD_DIGIT10_MEM & unused_bit_uper) | data;
            break;
        case 11:
            LCD_DIGIT11_MEM = (LCD_DIGIT11_MEM & unused_bit_uper) | data;
            break;
        case 12:
            LCD_DIGIT12_MEM = (LCD_DIGIT12_MEM & unused_bit_uper) | data;
            break;
        case 13:
            LCD_DIGIT13_MEM = (LCD_DIGIT13_MEM & unused_bit_uper) | data;
            break;
        case 14:
            LCD_DIGIT14_MEM = (LCD_DIGIT14_MEM & unused_bit_uper) | data;
            break;
        case 15:
            LCD_DIGIT15_MEM = (LCD_DIGIT15_MEM & unused_bit_uper) | data;
            break;
        case 16:
            //LCD_DIGIT16_MEM = (LCD_DIGIT16_MEM & unused_bit_uper) | data;
            break;
        default:
            break;
    }
#endif
    
}

void lcd_hide_digit(unsigned char  pos)
{
    uint8_t mask_uper;
    uint8_t mask_lower;

#if defined(_DT01M_RF_)
    mask_uper = swap4bit(DIGIT_8);
    mask_lower = DIGIT_8;
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_)
    mask_uper = DIGIT_8;
    mask_lower = swap4bit(DIGIT_8);
#else
    #warning "LCD: Check meter type define"
#endif
    switch (pos)
    {
        // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
            LCD_DIGIT1_MEM &= ~mask_lower; //~DIGIT_8;
            break;
        case 2:
            LCD_DIGIT2_MEM &= ~mask_lower;
            break;
        case 3:
            LCD_DIGIT3_MEM &= ~mask_lower;
            break;
        case 4:
            LCD_DIGIT4_MEM &= ~mask_lower;
            break;
        case 5:
            LCD_DIGIT5_MEM &= ~mask_lower;
            break;
        case 6:
            LCD_DIGIT6_MEM &= ~mask_lower;
            break;
        case 7:
            LCD_DIGIT7_MEM &= ~mask_lower;
            break;
        case 8:
            LCD_DIGIT8_MEM &= ~mask_lower;
            break;
        // vi tri tu 9...15 su dung bang ma lcd_seg_data2
        case 9:
            LCD_DIGIT9_MEM &= ~mask_uper; //~temp8;
            break;
        case 10:
            LCD_DIGIT10_MEM &= ~mask_uper;
            break;
        case 11:
            LCD_DIGIT11_MEM &= ~mask_uper;
            break;
        case 12:
            LCD_DIGIT12_MEM &= ~mask_uper;
            break;
        case 13:
            LCD_DIGIT13_MEM &= ~mask_uper;
            break;
        case 14:
            LCD_DIGIT14_MEM &= ~mask_uper;
            break;
        case 15:
            LCD_DIGIT15_MEM &= ~mask_uper;
            break;
        case 16:
            //LCD_DIGIT16_MEM &= ~mask_uper;
            break;
        default:
            break;
    }
}
/*===========================================================================*/
/* G L O B A L   F U N C T I O N S   P R O T O T Y P E S                     */
/*===========================================================================*/
//===========================================================================
//FUNCTION NAME:    lcd_clear_all
//PROTOTYPE:        void lcd_clear_all(void)
//INPUT:
//OUTPUT:
//DESCRIPTION:      xoa het cac ky tu tren LCD
//NOTE:
//============================================================================
void lcd_clear_all(void)
{
    int i;
    for (i = 0; i < 20; i++)
    {
        *(&LCDM1 + i) = 0x00;
    }
}

//===========================================================================
//FUNCTION NAME:    lcd_display_value
//PROTOTYPE:        void lcd_display_value(unsigned long value)
//INPUT:            value: so thap phan can hien thi
//OUTPUT:
//DESCRIPTION:      hien thi 8 chu so tren LCD
//NOTE:
//============================================================================
//void lcd_display_value(unsigned long value)
void lcd_display_value(long value)
{
    long value1;
    unsigned char temp;
    if (value >= 0) value1 = value;
    else value1 = -value;
    for(int i = 1; i < 9; i++)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        temp = value1 % 10;
        lcd_display_digit(temp, i);
        value1 /= 10;
        if (value1 == 0) //het chu so thi thoat
        {
            i = 9;
        }
    }
}

//===========================================================================
//FUNCTION NAME:    lcd_display_tariff
//PROTOTYPE:        void lcd_display_tariff(unsigned char value, char status)
//INPUT:            value: 2 chu so bieu gia, status: SHOW/ HIDE (blinking)
//OUTPUT:
//DESCRIPTION:      hien thi chu 'T' va cac gia tri tai chu 'T' tren LCD(vi tri so 9 va 16)
//NOTE:
//============================================================================
void lcd_display_tariff(unsigned char value, char status)
{
    if (status == SHOW)
    {
        //LCDM18 |= BIT3;
#if defined(_DT03M_RF_) || defined(_DT01M_RF_)
        LCD_TARIFF_MEM |= LCD_TARIFF_BIT;
#endif
        lcd_display_digit(value%10, 16);
        lcd_display_digit(value/10, 9);
        //tam thoi cho mach cu chan cua so 16 dung cho eeprom 2
        //lcd_display_digit(value%10,9);//dich hang don vi sang so 9
    }
    else
    {
        //thaodtn LCDM19 &= 0x08;//0x80;
#if defined(_DT03M_RF_) || defined(_DT01M_RF_)
        LCD_TARIFF_MEM &= ~LCD_TARIFF_BIT;
#endif
        lcd_hide_digit(16);
        lcd_hide_digit(9);
    }
}

//===========================================================================
//FUNCTION NAME:    lcd_display_obis
//PROTOTYPE:        void lcd_display_obis(unsigned long value)
//INPUT:            'value':gia tri ta muon hien thi
//OUTPUT:
//DESCRIPTION:      hien thi cac gia tri ma ta muon hien thi tren LCD(tai vi tri 10...15)
//NOTE:
//============================================================================
void lcd_display_obis(unsigned long value)
{
    unsigned long value1;
    unsigned char temp;
    unsigned char i;
    //j=15;//14;//16
    value1=value;
    lcd_point6(1);
    lcd_point7(1);
    if (value1 == 91)
    {
        lcd_display_digit(1,11);
        lcd_display_digit(9,12);
        lcd_display_digit(0,13);
    }
    else
    {
        temp = value1 % 100;
        if (temp == 0)
        {
            value1 /= 10;
            for(i = 11; i < 15; i++)
            {
                temp = value1%10;
                if (!((i == 14)&&(temp == 0)))
                {
                    lcd_display_digit(temp,i);
                }
                value1 /= 10;
            }
        }
        else
        {
            for (i = 10; i < 15; i++)
            {
                temp = value1 % 10;
                if (!((i == 14)&&(temp == 0)))
                {
                    lcd_display_digit(temp,i);
                }
                value1 /= 10;
            }
        }
    }
}
#if defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
void lcd_M(char status)
{
    if (status == SHOW)
    {
        LCD_S9_MEM |= LCD_S9_BIT;
        LCD_S11_MEM |= LCD_S11_BIT;
    }
    else
    {
        LCD_S9_MEM &= ~LCD_S9_BIT;
        LCD_S11_MEM &= ~LCD_S11_BIT;
    }
}
#endif
#if defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
void lcd_k(char status)
{
    if (status == SHOW)
    {
        LCD_S9_MEM |= LCD_S9_BIT;
        LCD_S10_MEM |= LCD_S10_BIT;
    }
    else
    {
        LCD_S9_MEM &= ~LCD_S9_BIT;
        LCD_S10_MEM &= ~LCD_S10_BIT;
    }
}
#endif
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_k_up(char status)
{
	if (status == SHOW)
	{
		LCD_K_UP_MEM |= LCD_K_UP_BIT;
	}
	else
	{
		LCD_K_UP_MEM &= ~LCD_K_UP_BIT;
	}
}
void lcd_k_down(char status)
{
	if (status == SHOW)
	{
		LCD_K_DOWN_MEM |= LCD_K_DOWN_BIT;
	}
	else
	{
		LCD_K_DOWN_MEM &= ~LCD_K_DOWN_BIT;
	}
}
#endif

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_V(char status)
{
    /*
	if (status == SHOW)
	{
		LCD_V_MEM |= LCD_V_BIT;
	}
	else
	{
		LCD_V_MEM &= ~LCD_V_BIT;
	}*/
    if (status == 1)
        LCDM1 |= BIT4;
    else LCDM1 &= ~BIT4;
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_A(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
	if (status == SHOW)
	{
		LCD_A_MEM |= LCD_A_BIT;
	}
	else
	{
		LCD_A_MEM &= ~LCD_A_BIT;
	}
#elif defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
    if (status == SHOW)
	{
		LCD_S14_MEM |= LCD_S14_BIT;
        LCD_S5_MEM |= LCD_S5_BIT;
	}
	else
	{
		LCD_S14_MEM &= ~LCD_S14_BIT;
        LCD_S5_MEM &= ~LCD_S5_BIT;
	}
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}

void lcd_r(char status)
{
	if (status == SHOW)
	{
		LCD_R_MEM |= LCD_R_BIT;
	}
	else
	{
		LCD_R_MEM &= ~LCD_R_BIT;
	}
}

void lcd_W(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
	if (status == SHOW)
	{
		LCD_W_MEM |= LCD_W_BIT;
	}
	else
	{
		LCD_W_MEM &= ~LCD_W_BIT;
	}
#elif defined(_LCD_L0712_) //(_PCB_FOR_LCD_BLAZE_V1_)
        if (status == SHOW)
	{
            lcd_V(status);
            LCD_S13_MEM |= LCD_S13_BIT;
            LCD_S14_MEM |= LCD_S14_BIT;
	}
	else
	{
            lcd_V(status);
            LCD_S13_MEM &= ~LCD_S13_BIT;
            LCD_S14_MEM &= ~LCD_S14_BIT;
	}
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
#if defined(_LCD_L0712_)//(_PCB_FOR_LCD_BLAZE_V1_)
void lcd_h(char status)
{
	if (status == SHOW)
	{
		LCD_H_MEM |= LCD_H_BIT;
	}
	else
	{
		LCD_H_MEM &= ~LCD_H_BIT;
	}
}

void lcd_h_left(char status)
{
	if (status == SHOW)
	{
		lcd_r(SHOW);
        LCD_S16_MEM |= LCD_S16_BIT;
        LCD_S17_MEM |= LCD_S17_BIT;
	}
	else
	{
		lcd_r(SHOW);
        LCD_S16_MEM &= ~LCD_S16_BIT;
        LCD_S17_MEM &= ~LCD_S17_BIT;
	}
}
#endif
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_h_up(char status)
{
	if (status == SHOW)
	{
		LCD_H_UP_MEM |= LCD_H_UP_BIT;
	}
	else
	{
		LCD_H_UP_MEM &= ~LCD_H_UP_BIT;
	}
}

void lcd_h_down(char status)
{
	if (status == SHOW)
	{
		LCD_H_DOWN_MEM |= LCD_H_DOWN_BIT;
	}
	else
	{
		LCD_H_DOWN_MEM &= ~LCD_H_DOWN_BIT;
	}
}
#endif
//===========================================================================
//FUNCTION NAME:    lcd_kWh
//PROTOTYPE:        void lcd_kWh(char status)
//INPUT:            status: SHOW/HIDE
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kWh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_down(status);
    lcd_W(status);
    lcd_h_down(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_W(status);
    lcd_h_left(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif    
}

//===========================================================================
//FUNCTION NAME:    lcd_Wh
//PROTOTYPE:        void lcd_Wh(char status)
//INPUT:            status: SHOW/HIDE
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_Wh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_W(status);
    lcd_h_down(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_W(status);
    lcd_h_left(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kVArh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_up(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h_up(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif    
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_varh
//  Created     : 12/6/2014, by thaodtn
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_VArh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h_up(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kVAh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_up(status);
    lcd_V(status);
    lcd_A(status);
    lcd_h_up(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_vah
//  Created     : 12/6/2014, by thaodtn
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_VAh(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_V(status);
    lcd_A(status);
    lcd_h_up(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kV(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_down(status);
    lcd_V(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_V(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif

//    if (status == SHOW)
//    {
//        LCDM11 |= BIT0;
//        LCDM10 |= BIT4;
//    }
//    else
//    {
//        LCDM11 &= ~BIT0;
//        LCDM10 &= ~(BIT4);
//    }
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kA(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_up(status);
    lcd_A(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_A(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kW(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_down(status);
    lcd_W(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_W(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_MW(char status)
{
    #if defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_M(status);
    lcd_W(status);
    #endif
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_VA(char status)
{
    lcd_V(status);
    lcd_A(status);
//    if (status == SHOW)
//    {
//        LCDM11 |= (BIT0+BIT1);
//    }
//    else
//    {
//        LCDM11 &= ~(BIT0+BIT1);
//    }
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kVA(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_up(status);
    lcd_V(status);
    lcd_A(status);
#elif defined(_LCD_L0712_)//defined (_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_V(status);
    lcd_A(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_VAr(char status)
{
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
//	if (status == SHOW)
//	{
//		LCDM11 |= (BIT0+BIT1+BIT2);
//	}
//	else
//	{
//		LCDM11 &= ~(BIT0+BIT1+BIT2);
//	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_kVAr(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
    lcd_k_up(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_MVAr(char status)
{
    #if defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_M(status);
    lcd_VAr(status);
    #endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_MVA(char status)
{
    #if defined(_LCD_L0712_) //defined(_PCB_FOR_LCD_BLAZE_V1_)
    lcd_M(status);
    lcd_VA(status);
    #endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_Hz(char status)
{
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
	if (status == SHOW)
	{
		LCD_HZ_MEM |= LCD_HZ_BIT;
	}
	else
	{
		LCD_HZ_MEM &= ~LCD_HZ_BIT;
	}
#elif defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
        if (status == SHOW)
	{
            LCD_H_MEM |= LCD_H_BIT;
            LCD_P5_MEM |= LCD_P5_BIT;
            LCD_Z_MEM |= LCD_Z_BIT;
	}
	else
	{
            LCD_H_MEM &= ~LCD_H_BIT;
            LCD_P5_MEM &= ~LCD_P5_BIT;
            LCD_Z_MEM &= ~LCD_Z_BIT;
	}
#else
#warning "check define LCD in emeter-hardware.h"
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_anten(char status)
{
	if (status == SHOW)
	{
		LCD_ANTEN_MEM |= LCD_ANTEN_BIT;
	}
	else
	{
		LCD_ANTEN_MEM &= ~LCD_ANTEN_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_warning(char status)
{
	if (status == SHOW)
	{
		//LCD_WARNING_MEM |= LCD_WARNING_BIT;
	}
	else
	{
		//LCD_WARNING_MEM &= ~LCD_WARNING_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_lowbat(char status)
{
	if (status == SHOW)
	{
		LCD_BATT_MEM |= LCD_BATT_BIT;
	}
	else
	{
		LCD_BATT_MEM &= ~LCD_BATT_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_reverse(char status)
{
	if (status == SHOW)
	{
		LCD_REVERSE_MEM |= LCD_REVERSE_BIT;
	}
	else
	{
		LCD_REVERSE_MEM &= ~LCD_REVERSE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_Er(char status)
{
    if (status == SHOW)
    {
        LCD_ER_MEM |= LCD_ER_BIT;
    }
    else
    {
        LCD_ER_MEM &= ~LCD_ER_BIT;
    }
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_ACB(char status)
{
	if (status == SHOW)
	{
		//LCD_ACB_MEM |= LCD_ACB_BIT;
	}
	else
	{
		//LCD_ACB_MEM &= ~LCD_ACB_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_SUM(char status)
{
	if (status == SHOW)
	{
		LCD_SUM_MEM |= LCD_SUM_BIT;
	}
	else
	{
		LCD_SUM_MEM &= ~LCD_SUM_BIT;
	}
}
#endif
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_MD(char status)
{
	if (status == SHOW)
	{
		//LCD_MD_MEM |= LCD_MD_BIT;
	}
	else
	{
		//LCD_MD_MEM &= ~LCD_MD_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_COL2(char status)
{
    /*
    if (status == SHOW)
    {
        LCD_COL2_MEM |= LCD_COL2_BIT;
    }
    else
    {
        LCD_COL2_MEM &= ~LCD_COL2_BIT;
    }
    */
    if (status == 1)
    {LCDM7 |= BIT5;LCDM5 |= BIT1;}
    else
    {LCDM7 &= ~BIT5;LCDM5 &= ~BIT1;}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_Err(char status)
{
	if (status == SHOW)
	{
		LCD_ERR_MEM |= LCD_ERR_BIT;
	}
	else
	{
		LCD_ERR_MEM &= ~LCD_ERR_BIT;
	}
}
#endif
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_L1(char status)
{
	if (status == SHOW)
	{
		LCD_L1_MEM |= LCD_L1_BIT;
	}
	else
	{
		LCD_L1_MEM &= ~LCD_L1_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_L2(char status)
{
	if (status == SHOW)
	{
		LCD_L2_MEM |= LCD_L2_BIT;
	}
	else
	{
		LCD_L2_MEM &= ~LCD_L2_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_L3(char status)
{
	if (status == SHOW)
	{
		LCD_L3_MEM |= LCD_L3_BIT;
	}
	else
	{
		LCD_L3_MEM &= ~LCD_L3_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_phA(char status)
{
    /*if (status == SHOW)
    {
        LCDM1 |= (BIT6|BIT5|BIT4|BIT2|BIT1|BIT0);
    }
    else
    {
        LCDM1 &= ~(BIT6|BIT5|BIT4|BIT2|BIT1|BIT0);
    }*/
	lcd_display_alphabet(3,8);
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_phB(char status)
{
    /*if (status == SHOW)
    {
        LCDM1 |= (BIT0|BIT1|BIT2|BIT6|BIT7);
    }
    else
    {
        LCDM1 &= ~(BIT0|BIT1|BIT2|BIT6|BIT7);
    }*/
	lcd_display_alphabet(4,8);
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_phC(char status)
{
    /*if (status == SHOW)
    {
        LCDM1 |=  (BIT0|BIT2|BIT4|BIT7);
    }
    else
    {
        LCDM1 &= ~(BIT0|BIT2|BIT4|BIT7);
    }*/
	lcd_display_alphabet(5,8);
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:  ko dung
//============================================================================
void lcd_Point2_4(char status)
{
    if (status == SHOW)
    {
        LCDM7 |= BIT5|BIT2;
    }
    else
    {
        LCDM7 &= ~BIT5|BIT2;
    }
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_point1(char status)
{
	if (status == SHOW)
	{
		LCD_POINT1_MEM |= LCD_POINT1_BIT;
	}
	else
	{
		LCD_POINT1_MEM &= ~LCD_POINT1_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_point2(char status)
{
	if (status == SHOW)
	{
		LCD_POINT2_MEM |= LCD_POINT2_BIT;
	}
	else
	{
		LCD_POINT2_MEM &= ~LCD_POINT2_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_point3(char status)
{
	if (status == SHOW)
	{
		LCD_POINT3_MEM |= LCD_POINT3_BIT;
	}
	else
	{
		LCD_POINT3_MEM &= ~LCD_POINT3_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_point4(char status)
{
	if (status == SHOW)
	{
		LCD_POINT4_MEM |= LCD_POINT4_BIT;
	}
	else
	{
		LCD_POINT4_MEM &= ~LCD_POINT4_BIT;
	}
}
#endif
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
#if defined(_LCD_OST11295_) || defined(_LCD_SEQ0460_)
void lcd_point5(char status)
{
	if (status == SHOW)
	{
		LCD_POINT5_MEM |= LCD_POINT5_BIT;
	}
	else
	{
		LCD_POINT5_MEM &= ~LCD_POINT5_BIT;
	}
}
#endif
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_point6(char status)
{
	if (status == SHOW)
	{
		LCD_POINT6_MEM |= LCD_POINT6_BIT;
	}
	else
	{
		LCD_POINT6_MEM &= ~LCD_POINT6_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_point7(char status)
{
	if (status == SHOW)
	{
		LCD_POINT7_MEM |= LCD_POINT7_BIT;
	}
	else
	{
		LCD_POINT7_MEM &= ~LCD_POINT7_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_point8(char status)
{
	if (status == SHOW)
	{
		LCD_POINT8_MEM |= LCD_POINT8_BIT;
	}
	else
	{
		LCD_POINT8_MEM &= ~LCD_POINT8_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================

void lcd_point9(char status)
{
	if (status == SHOW)
	{
		LCD_POINT9_MEM |= LCD_POINT9_BIT;
	}
	else
	{
		LCD_POINT9_MEM &= ~LCD_POINT9_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_GND(char status)
{
	if (status == SHOW)
	{
		LCD_GND_MEM |= LCD_GND_BIT;
	}
	else
	{
		LCD_GND_MEM &= ~LCD_GND_BIT;
	}
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_Q_reverse(char status)
{
	if (status == SHOW)
	{
		LCD_Q_REVERSE_MEM |= LCD_Q_REVERSE_BIT;
	}
	else
	{
		LCD_Q_REVERSE_MEM &= ~LCD_Q_REVERSE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_P_reverse(char status)
{
	if (status == SHOW)
	{
		LCD_P_REVERSE_MEM |= LCD_P_REVERSE_BIT;
	}
	else
	{
		LCD_P_REVERSE_MEM &= ~LCD_P_REVERSE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_P_active(char status)
{
	if (status == SHOW)
	{
		LCD_P_ACTIVE_MEM |= LCD_P_ACTIVE_BIT;
	}
	else
	{
		LCD_P_ACTIVE_MEM &= ~LCD_P_ACTIVE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_Q_active(char status)
{
	if (status == SHOW)
	{
		LCD_Q_ACTIVE_MEM |= LCD_Q_ACTIVE_BIT;
	}
	else
	{
		LCD_Q_ACTIVE_MEM &= ~LCD_Q_ACTIVE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_magnetic(char status)
{
	if (status == SHOW)
	{
		LCD_MAGNETIC_MEM |= LCD_MAGNETIC_BIT;
	}
	else
	{
		LCD_MAGNETIC_MEM &= ~LCD_MAGNETIC_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_unlock(char status)
{
	if (status == SHOW)
	{
		//LCD_UNLOCK_MEM |= LCD_UNLOCK_BIT;
	}
	else
	{
		//LCD_UNLOCK_MEM &= ~LCD_UNLOCK_BIT;
	}
}



//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:ok
//============================================================================
void lcd_powerfail(char status)
{
	if (status == SHOW)
	{
		LCD_POWERFAIL_MEM |= LCD_POWERFAIL_BIT;
	}
	else
	{
		LCD_POWERFAIL_MEM &= ~LCD_POWERFAIL_BIT;
	}
}


//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void lcd_display_alphabet(unsigned char value,unsigned char  pos)
{
    //if (value > 3) return;
    uint8_t data;
    //uint8_t unused_bit_uper; //bit ko su dung cua day chu so o tren
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o duoi

#if defined(_DT01M_RF_)
    //unused_bit_uper = 0x80;
    unused_bit_lower = 0x08;
    data = lcd_seg_alphabet[value];

#elif defined(_DT03M_RF_) || defined(_DT03P_RF_) //dt03m nguoc voi dt01m
    //unused_bit_uper = 0x08;
    unused_bit_lower = 0x80;
    data = swap4bit(lcd_seg_alphabet[value]);
    /* test
    data = lcd_seg_alphabet[value] & 0x0f;
    data = data<<4;
    data = data | (lcd_seg_alphabet[value] >> 4);
    */
#else
#warning "LCD: Check meter type define"
#endif
    switch (pos)
    {
        // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
            LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & unused_bit_lower) | data;//lcd_seg_alphabet[value];
            break;
        case 2:
            LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & unused_bit_lower) | data;
            break;
        case 3:
            LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & unused_bit_lower) | data;
            break;
        case 4:
            LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & unused_bit_lower) | data;
            break;
        case 5:
            LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & unused_bit_lower) | data;
            break;
        case 6:
            LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & unused_bit_lower) | data;
            break;
        case 7:
            LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & unused_bit_lower) | data;
            break;
        case 8:
            LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & unused_bit_lower) | data;
            break;
    }
}

#endif //#if defined(_LCD_OST11295_)  || defined(_LCD_L0712_)
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/


