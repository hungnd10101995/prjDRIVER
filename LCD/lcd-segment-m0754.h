#ifndef LCD_M0754_H_
#define LCD_M0754_H_

// Header prjEMETERS
#include "library.h"
#include "main.h"
#include "hardware.h"

/*
 __a___
|	   |
f      b
|__g___|
|      |
e      c
|__d___|

*/
#if defined(_LCD_M0754_)

#if defined(_DT01MRF_PCB_TI_S8M_T_V13_)
    #if defined(_LCD_M0754_V30_)
        #define SEG_A	0x80
        #define SEG_B	0x40
        #define SEG_C	0x04
        #define SEG_D	0x02
        #define SEG_E	0x08
        #define SEG_F	0x20
        #define SEG_G	0x10
    #elif defined(_LCD_M0754_V20_)
        #define SEG_A	0x08
        #define SEG_B	0x08
        #define SEG_C	0x02
        #define SEG_D	0x01
        #define SEG_E	0x02
        #define SEG_F	0x04
        #define SEG_G	0x04
    #endif
#else // for PCB khac
    #if defined(_LCD_M0754_V30_)
        #define SEG_A	0x80
        #define SEG_B	0x40
        #define SEG_C	0x04
        #define SEG_D	0x02
        #define SEG_E	0x08
        #define SEG_F	0x20
        #define SEG_G	0x10
    #elif defined(_LCD_M0754_V20_)
        #define SEG_A	0x08
        #define SEG_B	0x08
        #define SEG_C	0x02
        #define SEG_D	0x01
        #define SEG_E	0x02
        #define SEG_F	0x04
        #define SEG_G	0x04
    #endif
#endif
////////////////////////////////////////////////////////////////////////////////
#if defined(_LCD_M0754_V30_)
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
    #define CHAR_O          (DIGIT_8 ^ SEG_G)
    #define CHAR_Y      ((DIGIT_8^SEG_A)^SEG_E)
    #define CHAR_N      ((DIGIT_8^SEG_G)^SEG_D)
    #define CHAR_L      (SEG_F|SEG_E|SEG_D)
    #define CHAR_r      (SEG_G|SEG_E)
    #define CHAR_t      (SEG_F|SEG_G|SEG_E|SEG_D)
    #define CHAR_P      CHAR_A^SEG_C
    #define CHAR_I      SEG_E|SEG_F    
#elif defined(_LCD_M0754_V20_) // LCD M0754 version 2.0
    /* so */
    #define DIGIT_8_M1  (SEG_A|SEG_D|SEG_E|SEG_F)
    #define DIGIT_8_M2  (SEG_B|SEG_C|SEG_G)
    #define DIGIT_0_M1  (DIGIT_8_M1)
    #define DIGIT_0_M2  (DIGIT_8_M2 ^ SEG_G)
    #define DIGIT_1_M1  0
    #define DIGIT_1_M2  (SEG_B|SEG_C)
    #define DIGIT_2_M1  (SEG_A|SEG_E|SEG_D)
    #define DIGIT_2_M2  (SEG_B|SEG_G)
    #define DIGIT_3_M1  (SEG_A|SEG_D)
    #define DIGIT_3_M2  (SEG_B|SEG_C|SEG_G)
    #define DIGIT_4_M1  (SEG_F)
    #define DIGIT_4_M2  (SEG_G|SEG_B|SEG_C)
    #define DIGIT_5_M1  (SEG_A|SEG_F|SEG_D)
    #define DIGIT_5_M2  (SEG_G|SEG_C)
    #define DIGIT_6_M1  (DIGIT_8_M1)
    #define DIGIT_6_M2  (DIGIT_8_M2 ^ SEG_B)
    #define DIGIT_7_M1  (SEG_A)
    #define DIGIT_7_M2  (SEG_B|SEG_C)
    #define DIGIT_9_M1  (DIGIT_8_M1 ^ SEG_E)
    #define DIGIT_9_M2  (DIGIT_8_M2)
    /* ky tu */
    #define CHAR_A_M1		(DIGIT_8_M1 ^ SEG_D)
    #define CHAR_A_M2		(DIGIT_8_M2)
    #define CHAR_B_M1		((DIGIT_8_M1 ^ SEG_A))
    #define CHAR_B_M2		(DIGIT_8_M2 ^ SEG_B)
    #define CHAR_C_M1		(SEG_A|SEG_F|SEG_E|SEG_D)
    #define CHAR_C_M2		0
    #define CHAR_D_M1		((DIGIT_8 ^ SEG_F) ^ SEG_A)
    #define CHAR_D_M2		(DIGIT_8_M2)
    #define CHAR_E_M1		(DIGIT_8_M1)
    #define CHAR_E_M2		((DIGIT_8_M2 ^ SEG_B) ^ SEG_C)
    #define CHAR_F_M1      (CHAR_E_M1^SEG_D)
    #define CHAR_F_M2      (CHAR_E_M2)
    #define CHAR_V_M1      (DIGIT_8_M1^SEG_A)
    #define CHAR_V_M2      (DIGIT_8_M2^SEG_G)
    #define CHAR_R_M1      (SEG_A|SEG_F|SEG_E)
    #define CHAR_R_M2      0
    #define CHAR_DASH_M1   0 //dau gach ngang cua ngay thang nam
    #define CHAR_DASH_M2   SEG_G //dau gach ngang cua ngay thang nam
    #define CHAR_H_M1      (SEG_E|SEG_F)
    #define CHAR_H_M2      (SEG_B|SEG_C|SEG_G)
    #define CHAR_O_M1          (DIGIT_8_M1)
    #define CHAR_O_M2          (DIGIT_8_M2 ^ SEG_G)
    #define CHAR_Y_M1      ((DIGIT_8_M1^SEG_A)^SEG_E)
    #define CHAR_Y_M2      (DIGIT_8_M2)
    #define CHAR_N_M1      (DIGIT_8_M1^SEG_D)
    #define CHAR_N_M2      (DIGIT_8_M2^SEG_G)
    #define CHAR_L_M1      (SEG_F|SEG_E|SEG_D)
    #define CHAR_L_M2      0
    #define CHAR_r_M1      (SEG_E)
    #define CHAR_r_M2      (SEG_G)
    #define CHAR_t_M1      (SEG_F|SEG_E|SEG_D)
    #define CHAR_t_M2      (SEG_G)
    #define CHAR_P_M1      (CHAR_A_M1)
    #define CHAR_P_M2      (CHAR_A_M2^SEG_C)
