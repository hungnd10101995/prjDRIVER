#include <xc.h>

// Header prjEMETERS
#include "library.h"
#include "main.h"
#include "hardware.h"

// Header prjDRVIER
#include "lcd-segment-m0754.h"

#if defined(_LCD_M0754_) //endif to end of file
/*===========================================================================*/
// #ifdef PCF8545_DISPLAY_SUPPORT

#if defined(_LCD_M0754_V30_)
// bang ma de hien thi so(0...9) o cac vi tri 1..8 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit thap, seg D,E,G,F nam o 4 bit cao cua LCDMx(x=0...20)
const char lcd_seg_data[MAX_SEG_DATA] = {DIGIT_0,DIGIT_1,DIGIT_2,DIGIT_3,DIGIT_4,DIGIT_5,DIGIT_6,DIGIT_7,DIGIT_8,DIGIT_9};
   //cac so tuong ung:           0    1    2    3    4    5    6    7    8    9   -
//=================================================================================
// bang ma de hien thi so(0...9) o cac vi tri 9..10 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit cao, seg D,E,G,F nam o 4 bit thap cua LCDMx(x=0...20)
const char lcd_seg_data2[MAX_SEG_DATA_2] = {swap4bit(DIGIT_0),swap4bit(DIGIT_1),swap4bit(DIGIT_2),swap4bit(DIGIT_3),
								swap4bit(DIGIT_4),swap4bit(DIGIT_5),swap4bit(DIGIT_6),
								swap4bit(DIGIT_7),swap4bit(DIGIT_8),swap4bit(DIGIT_9)};
   //cac so tuong ung:            0    1    2    3    4    5    6    7    8    9   C
//bang ma hien thi chu cai (ver)
//const char lcd_seg_alphabet[6] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C};
const unsigned char lcd_seg_alphabet[] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,
                                            CHAR_C,CHAR_H,CHAR_DASH,CHAR_P,CHAR_O,
                                            CHAR_Y, CHAR_N, CHAR_L, CHAR_r, CHAR_t,CHAR_I}; 
//****************************************************************************************************************************************
#elif defined(_LCD_M0754_V20_)
// bang ma de hien thi so(0...9) o cac vi tri 1..8 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit thap, seg D,E,G,F nam o 4 bit cao cua LCDMx(x=0...20)
const char lcd_seg_data_m1[MAX_SEG_DATA] = {DIGIT_0_M1,DIGIT_1_M1,DIGIT_2_M1,DIGIT_3_M1,DIGIT_4_M1,DIGIT_5_M1,DIGIT_6_M1,DIGIT_7_M1,DIGIT_8_M1,DIGIT_9_M1};
const char lcd_seg_data_m2[MAX_SEG_DATA] = {DIGIT_0_M2,DIGIT_1_M2,DIGIT_2_M2,DIGIT_3_M2,DIGIT_4_M2,DIGIT_5_M2,DIGIT_6_M2,DIGIT_7_M2,DIGIT_8_M2,DIGIT_9_M2};
   //cac so tuong ung:           0    1    2    3    4    5    6    7    8    9   -
//=================================================================================
// bang ma de hien thi so(0...9) o cac vi tri 9..10 tren LCD
// cac vi tri nay co seg A,B,C,D nam o 4 bit cao, seg D,E,G,F nam o 4 bit thap cua LCDMx(x=0...20)
const char lcd_seg_data2_m1[MAX_SEG_DATA_2] = {swap4bit(DIGIT_0_M1),swap4bit(DIGIT_1_M1),swap4bit(DIGIT_2_M1),swap4bit(DIGIT_3_M1),
								swap4bit(DIGIT_4_M1),swap4bit(DIGIT_5_M1),swap4bit(DIGIT_6_M1),
								swap4bit(DIGIT_7_M1),swap4bit(DIGIT_8_M1),swap4bit(DIGIT_9_M1)};
const char lcd_seg_data2_m2[MAX_SEG_DATA_2] = {swap4bit(DIGIT_0_M2),swap4bit(DIGIT_1_M2),swap4bit(DIGIT_2_M2),swap4bit(DIGIT_3_M2),
								swap4bit(DIGIT_4_M2),swap4bit(DIGIT_5_M2),swap4bit(DIGIT_6_M2),
								swap4bit(DIGIT_7_M2),swap4bit(DIGIT_8_M2),swap4bit(DIGIT_9_M2)};
   //cac so tuong ung:            0    1    2    3    4    5    6    7    8    9   C
//bang ma hien thi chu cai (ver)
//const char lcd_seg_alphabet[6] = {CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C};
const unsigned char lcd_seg_alphabet_m1[] = {CHAR_V_M1, CHAR_E_M1, CHAR_R_M1,CHAR_A_M1,CHAR_B_M1,
                                            CHAR_C_M1,CHAR_H_M1,CHAR_DASH_M1,CHAR_P_M1,CHAR_O_M1,
                                            CHAR_Y_M1, CHAR_N_M1, CHAR_L_M1, CHAR_r_M1, CHAR_t_M1}; 
const unsigned char lcd_seg_alphabet_m2[] = {CHAR_V_M2, CHAR_E_M2, CHAR_R_M2,CHAR_A_M2,CHAR_B_M2,
                                            CHAR_C_M2,CHAR_H_M2,CHAR_DASH_M2,CHAR_P_M2,CHAR_O_M2,
                                            CHAR_Y_M2, CHAR_N_M2, CHAR_L_M2, CHAR_r_M2, CHAR_t_M2}; 
#endif
/*===========================================================================*/
/*  G L O B A L   D A T A   D E F I N I T I O N S                            */
/*===========================================================================*/
uint8_t pos_display;

/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/


/*===========================================================================*/
/* L O C A L   D A T A   D E F I N I T I O N S                               */
/*===========================================================================*/


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
#ifdef __MSP430__
    LCDCMEMCTL |= LCDCLRM;
//    int i;
//    for (i = 0; i < 20; i++)
//    {
//        *(&LCDM1 + i) = 0x00;
//    }
#endif
#ifdef __PIC24FJxxxGA410__
    int i;
    for (i = 0; i < 32; i++)
    {
        *(&LCDDATA0 + i) = 0;
    }
#endif
    
}

