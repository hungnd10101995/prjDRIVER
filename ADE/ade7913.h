/* 
 * File:   ade7913.h
 * Author: KHUE
 *
 * Created on October 7, 2014, 9:56 AM
 */

#ifndef _ADE7913_H_
#define _ADE7913_H_
#include "main.h"
#include "macro.h"
// ADE Register defines
// Thanh ghi 24 bit//READ END 0B100
#define IWV             (0x0  << 3 | 0b100)
#define V1WV            (0x1  << 3 | 0b100)
#define V2WV            (0x2  << 3 | 0b100)
// Thanh ghi 16 bit
#define ADC_CRC         (0x4  << 3 | 0b100)
//#define CTRL_CRC      0x50
#define CNT_SNAPSHOT    (0x7  << 3 | 0b100)
#define CONFIG_READ     (0x8  << 3 | 0b100)
#define STATUS0_READ    (0x9  << 3 | 0b100)
#define EMI_CTRL_READ   (0xE  << 3 | 0b100)

// Thanh ghi 8 bit//write end 0b000
#define CONFIG_WRITE     (0x8 << 3 | 0b000)
#define EMI_CTRL_WRITE   (0xE << 3 | 0b000)
#define SYNC_SNAP_WRITE  (0xB << 3 | 0b000)
#define LOCK_KEY_WRITE   (0xA << 3 | 0b000)
//#define COUNTER0    0xC0
//#define COUNTER1    0xD0

//#define STATUS1     0xF0
//#define TEMPOS      0x18

#define DUMMY_MSG        0x00               // Unused argument to SPI.Transfer()
#define LOCK_BYTE        0xCA
#define UNLOCK_BYTE      0x9C


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

#ifdef _ADE7913_
    #if defined PCB_01_100A_75MV //PCB 01 - SHUNT 100A - 75mV
        #define VRMS_REF            395770//for 50VDC-PCB01
        #define VRMS_REF_OFFSET     374336//for 0VDC-PCB01
        #define VOLTAGE_CALIBRATION 50 // Can chinh dien ap tai 50VDC
        #define IRMSA_REF           368177//for 3.0A-PCB01
        #define IRMSA_REF_OFFSET    362878//for 0A-PCB01
        #define CURRENT_CALIBRATION 3 // Can chinh dong dien tai 3.0A
    #elif defined PCB_02_100A_75MV //PCB 02 - SHUNT 100A - 75mV
        #define VRMS_REF            2366510//for 300VDC-PCB02
        #define VRMS_REF_OFFSET     364806//for 0VDC-PCB02
        #define VOLTAGE_CALIBRATION 300 // Can chinh dien ap tai 300VDC
        #define IRMSA_REF           344930//339316//for 3A - PCB02
        #define IRMSA_REF_OFFSET    338550//for 0A - PCB02
        #define CURRENT_CALIBRATION 3 // Can chinh dong dien tai 3.0A
    #elif defined PCB_02_SHUNT_P80 //PCB 02 - SHUNT 01P80
        #define VRMS_REF            2366510//for 300VDC-PCB02
        #define VRMS_REF_OFFSET     364806//for 0VDC-PCB02
        #define VOLTAGE_CALIBRATION 300 // Can chinh dien ap tai 300VDC
        #define IRMSA_REF           96364//339316//for 3A - PCB02
        #define IRMSA_REF_OFFSET    338934//for 0A - PCB02
        #define CURRENT_CALIBRATION 3 // Can chinh dong dien tai 3.0A
    #elif defined PCB_02_200A_50MV //PCB 02 - SHUNT 01P80
        #define VARMS_REF           304145//2388545;//2366510//for 300VDC-PCB02
        #define VARMS_REF_OFFSET    342404//384363;//374336;//364806//for 0VDC-PCB02
        #define VBRMS_REF           261507//2388545;//2366510//for 300VDC-PCB02
        #define VBRMS_REF_OFFSET    341642//384363;//374336;//364806//for 0VDC-PCB02
        #define VOLTAGE_CALIBRATION 50 // Can chinh dien ap tai 50VDC
        #define IRMSA_REF           402582;//415260//for 10A // 339316//for 3A - PCB02
        #define IRMSA_REF_OFFSET    386367;//339834//for 0A - PCB02
        #define CURRENT_CALIBRATION 10 //can chinh tai 10A // 3 //Can chinh dong dien tai 3.0A
    #elif defined PCB_01_200A_50MV
        #define VRMS_REF            395770//for 50VDC-PCB01
        #define VRMS_REF_OFFSET     374336//for 0VDC-PCB01
        #define VOLTAGE_CALIBRATION 50 // Can chinh dien ap tai 50VDC
        #define IRMSA_REF           415260//for 10A // 339316//for 3A - PCB02
        #define IRMSA_REF_OFFSET    339834//for 0A - PCB02
        #define CURRENT_CALIBRATION 10 //can chinh tai 10A // 3 //Can chinh dong dien tai 3.0A
    #else

    #endif
    /////////////////////
    #define AVERAGE_COUNTER     69 // average calculator     
    #define CURRENT_MIN         10 // 0.1 A
    #define VOLTAGE_MIN         1000 // 10-VOLATGE

