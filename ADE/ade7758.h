#ifndef ADE7758_H_
#define ADE7758_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include "../../prjEMETERS/library.h"
#include "../../LIBRARY/framework/macro.h"

#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#include "../../ROUTER/FW/hardware.h"
#endif

#ifdef ADE7858_SUPPORT

#define    AWATTHR       0x01  //16bit        
#define    BWATTHR       0x02        
#define    CWATTHR       0x03  
#define    AVARHR       0x04        
#define    BVARHR       0x05        
#define    CVARHR       0x06
      
#define    AVAHR       0x07        
#define    BVAHR       0x08        
#define    CVAHR       0x09

#define    AIRMS         0x0A   //24 bit
#define    BIRMS         0x0B
#define    CIRMS         0x0C

#define    AVRMS         0x0D   //24bit
#define    BVRMS         0x0E 
#define    CVRMS         0x0F    

#define    FREQ          0x10    
#define    TEMP          0x11
#define    WFORM         0x12  //24bit

#define    OPMODE       0x13   //8bit
#define    MMODE        0x14
#define    WAVMODE      0x15
#define    COMPMODE     0x16
#define    LCYCMODE     0x17

#define    MASK         0x18     //20bit
#define    STATUS       0x19
#define    RSTATUS      0x1A
#define    ZXTOUT       0x1B
#define    LINECYC      0x1C  //16bit
#define    SAGCYC       0x1D  
#define    SAGLVL       0x1E  
#define    VPINTLVL     0x1F  
#define    IPINTLVL     0x20
#define    VPEAK        0x21
#define    IPEAK        0x22   

#define    GAIN         0x23
#define    AVRMSGAIN    0x24
#define    BVRMSGAIN    0x25
#define    CVRMSGAIN    0x26

#define    AIGAIN       0x27
#define    BIGAIN       0x28
#define    CIGAIN       0x29
      
#define    AWG          0x2A        
#define    BWG          0x2B
#define    CWG          0x2C

#define    AVARG        0x2D     
#define    BVARG        0x2E 
#define    CVARG        0x2F  
  
#define    AVAG         0x30
#define    BVAG         0x31
#define    CVAG         0x32

#define    AVRMSOS      0x33
#define    BVRMSOS      0x34
#define    CVRMSOS      0x35

#define    AIRMSOS      0x36
#define    BIRMSOS      0x37
#define    CIRMSOS      0x38

#define    AWATTOS      0x39
#define    BWATTOS      0x3A
#define    CWATTOS      0x3B

#define    AVAROS       0x3C
#define    BVAROS       0x3D
#define    CVAROS       0x3E

#define    APHCAL       0x3F     //7 bit
#define    BPHCAL       0x40  
#define    CPHCAL       0x41  
                            
#define    WDIV         0x42
#define    VARDIV       0x43
#define    VADIV        0x44

#define    APCFNUM      0x45   //16bit  bit(15-13]: Sign of Active power of Phase A-B-C
#define    APCFDEN      0x46
#define    VARCFNUM     0x47   //Sign of Reactive power of Phase A-B-C
#define    VARCFDEN     0x48

#define     TERMSEL_C   BIT2
#define     TERMSEL_B   BIT3
#define     TERMSEL_A   BIT4

#define     FREQSEL_C   0
#define     FREQSEL_B   BIT0
#define     FREQSEL_A   BIT1

//define for PCB
#define     WATT_GAIN_A_ADDR     CWG
#define     WATT_GAIN_B_ADDR     BWG
#define     WATT_GAIN_C_ADDR     AWG

#define     VAR_GAIN_A_ADDR      CVARG
#define     VAR_GAIN_B_ADDR      BVARG
#define     VAR_GAIN_C_ADDR      AVARG

#define     PHASE_CAL_A_ADDR     CPHCAL
#define     PHASE_CAL_B_ADDR     BPHCAL
#define     PHASE_CAL_C_ADDR     APHCAL

#define     WATT_OFFSET_A_ADDR     CWATTOS //12 bit
#define     WATT_OFFSET_B_ADDR     BWATTOS //12 bit
#define     WATT_OFFSET_C_ADDR     AWATTOS //12 bit

#define     VAR_OFFSET_A_ADDR     CVAROS //12 bit
#define     VAR_OFFSET_B_ADDR     BVAROS //12 bit
#define     VAR_OFFSET_C_ADDR     AVAROS //12 bit

#define     V_SCALE     1000//502//1000 //427
#define     I_SCALE     1000//151//1000
//cs
#define ade7758_cs_dir_output()                 (ADE7858_CS_DIR clr ADE7858_CS_BIT)//TRISFbits.TRISF3 = 0
#define ade7758_cs_low()                        (ADE7858_CS_OUT clr ADE7858_CS_BIT)//LATFbits.LATF3 = 0
#define ade7758_cs_high()                       (ADE7858_CS_OUT set ADE7858_CS_BIT)//LATFbits.LATF3 = 1
#define ade7758_cs_tgl()                        (ADE7858_CS_OUT tgl ADE7858_CS_BIT)//LATFbits.LATF3 =!(LATFbits.LATF3)
//sdi
#define ade7758_sdi_dir_output()                TRISFbits.TRISF5 = 0
#define ade7758_sdi_output_high()               LATFbits.LATF5 = 1
#define ade7758_sdi_output_low()                LATFbits.LATF5 = 0
//clk
#define ade7758_sclk_dir_output()               TRISFbits.TRISF4 = 0
#define ade7758_sclk_output_high()              LATFbits.LATF4 = 1
#define ade7758_sclk_output_low()               LATFbits.LATF4 = 0
//sdo
#define ade7758_sdo_dir_input()                 TRISDbits.TRISD7 = 1
#define ade7758_sdo_input()                     PORTDbits.RD7

#define spi_change_falling_edge_mode(n)      1

extern unsigned char dread1,dread2,dread3;
extern void spi_init();
extern void ade7758_init(void);
extern void ade7758_config(void);
extern void write_7758_1byte(unsigned char address,unsigned char dwrite);
extern void write_7758_2byte(unsigned char address,unsigned char dwrite1,unsigned char dwrite2);
extern void write_7758_3byte(unsigned char address,unsigned char dwrite1,unsigned char dwrite2,unsigned char dwrite3);
extern void read_7758(unsigned int address,unsigned char bytecount);
extern void measure_task(void);
extern void measure_vrms(void);
extern void measure_irms(void);
extern void measure_activepower(void);
extern void measure_apparentpower(void);
extern void measure_freq(void);
extern void measure_temperature(void);
extern void measure_line_cycles(void);
extern void check_reverse_active_power(void);
extern void check_reverse_reactive_power(void);
unsigned char ade7758_spi_get_byte(void);
void ade7758_spi_send_byte(unsigned char value);
void ade7758_spi_send_3byte(unsigned long value);

#endif //ADE7858_SUPPORT
#endif /* ADE7758_H_ */
