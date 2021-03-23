//#include <msp430x471x7.h>
//#include <emeter-toolkit.h>
//#include "emeter-structs.h"
//#include <msp430f6731.h>
#if defined(__XC8)
    #include <xc.h>
#endif
#include <stdint.h>
#include <stdlib.h>
#ifdef __MSP430__
#include <io.h>
#endif
#include "..\library.h"

#ifdef _LCD_OST11248_
//----------------------------------------------------------------------------
#include "lcd-segment-ost11248.h"
#include "../../LIBRARY/framework/delay.h"
#include "..\framework\macro.h"
#include "../../LIBRARY/framework/process.h"
#if defined(_DT01P_RF_) && defined(__MSP430__)
#include "..\..\dt01p80-rf\fw\emeter-main.h"
#include "..\..\dt01p80-rf\fw\emeter-hardware.h"
#endif
#if defined(_DT01P_RF_) && defined(_18F86J94) 
#include "..\..\dt01p80-rf\fw\main.h"
#include "..\..\dt01p80-rf\fw\hardware.h"
#endif

#ifdef _PREPAID_METER_
#include "..\..\prepaid-meter\fw\emeter\emeter-main.h"
#include "..\..\prepaid-meter\fw\emeter\emeter-hardware.h"
#endif

#if defined(_TI_MEASURE_1P_)
#include "../../LIBRARY/1-phase-measurement/measure-struct.h"
#include "../../LIBRARY/1-phase-measurement/measure-foreground.h"
#elif defined(_ADE7953_)
//#include "../../LIBRARY/ade/ade-1ph-measure.h"
#endif

#ifdef _IHD_
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
#elif defined(PCB_EXT_V2)
    #define SEG_A	0x80//0x10
    #define SEG_B	0x08//0x01
    #define SEG_C	0x02//0x04
    #define SEG_D	0x10//0x80
    #define SEG_E	0x20//0x40
    #define SEG_F	0x40//0x20
    #define SEG_G	0x04//0x02
#elif defined(_DT01PRF_PCB_TI_2014_)||defined(_DT01PRF_PCB_TI_2016_) || defined(_DT01P80_PCB_TI_S4_T_V24_)
    #define SEG_A	0x01
    #define SEG_B	0x10
    #define SEG_C	0x40
    #define SEG_D	0x08
    #define SEG_E	0x04
    #define SEG_F	0x02
    #define SEG_G	0x20
#elif defined (_IHD_)
    #define SEG_A	BIT4
    #define SEG_B	BIT5
    #define SEG_C	BIT6
    #define SEG_D	BIT7
    #define SEG_E	BIT2
    #define SEG_F	BIT0
    #define SEG_G	BIT1
#elif defined(_DT01P80_EXP_PCB_PIC_S4_T_V10_)
    #define SEG_A	0x01
    #define SEG_B	0x02
    #define SEG_C	0x04
    #define SEG_D	0x08
    #define SEG_E	0x10
    #define SEG_F	0x20
    #define SEG_G	0x40
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
//const char lcd_seg_data[16] = {0xD7,0x06,0xE3,0xA7,0x36,0xB5,0xF5,0x07,0xF7,0xB7,0x77,0x77,0x77,0x77,0x77,0x77}; // vinh test
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
                                            CHAR_Y, CHAR_N, CHAR_L, CHAR_r, CHAR_t}; 
unsigned char flag_display_all = 0;
uint8_t pos_display;
void lcd_clear_all(void)
{
#if defined(__MSP430__)
    LCDCMEMCTL set LCDCLRM;
    // unsigned char i;
    //  for (i=0;i<20;i++) *(&LCDM1+i)=0x00;
#elif defined(_18F86J94)
    unsigned char i;
    for (i=0;i<30;i++) *(&LCDDATA0+i)=0x00;
//    LCDDATA0 = 0x00;  LCDDATA1 = 0x00;  LCDDATA2 = 0x00;  LCDDATA3 = 0x00;  LCDDATA4 = 0x00;
//    LCDDATA5 = 0x00;  LCDDATA6 = 0x00;  LCDDATA7 = 0x00;  LCDDATA8 = 0x00;  LCDDATA9 = 0x00;
//    LCDDATA10 = 0x00; LCDDATA11 = 0x00; LCDDATA12 = 0x00; LCDDATA13 = 0x00; LCDDATA14 = 0x00;
//    LCDDATA15 = 0x00; LCDDATA16 = 0x00; LCDDATA17 = 0x00; LCDDATA18 = 0x00; LCDDATA19 = 0x00;
//    LCDDATA20 = 0x00; LCDDATA21 = 0x00; LCDDATA22 = 0x00; LCDDATA23 = 0x00; LCDDATA24 = 0x00;
//    LCDDATA25 = 0x00; LCDDATA26 = 0x00; LCDDATA27 = 0x00; LCDDATA28 = 0x00; LCDDATA29 = 0x00;                               
#endif
}

