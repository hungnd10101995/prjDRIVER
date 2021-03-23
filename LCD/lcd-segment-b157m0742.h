//************************************************************************************************//
// Author :  tuantm
// Email:
// Created:
// Version: 1.0
// Title:
// Description:
// File :
//************************************************************************************************//
#ifndef _LCD_DISPLAY_H_
#define _LCD_DISPLAY_H_
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
#define F_CHAR      15
#define c_CHAR      16
#define h_CHAR      17
#define o_CHAR      18
#define i_CHAR      19
#define b_CHAR      20
#define ll_CHAR     21
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

//================================================================================================//
//		Functions
//================================================================================================//
extern void lcd_display_item(char item);
extern void lcd_logo(char level);
extern void lcd_wh(char status);
extern void lcd_kwh(char status);
extern void lcd_mwh(char status);
extern void lcd_var(char status);
extern void lcd_kvarh(char status);
extern void lcd_A(char status);
extern void lcd_V(char status);
extern void lcd_p10(char status);
extern void lcd_p100(char status);
extern void lcd_p1000(char status);
extern void lcd_p1000000(char status);
extern void lcd_anten(char level);
extern void lcd_battery(char level);
extern void lcd_reverse(char status);
extern void lcd_history(char status);
extern void lcd_id(char status);
extern void lcd_warning(char status);
extern void lcd_setting(char status);
extern void lcd_dola(char status);
extern void lcd_time(char status);
extern void lcd_date(char status);
extern void lcd_col1(char status);
extern void lcd_col2(char status);
extern void lcd_clock_dot(char status);
extern void lcd_t1(char status);
extern void lcd_tariff(char status);
extern void lcd_split(char status);
extern void lcd_temp_icon(char type);
extern void clock_dot_tgl();
extern void lcd_CPC_tgl();
extern void lcd_EVN_tgl();
extern void lcd_CPC(char status);
extern void lcd_EVN(char status);
extern void lcd_logoEVN_tgl();
extern void lcd_logoEVN(char status);
void lcd_display_digit(unsigned char value,unsigned char  pos);
void lcd_clear_all();
void lcd_display_value(unsigned long value);
void lcd_display_error_value(unsigned char value);
void lcd_display_kwh(unsigned long value);
void lcd_display_alphabet(unsigned char value,unsigned char pos);
void lcd_hide_digit(unsigned char  pos);
extern void lcd_clear_8digit(void) ;
extern void lcd_test();
#endif //_LCD_DISPLAY_H_