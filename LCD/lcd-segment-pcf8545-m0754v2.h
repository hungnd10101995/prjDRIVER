#ifndef LCD_PCF8545_M0754V2_H_
#define LCD_PCF8545_M0754V2_H_
#include "../library.h"
#ifdef _LCD_M0754v2_PCF8545_//PCF8545_DISPLAY_SUPPORT//endif to end of file
/*===========================================================================*/
#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#include "../../ROUTER/FW/hardware.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-main.h"
#include "..\..\dt03p-rf\fw\emeter-hardware.h"
#endif
#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter-main.h"
#include "..\..\dt03m-rf\fw\emeter-hardware.h"
#endif
/*COMMAND BYTES*/
//#define CMD_DISABLE_DISPLAY   0x50 
//#define CMD_ENABLE_DISPLAY    0x51
//#define CMD_INITIALIZE        0x16
//#define CMD_OTP_REFRESH       0xF0
//#define CMD_MODE_SETTING      0x50 // BPS =INV = 0
//#define CMD_SET_MUX_MODE      0x03 // 1:4 multiplex
//#define CMD_SET_BIAS_MODE     0x06 // 1/2 bias
//#define CMD_FRAME_FREQUENCY   0x2E // default 199.7 Hz
//#define CMD_OSCILLATOR_CONTROL 0x18 // default
//#define CMD_LOAD_DATA_POINTER  0x80 // Start from S0

//command fix value
#define CMD_INITIALIZE        0x16
#define CMD_OTP_REFRESH       0xF0
#define CMD_MODE_SETTING      0x50 // BPS =INV = 0
#define CMD_OSCILLATOR_CONTROL 0x18 // default
#define CMD_SET_MUX_MODE      0x00
#define CMD_SET_BIAS_MODE     0x04
#define CMD_FRAME_FREQUENCY   0x20 // default 199.7 Hz
#define CMD_LOAD_DATA_POINTER  0x80 // Start from S0

//value in command
#define BPS_ENABLE              BIT_3
#define INV_ENABLE              BIT_2
#define PD_ENABLE               BIT_1 //power down mode
#define DISPLAY_ENABLE          BIT_0
#define EFR_9600Hz              0
#define EFR_230000Hz            BIT_2
#define COE_ENABLE              BIT_1
#define OSC_EXTERNAL            BIT_0 //default = 0 la internal
#define MUX_1_8                 0x01
#define MUX_1_6                 0x02
#define MUX_1_4                 0x03
#define BIAS_1_4                0x01
#define BIAS_1_3                0x03
#define BIAS_1_2                0x02
#define FREQ_199_7Hz            0x0E

/*CONTROL BYTES*/
#define CTR_LAST_CMD_REG      0x00
#define CTR_LAST_RAM          0x20      
#define CTR_CONT_CMD_REG      0x80
#define CTR_CONT_RAM          0xA0

/*READ_WRITE*/
#define READ_LCD    0xA0
#define WRITE_LCD   0x20 

#define READ_RTC    0xA3
#define WRITE_RTC   0x23

/*DATA POINTER: LCD PIN to PCF segment*/
#if defined(_ROUTER_)
#define PIN_1       35
#define PIN_2       34
#define PIN_3       33
#define PIN_4       32
#define PIN_5       31
#define PIN_6       30
#define PIN_7       29
#define PIN_8       12
#define PIN_9       13
#define PIN_10      14
#define PIN_11      15
#define PIN_12      16
#define PIN_13      17
#define PIN_14      28
#define PIN_15      27
#define PIN_16      26
#define PIN_17      25
#define PIN_18      24
#define PIN_19      23
#define PIN_20      22
#define PIN_21      21
#define PIN_22      20
#elif defined(_DT03P_RF_) && defined(_PCB_DT03P_TE3023_)
#define PIN_1       15
#define PIN_2       14
#define PIN_3       13
#define PIN_4       12
#define PIN_5       11
#define PIN_6       10
#define PIN_7       9
#define PIN_8       8
#define PIN_9       7
#define PIN_10      6
#define PIN_11      5
#define PIN_12      4
#define PIN_13      3
#define PIN_14      2
#define PIN_15      1
#define PIN_16      0
#define PIN_17      37
#define PIN_18      36
#define PIN_19      35
#define PIN_20      34
#define PIN_21      33
#define PIN_22      32
#elif defined(_LCD_PANNEL_PCF8545_M0754_V3_) //pannel LCD
#define PIN_1       34
#define PIN_2       33
#define PIN_3       32
#define PIN_4       31
#define PIN_5       30
#define PIN_6       29
#define PIN_7       24
#define PIN_8       23
#define PIN_9       22
#define PIN_10      21
#define PIN_11      20
#define PIN_12      19
#define PIN_13      18
#define PIN_14      17
#define PIN_15      16
#define PIN_16      15
#define PIN_17      14
#define PIN_18      13
#define PIN_19      12
#define PIN_20      11
#define PIN_21      10
#define PIN_22      9
#elif defined(_DT03M_RF_) && defined(_DT03MRF_PCB_TI_S8_S_V271_)
#define PIN_1       15
#define PIN_2       14
#define PIN_3       13
#define PIN_4       12
#define PIN_5       11
#define PIN_6       10
#define PIN_7       9
#define PIN_8       8
#define PIN_9       7
#define PIN_10      6
#define PIN_11      5
#define PIN_12      4
#define PIN_13      3
#define PIN_14      2
#define PIN_15      1
#define PIN_16      38
#define PIN_17      37
#define PIN_18      36
#define PIN_19      35
#define PIN_20      34
#define PIN_21      33
#define PIN_22      32
#endif