void lcd_clear_8digit(void)
{
    #if defined(__MSP430__)
    unsigned char i;
    for (i=6;i<14;i++) *(&LCDM1+i)=0x00;
    #endif
}
#if defined(_18F86J94)
/*************************************************************************************************************
* Function: lcd_display_digit
* Desciption: hien thi cac so(0...9) tai cac vi tri 1...7
* Input: 'value':gia tri ta muon hien thi(0...9), 'pos': vi tri hien thi gia tri(1...7)
* Output: none
*************************************************************************************************************/
void lcd_display_value_digit(unsigned char *lcd_seg, unsigned char pos_digit)
{
    switch (pos_digit)
    {
        case 0:   //vi. tri' 0
            d0G = lcd_seg[6];
            d0F = lcd_seg[5];
            d0E = lcd_seg[4];
            d0D = lcd_seg[3];
            d0C = lcd_seg[2];
            d0B = lcd_seg[1];
            d0A = lcd_seg[0];
	     	break;
        case 1:    //vi. tri' 1
        d1G = lcd_seg[6];
        d1F = lcd_seg[5];
        d1E = lcd_seg[4];		
        d1D = lcd_seg[3];
        d1C = lcd_seg[2];
        d1B = lcd_seg[1];
        d1A = lcd_seg[0];
        break;
        case 2:
        d2G = lcd_seg[6];
        d2F = lcd_seg[5];
        d2E = lcd_seg[4];		
        d2D = lcd_seg[3];
        d2C = lcd_seg[2];
        d2B = lcd_seg[1];
        d2A = lcd_seg[0];
        break;	    
        case 3:
        d3G = lcd_seg[6];
        d3F = lcd_seg[5];
        d3E = lcd_seg[4];		
        d3D = lcd_seg[3];
        d3C = lcd_seg[2];
        d3B = lcd_seg[1];
        d3A = lcd_seg[0];
        break;
        case 4:
        d4G = lcd_seg[6];
        d4F = lcd_seg[5];
        d4E = lcd_seg[4];		
        d4D = lcd_seg[3];
        d4C = lcd_seg[2];
        d4B = lcd_seg[1];
        d4A = lcd_seg[0];
        break;
        case 5:
        d5G = lcd_seg[6];
        d5F = lcd_seg[5];
        d5E = lcd_seg[4];		
        d5D = lcd_seg[3];
        d5C = lcd_seg[2];
        d5B = lcd_seg[1];
        d5A = lcd_seg[0];
        break;
        case 6:
        d6G = lcd_seg[6];
        d6F = lcd_seg[5];
        d6E = lcd_seg[4];		
        d6D = lcd_seg[3];
        d6C = lcd_seg[2];
        d6B = lcd_seg[1];
        d6A = lcd_seg[0];
        break;
        case 7:
        d7G = lcd_seg[6];
        d7F = lcd_seg[5];
        d7E = lcd_seg[4];		
        d7D = lcd_seg[3];
        d7C = lcd_seg[2];
        d7B = lcd_seg[1];
        d7A = lcd_seg[0];
        break; 		
        case 8:
        d8G = lcd_seg[6];
        d8F = lcd_seg[5];
        d8E = lcd_seg[4];		
        d8D = lcd_seg[3];
        d8C = lcd_seg[2];
        d8B = lcd_seg[1];
        d8A = lcd_seg[0];
        break; 	
        case 9:
        d9G = lcd_seg[6];
        d9F = lcd_seg[5];
        d9E = lcd_seg[4];
        d9D = lcd_seg[3];
        d9C = lcd_seg[2];
        d9B = lcd_seg[1];
        d9A = lcd_seg[0];
        break; 	
    }
}
#endif
/*************************************************************************************************************
* Function: lcd_display_digit
* Desciption: hien thi cac so(0...9) tai cac vi tri 1...7
* Input: 'value':gia tri ta muon hien thi(0...9), 'pos': vi tri hien thi gia tri(1...7)
* Output: none
*************************************************************************************************************/
#if defined(__MSP430__)
#ifdef LCD_8MUX_MODE
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    switch (pos)
    {
        /*4 COM dau tien*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0xF8)|(lcd_seg_data[value]&0x0F);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0xF0)|(lcd_seg_data[value]>>4);
        break;
        /*4 COM tiep theo*/
    case 9:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 10:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 11:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 12:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 13:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 14:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 15:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    case 16:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F)|(lcd_seg_data[value]<<4);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x0F)|(lcd_seg_data[value]&0xF0);
        break;
    }
}
#else // mode 4 com

