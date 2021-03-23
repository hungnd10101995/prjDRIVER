/* 
 * File:   ade7953.h
 * Author: KHUE
 *
 * Created on October 7, 2014, 9:56 AM
 */

#ifndef _ADE7953_H_
#define _ADE7953_H_
#include "main.h"
#include "macro.h"
#ifdef _ADE7953_
// ADE Register defines
// Thanh ghi 8 bit
#define DISNOLOAD   0x001
#define LCYCMODE    0x004
#define PGA_V	    0x007
#define PGA_IA	    0x008
#define PGA_IB	    0x009
#define VERSION 	0x702
// Thanh ghi 16 bit
#define CONFIG	    0x102
#define CFMODE	    0x107
#define LINECYC	    0x101
#define PHCALA   	0x108
#define PHCALB   	0x109
#define PFA   	    0x10A
#define PFB   	    0x10B
#define ANGLE_A   	0x10C
#define ANGLE_B   	0x10D
#define PERIOD   	0x10E
#define CF1DEN          0x103
#define CF2DEN          0x104
#define ALT_OUTPUT      0x110
// thanh ghi 32 bit
#define ACCMODE	    0x301//0x201
#define IRMSA		0x31A//0x21A
#define IRMSB  		0x31B//0x21B
#define VRMS   		0x31C//0x21C
#define AWATT   	0x312
#define BWATT   	0x313
#define AVAR   	    0x314//0x214
#define BVAR   	    0x315//0x215
#define IA          0x316
#define IB          0x317
#define VA          0x318
#define AVA         0x310//0x210
#define BVA         0x311//0x211
#define AENERGYA   	0x31E//0x21E
#define AENERGYB   	0x31F//0x21F
#define RENERGYA   	0x320//0x220
#define RENERGYB   	0x321//0x221
#define IAPEAK      0x328
#define VPEAK       0x326
#define AIGAIN          0x380//0x280
#define AVGAIN          0x381//0x281
#define AWGAIN          0x382//0x282
#define AVARGAIN        0x383//0x283
#define AVAGAIN         0x384//0x284
#define AIRMSOS         0x386//0x286
#define VRMSOS          0x388//0x288
#define AWATTOS         0x389//0x289
#define AVAROS          0x38A//0x28A
#define AVAOS           0x38B//0x28B

#define BIGAIN          0x38C//0x28C
#define BVGAIN          0x38D//0x28D
#define BWGAIN          0x38E//0x28E
#define BVARGAIN        0x38F//0x28F
#define BVAGAIN         0x390//0x290
#define BIRMSOS         0x392//0x292
#define BWATTOS         0x395//0x295
#define BVAROS          0x396//0x296
#define BVAOS           0x397//0x297

#define IRQENA          0x32C//0x22C
#define IRQSTATA        0x32D//0x22D
#define RSTIRQSTATA     0x32E//0x22E
#define IRQENB          0x32F//0x22F
#define IRQSTATB        0x330//0x230
#define RSTIRQSTATB     0x331//0x231
#define AP_NOLOAD       0x303
#define VAR_NOLOAD      0x304
#define VA_NOLOAD       0x305

#define APSIGN_A_BIT        BIT_10
#define APSIGN_B_BIT        BIT_11
#define VARSIGN_A_BIT       BIT_12
#define VARSIGN_B_BIT       BIT_13

// REF defines
#define CFDEN_SHUNT		0x1285//0x4A15  //1000 imp/kwh shunt 200u 5A-220V
//#define CFDEN_SHUNT		0x380C  //1600 imp/kwh shunt 200u
#define	CFDEN_CT		0x38B8//0x7452   //1000 imp/kwh CT 1:2000 10Ohm
//#ifdef _15770001
// <default calibration value>
#ifdef METER_DC
	#define VRMSOS_REF      0xF307B0//0x15946D//0x15DB3C//0//0x5CF592
	#define AIRMSOS_REF		0xFF80CF48// for DC 
	#define AWGAIN_REF		0x400F09//calib theo aenergy 300V,10A////0xCCCCC//0x3E5AB7//0x3CFD80//0x3DC562//0x3E12A7 /*0x3F5597*/ //0x3F6D32
	#define AWATTOS_REF		0xFFE5F000// for DC //  tang 0x001000 don vi la tang 0.05%
	#define PHCALA_REF		0x038C//0xFF8D//0x0020 //0x0005// 0x0002 // tang moi don vi la tang 0.05%
    #define BIRMSOS_REF     0//0xFF80CF48//0x02FD//0//0x0029E7
    #define BWGAIN_REF		4038554//0x400F09//0x3DB8E2//0x400F67
    #define BWATTOS_REF		0//0xFFE5F000//0xFF8089//0x004032
	#define PHCALB_REF		0x038D//0x0001

    #define VRMS_REF		3816971// for 300VDC //0x000B6AB2//for v=60v, gain =1// 5507302 // for gain =2//0x5504D8// 0x55056E
    #define IRMSA_REF		992758// for 10A //180805// 
    #define IRMSB_REF		785840// for 10A //992758//312943//0x9C791
    #define AWATT_REF       0x45D//0x21B
    #define BWATT_REF       0x45D//0x340
    #define ANGLEA_REF      0x13CEA
    #define ANGLEB_REF      0x13CEA
    #define PFA_REF         0x7FFF
    #define PFB_REF         0x7FFF
    #define AIGAIN_REF   10000
    #define BIGAIN_REF   9875//10000
    #define VGAIN_REF    9860