#endif

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
#define N_CHAR  11
#define L_CHAR  12
#define I_CHAR  15

#define MAX_SEG_DATA    10
#define MAX_SEG_DATA_2  10

#define MAX_DIGITS      10

#define DISPLAY_OBIS_CHARACTER_C    0xFF
#define DISPLAY_DASH                '-' // 0xFE
#define DISPLAY_OBIS_CHARACTER_C1   0xFD
#define DISPLAY_OBIS_CHARACTER_C2   0xFC
#define DISPLAY_OBIS_CHARACTER_C3   0xFB
/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/
// const char lcd_seg_data[MAX_SEG_DATA];
// const char lcd_seg_data2[MAX_SEG_DATA_2];
// const unsigned char lcd_seg_alphabet[];
extern uint8_t pos_display;
/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/

extern void lcd_clear_all();
extern unsigned char swap_byte(unsigned char in_byte);

extern void lcd_display_digit(unsigned char value,unsigned char  pos);
extern void lcd_hide_digit(unsigned char  pos);
extern void lcd_hide_value(void);

extern void lcd_display_alphabet(unsigned char value,unsigned char  pos);
extern void lcd_display_value(uint64_t value);

extern void lcd_anten(char status);
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
extern void lcd_Q_reverse(char status);
extern void lcd_P_reverse(char status);
extern void lcd_P_active(char status);
extern void lcd_Q_active(char status);
extern void lcd_point1(char status);
extern void lcd_point2(char status);
extern void lcd_point3(char status);
extern void lcd_point6(char status);
extern void lcd_point7(char status);
extern void lcd_point8(char status);
extern void lcd_point9(char status);
extern void lcd_M(char status);
extern void lcd_k(char status);
extern void lcd_V(char status);
extern void lcd_A(char status);
extern void lcd_r(char status);
extern void lcd_W(char status);
extern void lcd_h(char status);
extern void lcd_h_left(char status);
extern void lcd_kWh(char status);
extern void lcd_Wh(char status);
extern void lcd_kVArh(char status);
extern void lcd_VArh(char status);
extern void lcd_kVAh(char status);
extern void lcd_VAh(char status);
extern void lcd_kV(char status);
extern void lcd_kA(char status);
extern void lcd_kW(char status);
extern void lcd_VA(char status);
extern void lcd_kVA(char status);
extern void lcd_VAr(char status);
extern void lcd_kVAr(char status);
extern void lcd_Hz(char status);
extern void lcd_V_high(char status);
extern void lcd_V_low(char status);
extern void lcd_A_high(char status);
extern void lcd_phA(char status);
extern void lcd_phB(char status);
extern void lcd_phC(char status);
extern void lcd_warning(char status);
extern void lcd_MW(char status);
extern void lcd_MVAr(char status);
extern void lcd_MVA(char status);

//extern void lcd_display_obis(unsigned long value);
extern void lcd_display_tariff(unsigned char value, char status);
extern void lcd_display_all(void);
void lcd_version(unsigned int version);

extern void lcd_process_hide_digits(uint8_t position);
//===========================================================================================
#endif
//===========================================================================================
#endif /* LCD_M0754_H_ */