//===========================================================================
//FUNCTION NAME:    lcd_display_digit
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     dung cho PIC
//NOTE:
//============================================================================
#ifdef __PIC24FJxxxGA410__
    
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    volatile unsigned int *segA, *segB, *segC, *segD, *segE, *segF, *segG;
    unsigned int pin;
    //tim vi tri digit =========================================================
    switch (pos)
    {
        case 1:
            segA = &LCD_DIGIT1_SEGA_MEM;
            segB = &LCD_DIGIT1_SEGB_MEM;
            segC = &LCD_DIGIT1_SEGC_MEM;
            segD = &LCD_DIGIT1_SEGD_MEM;
            segE = &LCD_DIGIT1_SEGE_MEM;
            segF = &LCD_DIGIT1_SEGF_MEM;
            segG = &LCD_DIGIT1_SEGG_MEM;
            pin = LCD_DIGIT1_BIT;
            break;
        case 2:
            segA = &LCD_DIGIT2_SEGA_MEM;
            segB = &LCD_DIGIT2_SEGB_MEM;
            segC = &LCD_DIGIT2_SEGC_MEM;
            segD = &LCD_DIGIT2_SEGD_MEM;
            segE = &LCD_DIGIT2_SEGE_MEM;
            segF = &LCD_DIGIT2_SEGF_MEM;
            segG = &LCD_DIGIT2_SEGG_MEM;
            pin = LCD_DIGIT2_BIT;
            break;
        case 3:
            segA = &LCD_DIGIT3_SEGA_MEM;
            segB = &LCD_DIGIT3_SEGB_MEM;
            segC = &LCD_DIGIT3_SEGC_MEM;
            segD = &LCD_DIGIT3_SEGD_MEM;
            segE = &LCD_DIGIT3_SEGE_MEM;
            segF = &LCD_DIGIT3_SEGF_MEM;
            segG = &LCD_DIGIT3_SEGG_MEM;
            pin = LCD_DIGIT3_BIT;
            break;
        case 4:
            segA = &LCD_DIGIT4_SEGA_MEM;
            segB = &LCD_DIGIT4_SEGB_MEM;
            segC = &LCD_DIGIT4_SEGC_MEM;
            segD = &LCD_DIGIT4_SEGD_MEM;
            segE = &LCD_DIGIT4_SEGE_MEM;
            segF = &LCD_DIGIT4_SEGF_MEM;
            segG = &LCD_DIGIT4_SEGG_MEM;
            pin = LCD_DIGIT4_BIT;
            break;
        case 5:
            segA = &LCD_DIGIT5_SEGA_MEM;
            segB = &LCD_DIGIT5_SEGB_MEM;
            segC = &LCD_DIGIT5_SEGC_MEM;
            segD = &LCD_DIGIT5_SEGD_MEM;
            segE = &LCD_DIGIT5_SEGE_MEM;
            segF = &LCD_DIGIT5_SEGF_MEM;
            segG = &LCD_DIGIT5_SEGG_MEM;
            pin = LCD_DIGIT5_BIT;
            break;
        case 6:
            segA = &LCD_DIGIT6_SEGA_MEM;
            segB = &LCD_DIGIT6_SEGB_MEM;
            segC = &LCD_DIGIT6_SEGC_MEM;
            segD = &LCD_DIGIT6_SEGD_MEM;
            segE = &LCD_DIGIT6_SEGE_MEM;
            segF = &LCD_DIGIT6_SEGF_MEM;
            segG = &LCD_DIGIT6_SEGG_MEM;
            pin = LCD_DIGIT6_BIT;
            break;    
        case 7:
            segA = &LCD_DIGIT7_SEGA_MEM;
            segB = &LCD_DIGIT7_SEGB_MEM;
            segC = &LCD_DIGIT7_SEGC_MEM;
            segD = &LCD_DIGIT7_SEGD_MEM;
            segE = &LCD_DIGIT7_SEGE_MEM;
            segF = &LCD_DIGIT7_SEGF_MEM;
            segG = &LCD_DIGIT7_SEGG_MEM;
            pin = LCD_DIGIT7_BIT;
            break;
        case 8:
            segA = &LCD_DIGIT8_SEGA_MEM;
            segB = &LCD_DIGIT8_SEGB_MEM;
            segC = &LCD_DIGIT8_SEGC_MEM;
            segD = &LCD_DIGIT8_SEGD_MEM;
            segE = &LCD_DIGIT8_SEGE_MEM;
            segF = &LCD_DIGIT8_SEGF_MEM;
            segG = &LCD_DIGIT8_SEGG_MEM;
            pin = LCD_DIGIT8_BIT;
            break;
        case 9:
            segA = &LCD_DIGIT9_SEGA_MEM;
            segB = &LCD_DIGIT9_SEGB_MEM;
            segC = &LCD_DIGIT9_SEGC_MEM;
            segD = &LCD_DIGIT9_SEGD_MEM;
            segE = &LCD_DIGIT9_SEGE_MEM;
            segF = &LCD_DIGIT9_SEGF_MEM;
            segG = &LCD_DIGIT9_SEGG_MEM;
            pin = LCD_DIGIT9_BIT;
            break;
        case 10:
            segA = &LCD_DIGIT10_SEGA_MEM;
            segB = &LCD_DIGIT10_SEGB_MEM;
            segC = &LCD_DIGIT10_SEGC_MEM;
            segD = &LCD_DIGIT10_SEGD_MEM;
            segE = &LCD_DIGIT10_SEGE_MEM;
            segF = &LCD_DIGIT10_SEGF_MEM;
            segG = &LCD_DIGIT10_SEGG_MEM;
            pin = LCD_DIGIT10_BIT;
            break;
        case 11:
            segA = &LCD_DIGIT11_SEGA_MEM;
            segB = &LCD_DIGIT11_SEGB_MEM;
            segC = &LCD_DIGIT11_SEGC_MEM;
            segD = &LCD_DIGIT11_SEGD_MEM;
            segE = &LCD_DIGIT11_SEGE_MEM;
            segF = &LCD_DIGIT11_SEGF_MEM;
            segG = &LCD_DIGIT11_SEGG_MEM;
            pin = LCD_DIGIT11_BIT;
            break;
        case 12:
            segA = &LCD_DIGIT12_SEGA_MEM;
            segB = &LCD_DIGIT12_SEGB_MEM;
            segC = &LCD_DIGIT12_SEGC_MEM;
            segD = &LCD_DIGIT12_SEGD_MEM;
            segE = &LCD_DIGIT12_SEGE_MEM;
            segF = &LCD_DIGIT12_SEGF_MEM;
            segG = &LCD_DIGIT12_SEGG_MEM;
            pin = LCD_DIGIT12_BIT;
            break;
        case 13:
            segA = &LCD_DIGIT13_SEGA_MEM;
            segB = &LCD_DIGIT13_SEGB_MEM;
            segC = &LCD_DIGIT13_SEGC_MEM;
            segD = &LCD_DIGIT13_SEGD_MEM;
            segE = &LCD_DIGIT13_SEGE_MEM;
            segF = &LCD_DIGIT13_SEGF_MEM;
            segG = &LCD_DIGIT13_SEGG_MEM;
            pin = LCD_DIGIT13_BIT;
            break;
        case 14:
            segA = &LCD_DIGIT14_SEGA_MEM;
            segB = &LCD_DIGIT14_SEGB_MEM;
            segC = &LCD_DIGIT14_SEGC_MEM;
            segD = &LCD_DIGIT14_SEGD_MEM;
            segE = &LCD_DIGIT14_SEGE_MEM;
            segF = &LCD_DIGIT14_SEGF_MEM;
            segG = &LCD_DIGIT14_SEGG_MEM;
            pin = LCD_DIGIT14_BIT;
            break;
        case 15:
            segA = &LCD_DIGIT15_SEGA_MEM;
            segB = &LCD_DIGIT15_SEGB_MEM;
            segC = &LCD_DIGIT15_SEGC_MEM;
            segD = &LCD_DIGIT15_SEGD_MEM;
            segE = &LCD_DIGIT15_SEGE_MEM;
            segF = &LCD_DIGIT15_SEGF_MEM;
            segG = &LCD_DIGIT15_SEGG_MEM;
            pin = LCD_DIGIT15_BIT;
            break;
        case 16:
            segA = &LCD_DIGIT16_SEGA_MEM;
            segB = &LCD_DIGIT16_SEGB_MEM;
            segC = &LCD_DIGIT16_SEGC_MEM;
            segD = &LCD_DIGIT16_SEGD_MEM;
            segE = &LCD_DIGIT16_SEGE_MEM;
            segF = &LCD_DIGIT16_SEGF_MEM;
            segG = &LCD_DIGIT16_SEGG_MEM;
            pin = LCD_DIGIT16_BIT;
            break;
        case 17:
            segA = &LCD_DIGIT17_SEGA_MEM;
            segB = &LCD_DIGIT17_SEGB_MEM;
            segC = &LCD_DIGIT17_SEGC_MEM;
            segD = &LCD_DIGIT17_SEGD_MEM;
            segE = &LCD_DIGIT17_SEGE_MEM;
            segF = &LCD_DIGIT17_SEGF_MEM;
            segG = &LCD_DIGIT17_SEGG_MEM;
            pin = LCD_DIGIT17_BIT;
            break;
        case 18:
            segA = &LCD_DIGIT18_SEGA_MEM;
            segB = &LCD_DIGIT18_SEGB_MEM;
            segC = &LCD_DIGIT18_SEGC_MEM;
            segD = &LCD_DIGIT18_SEGD_MEM;
            segE = &LCD_DIGIT18_SEGE_MEM;
            segF = &LCD_DIGIT18_SEGF_MEM;
            segG = &LCD_DIGIT18_SEGG_MEM;
            pin = LCD_DIGIT18_BIT;
            break;
        case 19:
            segA = &LCD_DIGIT19_SEGA_MEM;
            segB = &LCD_DIGIT19_SEGB_MEM;
            segC = &LCD_DIGIT19_SEGC_MEM;
            segD = &LCD_DIGIT19_SEGD_MEM;
            segE = &LCD_DIGIT19_SEGE_MEM;
            segF = &LCD_DIGIT19_SEGF_MEM;
            segG = &LCD_DIGIT19_SEGG_MEM;
            pin = LCD_DIGIT19_BIT;
            break;
            
    }
    //xoa gia tri cu
    lcd_hide_digit(pos);
    //tinh gia tri segment =====================================================
    switch (value)
    {
        case 0:
        case 'O':
            *segA |= pin; *segB |= pin; *segC |= pin; *segD |= pin; *segE |= pin; *segF |= pin;            
            break;
        case 1:
        case 'l':
            *segB |= pin; *segC |= pin;
            break;
        case 2:
            *segA |= pin; *segB |= pin; *segG |= pin; *segE |= pin; *segD |= pin;
            break;
        case 3:
            *segA |= pin; *segB |= pin; *segG |= pin; *segC |= pin; *segD |= pin;           
            break;
        case 4:
            *segB |= pin; *segC |= pin; *segF |= pin; *segG |= pin;
            break;            
        case 5:
            *segA |= pin; *segC |= pin; *segD |= pin; *segF |= pin; *segG |= pin;
            break;
        case 6:
            *segA |= pin; *segC |= pin; *segD |= pin; *segE |= pin; *segF |= pin; *segG |= pin;
            break;
        case 7:
            *segA |= pin; *segB |= pin; *segC |= pin;            
            break;
        case 8:
            *segA |= pin; *segB |= pin; *segC |= pin; *segD |= pin; *segE |= pin; *segF |= pin; *segG |= pin;
            break;
        case 9:
        case 'g':
            *segA |= pin; *segB |= pin; *segC |= pin; *segD |= pin; *segF |= pin; *segG |= pin;
            break;
        case 'V':            
            *segB |= pin; *segC |= pin; *segD |= pin; *segE |= pin; *segF |= pin;
            break;
        case 'E':
            *segA |= pin; *segD |= pin; *segE |= pin; *segF |= pin; *segG |= pin;
            break;
        case 'R':
            *segA |= pin; *segE |= pin; *segF |= pin;
            break;
        case 'i':
            *segC |= pin;  
            break;
        case 'n':
            *segE |= pin; *segG |= pin; *segC |= pin;            
            break;
        case 'b':
            *segC |= pin; *segD |= pin; *segE |= pin; *segF |= pin; *segG |= pin;
            break;
        case '-':
            *segG |= pin;
            break;
    }
}
#endif
//===========================================================================
//FUNCTION NAME:    lcd_display_digit
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     dung cho MSP430
//NOTE:
//============================================================================
#ifdef __MSP430__
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    uint8_t unused_bit_uper; //bit ko su dung cua day chu so o duoi
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o tren
#if defined(_LCD_M0754_V30_)
    uint8_t data;
    unused_bit_uper = 0x01;
    unused_bit_lower = 0x01;
    if (value == DISPLAY_OBIS_CHARACTER_C)
    {
        data = CHAR_C;
    }
    else if (value == '-')//DISPLAY_DASH)
    {
        data = CHAR_DASH;
    }
    else if (value < 10) //hien thi so
    {
        data = lcd_seg_data[value];
    }
    else //truong hop value >= 10
    {
        //do nothing
        value = value % 10; //chia 10 lay du    
        data = lcd_seg_data[value];
    }
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
            LCD_DIGIT16_MEM = (LCD_DIGIT16_MEM & unused_bit_uper) | data;
            break;
        case 17:
            LCD_DIGIT17_MEM = (LCD_DIGIT17_MEM & unused_bit_uper) | data;
            break;
        case 18:
            LCD_DIGIT18_MEM = (LCD_DIGIT18_MEM & unused_bit_uper) | data;
            break;
        case 19:
            LCD_DIGIT19_MEM = (LCD_DIGIT19_MEM & unused_bit_uper) | data;
            break;			
        default:
            break;
    }