//define for PCF8545
#define BP_0         BIT_7
#define BP_1         BIT_6
#define BP_2         BIT_5
#define BP_3         BIT_4
#define BP_4         BIT_3
#define BP_5         BIT_2
#define BP_6         BIT_1
#define BP_7         BIT_0

#if defined(_DT03MRF_PCB_TI_S8_S_V271_)
#define COM0        BP_0
#define COM1        BP_1
#define COM2        BP_2
#define COM3        BP_3
#define COM4        BP_4
#define COM5        BP_5
#define COM6        BP_6
#define COM7        BP_7
#else
#define COM0        BP_0
#define COM1        BP_1
#define COM2        BP_2
#define COM3        BP_3
#define COM4        BP_4
#define COM5        BP_5
#define COM6        BP_6
#define COM7        BP_7
#endif

/*ALPHABET CHARACTER*/
#define CHAR_A		0xEE
#define CHAR_B		0x3E
#define CHAR_C		0x9A
#define CHAR_D		0x76
#define CHAR_E		0x9E
#define CHAR_F      0x8E
#define CHAR_V      0x7A
#define CHAR_R      0x8A
#define CHAR_DASH   0x04

#define EMPTY		    0x00

/*SPECIAL CHARACTER*/
//COM 0
#define S29			    COM0
#define PIN_S29         PIN_1
#define S27             COM0
#define PIN_S27         PIN_3
#define S28             COM0
#define PIN_S28         PIN_5
#define ACB             COM0
#define PIN_ACB         PIN_6
#define P3              COM0
#define PIN_P3          PIN_8
#define P2              COM0
#define PIN_P2          PIN_10
#define S22             COM0
#define PIN_S22         PIN_11
#define P1              COM0
#define PIN_P1          PIN_13
#define S18             COM0
#define PIN_S18         PIN_15
#define S30             COM0
#define PIN_S30         PIN_17
#define S7              COM0
#define PIN_S7          PIN_19
#define S6              COM0
#define PIN_S6          PIN_21
//COM 1
#define S8              COM1
#define PIN_S8          PIN_6
#define S21             COM1
#define PIN_S21         PIN_11
//COM 2
#define S26             COM2
#define PIN_S26         PIN_6
#define S20             COM2
#define PIN_S20         PIN_11
//COM 3
#define COL2            COM3
#define PIN_COL2        PIN_6
#define S19             COM3
#define PIN_S19         PIN_11
//COM 4
#define P6              COM4
#define PIN_P6          PIN_1
#define S1              COM4
#define PIN_S1          PIN_6
#define S13             COM4
#define PIN_S13         PIN_11
#define S14             COM4
#define PIN_S14         PIN_12
#define S15             COM4
#define PIN_S15         PIN_13
#define S16             COM4
#define PIN_S16         PIN_14
#define P4              COM4
#define PIN_P4          PIN_15
#define P7              COM4
#define PIN_P7          PIN_17
#define P8              COM4
#define PIN_P8          PIN_19
#define P9              COM4
#define PIN_P9          PIN_21
//COM 5
#define S2              COM5
#define PIN_S2          PIN_6
#define S12             COM5
#define PIN_S12         PIN_11
#define S5              COM5
#define PIN_S5          PIN_12
#define S17             COM5
#define PIN_S17         PIN_13
#define S24             COM5
#define PIN_S24         PIN_14
#define P5              COM5
#define PIN_P5          PIN_15
//COM 6
#define S3              COM6
#define PIN_S3          PIN_6
#define S11             COM6
#define PIN_S11         PIN_11
//COM 7
#define S23             COM7
#define PIN_S23         PIN_3
#define S25             COM7
#define PIN_S25         PIN_5
#define S4              COM7
#define PIN_S4          PIN_6
#define T               COM7
#define PIN_T           PIN_8
#define S10             COM7
#define PIN_S10         PIN_11
#define S9              COM7
#define PIN_S9          PIN_15