#else
	#define VRMSOS_REF      0x15DB3C//0//0x5CF592
	#define AIRMSOS_REF		0x0AF2//0//0xFFF764
	#define AWGAIN_REF		0x3E5AB7//0x3CFD80//0x3DC562//0x3E12A7 /*0x3F5597*/ //0x3F6D32
	#define AWATTOS_REF		0x002AEC//0x002FFF//0xFF9589 /*0xFF0265*/ //0x0127C4 // tang 0x001000 don vi la tang 0.05%
	#define PHCALA_REF		0x038C//0xFF8D//0x0020 //0x0005// 0x0002 // tang moi don vi la tang 0.05%
    #define BIRMSOS_REF     0x02FD//0//0x0029E7
    #define BWGAIN_REF		0x3DB8E2//0x400F67
    #define BWATTOS_REF		0xFF8089//0x004032
	#define PHCALB_REF		0x038D//0x0001

    #define VRMS_REF		5507302//0x5504D8// 0x55056E
    #define IRMSA_REF		408742//4087417//0x658AB//0x665DB // 
    #define IRMSB_REF		312943//0x9C791
    #define AWATT_REF       0x21B
    #define BWATT_REF       0x340
    #define ANGLEA_REF      0x13CEA
    #define ANGLEB_REF      0x13CEA
    #define PFA_REF         0x7FFF
    #define PFB_REF         0x7FFF
    #define AIGAIN_REF   9799
    #define BIGAIN_REF   9681
    #define VGAIN_REF    9834
#endif


#define     A_P_SCALE_C     10000
#define     B_P_SCALE_C     10000
#define     CAL_POW     8388608 //2^23
#define     CAL_UI      10000
#define DIGITAL_FILTER_CLK  223000
#define F_LINE     50 //Hz

#define CF1_CHANEL_A     0x0040
#define CF1_CHANEL_B     0x0004



#define MEASUREMENT_CHANEL_B

#if defined(MEASUREMENT_CHANEL_B)
#define AENERGYx    AENERGYB
#define AENERGYx_CONST  113 // ID 17820010 // 106 // 0.000106 Wh/LSB for CHANEL B //85 // 0.000085 Wh/LSB = 85/1000000
#else
#define AENERGYx    AENERGYA
#define AENERGYA_CONST  113 // ID 17820010 // 106 // 0.000106 Wh/LSB for CHANEL B //85 // 0.000085 Wh/LSB = 85/1000000
#endif
typedef enum {ANGLE_UI = 0x01, ANGLE_UU = 0x02, ANGLE_II = 0x03} ANGLE_MODE;
typedef struct 
{
    struct{
        unsigned long vrmsA;
        unsigned long vrmsB;
//        unsigned long vrmsC;
        unsigned long irmsA;
        unsigned long irmsB;
//        unsigned long irmsC;
        long active_powerA; //active power A
        long active_powerB; //active power B
//        long active_powerC; //active power C  
//        long active_power; //total  
        long reactive_powerA; //reactive power A
        long reactive_powerB; //reactive power B
//        long reactive_powerC; //reactive power C    
//        long reactive_power; // total
        long apparent_powerA; //active power A
        long apparent_powerB; //active power B
//        long apparent_powerC; //active power C   
        int power_factorA; //active power A
        int power_factorB; //active power B
//        int power_factorC; //active power C  
        float phase_angleA;
        float phase_angleB;
//        float phase_angleC;
        long freq;
        unsigned long line_cycles;
        long temperature;
    } measure;
    struct 
    {
        struct
        {
            int32_t i_gain;
            int32_t v_gain;
            int32_t irms_gain;
            int32_t vrms_gain;   
            int32_t watt_gain;
            int32_t var_gain;
            int32_t va_gain;
            int16_t phase;
            int32_t dips_scale; //he so hien thi active power
            int32_t watt_os;
        }pA, pB;
    }calib;
    struct 
    {
        signed register_error:1;
        signed spi_error:1;
        signed pA_set_offset:1;
        signed pB_set_offset:1;
//        signed pC_set_offset:1;
        signed angle_UU_mode:1; //==1 neu dang do goc U^U, ==0 neu dang do dem U^I
    }flags;
    ANGLE_MODE angle_mode;
    uint16_t addr_reg;
    uint8_t  reg_uint8;
    uint16_t reg_uint16;
    uint32_t reg_uint32;     
    uint32_t reg_value;    // Kiem soat lai va xoa 
} ADE_STRUCT;