#elif defined(_LCD_M0754_V20_)
    uint8_t data1, data2, unused_bit_lower_m1, unused_bit_uper_m1;
    unused_bit_lower_m1 = 0xF0;
    unused_bit_uper_m1 = 0x0F;
    unused_bit_uper = 0x1F;
    unused_bit_lower = 0xF1;
    if (value == DISPLAY_OBIS_CHARACTER_C)
    {
        data1 = CHAR_C_M1;
        data2 = CHAR_C_M2;
    }
    else if (value == '-')//DISPLAY_DASH)
    {
        data1 = CHAR_DASH_M1;
        data2 = CHAR_DASH_M2;
    }
    else if (value < 10) //hien thi so
    {
        if((pos < 9) || (pos >16))
        {
            data1 = lcd_seg_data_m1[value];
            data2 = lcd_seg_data_m2[value];
        }
        else 
        {
            data1 = lcd_seg_data2_m1[value];
            data2 = lcd_seg_data2_m2[value];
        }
    }
    switch (pos)
    {
        // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
            LCD_DIGIT1_MEM1 = (LCD_DIGIT1_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT1_MEM2 = ((LCD_DIGIT1_MEM2 & unused_bit_lower) | data2);
            break;
        case 2:
            LCD_DIGIT2_MEM1 = (LCD_DIGIT2_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT2_MEM2 = ((LCD_DIGIT2_MEM2 & unused_bit_lower) | data2);
            break;
        case 3:
            LCD_DIGIT3_MEM1 = (LCD_DIGIT3_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT3_MEM2 = ((LCD_DIGIT3_MEM2 & unused_bit_lower) | data2);
            break;
        case 4:
            LCD_DIGIT4_MEM1 = (LCD_DIGIT4_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT4_MEM2 = ((LCD_DIGIT4_MEM2 & unused_bit_lower) | data2);
            break;
        case 5:
            LCD_DIGIT5_MEM1 = (LCD_DIGIT5_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT5_MEM2 = ((LCD_DIGIT5_MEM2 & unused_bit_lower) | data2);
            break;
        case 6:
            LCD_DIGIT6_MEM1 = (LCD_DIGIT6_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT6_MEM2 = ((LCD_DIGIT6_MEM2 & unused_bit_lower) | data2);
            break;
        case 7:
            LCD_DIGIT7_MEM1 = (LCD_DIGIT7_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT7_MEM2 = ((LCD_DIGIT7_MEM2 & unused_bit_lower) | data2);
            break;
        case 8:
            LCD_DIGIT8_MEM1 = (LCD_DIGIT8_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT8_MEM2 = ((LCD_DIGIT8_MEM2 & unused_bit_lower) | data2);
            break;
            // vi tri tu 9...15 su dung bang ma lcd_seg_data2
        case 9:  ////////////////
            LCD_DIGIT9_MEM1 = (LCD_DIGIT9_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT9_MEM2 = ((LCD_DIGIT9_MEM2 & 0x8F) | (data2>>1));
            break;
        case 10: /////////////////////
            LCD_DIGIT10_MEM1 = (LCD_DIGIT10_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT10_MEM2 = ((LCD_DIGIT10_MEM2 & 0x8F) | (data2>>1));
            break;
        case 11: ////////////////////////
            LCD_DIGIT11_MEM1 = (LCD_DIGIT11_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT11_MEM2 = ((LCD_DIGIT11_MEM2 & 0x8F) | (data2>>1));
            break;
        case 12:
            LCD_DIGIT12_MEM1 = (LCD_DIGIT12_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT12_MEM2 = ((LCD_DIGIT12_MEM2 & unused_bit_uper) | data2);
            break;
        case 13:
            LCD_DIGIT13_MEM1 = (LCD_DIGIT13_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT13_MEM2 = ((LCD_DIGIT13_MEM2 & unused_bit_uper) | data2);
            break;
        case 14:
            LCD_DIGIT14_MEM1 = (LCD_DIGIT14_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT14_MEM2 = ((LCD_DIGIT14_MEM2 & unused_bit_uper) | data2);
            break;
        case 15:
            LCD_DIGIT15_MEM1 = (LCD_DIGIT15_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT15_MEM2 = ((LCD_DIGIT15_MEM2 & unused_bit_uper) | data2);
            break;
        case 16: ///////////////////////////////////
            LCD_DIGIT16_MEM1 = (LCD_DIGIT16_MEM1 & unused_bit_uper_m1) | data1;
            LCD_DIGIT16_MEM2 = ((LCD_DIGIT16_MEM2 & 0x8F) | (data2>>1));
            break;
        case 17:
            LCD_DIGIT17_MEM1 = (LCD_DIGIT17_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT17_MEM2 = ((LCD_DIGIT17_MEM2 & unused_bit_lower) | data2);
            break;
        case 18:
            LCD_DIGIT18_MEM1 = (LCD_DIGIT18_MEM1 & unused_bit_lower_m1) | data1;
            LCD_DIGIT18_MEM2 = ((LCD_DIGIT18_MEM2 & unused_bit_lower) | data2);
            break;			
        default:
            break;
    }    
#endif
}
#endif

//===========================================================================
//FUNCTION NAME:    lcd_hide_digit
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     dung cho PIC
//NOTE:
//============================================================================
#ifdef __PIC24FJxxxGA410__
void lcd_hide_digit(unsigned char  pos)
{   // hungnd 26/12/2020 khai bao con tro kieu volatile
    volatile unsigned int *segA, *segB, *segC, *segD, *segE, *segF, *segG; 
    unsigned int pin;
    //tim vi tri digit =========================================================
    switch (pos)
    {
        case 1:
            segA = &LCD_DIGIT1_SEGA_MEM;
            segB = &LCD_DIGIT1_SEGB_MEM;
            segC = &LCD_DIGIT1_SEGC_MEM;
            segD = &LCD_DIGIT1_SEGD_MEM;
            segE = &LCD_DIGIT1_SEGE_MEM;
            segF = &LCD_DIGIT1_SEGF_MEM;
            segG = &LCD_DIGIT1_SEGG_MEM;
            pin = LCD_DIGIT1_BIT;
            break;
        case 2:
            segA = &LCD_DIGIT2_SEGA_MEM;
            segB = &LCD_DIGIT2_SEGB_MEM;
            segC = &LCD_DIGIT2_SEGC_MEM;
            segD = &LCD_DIGIT2_SEGD_MEM;
            segE = &LCD_DIGIT2_SEGE_MEM;
            segF = &LCD_DIGIT2_SEGF_MEM;
            segG = &LCD_DIGIT2_SEGG_MEM;
            pin = LCD_DIGIT2_BIT;
            break;
        case 3:
            segA = &LCD_DIGIT3_SEGA_MEM;
            segB = &LCD_DIGIT3_SEGB_MEM;
            segC = &LCD_DIGIT3_SEGC_MEM;
            segD = &LCD_DIGIT3_SEGD_MEM;
            segE = &LCD_DIGIT3_SEGE_MEM;
            segF = &LCD_DIGIT3_SEGF_MEM;
            segG = &LCD_DIGIT3_SEGG_MEM;
            pin = LCD_DIGIT3_BIT;
            break;
        case 4:
            segA = &LCD_DIGIT4_SEGA_MEM;
            segB = &LCD_DIGIT4_SEGB_MEM;
            segC = &LCD_DIGIT4_SEGC_MEM;
            segD = &LCD_DIGIT4_SEGD_MEM;
            segE = &LCD_DIGIT4_SEGE_MEM;
            segF = &LCD_DIGIT4_SEGF_MEM;
            segG = &LCD_DIGIT4_SEGG_MEM;
            pin = LCD_DIGIT4_BIT;
            break;
        case 5:
            segA = &LCD_DIGIT5_SEGA_MEM;
            segB = &LCD_DIGIT5_SEGB_MEM;
            segC = &LCD_DIGIT5_SEGC_MEM;
            segD = &LCD_DIGIT5_SEGD_MEM;
            segE = &LCD_DIGIT5_SEGE_MEM;
            segF = &LCD_DIGIT5_SEGF_MEM;
            segG = &LCD_DIGIT5_SEGG_MEM;
            pin = LCD_DIGIT5_BIT;
            break;
        case 6:
            segA = &LCD_DIGIT6_SEGA_MEM;
            segB = &LCD_DIGIT6_SEGB_MEM;
            segC = &LCD_DIGIT6_SEGC_MEM;
            segD = &LCD_DIGIT6_SEGD_MEM;
            segE = &LCD_DIGIT6_SEGE_MEM;
            segF = &LCD_DIGIT6_SEGF_MEM;
            segG = &LCD_DIGIT6_SEGG_MEM;
            pin = LCD_DIGIT6_BIT;
            break;    
        case 7:
            segA = &LCD_DIGIT7_SEGA_MEM;
            segB = &LCD_DIGIT7_SEGB_MEM;
            segC = &LCD_DIGIT7_SEGC_MEM;
            segD = &LCD_DIGIT7_SEGD_MEM;
            segE = &LCD_DIGIT7_SEGE_MEM;
            segF = &LCD_DIGIT7_SEGF_MEM;
            segG = &LCD_DIGIT7_SEGG_MEM;
            pin = LCD_DIGIT7_BIT;
            break;
        case 8:
            segA = &LCD_DIGIT8_SEGA_MEM;
            segB = &LCD_DIGIT8_SEGB_MEM;
            segC = &LCD_DIGIT8_SEGC_MEM;
            segD = &LCD_DIGIT8_SEGD_MEM;
            segE = &LCD_DIGIT8_SEGE_MEM;
            segF = &LCD_DIGIT8_SEGF_MEM;
            segG = &LCD_DIGIT8_SEGG_MEM;
            pin = LCD_DIGIT8_BIT;
            break;
        case 9:
            segA = &LCD_DIGIT9_SEGA_MEM;
            segB = &LCD_DIGIT9_SEGB_MEM;
            segC = &LCD_DIGIT9_SEGC_MEM;
            segD = &LCD_DIGIT9_SEGD_MEM;
            segE = &LCD_DIGIT9_SEGE_MEM;
            segF = &LCD_DIGIT9_SEGF_MEM;
            segG = &LCD_DIGIT9_SEGG_MEM;
            pin = LCD_DIGIT9_BIT;
            break;
        case 10:
            segA = &LCD_DIGIT10_SEGA_MEM;
            segB = &LCD_DIGIT10_SEGB_MEM;
            segC = &LCD_DIGIT10_SEGC_MEM;
            segD = &LCD_DIGIT10_SEGD_MEM;
            segE = &LCD_DIGIT10_SEGE_MEM;
            segF = &LCD_DIGIT10_SEGF_MEM;
            segG = &LCD_DIGIT10_SEGG_MEM;
            pin = LCD_DIGIT10_BIT;
            break;
        case 11:
            segA = &LCD_DIGIT11_SEGA_MEM;
            segB = &LCD_DIGIT11_SEGB_MEM;
            segC = &LCD_DIGIT11_SEGC_MEM;
            segD = &LCD_DIGIT11_SEGD_MEM;
            segE = &LCD_DIGIT11_SEGE_MEM;
            segF = &LCD_DIGIT11_SEGF_MEM;
            segG = &LCD_DIGIT11_SEGG_MEM;
            pin = LCD_DIGIT11_BIT;
            break;
        case 12:
            segA = &LCD_DIGIT12_SEGA_MEM;
            segB = &LCD_DIGIT12_SEGB_MEM;
            segC = &LCD_DIGIT12_SEGC_MEM;
            segD = &LCD_DIGIT12_SEGD_MEM;
            segE = &LCD_DIGIT12_SEGE_MEM;
            segF = &LCD_DIGIT12_SEGF_MEM;
            segG = &LCD_DIGIT12_SEGG_MEM;
            pin = LCD_DIGIT12_BIT;
            break;
        case 13:
            segA = &LCD_DIGIT13_SEGA_MEM;
            segB = &LCD_DIGIT13_SEGB_MEM;
            segC = &LCD_DIGIT13_SEGC_MEM;
            segD = &LCD_DIGIT13_SEGD_MEM;
            segE = &LCD_DIGIT13_SEGE_MEM;
            segF = &LCD_DIGIT13_SEGF_MEM;
            segG = &LCD_DIGIT13_SEGG_MEM;
            pin = LCD_DIGIT13_BIT;
            break;
        case 14:
            segA = &LCD_DIGIT14_SEGA_MEM;
            segB = &LCD_DIGIT14_SEGB_MEM;
            segC = &LCD_DIGIT14_SEGC_MEM;
            segD = &LCD_DIGIT14_SEGD_MEM;
            segE = &LCD_DIGIT14_SEGE_MEM;
            segF = &LCD_DIGIT14_SEGF_MEM;
            segG = &LCD_DIGIT14_SEGG_MEM;
            pin = LCD_DIGIT14_BIT;
            break;
        case 15:
            segA = &LCD_DIGIT15_SEGA_MEM;
            segB = &LCD_DIGIT15_SEGB_MEM;
            segC = &LCD_DIGIT15_SEGC_MEM;
            segD = &LCD_DIGIT15_SEGD_MEM;
            segE = &LCD_DIGIT15_SEGE_MEM;
            segF = &LCD_DIGIT15_SEGF_MEM;
            segG = &LCD_DIGIT15_SEGG_MEM;
            pin = LCD_DIGIT15_BIT;
            break;
        case 16:
            segA = &LCD_DIGIT16_SEGA_MEM;
            segB = &LCD_DIGIT16_SEGB_MEM;
            segC = &LCD_DIGIT16_SEGC_MEM;
            segD = &LCD_DIGIT16_SEGD_MEM;
            segE = &LCD_DIGIT16_SEGE_MEM;
            segF = &LCD_DIGIT16_SEGF_MEM;
            segG = &LCD_DIGIT16_SEGG_MEM;
            pin = LCD_DIGIT16_BIT;
            break;
        case 17:
            segA = &LCD_DIGIT17_SEGA_MEM;
            segB = &LCD_DIGIT17_SEGB_MEM;
            segC = &LCD_DIGIT17_SEGC_MEM;
            segD = &LCD_DIGIT17_SEGD_MEM;
            segE = &LCD_DIGIT17_SEGE_MEM;
            segF = &LCD_DIGIT17_SEGF_MEM;
            segG = &LCD_DIGIT17_SEGG_MEM;
            pin = LCD_DIGIT17_BIT;
            break;
        case 18:
            segA = &LCD_DIGIT18_SEGA_MEM;
            segB = &LCD_DIGIT18_SEGB_MEM;
            segC = &LCD_DIGIT18_SEGC_MEM;
            segD = &LCD_DIGIT18_SEGD_MEM;
            segE = &LCD_DIGIT18_SEGE_MEM;
            segF = &LCD_DIGIT18_SEGF_MEM;
            segG = &LCD_DIGIT18_SEGG_MEM;
            pin = LCD_DIGIT18_BIT;
            break;
        case 19:
            segA = &LCD_DIGIT19_SEGA_MEM;
            segB = &LCD_DIGIT19_SEGB_MEM;
            segC = &LCD_DIGIT19_SEGC_MEM;
            segD = &LCD_DIGIT19_SEGD_MEM;
            segE = &LCD_DIGIT19_SEGE_MEM;
            segF = &LCD_DIGIT19_SEGF_MEM;
            segG = &LCD_DIGIT19_SEGG_MEM;
            pin = LCD_DIGIT19_BIT;
            break;
            
    }
    *segA &= ~pin;
    *segB &= ~pin;
    *segC &= ~pin;
    *segD &= ~pin;
    *segE &= ~pin;
    *segF &= ~pin;
    *segG &= ~pin;    
}
#endif
//===========================================================================
//FUNCTION NAME:    lcd_hide_digit
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
#ifdef __MSP430__
void lcd_hide_digit(unsigned char  pos)
{
#if defined(_LCD_M0754_V30_)
    uint8_t mask_uper;
    uint8_t mask_lower;
#if defined(_DT01M_RF_)
    mask_uper = DIGIT_8;
    mask_lower = DIGIT_8;
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_)

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
            LCD_DIGIT16_MEM &= ~mask_uper;
            break;
		case 17:
            LCD_DIGIT17_MEM &= ~mask_uper;
            break;
		case 18:
            LCD_DIGIT18_MEM &= ~mask_uper;
            break;
		case 19:
            LCD_DIGIT19_MEM &= ~mask_uper;
            break;
        default:
            break;
    }
//===================================================================================================
#elif defined(_LCD_M0754_V20_)
    uint8_t mask_uper_m1, mask_uper_m2;
    uint8_t mask_lower_m1, mask_lower_m2;
#if defined(_DT01M_RF_)
    mask_uper_m1 = swap4bit(DIGIT_8_M1);
        mask_uper_m2 = swap4bit(DIGIT_8_M2);
    mask_lower_m1 = DIGIT_8_M1;
        mask_lower_m2 = DIGIT_8_M2;
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_)

#else
    #warning "LCD: Check meter type define"
#endif
    switch (pos)
    {
            // vi tri tu 1...8 su dung bang ma lcd_seg_data
        case 1:
            LCD_DIGIT1_MEM1 &= ~mask_lower_m1; //~DIGIT_8;
            LCD_DIGIT1_MEM2 &= ~mask_lower_m2; //~DIGIT_8;
            break;
        case 2:
            LCD_DIGIT2_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT2_MEM2 &= ~mask_lower_m2;
            break;
        case 3:
            LCD_DIGIT3_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT3_MEM2 &= ~mask_lower_m2;
            break;
        case 4:
            LCD_DIGIT4_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT4_MEM2 &= ~mask_lower_m2;
            break;
        case 5:
            LCD_DIGIT5_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT5_MEM2 &= ~mask_lower_m2;
            break;
        case 6:
            LCD_DIGIT6_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT6_MEM2 &= ~mask_lower_m2;
            break;
        case 7:
            LCD_DIGIT7_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT7_MEM2 &= ~mask_lower_m2;
            break;
        case 8:
            LCD_DIGIT8_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT8_MEM2 &= ~mask_lower_m2;
            break;
            // vi tri tu 9...15 su dung bang ma lcd_seg_data2
        case 9: ///////////////
            LCD_DIGIT9_MEM1 &= ~mask_uper_m1; //~temp8;
            LCD_DIGIT9_MEM2 &= ~((mask_uper_m2>>1) & 0x7F);
            break;
        case 10: /////////////////
            LCD_DIGIT10_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT10_MEM2 &= ~((mask_uper_m2>>1) & 0x7F);
            break;
        case 11: ////////////////
            LCD_DIGIT11_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT11_MEM2 &= ~((mask_uper_m2>>1) & 0x7F);
            break;
        case 12:
            LCD_DIGIT12_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT12_MEM2 &= ~mask_uper_m2;
            break;
        case 13:
            LCD_DIGIT13_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT13_MEM2 &= ~mask_uper_m2;
            break;
        case 14:
            LCD_DIGIT14_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT14_MEM2 &= ~mask_uper_m2;
            break;
        case 15:
            LCD_DIGIT15_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT15_MEM2 &= ~mask_uper_m2;
            break;
        case 16: ////////////////////
            LCD_DIGIT16_MEM1 &= ~mask_uper_m1;
            LCD_DIGIT16_MEM2 &= ~((mask_uper_m2>>1) & 0x7F);
            break;
        case 17:
            LCD_DIGIT17_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT17_MEM2 &= ~mask_lower_m2;
            break;
        case 18:
            LCD_DIGIT18_MEM1 &= ~mask_lower_m1;
            LCD_DIGIT18_MEM2 &= ~mask_lower_m2;
            break;
        default:
            break;
    }
#endif
}
#endif

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

void lcd_hide_value(void)
{
    uint8_t mask_uper;
    uint8_t mask_lower;

#if defined(_DT01M_RF_)
    #ifdef _LCD_M0754_V20_
    mask_uper = DIGIT_8_M2;
    mask_lower = DIGIT_8_M1;
    #else // lcd 3.0, _DT01MRF_PCB_TI_S8M_T_V13_
    mask_uper = DIGIT_8;
    mask_lower = DIGIT_8;
    #endif
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_)
    #ifdef _LCD_M0754_V20_
    mask_uper = DIGIT_8_M2;
    mask_lower = DIGIT_8_M1;
    #else // lcd 3.0
    mask_uper = DIGIT_8;
    mask_lower = swap4bit(DIGIT_8);
    #endif
#else
    #warning "LCD: Check meter type define"
#endif

    #ifdef _LCD_M0754_V20_
//    LCD_DIGIT1_MEM1 &= ~mask_lower; //~DIGIT_8;    
//    LCD_DIGIT1_MEM2 &= ~mask_uper; //~DIGIT_8;    
//    LCD_DIGIT2_MEM1 &= ~mask_lower;       
//    LCD_DIGIT2_MEM2 &= ~mask_uper;    
//    LCD_DIGIT3_MEM1 &= ~mask_lower;
//    LCD_DIGIT3_MEM2 &= ~mask_uper;
//    LCD_DIGIT4_MEM1 &= ~mask_lower;
//    LCD_DIGIT4_MEM2 &= ~mask_uper;
//    LCD_DIGIT5_MEM1 &= ~mask_lower;
//    LCD_DIGIT5_MEM2 &= ~mask_uper;
//    LCD_DIGIT6_MEM1 &= ~mask_lower;
//    LCD_DIGIT6_MEM2 &= ~mask_uper;
//    LCD_DIGIT7_MEM1 &= ~mask_lower;
//    LCD_DIGIT7_MEM2 &= ~mask_uper;
//    LCD_DIGIT8_MEM1 &= ~mask_lower;
//    LCD_DIGIT8_MEM2 &= ~mask_uper;
//    LCD_DIGIT17_MEM1 &= ~mask_lower;
//    LCD_DIGIT17_MEM2 &= ~mask_uper;
//    LCD_DIGIT18_MEM1 &= ~mask_lower;
//    LCD_DIGIT18_MEM2 &= ~mask_uper;
    lcd_hide_digit(1);
    lcd_hide_digit(2);
    lcd_hide_digit(3);
    lcd_hide_digit(4);
    lcd_hide_digit(5);
    lcd_hide_digit(6);
    lcd_hide_digit(7);
    lcd_hide_digit(8);
    lcd_hide_digit(17); 
    lcd_hide_digit(18);
    #else
#ifdef __MSP430__
    LCD_DIGIT1_MEM &= ~mask_lower; //~DIGIT_8;            
    LCD_DIGIT2_MEM &= ~mask_lower;            
    LCD_DIGIT3_MEM &= ~mask_lower;
    LCD_DIGIT4_MEM &= ~mask_lower;
    LCD_DIGIT5_MEM &= ~mask_lower;
    LCD_DIGIT6_MEM &= ~mask_lower;
    LCD_DIGIT7_MEM &= ~mask_lower;
    LCD_DIGIT8_MEM &= ~mask_lower;
    LCD_DIGIT17_MEM &= ~mask_lower;
    LCD_DIGIT18_MEM &= ~mask_lower;
#else
    lcd_hide_digit(1);
    lcd_hide_digit(2);
    lcd_hide_digit(3);
    lcd_hide_digit(4);
    lcd_hide_digit(5);
    lcd_hide_digit(6);
    lcd_hide_digit(7);
    lcd_hide_digit(8);
    lcd_hide_digit(17); 
    lcd_hide_digit(18);
#endif
    #endif
            
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
#ifdef __PIC24FJxxxGA410__
void lcd_display_alphabet(unsigned char value,unsigned char  pos)
{
    lcd_display_digit(value, pos);
}
#endif
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION: dung cho MSP430
//NOTE:
//============================================================================
#ifdef __MSP430__
void lcd_display_alphabet(unsigned char value,unsigned char  pos)
{
    //if (value > 3) return;
#if defined(_LCD_M0754_V30_)
    uint8_t data;
//    uint8_t unused_bit_uper; //bit ko su dung cua day chu so o tren
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o duoi

    #if defined(_DT01M_RF_)
//    unused_bit_uper = 0x01;
    unused_bit_lower = 0x01;
    data = lcd_seg_alphabet[value];

    #elif defined(_DT03M_RF_) || defined(_DT03P_RF_) //dt03m nguoc voi dt01m
    //unused_bit_uper = 0x08;
    // unused_bit_lower = 0x80;
    // data = swap4bit(lcd_seg_alphabet[value]);
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
        case 17:
            LCD_DIGIT17_MEM = (LCD_DIGIT17_MEM & unused_bit_lower) | data;
            break;
        case 18:
            LCD_DIGIT18_MEM = (LCD_DIGIT18_MEM & unused_bit_lower) | data;
            break;
    }
//=========================================================================================    
#elif defined(_LCD_M0754_V20_)
    uint8_t data1, data2;
//    uint8_t unused_bit_uper; //bit ko su dung cua day chu so o tren
    uint8_t unused_bit_lower; //bit ko su dung cua day chu so o duoi

    #if defined(_DT01M_RF_)
//    unused_bit_uper = 0x10;
    unused_bit_lower = 0xF1;
    data1 = lcd_seg_alphabet_m1[value];
    data2 = lcd_seg_alphabet_m2[value];
    #elif defined(_DT03M_RF_) || defined(_DT03P_RF_) //dt03m nguoc voi dt01m
    //unused_bit_uper = 0x08;
    // unused_bit_lower = 0x80;
    // data = swap4bit(lcd_seg_alphabet[value]);
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
            LCD_DIGIT1_MEM1 |= data1;//lcd_seg_alphabet[value];
            LCD_DIGIT1_MEM2 = ((LCD_DIGIT1_MEM2 & unused_bit_lower) | data2);//lcd_seg_alphabet[value];
            break;
        case 2:
            LCD_DIGIT2_MEM1 |= data1;
            LCD_DIGIT2_MEM2 = ((LCD_DIGIT2_MEM2 & unused_bit_lower) | data2);
            break;
        case 3:
            LCD_DIGIT3_MEM1 |= data1;
            LCD_DIGIT3_MEM2 = ((LCD_DIGIT3_MEM2 & unused_bit_lower) | data2);
            break;
        case 4:
            LCD_DIGIT4_MEM1 |= data1;
            LCD_DIGIT4_MEM2 = ((LCD_DIGIT4_MEM2 & unused_bit_lower) | data2);
            break;
        case 5:
            LCD_DIGIT5_MEM1 |= data1;
            LCD_DIGIT5_MEM2 = ((LCD_DIGIT5_MEM2 & unused_bit_lower) | data2);
            break;
        case 6:
            LCD_DIGIT6_MEM1 |= data1;
            LCD_DIGIT6_MEM2 = ((LCD_DIGIT6_MEM2 & unused_bit_lower) | data2);
            break;
        case 7:
            LCD_DIGIT7_MEM1 |= data1;
            LCD_DIGIT7_MEM2 = ((LCD_DIGIT7_MEM2 & unused_bit_lower) | data2);
            break;
        case 8:
            LCD_DIGIT8_MEM1 |= data1;
            LCD_DIGIT8_MEM2 = ((LCD_DIGIT8_MEM2 & unused_bit_lower) | data2);
            break;
        case 17:
            LCD_DIGIT17_MEM1 |= data1;
            LCD_DIGIT17_MEM2 = ((LCD_DIGIT17_MEM2 & unused_bit_lower) | data2);
            break;
        case 18:
            LCD_DIGIT18_MEM1 |= data1;
            LCD_DIGIT18_MEM2 = ((LCD_DIGIT18_MEM2 & unused_bit_lower) | data2);
            break;
    }
#endif
}
#endif
//===========================================================================
//FUNCTION NAME:    lcd_display_value
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
//void lcd_display_value(unsigned long value)
void lcd_display_value(uint64_t value)
{
    uint64_t temp;
    int idx, idx_tmp ;
    unsigned char digit;//, flag_hide_digit;
//    flag_hide_digit = FALSE;
    temp = value;
    for( idx = 1; idx <= MAX_DIGITS; idx++)        
    {
//        if (flag_hide_digit == FALSE)
//        {
        idx_tmp = idx;
        pos_display = idx;
        digit = temp % 10;
        if(idx == 9) idx_tmp = 17;
        if(idx == 10) idx_tmp = 18;
        lcd_display_digit(digit, idx_tmp);
        temp /= 10;
//        }
        if (temp == 0) //thoat
        {
//            flag_hide_digit = TRUE;
//            if(idx == 8) idx_tmp = 17;
//            else if(idx == 9) idx_tmp = 18;
//            else if (idx < 8) idx_tmp++;
//            if (idx < 10) lcd_hide_digit(idx_tmp);
            idx = MAX_DIGITS + 1;
        }
    }
    pos_display++;
}
//===========================================================================
//FUNCTION NAME:    lcd_anten
//PROTOTYPE:        void lcd_anten(char status)
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
//-----------------------------------------------------------------------------
void lcd_anten(char status)
{
	if(status == SHOW)
	{
		LCD_ANTEN_MEM |= LCD_ANTEN_BIT;		
	}else
	{
		LCD_ANTEN_MEM &= ~LCD_ANTEN_BIT;		
	}	
}
//===========================================================================
//FUNCTION NAME:    lcd_lowbat
//PROTOTYPE:        void lcd_lowbat(char status)
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_lowbat(char status)
{
	if(status == SHOW)
	{
		LCD_BATT_MEM |= LCD_BATT_BIT;		
	}else
	{
		LCD_BATT_MEM &= ~LCD_BATT_BIT;		
	}	
}
//===========================================================================
//FUNCTION NAME:    lcd_reverse
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_reverse(char status)
{
	if(status == SHOW)
	{
		LCD_REVERSE_MEM |= LCD_REVERSE_BIT;		
	}else
	{
		LCD_REVERSE_MEM &= ~LCD_REVERSE_BIT;		
	}	
}
//===========================================================================
//FUNCTION NAME:    lcd_Er
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_Er(char status)
{
	if(status == SHOW)
	{
		LCD_ER_MEM |= LCD_ER_BIT;		
	}else
	{
		LCD_ER_MEM &= ~LCD_ER_BIT;		
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_ACB
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_ACB(char status)
{
	if(status == SHOW)
	{
		LCD_ACB_MEM |= LCD_ACB_BIT;		
	}else
	{
		LCD_ACB_MEM &= ~LCD_ACB_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_MD
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_MD(char status)
{
	if(status == SHOW)
	{
		LCD_MD_MEM |= LCD_MD_BIT;		
	}else
	{
		LCD_MD_MEM &= ~LCD_MD_BIT;
	}	
}
//===========================================================================
//FUNCTION NAME:    lcd_COL2
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_COL2(char status)
{
	if(status == SHOW)
	{
		LCD_COL2_MEM |= LCD_COL2_BIT;
	}else
	{
		LCD_COL2_MEM &= ~LCD_COL2_BIT;
	}
} 
//===========================================================================
//FUNCTION NAME:    lcd_L1
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_L1(char status)
{
	if(status == SHOW)
	{
		LCD_L1_MEM |= LCD_L1_BIT;		
	}else
	{
		LCD_L1_MEM &= ~LCD_L1_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_L2
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_L2(char status)
{
	if(status == SHOW)
	{
		LCD_L2_MEM |= LCD_L2_BIT;		
	}else
	{
		LCD_L2_MEM &= ~LCD_L2_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_L3
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_L3(char status)
{
	if(status == SHOW)
	{
		LCD_L3_MEM |= LCD_L3_BIT;
		
	}else
	{
		LCD_L3_MEM &= ~LCD_L3_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_GND
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_GND(char status)
{
	if(status == SHOW)
	{
		LCD_GND_MEM |= LCD_GND_BIT;		
	}else
	{
		LCD_GND_MEM &= ~LCD_GND_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_magnetic
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_magnetic(char status)
{
	if(status == SHOW)
	{
		LCD_MAGNETIC_MEM |= LCD_MAGNETIC_BIT;		
	}else
	{
		LCD_MAGNETIC_MEM &= ~LCD_MAGNETIC_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_unlock
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_unlock(char status)
{
	if(status == SHOW)
	{
		LCD_UNLOCK_MEM |= LCD_UNLOCK_BIT;		
	}else
	{
		LCD_UNLOCK_MEM &= ~LCD_UNLOCK_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_powerfail
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_powerfail(char status)
{
	if(status == SHOW)
	{
		LCD_POWERFAIL_MEM |= LCD_POWERFAIL_BIT;
		
	}else
	{
		LCD_POWERFAIL_MEM &= ~LCD_POWERFAIL_BIT;
	}
}

//===========================================================================
//FUNCTION NAME:    lcd_phA
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_phA(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('A',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_phB
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_phB(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('B',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_phC
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_phC(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('C',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}

//===========================================================================
//FUNCTION NAME:    lcd_T
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_T(char status)
{
    if(status == SHOW)
	{
		LCD_TARIFF_MEM |= LCD_TARIFF_BIT;		
	}else
	{
		LCD_TARIFF_MEM &= ~LCD_TARIFF_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_Q_reverse
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_Q_reverse(char status)
{
	if(status == SHOW)
	{
		LCD_Q_REVERSE_MEM |= LCD_Q_REVERSE_BIT;		
	}else
	{
		LCD_Q_REVERSE_MEM &= ~LCD_Q_REVERSE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_P_reverse
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_P_reverse(char status)
{
	if(status == SHOW)
	{
		LCD_P_REVERSE_MEM |= LCD_P_REVERSE_BIT;		
	}else
	{
		LCD_P_REVERSE_MEM &= ~LCD_P_REVERSE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_P_active
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_P_active(char status)
{
	if(status == SHOW)
	{
		LCD_P_ACTIVE_MEM |= LCD_P_ACTIVE_BIT;		
	}else
	{
		LCD_P_ACTIVE_MEM &= ~LCD_P_ACTIVE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_Q_active
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_Q_active(char status)
{
	if(status == SHOW)
	{
		LCD_Q_ACTIVE_MEM |= LCD_Q_ACTIVE_BIT;		
	}else
	{
		LCD_Q_ACTIVE_MEM &= ~LCD_Q_ACTIVE_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point0
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
#if defined(_LCD_M0754_V30_)
void lcd_point0(char status)
{
	if(status == SHOW)
	{
		LCD_POINT0_MEM |= LCD_POINT0_BIT;		
	}else
	{
		LCD_POINT0_MEM &= ~LCD_POINT0_BIT;
	}
}
#else // lcd v2.0
void lcd_point0(char status) {}
#endif
//===========================================================================
//FUNCTION NAME:    lcd_point1
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point1(char status)
{
	if(status == SHOW)
	{
		LCD_POINT1_MEM |= LCD_POINT1_BIT;		
	}else
	{
		LCD_POINT1_MEM &= ~LCD_POINT1_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point2
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point2(char status)
{
	if(status == SHOW)
	{
		LCD_POINT2_MEM |= LCD_POINT2_BIT;		
	}else
	{
		LCD_POINT2_MEM &= ~LCD_POINT2_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point3
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point3(char status)
{
	if(status == SHOW)
	{
		LCD_POINT3_MEM |= LCD_POINT3_BIT;		
	}else
	{
		LCD_POINT3_MEM &= ~LCD_POINT3_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point6
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point6(char status)
{
	if(status == SHOW)
	{
		LCD_POINT6_MEM |= LCD_POINT6_BIT;		
	}else
	{
		LCD_POINT6_MEM &= ~LCD_POINT6_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point7
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point7(char status)
{
	if(status == SHOW)
	{
		LCD_POINT7_MEM |= LCD_POINT7_BIT;		
	}else
	{
		LCD_POINT7_MEM &= ~LCD_POINT7_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point8
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point8(char status)
{
	if(status == SHOW)
	{
		LCD_POINT8_MEM |= LCD_POINT8_BIT;		
	}else
	{
		LCD_POINT8_MEM &= ~LCD_POINT8_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_point9
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_point9(char status)
{
	if(status == SHOW)
	{
		LCD_POINT9_MEM |= LCD_POINT9_BIT;		
	}else
	{
		LCD_POINT9_MEM &= ~LCD_POINT9_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_z
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_z(char status)
{
	if(status == SHOW)
	{
		LCD_Z_MEM |= LCD_Z_BIT;		
	}else
	{
		LCD_Z_MEM &= ~LCD_Z_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_p5
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_p5(char status)
{
	if(status == SHOW)
	{
		LCD_P5_MEM |= LCD_P5_BIT;		
	}else
	{
		LCD_P5_MEM &= ~LCD_P5_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s9
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s9(char status)
{
	if(status == SHOW)
	{
		LCD_S9_MEM |= LCD_S9_BIT;		
	}else
	{
		LCD_S9_MEM &= ~LCD_S9_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s10
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s10(char status)
{
	if(status == SHOW)
	{
		LCD_S10_MEM |= LCD_S10_BIT;		
	}else
	{
		LCD_S10_MEM &= ~LCD_S10_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s11
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s11(char status)
{
	if(status == SHOW)
	{
		LCD_S11_MEM |= LCD_S11_BIT;		
	}else
	{
		LCD_S11_MEM &= ~LCD_S11_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_V
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_V(char status)
{
	if(status == SHOW)
	{
		LCD_V_MEM |= LCD_V_BIT;		
	}else
	{
		LCD_V_MEM &= ~LCD_V_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s13
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s13(char status)
{
	if(status == SHOW)
	{
		LCD_S13_MEM |= LCD_S13_BIT;		
	}else
	{
		LCD_S13_MEM &= ~LCD_S13_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s14
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s14(char status)
{
	if(status == SHOW)
	{
		LCD_S14_MEM |= LCD_S14_BIT;		
	}else
	{
		LCD_S14_MEM &= ~LCD_S14_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s5
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s5(char status)
{
	if(status == SHOW)
	{
		LCD_S5_MEM |= LCD_S5_BIT;		
	}else
	{
		LCD_S5_MEM &= ~LCD_S5_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_r
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_r(char status)
{
	if(status == SHOW)
	{
		LCD_R_MEM |= LCD_R_BIT;		
	}else
	{
		LCD_R_MEM &= ~LCD_R_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s17
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s17(char status)
{
	if(status == SHOW)
	{
		LCD_S17_MEM |= LCD_S17_BIT;		
	}else
	{
		LCD_S17_MEM &= ~LCD_S17_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_s16
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_s16(char status)
{
	if(status == SHOW)
	{
		LCD_S16_MEM |= LCD_S16_BIT;		
	}else
	{
		LCD_S16_MEM &= ~LCD_S16_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_h
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_h(char status)
{
	if(status == SHOW)
	{
		LCD_H_MEM |= LCD_H_BIT;		
	}else
	{
		LCD_H_MEM &= ~LCD_H_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_V_high
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_V_high(char status)
{
    if(status == SHOW)
	{
		LCD_V_HIGH_MEM |= LCD_V_HIGH_BIT;		
	}else
	{
		LCD_V_HIGH_MEM &= ~LCD_V_HIGH_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_V_low
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_V_low(char status)
{
    if(status == SHOW)
	{
		LCD_V_LOW_MEM |= LCD_V_LOW_BIT;		
	}else
	{
		LCD_V_LOW_MEM &= ~LCD_V_LOW_BIT;
	}
}
//===========================================================================
//FUNCTION NAME:    lcd_A_high
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_A_high(char status)
{
    if(status == SHOW)
	{
		LCD_A_HIGH_MEM |= LCD_A_HIGH_BIT;		
	}else
	{
		LCD_A_HIGH_MEM &= ~LCD_A_HIGH_BIT;
	}
}

//===========================================================================
//FUNCTION NAME:    lcd_M
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_M(char status)
{
	lcd_s9(status);
    lcd_s11(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_k
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_k(char status)
{
	lcd_s9(status);
    lcd_s10(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_V
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
// void lcd_V(char status)
// {
	// lcd_s12(status);
// }
//===========================================================================
//FUNCTION NAME:    lcd_A
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_A(char status)
{
	lcd_s14(status);
    lcd_s5(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_r
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
// void lcd_r(char status)
// {
	// lcd_s15(status);
// }
//===========================================================================
//FUNCTION NAME:    lcd_W
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_W(char status)
{
	lcd_V(status);
    lcd_s13(status);
    lcd_s14(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_h
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
// void lcd_h(char status)
// {
	// lcd_s24(status);
// }
//===========================================================================
//FUNCTION NAME:    lcd_h_left
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_h_left(char status)
{
	lcd_s17(status);
    lcd_r(status);
    lcd_s16(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kWh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kWh(char status)
{
	lcd_k(status);
    lcd_W(status);
    lcd_h_left(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_Wh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_Wh(char status)
{
	lcd_W(status);
    lcd_h_left(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kVArh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kVArh(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_VArh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_VArh(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kVAh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kVAh(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_VAh
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_VAh(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kV
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kV(char status)
{
	lcd_k(status);
    lcd_V(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kA
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kA(char status)
{
	lcd_k(status);
    lcd_A(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kW
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kW(char status)
{
	lcd_k(status);
    lcd_W(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_VA
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_VA(char status)
{
	lcd_V(status);
    lcd_A(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kVA
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kVA(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_VAr
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_VAr(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_kVAr
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_kVAr(char status)
{
	lcd_k(status);
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_Hz
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_Hz(char status)
{
	lcd_h(status);
    lcd_p5(status);
    lcd_z(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_warning
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_warning(char status)
{
    
}
//===========================================================================
//FUNCTION NAME:    lcd_MW
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_MW(char status)
{
    lcd_M(status);
    lcd_W(status);
}
//===========================================================================
//FUNCTION NAME:    lcd_MVAr
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_MVAr(char status)
{
    lcd_M(status);
    lcd_VAr(status);
}
//===========================================================================
//===========================================================================
//FUNCTION NAME:    lcd_MVA
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_MVA(char status)
{
    lcd_M(status);
    lcd_VA(status);
}
////==========================================================================
////FUNCTION NAME:    lcd_display_obis_code
////PROTOTYPE:        
////INPUT:
////OUTPUT:
////DESCRIPTION:     
////NOTE:
////============================================================================
//void lcd_display_obis_code(unsigned char c, unsigned char d, unsigned char e, unsigned char ph)
//{
//	unsigned char e1, e0;
//    switch(ph)
//	{
//        case PHASE_A:
//            c = c + OBIS_PHASE_A;
//            break;
//        case PHASE_B:
//            c = c + OBIS_PHASE_B;
//            break;
//        case PHASE_C:
//            c = c + OBIS_PHASE_C;
//            break;
//        default:
//            break;
//	}
//	lcd_point6(SHOW);
//	lcd_point7(SHOW);
//	//hien thi C
//	if (c == DISPLAY_OBIS_CHARACTER_C)
//	{
//		lcd_display_digit(DISPLAY_OBIS_CHARACTER_C,13);
//	}
//	else
//	{
//		unsigned char c1, c0;
//		c0 = c % 10;
//		c1 = (c/10) % 10;
//		lcd_display_digit(c0,13);
//		if (c1 != 0)
//		{
//			lcd_display_digit(c1,14);
//		}
//	}
//	//hien thi D
//	lcd_display_digit(d,12);
//	//hien thi E
//	
//	e0 = e % 10;
//	e1 = (e/10) % 10;
//	if (e1 != 0)
//	{
//		lcd_display_digit(e1,11);
//		lcd_display_digit(e0,10);
//	}
//	else
//	{
//		lcd_display_digit(e0,11);
//	}
//
//}
////===========================================================================
////FUNCTION NAME:    lcd_display_obis
////PROTOTYPE:        
////INPUT:
////OUTPUT:
////DESCRIPTION:     
////NOTE:
////============================================================================
//void lcd_display_obis(unsigned long value)
//{
//    unsigned long value1;
//    unsigned char temp;
//    unsigned char i;
//
//    value1 = value;
//    lcd_point6(1);
//    lcd_point7(1);
//    if (value1 == 91)
//    {
//        lcd_display_digit(1,11);
//        lcd_display_digit(9,12);
//        lcd_display_digit(0,13);
//    }
//    else
//    {
//        temp = value1 % 100;
//        if (temp == 0)
//        {
//            value1 /= 10;
//            for(i = 11; i < 15; i++)
//            {
//                temp = value1%10;
//                if (!((i == 14)&&(temp == 0)))
//                {
//                    lcd_display_digit(temp,i);
//                }
//                value1 /= 10;
//            }
//        }
//        else
//        {
//            for (i = 10; i < 15; i++)
//            {
//                temp = value1 % 10;
//                if (!((i == 14)&&(temp == 0)))
//                {
//                    lcd_display_digit(temp,i);
//                }
//                value1 /= 10;
//            }
//        }
//    }
//}
//===========================================================================
//FUNCTION NAME:    lcd_display_tariff
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_display_tariff(unsigned char value, char status)
{
    lcd_T(status);
	if (status == SHOW)
    {  
        lcd_display_digit(value%10, 16);
        lcd_display_digit(value/10, 9);        
    }
    else
    {
        lcd_hide_digit(16);
        lcd_hide_digit(9);
    }
}
//===========================================================================
//FUNCTION NAME:    lcd_display_all
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_display_all(void)
{
	lcd_clear_all();
    lcd_anten(SHOW);
    lcd_lowbat(SHOW);
    lcd_reverse(SHOW);
	lcd_point1(SHOW);
	lcd_point2(SHOW);
	lcd_point3(SHOW);
	lcd_point6(SHOW);
	lcd_point7(SHOW);
	lcd_point8(SHOW);
	lcd_point9(SHOW);
    lcd_point0(SHOW);
    int i = 0;
    for (i = 1; i <= 18; i++)
    {
        lcd_display_digit(0, i);
    }
    lcd_clear_all();
    lcd_version(456);
    lcd_clear_all();
	lcd_display_value(88888888); //Init system
	//display_obis_code(88,8,88,88);//lcd_display_obis(888888);
    lcd_clear_all();
	lcd_display_digit(8, 15);
	lcd_display_tariff(88,1);
    lcd_clear_all();
	lcd_kVArh(SHOW);
    lcd_clear_all();
    lcd_M(SHOW);
    lcd_clear_all();
    lcd_kWh(SHOW);
    lcd_clear_all();
	lcd_kW(SHOW);
    lcd_clear_all();
	lcd_Hz(SHOW);
    lcd_clear_all();
	lcd_anten(SHOW);    
	lcd_lowbat(SHOW);
	lcd_reverse(SHOW);
	lcd_L1(SHOW);
	lcd_L2(SHOW);
	lcd_L3(SHOW);
	lcd_phA(SHOW);
	lcd_phB(SHOW);
	lcd_phC(SHOW);
	lcd_COL2(SHOW);
	lcd_GND(SHOW);
	lcd_P_active(SHOW);
	lcd_Q_active(SHOW);
	lcd_P_reverse(SHOW);
	lcd_Q_reverse(SHOW);
	lcd_magnetic(SHOW);
	lcd_unlock(SHOW);
	lcd_MD(SHOW);
	lcd_ACB(SHOW);
	lcd_powerfail(SHOW);
	lcd_Er(SHOW);
    lcd_V_high(SHOW);
    lcd_V_low(SHOW);
    lcd_A_high(SHOW);
//    LCDCMEMCTL |= LCDCLRM;
//    int i;
//    for (i = 0; i < 8; i++)
//    {
//        *(&LCDM1 + i) = 0xFF;
//    }
//    for (i = 16; i < 30; i++)
//    {
//        *(&LCDM1 + i) = 0xFF;   
//    }
}
//===========================================================================
//FUNCTION NAME:    lcd_version
//PROTOTYPE:        
//INPUT:
//OUTPUT:
//DESCRIPTION:     
//NOTE:
//============================================================================
void lcd_version(unsigned int version)
{
    unsigned int i = 0;
    unsigned int v = 0;
    unsigned int v1 = version;
    for (i = 1; i <= 3; i++)
    {
        v = v1 % 10;
        lcd_display_digit(v, i);
        v1 = v1 / 10;
    }
    lcd_display_digit('V', 18);
    lcd_display_digit('E', 17);
    lcd_display_digit('R', 8);
    lcd_point1(SHOW);
    lcd_point2(SHOW);    
}
//===================================================================
#endif // _LCD_M0754_