//#define P1 			    COM0 // PIN_13
//#define P2 			    COM0 // PIN_10 (P2)
//#define P3 			    COM0 // PIN_8
//#define P6			    COM4 // P6
//#define P7			    COM4 // P7
//#define P8			    COM4 // P8
//#define P9			    COM4 // P9
//#define S6	            COM0 // PIN_21 (RF)
//#define S7	            COM0 // PIN_19 (LOW BATTERY)
//#define S30             COM0 // PIN_17 //V high
//#define S29             COM0 // PIN_1 //V low
//#define S27             COM0 // PIN_3 //A high
//#define S28             COM0 // PIN_5 //unlock
//#define ACB             COM0 // PIN_6 //ACB
//#define S22             COM0 // PIN_11 // L1
//#define S18             COM0 // PIN_15 //MAGNETIC
//#define S21             COM1 // PIN_11 //L2
//#define S26             COM2 // PIN_6 //set
//#define COL2            COM3 // PIN_6 //col
//#define S20             COM2 // PIN_11//L3
//#define S19             COM3 // PIN_11//ground
//#define S8	            COM1 // PIN_6 (LEFT_ARROW)
//#define S23             COM7 // PIN_3 //Er
//#define S25             COM7 // PIN_5 //MD
//#define S1              COM4
//#define S2              COM5
//#define S3              COM6
//#define S4              COM7
//#define T               COM7
//#define S13             COM4
//#define S12             COM5
//#define S11             COM6
//#define S10             COM7
//#define S14             COM4
//#define S5              COM5
//#define S15             COM4
//#define S17             COM5
//#define S16             COM4
//#define S24             COM5
//#define P4              COM4
//#define P5              COM5
//#define S9              COM7
//==============================================================================
#define MAX_DIGITS                      10 //10 chu so

#define OBIS_PHASE_A                    20
#define OBIS_PHASE_B                    40
#define OBIS_PHASE_C                    60
#define DISPLAY_OBIS_CHARACTER_C        0xFF

#define PHASE_A 0
#define PHASE_B 1
#define PHASE_C 2
#define PHASE_TOTAL 5
#define pcf8545_cs_dir_output()			lcd_driver_cs_dir_output()	//(LCD_CS_DIR |= LCD_CS_BIT)
#define pcf8545_cs_output_low()			lcd_driver_cs_output_low()		//(LCD_CS_OUT &= ~LCD_CS_BIT)
#define pcf8545_cs_output_high()		lcd_driver_cs_output_high()			//(LCD_CS_OUT |= LCD_CS_BIT)

#define pcf8545_reset_dir_output()		lcd_driver_reset_dir_output()	//(LCD_RESET_DIR |= LCD_RESET_BIT)
#define pcf8545_reset_output_low()		lcd_driver_reset_output_low()	//(LCD_RESET_OUT &= ~LCD_RESET_BIT)
#define pcf8545_reset_output_high()		lcd_driver_reset_output_high()	//(LCD_RESET_OUT |= LCD_RESET_BIT)

#define pcf8545_sdi_dir_output()        lcd_driver_spi_sdi_dir_output()
#define pcf8545_sdi_output_high()       lcd_driver_spi_sdi_output_high()
#define pcf8545_sdi_output_low()        lcd_driver_spi_sdi_output_low()

#define pcf8545_sclk_dir_output()       lcd_driver_spi_scl_dir_output()
#define pcf8545_sclk_output_high()      lcd_driver_spi_scl_output_high()
#define pcf8545_sclk_output_low()       lcd_driver_spi_scl_output_low()

extern void startup_pcf8545(void);
extern void init_character();
extern void lcd_disable();
extern void lcd_enable();
extern void load_data_pointer(unsigned char start_segment);
extern void lcd_clear_all();
extern void display_character(unsigned char data_pointer, unsigned char character);
extern unsigned char swap_byte(unsigned char in_byte);
extern unsigned char encode_lcd(unsigned char in_byte);
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
extern void lcd_display_value(uint64_t value);
extern void lcd_display_digit(unsigned char value,unsigned char  pos);
extern void lcd_hide_digit(unsigned char  pos);
extern void lcd_display_tariff(unsigned char value, char status);
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
extern void lcd_display_obis(unsigned long value);
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
extern void lcd_MW(char status);
extern void lcd_VA(char status);
extern void lcd_kVA(char status);
extern void lcd_MVA(char status);
extern void lcd_VAr(char status);
extern void lcd_kVAr(char status);
extern void lcd_MVAr(char status);
extern void lcd_Hz(char status);
extern void lcd_V_high(char status);
extern void lcd_V_low(char status);
extern void lcd_A_high(char status);
extern void lcd_phA(char status);
extern void lcd_phB(char status);
extern void lcd_phC(char status);
extern void lcd_warning(char status);
extern void lcd_display_alphabet(unsigned char value,unsigned char  pos);
extern void lcd_display_all(void);
extern void pcf8545_display_init();

//cac symbol ko co
extern void lcd_Err(char status);
extern void lcd_point4(char status);
extern void lcd_point5(char status);
extern void lcd_SUM(char status);
#endif //PCF8545_DISPLAY_SUPPORT
#endif /* LCD_PCF8545_M0754V2_H_ */
