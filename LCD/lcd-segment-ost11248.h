//************************************************************************************************//
// Author :  
// Email:
// Created:
// Version: 1.0
// Title:
// Description:
// File :
//************************************************************************************************//
#ifndef _LCD_OST11248_H_
#define _LCD_OST11248_H_
//================================================================================================//
//		History
//================================================================================================//

//================================================================================================//
//		Include Files
//================================================================================================//

//================================================================================================//
//		Const
//================================================================================================//
//#define DISPLAY_ITEM_TIME              1
//#define DISPLAY_ITEM_DATE              2
//#define DISPLAY_ITEM_ACTIVE_ENERGY    10
//#define DISPLAY_ITEM_REVERSE_ACTIVE_ENERGY  20
//#define DISPLAY_ITEM_ACTIVE_POWER     30
//#define DISPLAY_ITEM_REACTIVE_POWER   40
//#define DISPLAY_ITEM_VOLTAGE          50
//#define DISPLAY_ITEM_CURRENT          60
//#define DISPLAY_ITEM_APPARENT_POWER   70
//#define DISPLAY_ITEM_APPARENT_ENERGY  80
//#define DISPLAY_ITEM_POWER_FATOR      100

//#define SHOW            1 //tuantm - hien thi item tren LCD
//#define HIDE            0 //tuantm - an item tren LCD
#define MAX_SEG_DATA    11
#define MAX_SEG_DATA_2  11

#define DISPLAY_OBIS_CHARACTER_C    0xFF
#define DISPLAY_DASH                0xFE
///////////////////////////////////////////////////////////////////////////////////
#define V_CHAR      0
#define E_CHAR      1
#define R_CHAR      2
#define A_CHAR      3
#define B_CHAR      4
#define C_CHAR      5
#define H_CHAR      6
#define DASH_CHAR   7
#define P_CHAR      8
#define O_CHAR      9
#define Y_CHAR      10
#define N_CHAR      11
#define L_CHAR      12
#define r_CHAR      13
#define t_CHAR      14

#define MAX_DIGITS      8
/////////////////////////////////////////////////////////////////////////////////
//static const char DISPLAY_ITEM[MAX_DISPLAY_ITEM]=
//{
//	DISPLAY_ITEM_ACTIVE_ENERGY,
//	DISPLAY_ITEM_ACTIVE_ENERGY,
//	DISPLAY_ITEM_REVERSE_ACTIVE_ENERGY,
//	DISPLAY_ITEM_VOLTAGE,
//	DISPLAY_ITEM_CURRENT,
//	DISPLAY_ITEM_POWER_FATOR
//};
//================================================================================================//
//		Variable
//================================================================================================//
extern unsigned char flag_display_all;
extern uint8_t pos_display;;
//================================================================================================//
//		Functions
//================================================================================================//
extern void lcd_display_item(char item);

extern void lcd_anten(char status);
extern void lcd_warning(char status);
extern void lcd_GND(char status);
extern void lcd_high_ampe(char status); //hien thi qua dong
extern void lcd_high_vol(char status); //hien thi qua ap
extern void lcd_A(char status);
extern void lcd_V(char status);
extern void lcd_low_vol(char status); //ap thap
extern void lcd_kWh(char status);
extern void lcd_point3(char status); //hien thi dau . o vi tri 3
extern void lcd_point2(char status); //hien thi dau . o vi tri 2
extern void lcd_reverse(char status);
extern void lcd_lowbat(char status); //het / yeu pin
extern void lcd_Er(char status);
extern void lcd_over(char status);
extern void lcd_lo(char status);
extern void lcd_col(char status);
extern void lcd_unlock(char status); //hien thi mode calibaration
extern void display_version(void);
extern void lcd_magnetic(char status);
//void display_CALIB_MODE(char status);
extern void lcd_powerfail(char status);

extern void lcd_display_digit(unsigned char value,unsigned char  pos);
extern void lcd_clear_all(void);
extern void lcd_display_value(unsigned long value);
extern void lcd_display_error_value(unsigned char value);
extern void lcd_display_kwh(unsigned long value);
extern void lcd_display_alphabet(unsigned char value,unsigned char pos);
extern void lcd_hide_digit(unsigned char  pos);
extern void lcd_process_hide_digits(uint8_t position);
#endif //_LCD_DISPLAY_H_