extern ADE_STRUCT ade7953;
//#endif
/*
#define PULSE_REF   10068
#define AENERGYA_REF     0x00003F52
#define AENERGYB_REF     0x00003F52
#define VRMS_REF    0x0063FF88
#define IRMSA_REF   0x000EEC00
#define IRMSB_REF   0x000EC388
#define AWATT_REF   0x0005DFF7
#define BWATT_REF   0x0005D7F7
#define AVAR_REF    0x0005DFF7
#define BVAR_REF    0x0005D7F7
#define PERIOD_REF  0x1172
#define PFA_REF     0x7FFF
#define PFB_REF     0x7FFF
*/
/*extern unsigned long PULSE_REF;
extern unsigned long AENERGYA_REF;
extern unsigned long AENERGYB_REF;
extern unsigned long VRMS_REF;
extern unsigned long IRMSA_REF;
extern unsigned long IRMSB_REF;
extern unsigned long AWATT_REF;
extern unsigned long BWATT_REF;
extern unsigned long AVAR_REF;
extern unsigned long BVAR_REF;
extern unsigned short PFA_REF;
extern unsigned short PFB_REF;
extern unsigned short PERIOD_REF;
extern unsigned short PHCALA_REF;
extern unsigned short PHCALB_REF;
//
extern unsigned long ADE_AENERGYA;
extern unsigned long ADE_AENERGYB;
extern unsigned long VRMS_ADE;
extern unsigned long IRMSA_ADE;
extern unsigned long IRMSB_ADE;
extern unsigned long AWATT_ADE;
extern unsigned long BWATT_ADE;
extern unsigned long AVAR_ADE;
extern unsigned long BVAR_ADE;
extern unsigned short PFA_ADE;
extern unsigned short PFB_ADE;
extern unsigned short PERIOD_ADE;
extern unsigned short PHCALA_ADE;
extern unsigned short PHCALB_ADE;*/
//
//extern unsigned char MPL_Flag;
//extern unsigned char PHCAL_Flag;
//extern unsigned char ADE_Flag;
//extern unsigned char ADE_WriteData[4];
//extern unsigned char ADE_ReadData[4];
extern void ade7953_write(unsigned short address, unsigned char *buffer, unsigned char length);
extern void ade7953_read(unsigned short address, unsigned char *buffer, unsigned char length);
extern long ade7953_read_32bit(unsigned short address);
extern unsigned long ade7953_read_24bit(unsigned short address);
extern unsigned short ade7953_read_16bit(unsigned short address);
extern unsigned char ade7953_read_8bit(unsigned short address);
extern BOOL ade7953_write_32bit(unsigned short address, unsigned long _data);
extern BOOL ade7953_write_24bit(unsigned short address, unsigned long _data);
extern BOOL ade7953_write_16bit(unsigned short address, unsigned short _data);
extern BOOL ade7953_write_8bit(unsigned short address, unsigned char _data);
//extern void ade7953_config(void);
extern void ade7953_init(void);
//extern void ade7953_get_vrms(void);
//extern void ade7953_get_irms_a(void);
//extern void ade7953_get_irms_b(void);
//extern void ade7953_get_awatt(void);
//extern void ade7953_get_bwatt(void);
//extern void ade7953_get_pfa(void);
//extern void ade7953_get_pfb(void);
extern unsigned char ade7953_check_arevp(void);
extern unsigned char ade7953_check_brevp(void);
extern void ade7953_check_param(void);

void ade7953_struct_calib(uint16_t address);
BOOL ade7953_set_calib_value_phaseA(BOOL wr);
BOOL ade7953_set_calib_value_phaseB(BOOL wr);
void ade7953_get_calib_value_phaseA(void);
void ade7953_get_calib_value_phaseB(void);

extern void ade7953_write_SUBCMD_RW_ADE_REGISTER(uint8_t module);
//void ade7953_write_SUBCMD_RW_ADE_REGISTER(uint8_t *rx_buffer, uint8_t module);
extern void ade7953_read_SUBCMD_RW_ADE_REGISTER(uint8_t module);
extern BOOL ade7953_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length);
extern BOOL ade7953_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase);
extern BOOL ade7953_load_calib_value(void);
#endif //  _ADE7953_=================
//============================
#endif // _ADE7953_H_