void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    switch (pos)
    {
#ifndef _IHD_
    case 0:
#if defined(PCB_EXT_V0)
        LCD_DIGIT0_MEM1 = (LCD_DIGIT0_MEM1 & 0x0F)|(lcd_seg_data[value]);  //& 0xF0
        LCD_DIGIT0_MEM2 = (LCD_DIGIT0_MEM2 & 0xF8)|(lcd_seg_data[value]);
#elif defined(PCB_EXT_V1)
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x8F)|(lcd_seg_data[value]<<4);
#elif defined(PCB_EXT_V2)        
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x1F)|(lcd_seg_data[value]<<4);
#elif defined(_DT01P80_PCB_TI_S4_T_V24_)
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x80)|lcd_seg_data[value];
#else
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x8F)|(lcd_seg_data[value]<<4);
#endif
        break;
#endif
    case 1:
#if defined(PCB_EXT_V1)
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x01)|lcd_seg_data[value];
#elif defined(_DT01P80_PCB_TI_S4_T_V24_)
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x8F)|(lcd_seg_data[value]<<4);    
#else
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 2:
#if defined(PCB_EXT_V1)
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 3:
#if defined(PCB_EXT_V1)
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x01)|lcd_seg_data[value];     
#else
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 4:
#if defined(PCB_EXT_V1)     
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 5:
#if defined(PCB_EXT_V1)    
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 6:
#if defined(PCB_EXT_V1)  
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 7:
#if defined(PCB_EXT_V1)  
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
    case 8:
#if defined(PCB_EXT_V1)       
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x08)|lcd_seg_data[value];
#elif defined(_IHD_)
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V2)
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x01)|lcd_seg_data[value];
#else
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
#ifndef _IHD_
    case 9:
#if defined(PCB_EXT_V0)
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x08)|lcd_seg_data[value];
#elif defined(PCB_EXT_V1)
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x8F)|(lcd_seg_data[value]<<4);
#elif defined(PCB_EXT_V2)
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0xF0)|(lcd_seg_data[value]>>4);  //& 0xF0
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x1F)|(lcd_seg_data[value]<<4);
#else
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x80)|lcd_seg_data[value];
#endif
        break;
