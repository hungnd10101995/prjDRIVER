/*===========================================================================*/
/*
SOURCE_FILE:		aa1024.h
APPLICATION:		DT03M-RF
MODULE:				eeprom
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
#ifndef _LCD_SEGMENT_H_
#define _LCD_SEGMENT_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#if defined(_LCD_OST11295_) || defined(_LCD_L0712_) //endif to end of file
/*===========================================================================*/

/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
#define SHOW    1
#define HIDE    0
#define MAX_SEG_DATA    11
#define MAX_SEG_DATA_2  11

#define MAX_DIGITS      8 //10

#define DISPLAY_OBIS_CHARACTER_C    0xFF
#define DISPLAY_DASH                '-'//0xFE
#define DISPLAY_OBIS_CHARACTER_C1   0xFD
#define DISPLAY_OBIS_CHARACTER_C2   0xFC
#define DISPLAY_OBIS_CHARACTER_C3   0xFB

/*
 __a___
|	   |
f      b
|__g___|
|      |
e      c
|__d___|

*/
#define SEG_A	0x10
#define SEG_B	0x20
#define SEG_C	0x40
#define SEG_D	0x80
#define SEG_E	0x04
#define SEG_F	0x01
#define SEG_G	0x02
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
#define CHAR_O          (DIGIT_8 ^ SEG_G)
#define CHAR_Y      ((DIGIT_8^SEG_A)^SEG_E)
#define CHAR_N      ((DIGIT_8^SEG_G)^SEG_D)
#define CHAR_L      (SEG_F|SEG_E|SEG_D)
#define CHAR_r      (SEG_G|SEG_E)
#define CHAR_t      (SEG_F|SEG_G|SEG_E|SEG_D)
#define CHAR_P      CHAR_A^SEG_C
#define CHAR_I      (SEG_E | SEG_F)

//thu tu trong mang lcd_seg_alphabet[]
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
#define Y_CHAR  10
#define N_CHAR  11
#define L_CHAR  12
#define r_CHAR  13
#define t_CHAR  14
#define I_CHAR  15


/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/
extern uint8_t pos_display;

/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/
extern void lcd_clear_all(void);
extern void lcd_COL2(char status);
extern void lcd_display_tariff(unsigned char value,char status);
extern void lcd_display_10digit_id(unsigned char value, char status);
extern void lcd_Reverse(char status);
extern void lcd_L1(char status);
extern void lcd_L2(char status);
extern void lcd_L3(char status);

extern void lcd_antenna(char status);
extern void lcd_display_value(unsigned long value);
extern void lcd_display_obis(unsigned long value);
extern void lcd_kVArh(char status);
extern void lcd_kWh(char status);
extern void lcd_anten(char status);
extern void lcd_powerfail(char status);
extern void lcd_Q_reverse(char status);
extern void lcd_P_reverse(char status);
extern void lcd_P_active(char status);
extern void lcd_Q_active(char status);

#if defined(_LCD_L0712_)//defined(_PCB_FOR_LCD_BLAZE_V1_)
extern void lcd_M(char status);
#endif
extern void lcd_k(char status);
extern void lcd_Wh(char status);
extern void lcd_VArh(char status);
extern void lcd_VAh(char status);
extern void lcd_kW(char status);
extern void lcd_kVA(char status);
extern void lcd_kVAr(char status);
extern void lcd_VAr(char status);
extern void lcd_kV(char status);
extern void lcd_kA(char status);
extern void lcd_VA(char status);
extern void lcd_kVAh(char status);
extern void lcd_Hz(char status);
extern void lcd_W(char status);
extern void lcd_A(char status);
extern void lcd_V(char status);
#if defined(_LCD_L0712_)
extern void lcd_MW(char status);
extern void lcd_MVAr(char status);
extern void lcd_MVA(char status);
#endif

extern void lcd_warning(char status);
extern void lcd_magnetic(char status);
extern void lcd_lowbat(char status);

extern void lcd_ACB(char status);
extern void lcd_Er(char status);
extern void lcd_MD(char status);
extern void lcd_unlock(char status);
extern void lcd_GND(char status);
extern void lcd_phA(char status);
extern void lcd_phB(char status);
extern void lcd_phC(char status);
extern void lcd_reverse(char status);
extern void lcd_display_alphabet(unsigned char value,unsigned char  pos);
extern void lcd_hide_digit(unsigned char  pos);
extern void lcd_hide_value(void);

extern void lcd_point1(char status);
extern void lcd_point2(char status);
extern void lcd_point3(char status);

extern void lcd_point6(char status);
extern void lcd_point7(char status);
extern void lcd_point8(char status);
extern void lcd_point9(char status);
extern void lcd_Point2_4(char status);
#if defined(_LCD_OST11295_)
extern void lcd_Err(char status);
extern void lcd_point4(char status);
extern void lcd_point5(char status);
extern void lcd_SUM(char status);
#endif
/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/
extern void lcd_display_digit(unsigned char value,unsigned char  pos);
extern const unsigned char lcd_seg_alphabet[];

extern void lcd_M(char status);
extern void lcd_k(char status);
extern void lcd_process_hide_digits(uint8_t position);
#endif //defined(_LCD_OST11295_) || defined(_LCD_L0712_) 
/*===========================================================================*/
/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/
#endif /* _LCD_SEGMENT_H_ */