#else
    #define VRMS_REF		3816971// for 300VDC //0x000B6AB2//for v=60v, gain =1// 5507302 // for gain =2//0x5504D8// 0x55056E
    #define IRMSA_REF		992758// for 10A //180805// 
    #define IRMSB_REF		785840// for 10A //992758//312943//0x9C791
#endif

    #define AIGAIN_REF      10000 // thong so nay dung de can chinh
    #define BIGAIN_REF      10000 // 9875// thong so nay dung de can chinh
    #define VGAIN_REF       10000 // 9860 // thong so nay dung de can chinh
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
#define     CAL_POW         8388608 //2^23
#define     CAL_UI          10000
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
    uint8_t enable;
    uint8_t status;
    uint8_t count;
}MEANSURE_ADE7913_STRUCT;

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
            int32_t i_ref;//i_gain; // i calib
            int32_t v_ref;//v_gain; // v calib
            int32_t irms_offset; //irms_gain; 
            int32_t vrms_offset; //vrms_gain;  
            int32_t irms_ref; //watt_gain; 
            int32_t var_gain;
            int32_t va_gain;
            int16_t phase;
            int32_t dips_scale; //he so hien thi active power
            int32_t vrms_ref;//watt_os;
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
        MEANSURE_ADE7913_STRUCT current;
        MEANSURE_ADE7913_STRUCT voltage;
        uint8_t count_10ms;
        uint8_t count_cf;
        uint8_t count_update_ade7913;
        uint8_t count_250ms;
        uint8_t enable;
        uint8_t count;
        uint8_t status;
//        uint8_t cal_offset_rms;
        uint8_t cal_rms;
    }flags;
    ANGLE_MODE angle_mode;
    uint16_t addr_reg;
    uint8_t  reg_uint8;
    uint16_t reg_uint16;
    uint32_t reg_uint32;     
    uint32_t reg_value;    // Kiem soat lai va xoa 
    uint32_t vrms_offset;
    uint32_t vrms_temp;
    uint32_t v2rms_temp;
    uint32_t irms_offset;
    uint32_t irms_temp;
    uint32_t vrms_calib;
    uint32_t irms_calib;
    uint8_t count_calib;
} ADE_STRUCT;

extern ADE_STRUCT ade7913;
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
extern void ade7913_write(unsigned short address, unsigned char *buffer, unsigned char length);
extern void ade7913_read(unsigned short address, unsigned char *buffer, unsigned char length);
extern unsigned long ade7913_read_24bit(unsigned short address);
extern unsigned short ade7913_read_16bit(unsigned short address);
extern unsigned char ade7913_read_8bit(unsigned short address);
extern unsigned char ade7913_clkout_ready(void);
extern BOOL ade7913_write_24bit(unsigned short address, unsigned long _data);
extern BOOL ade7913_write_16bit(unsigned short address, unsigned short _data);
extern BOOL ade7913_write_8bit(unsigned short address, unsigned char _data);
//extern void ade7913_config(void);
extern void ade7913_init(void);
extern void ade7913_write_configure(void);
//extern void ade7913_get_vrms(void);
//extern void ade7913_get_irms_a(void);
//extern void ade7913_get_irms_b(void);
//extern void ade7913_get_awatt(void);
//extern void ade7913_get_bwatt(void);
//extern void ade7913_get_pfa(void);
//extern void ade7913_get_pfb(void);
extern unsigned char ade7913_check_arevp(void);
extern unsigned char ade7913_check_brevp(void);
extern void ade7913_check_param(void);

void ade7913_struct_calib(uint16_t address);
BOOL ade7913_set_calib_value_phaseA(BOOL wr);
BOOL ade7913_set_calib_value_phaseB(BOOL wr);
void ade7913_get_calib_value_phaseA(void);
void ade7913_get_calib_value_phaseB(void);

extern void ade7913_write_SUBCMD_RW_ADE_REGISTER(uint8_t module);
//void ade7913_write_SUBCMD_RW_ADE_REGISTER(uint8_t *rx_buffer, uint8_t module);
extern void ade7913_read_SUBCMD_RW_ADE_REGISTER(uint8_t module);
extern BOOL ade7913_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length);
extern BOOL ade7913_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase);
extern BOOL ade7913_load_calib_value(void);
#endif