#endif
    }
}
#endif // LCD_8MUX_MODE
#elif defined(_18F86J94)
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    unsigned char s, segment[7];
    for (s=0;s<7;s++)
    {
        if (bit_test(lcd_seg_data[value],s))segment[s]= ON;
        else segment[s]= OFF;
    }
    lcd_display_value_digit((unsigned char *)segment, pos);
}
#endif
////////////////////////////////////////////////////////////////////////////////
//  Function    : lcd_display_alphabet
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi ky tu len LCD
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(__MSP430__)
#ifdef LCD_8MUX_MODE
void lcd_display_alphabet(unsigned char value,unsigned char pos)
{
    switch (pos)
    {
        /*4 COM dau tien*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0xF0)|(lcd_seg_alphabet[value]&0x0F);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0xF0)|(lcd_seg_alphabet[value]>>4);
        break;

        /*4 COM tiep theo*/
    case 9:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 10:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 11:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 12:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 13:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 14:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 15:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    case 16:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F)|(lcd_seg_alphabet[value]<<4);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x0F)|(lcd_seg_alphabet[value]&0xF0);
        break;
    }
}
#else
void lcd_display_alphabet(unsigned char value,unsigned char pos)
{
    switch (pos)
    {
    case 0:
#if defined(PCB_EXT_V0)
        LCD_DIGIT0_MEM1 = (LCD_DIGIT0_MEM1 & 0x0F)|(lcd_seg_alphabet[value]);  //& 0xF0
        LCD_DIGIT0_MEM2 = (LCD_DIGIT0_MEM2 & 0xF8)|(lcd_seg_alphabet[value]);
#elif defined(PCB_EXT_V1)
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x8F)|(lcd_seg_alphabet[value]<<4);
#elif defined(PCB_EXT_V2)
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x1F)|(lcd_seg_alphabet[value]<<4);
#elif defined(_DT01P80_PCB_TI_S4_T_V24_)
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x08)|lcd_seg_alphabet[value];    
#else
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x8F)|(lcd_seg_alphabet[value]<<4);
#endif
        break;
#if defined(PCB_EXT_V1)

    case 1:
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 2:
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 3:
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 4:
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 5:
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 6:
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 7:
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
    case 8:
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x08)|lcd_seg_alphabet[value];

        break;
#elif defined(PCB_EXT_V2)

     case 1:
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 2:
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 3:
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 4:
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 5:
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 6:
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 7:
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x01)|lcd_seg_alphabet[value];

        break;
    case 8:
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x01)|lcd_seg_alphabet[value];

        break;   
#else // pcb2015, pcb2016, _DT01P80_PCB_TI_S4_T_V24_
    case 1:
        #if defined(_DT01P80_PCB_TI_S4_T_V24_)
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x8F)|(lcd_seg_alphabet[value]<<4);
        #else
        LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x08)|lcd_seg_alphabet[value];
        #endif
        break;
    case 2:
        LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 3:
        LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 4:
        LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 5:
        LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 6:
        LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 7:
        LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
    case 8:
        LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
#endif
#if defined(PCB_EXT_V0)
    case 9:
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x08)|lcd_seg_alphabet[value];
        break;
#elif defined(PCB_EXT_V1)
    case 9:
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x8F)|(lcd_seg_alphabet[value]<<4);
        break;
#elif defined(PCB_EXT_V2)
         case 9:
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0xF0)|(lcd_seg_alphabet[value]>>4);  //& 0xF0
        LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x1F)|(lcd_seg_alphabet[value]<<4);
        break;  
#endif
    }
}
#endif // end of 8mux mode
#elif defined(_18F86J94)
void lcd_display_alphabet(unsigned char value,unsigned char pos)
{
    unsigned char s, segment[7];
    for (s=0;s<7;s++)
    {
        if (bit_test(lcd_seg_alphabet[value],s))segment[s]= ON;
        else segment[s]= OFF;
    }
    lcd_display_value_digit((unsigned char *)segment, pos);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __MSP430__
#ifdef LCD_8MUX_MODE
void lcd_hide_digit(unsigned char  pos)
{
    switch (pos)
    {
        /*4 COM dau tien*/
    case 1:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0xF0);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0xF0);
        break;
    case 2:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0xF0);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0xF0);
        break;
    case 3:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0xF0);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0xF0);
        break;
    case 4:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0xF0);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0xF0);
        break;
    case 5:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0xF0);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0xF0);
        break;
    case 6:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0xF0);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0xF0);
        break;
    case 7:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0xF0);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0xF0);
        break;
    case 8:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0xF0);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0xF0);
        break;
        /*4 COM tiep theo*/
    case 9:
        LCD_DIGIT1_MEM0 = (LCD_DIGIT1_MEM0&0x0F);
        LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1&0x0F);
        break;
    case 10:
        LCD_DIGIT2_MEM0 = (LCD_DIGIT2_MEM0&0x0F);
        LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1&0x0F);
        break;
    case 11:
        LCD_DIGIT3_MEM0 = (LCD_DIGIT3_MEM0&0x0F);
        LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1&0x0F);
        break;
    case 12:
        LCD_DIGIT4_MEM0 = (LCD_DIGIT4_MEM0&0x0F);
        LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1&0x0F);
        break;
    case 13:
        LCD_DIGIT5_MEM0 = (LCD_DIGIT5_MEM0&0x0F);
        LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1&0x0F);
        break;
    case 14:
        LCD_DIGIT6_MEM0 = (LCD_DIGIT6_MEM0&0x0F);
        LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1&0x0F);
        break;
    case 15:
        LCD_DIGIT7_MEM0 = (LCD_DIGIT7_MEM0&0x0F);
        LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1&0x0F);
        break;
    case 16:
        LCD_DIGIT8_MEM0 = (LCD_DIGIT8_MEM0&0x0F);
        LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1&0x0F);
        break;
    }
}
#else
void lcd_hide_digit(unsigned char  pos)
{
    switch (pos)
    {
    #ifndef _IHD_
        case 0:
            LCD_DIGIT0_MEM = (LCD_DIGIT0_MEM & 0x08);
            break;
    #endif
        case 1:
            LCD_DIGIT1_MEM = (LCD_DIGIT1_MEM & 0x80);
            break;
        case 2:
            LCD_DIGIT2_MEM = (LCD_DIGIT2_MEM & 0x80);
            break;
        case 3:
            LCD_DIGIT3_MEM = (LCD_DIGIT3_MEM & 0x80);
            break;
        case 4:
            LCD_DIGIT4_MEM = (LCD_DIGIT4_MEM & 0x80);
            break;
        case 5:
            LCD_DIGIT5_MEM = (LCD_DIGIT5_MEM & 0x80);
            break;
        case 6:
            LCD_DIGIT6_MEM = (LCD_DIGIT6_MEM & 0x80);
            break;
        case 7:
            LCD_DIGIT7_MEM = (LCD_DIGIT7_MEM & 0x80);
            break;
        case 8:
            LCD_DIGIT8_MEM = (LCD_DIGIT8_MEM & 0x80);
            break;
    #ifndef _IHD_
        case 9:
            LCD_DIGIT9_MEM = (LCD_DIGIT9_MEM & 0x80);
            break;
    #endif
    }
}
#endif
#else
void lcd_hide_digit(unsigned char  pos)
{
    unsigned char s, segment[7];
    for (s=0;s<7;s++)
        segment[s] = 0;
    lcd_display_value_digit((unsigned char *)segment, pos);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_display_value(unsigned long value)
{

#ifdef _IHD_
        unsigned long value1;
    unsigned char temp;
    int i;
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
    unsigned long value1;
    unsigned char temp;
    int i;
    value1=value;
    for(i=8;i>0;i--)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        temp = value1%10;
        lcd_display_digit(temp,i-1);
        value1 /= 10;
        if (value1 == 0) i = 0;
    }
#endif 
}

void lcd_process_hide_digits(uint8_t position)
{
    int idx, idx_tmp ;
    for (idx = position; idx <= MAX_DIGITS; idx++)        
    {
        idx_tmp = idx;
        if(idx == 9) idx_tmp = 17;
        if(idx == 10) idx_tmp = 18;
        lcd_hide_digit(idx_tmp);
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
    for(i=10;i>8;i--)        //la`n lu.ot hie?n thi cac digit tu` 8 -> 9
    {
        temp = value1%10;
        lcd_display_digit(temp,i-1);
        value1 /= 10;
        if (value1 == 0) i = 0;
    }
}
/////////////////vinh//////////////////
void lcd_anten(char status) // S1
{
#if defined(__MSP430__)
    if (status == 1)
        //{LCDM7 = 0xFF; LCDM6 = 0xFF;}
        LCD_ANTEN_MEM set LCD_ANTEN_BIT;
    else if(flag_display_all == 0)//||(emeter.flags.calib_enable==0))	
        LCD_ANTEN_MEM clr LCD_ANTEN_BIT;
#else
    if (status == 1)
        dRF = ON;
    else if(flag_display_all == 0)
        dRF = OFF;
#endif
}

void lcd_lowbat(char status) // S4
{
#if defined(__MSP430__) 
    if (status == 1)
        LCD_BATT_MEM set LCD_BATT_BIT; //LCDM11 set 0x80;
    else if(flag_display_all == 0)//||(emeter.flags.calib_enable==0))
        LCD_BATT_MEM clr LCD_BATT_BIT;
#else
    if (status == 1)
        dK1_BAT = ON;
    else if(flag_display_all == 0)
        dK1_BAT = OFF;
#endif
}

void lcd_Er(char status)
{
#if defined(__MSP430__)
    if (status == 1)
	LCD_ER_MEM set LCD_ER_BIT;
    else if(flag_display_all == 0)

	LCD_ER_MEM clr LCD_ER_BIT;
#else
    if (status == 1)
        dEr = ON;
    else if(flag_display_all == 0)
        dEr = OFF;    
#endif
}
void lcd_point2(char status)
{
#if defined(__MSP430__)
    if(status == 1)
	LCD_POINT2_MEM set LCD_POINT2_BIT;
    else
	LCD_POINT2_MEM clr LCD_POINT2_BIT;
#else
    if (status == 1)
        dDp2 = ON;
    else if(flag_display_all == 0)
        dDp2 = OFF;        
#endif
}
void lcd_point3(char status)
{
#if defined(__MSP430__)    
    if(status == 1)
	LCD_POINT3_MEM set LCD_POINT3_BIT;
    else
	LCD_POINT3_MEM clr LCD_POINT3_BIT;
#else
    if (status == 1)
        dDp3 = ON;
    else if(flag_display_all == 0)
        dDp3 = OFF;       
#endif
}
void lcd_kWh(char status)
{
#if defined(__MSP430__)       
    if(status == 1)
	LCD_KWH_MEM set LCD_KWH_BIT;
    else
	LCD_KWH_MEM clr LCD_KWH_BIT;
#else
    if (status == 1)
        dkWh = ON;
    else if(flag_display_all == 0)
        dkWh = OFF;         
#endif
}

void lcd_over(char status)
{
#if defined(__MSP430__)    
    if(status == 1)
	LCD_OVER_MEM set LCD_OVER_BIT;
    else if(flag_display_all == 0)

	LCD_OVER_MEM clr LCD_OVER_BIT;
#else
    if (status == 1)
        dOver = ON;
    else if(flag_display_all == 0)
        dOver = OFF;     
#endif
}

void lcd_warning(char status)
{
#if defined(__MSP430__)      
    if (status == 1)
	LCD_WARNING_MEM set LCD_WARNING_BIT;
    else if(flag_display_all == 0)

	LCD_WARNING_MEM clr LCD_WARNING_BIT;
#else
    if (status == 1)
        dWarning = ON;
    else if(flag_display_all == 0)
        dWarning = OFF;       
#endif
}

void lcd_A(char status)
{
#if defined(__MSP430__)       
    if (status == 1)
	LCD_A_MEM set LCD_A_BIT;
    else if(flag_display_all == 0)

	LCD_A_MEM clr LCD_A_BIT;
#else
    if (status == 1)
        dA = ON;
    else if(flag_display_all == 0)
        dA = OFF;         
#endif
}

void lcd_V(char status)
{
#if defined(__MSP430__)     
    if (status == 1)
	LCD_V_MEM set LCD_V_BIT;
    else if(flag_display_all == 0)

	LCD_V_MEM clr LCD_V_BIT;
#else
    if (status == 1)
        dV = ON;
    else if(flag_display_all == 0)
        dV = OFF;    
#endif
}

void lcd_unlock(char status)
{
#if defined(__MSP430__)     
    if(status == 1)
	LCD_UNLOCK_MEM set LCD_UNLOCK_BIT;
    else if(flag_display_all == 0)

	LCD_UNLOCK_MEM clr LCD_UNLOCK_BIT;
#else
    if (status == 1)
        dLock = ON;
    else if(flag_display_all == 0)
        dLock = OFF;       
#endif
}

void lcd_powerfail(char status)
{
#if defined(__MSP430__)         
    if(status == 1)
	LCD_POWER_FAIL_MEM set LCD_POWER_FAIL_BIT;
    else
	LCD_POWER_FAIL_MEM clr LCD_POWER_FAIL_BIT;
#else
    if (status == 1)
        dPowerFail = ON;
    else if(flag_display_all == 0)
        dPowerFail = OFF;         
#endif
}

void lcd_lo(char status)
{
#if defined(__MSP430__)      
    if(status == 1)
	LCD_LOW_MEM set LCD_LOW_BIT;
    else if(flag_display_all == 0)

	LCD_LOW_MEM clr LCD_LOW_BIT;
#else
    if (status == 1)
        dLo = ON;
    else if(flag_display_all == 0)
        dLo = OFF;       
#endif
}

void lcd_col(char status)
{
#if defined(__MSP430__)      
    if(status == 1)
	LCD_COL_MEM set LCD_COL_BIT;
    else
	LCD_COL_MEM clr LCD_COL_BIT;
#else
    if (status == 1)
        dCol = ON;
    else if(flag_display_all == 0)
        dCol = OFF;         
#endif
}

void lcd_reverse(char status)
{
#if defined(__MSP430__)    
    if(status == 1)
	LCD_REVERSE_MEM set LCD_REVERSE_BIT;
    else if(flag_display_all == 0)
	LCD_REVERSE_MEM clr LCD_REVERSE_BIT;
#else
    if (status == 1)
        dReverse = ON;
    else if(flag_display_all == 0)
        dReverse = OFF;         
#endif
}

void lcd_GND(char status)
{
#if defined(__MSP430__)      
    if(status == 1)
	LCD_GND_MEM set LCD_GND_BIT;
    else if(flag_display_all == 0)

	LCD_GND_MEM clr LCD_GND_BIT;
#else
    if (status == 1)
        dGND = ON;
    else if(flag_display_all == 0)
        dGND = OFF;     
#endif
}
void lcd_magnetic(char status)
{
#if defined (PCB_EXT_V1) || defined (PCB_EXT_V2) 
    if(status == 1)
        LCD_MAGNETIC_MEM set LCD_MAGNETIC_BIT;
    else
        LCD_MAGNETIC_MEM clr LCD_MAGNETIC_BIT;
#elif defined(_DT01P80_EXP_PCB_PIC_S4_T_V10_)
//    if (status == 1)
//        dGND = ON;
//    else if(flag_display_all == 0)
//        dGND = OFF;         
#endif
}
void lcd_high_vol(char status)
{
    lcd_V(status);
    lcd_over(status);
}

void lcd_low_vol(char status)
{
    lcd_V(status);
    lcd_lo(status);
}

void lcd_high_ampe(char status)
{
    lcd_A(status);
    lcd_over(status);
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

//void lcd_display_Vrms(void)
//{
//    int32_t x;
//
//    // Display RMS voltage in 0.1V increments
//    x =  current_rms_voltage(LIVE)/10; //current_rms_neutral_current()/10;//
//    if (x<100)
//    {
//        lcd_display_digit(0,5);
//        lcd_display_digit(0,6);
//    }
//    lcd_display_value(x);
//    //LCD_display_value(2213); //fake Vol: tuantm, dis Low vol display
//
//    lcd_point3(SHOW);lcd_point2(HIDE);
//    lcd_V(SHOW);
//}
//
//void lcd_display_Irms(void)
//{
//    int32_t x;
//#if defined(USE_CT_FOR_I1)
//	x = current_rms_neutral_current()/10;
//#else
//    x = current_rms_current(LIVE)/10;
//#endif
//    //if (x<5) x=0;  //tuantm 06/11/2014 //I <50 mA: noise
//    if (x<100)
//    {
//        lcd_display_digit(0,5);
//        lcd_display_digit(0,6);
//    }
//    lcd_display_value(x);
//    lcd_point2(SHOW);lcd_point3(HIDE);
//    lcd_A(SHOW);
//}
//
//void lcd_display_Neutral_Irms(void)
//{
//    int32_t x;
//
//    x = current_rms_neutral_current()/10;//
//    if (x<5) x=0;  //I <50 mA: noise
//    if (x<100)
//    {
//        lcd_display_digit(0,5);
//        lcd_display_digit(0,6);
//    }
//    lcd_display_value(x);
//    //LCD_A(1);
//    lcd_point2(SHOW); lcd_point3(HIDE);
//    lcd_A(SHOW);
//}
//
//void lcd_display_consumed_active_energy(void)
//{
//    int32_t x;
//
//    //Display per phase or total consumed energy in 0.01kWh increments
//#ifdef _IHD_
//    x =  5678;
//#else
//    x =  emeter.active.forward.value/10;  //12345678;//
//#endif
//    //x /= 10;
//    lcd_display_value(x);
//    if (x<10) lcd_display_digit(0,6);
//    lcd_kWh(SHOW);
//    lcd_point3(SHOW);lcd_point2(HIDE);
//}
//
//void lcd_display_consumed_reverse_active_energy(void)
//{
//    int32_t x;
//
//    //Display per phase or total consumed energy in 0.01kWh increments
//#ifdef _IHD_
//    x =  1234;
//#else
//    x =  emeter.active.reverse.value/10; //98765432;//
//#endif
//    lcd_display_value(x);
//    if (x<10) lcd_display_digit(0,6);
//    lcd_kWh(SHOW);
//    lcd_point3(SHOW);lcd_point2(HIDE);
//    lcd_reverse(SHOW);
//}

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
//#define HIHIHIHAHAHA
#ifdef HIHIHIHAHAHA
////////////////////////////////////////////////////////////////////////////////
//  Function    : display_reactive_power
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi cong suat phan khang
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//void display_reactive_power(int ph)
//{
//  int32_t x;
//
//  //Display reactive power in 0.01W increments
//  x = current_reactive_power(LIVE);
//  x /= 10;
//  lcd_display_value(x);
//  //lcd_var(1);
//  //lcd_point1(1);
//  //display_phase_icon(ph);
//  //display_obis_code(REACTIVE_POWER_CODE,ph);
//}
////////////////////////////////////////////////////////////////////////////////
//  Function    : display_apparent_power
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi cong suat tong
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

//void display_apparent_power(int ph)
//{
//  int32_t x;
//
//  //Display apparent (VA) power in 0.01W increments
//  x = current_apparent_power(LIVE);
//  x /= 10;
//  lcd_display_value(x);
//  //LCD_VA(1);
//  //LCD_Point1(1);
//  //display_phase_icon(ph);
//  //display_obis_code(APPARENT_POWER_CODE,ph);
//}
////////////////////////////////////////////////////////////////////////////////
//  Function    : display_active_power
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi cong suat thuc P
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_display_active_power(void)
{
  int32_t x;

  //Display per phase or total power in 0.01W increments
  x = current_active_power(LIVE);
  lcd_display_value(x);
  lcd_point2(SHOW);
  //LCD_W(1);
  //LCD_Point3(1);//LCD_Point1(1);
  //display_phase_icon(ph);
  //display_obis_code(ACTIVE_POWER_CODE,ph);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : display_power_factor
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi he so cong suat
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_display_power_factor(void)
{
  int16_t factor, tmp;
  //Display per phase or total power in 0.01W increments
  if(i1<I_MIN)
	  tmp = 0;
  else
	  tmp = (int16_t)current_power_factor(LIVE);
  factor = (abs(tmp)/100+(((abs(tmp)%100)>=50)?1:0)); //la`m tron`
  lcd_display_alphabet(6,0);//H
  lcd_display_digit(5,1); //S
  //lcd_display_alphabet(3,2);//C
  //lcd_display_alphabet(4,3); //u
  if(tmp<0)
	  lcd_display_alphabet(5,4); //- am
  if(factor<100)
	  lcd_display_digit(0,5);
  else
	  lcd_display_digit(1,5);
  if(factor<10)
	  lcd_display_digit(0,6);

  lcd_display_value(factor); //phi
  lcd_point2(SHOW);       //dot
  //hien thi goc phan tu
//  lcd_display_digit(quadrant(LIVE),8);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : display_phase_angle
//  Created     : 19/07/2014, by tuantm
//  Description : hien thi goc lech pha giua U va I
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void lcd_display_phase_angle(void)
{
   //float x;
   //x = current_phase_angle(LIVE);
   //lcd_display_value(x);
}
#endif
//----------------------------------------------------------------------------------------
#endif // 

