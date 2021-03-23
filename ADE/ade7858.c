#include <stdio.h>

// Header prjEMETERS
#include "devices.h"
#include "main.h"
#include "hardware.h"

// Header prjLIB
#if defined(ADE7858_SUPPORT) || defined(ADE7880_SUPPORT)
#include "ade-measure-foreground.h"
#include "delay.h"
#include "macro.h"
#include "communication.h"
#include "eeprom.h"
#include "string.h"

// Header prjDRVIER
#include "ade7858.h"


ADE_STRUCT ade7858;
unsigned char dread1, dread2, dread3, dread4;
/******************************************************************************/
unsigned char ade7858_spi_get_byte(void)
{
   unsigned char i;
   unsigned char value = 0;
   for (i = 0; i < 8; i++)
   {
        ade7858_sclk_output_high();
        delay_us(1); 
        ade7858_sclk_output_low();
        delay_us(1); 
        value <<= 1;
        if (ade7858_sdo_input() != 0) {
            value |= 0x01;
        }
   }    
   return value;
}
/*=======================================================================
FUNCTION NAME: spi_send_byte
PROTOTYPE: void spi_send_byte(unsigned char value)
INPUT: value
OUTPUT: none
DESCRIPTION: 
NOTE: 
=========================================================================*/
void ade7858_spi_send_byte(unsigned char value)
{    
   unsigned char i;
   delay_us(1);
   for (i = 0; i < 8; i++)
   {
      if ((value & 0x80) != 0)
      {
          ade7858_sdi_output_high();          
      }
      else
      {
          ade7858_sdi_output_low();          
      }
      ade7858_sclk_output_low();
      delay_us(1);
      ade7858_sclk_output_high();
      delay_us(1);
      value <<= 1;
   }
  ade7858_sclk_output_high();
  ade7858_sdi_output_high();
  delay_us(1);//nop();
}
/*=======================================================================
FUNCTION NAME: ade7858_spi_setup
PROTOTYPE: void ade7858_spi_setup
INPUT: value
OUTPUT: none
DESCRIPTION: 
NOTE: 
=========================================================================*/
void ade7858_spi_setup(void)
{
    ade7858_sdi_dir_output();
    ade7858_sclk_dir_output();
    ade7858_sdo_dir_input();
    ade7858_sdi_output_high(); //DIN = 0;
    ade7858_sclk_output_high(); //SCLK=0;
    ade7858_cs_dir_output();
    ade7858_cs_high(); 
}
/*=======================================================================
FUNCTION NAME: ade7858_write_1byte
PROTOTYPE: void sade7858_write_1byte
INPUT: value
OUTPUT: none
DESCRIPTION: 
NOTE: 
=========================================================================*/
BOOL ade7858_write_1byte(unsigned int address, unsigned char dwrite) {
    ade7858_cs_low(); //CS=0;   
    //enable to write
    ade7858_spi_send_byte(0x00); //write
    //address
    ade7858_spi_send_byte(address >> 8);
    ade7858_spi_send_byte(address);
    //data
    ade7858_spi_send_byte(dwrite);
    ade7858_cs_high(); //CS = 1; 
    //check data
    if(ade7858_read_1byte(address) != dwrite)
        return FALSE;
    return TRUE;
}
/******************************************************************************/

// Write 7858 thanh ghi 2 bytes dwrite1: MSB, dwrite2: LSB

BOOL ade7858_write_2byte(unsigned int address, unsigned int dwrite) 
{
    unsigned int check_data = 0;
    ade7858_cs_low();
    delay_us(1);
    //enable to write
    ade7858_spi_send_byte(0x00); 
    //address
    ade7858_spi_send_byte(address >> 8);
    ade7858_spi_send_byte(address);
    //data
    ade7858_spi_send_byte(dwrite>>8);
    ade7858_spi_send_byte(dwrite);
    ade7858_cs_high();
    //check data
    check_data = ade7858_read_2byte(address);
    if(check_data != dwrite)
        return FALSE;
    return TRUE;
}
/******************************************************************************/

// Write 7858 thanh ghi 2 bytes dwrite1: MSB, dwrite2: LSB

BOOL ade7858_write_3byte(unsigned int address, unsigned long dwrite) {
    ade7858_cs_low();
    //enable to write
    ade7858_spi_send_byte(0x00); 
    //address
    ade7858_spi_send_byte(address >> 8);
    ade7858_spi_send_byte(address);
    //data
    ade7858_spi_send_byte(dwrite >> 16);
    ade7858_spi_send_byte(dwrite >> 8);
    ade7858_spi_send_byte(dwrite);
    ade7858_cs_high();
    //check data
    if(ade7858_read_3byte(address) != dwrite)
        return FALSE;
    return TRUE;
}
/******************************************************************************/
// Write 7858 thanh ghi 4 bytes dwrite1: MSB, dwrite2: LSB, dwrite2: LSB
BOOL ade7858_write_4byte(unsigned int address, unsigned long dwrite) {
    ade7858_cs_low(); //CS=0;
    //enable to write
    ade7858_spi_send_byte(0x00);
    //address
    ade7858_spi_send_byte(address >> 8);
    ade7858_spi_send_byte(address);
    //data
    ade7858_spi_send_byte(dwrite >> 24);
    ade7858_spi_send_byte(dwrite >> 16);
    ade7858_spi_send_byte(dwrite >> 8);
    ade7858_spi_send_byte(dwrite);
    delay_us(1);
    ade7858_cs_high();
    //check data
    if(ade7858_read_4byte(address) != dwrite)
        return FALSE;
    return TRUE;
}

/******************************************************************************/
void ade7858_read(unsigned int address, unsigned char bytecount) {
    delay_us(1);
    ade7858_cs_low(); //CS = 0;
    delay_us(1);
    //enable to read
    ade7858_spi_send_byte(0x01);//read - 0x01, 0x06 = 0110
    //address
    ade7858_spi_send_byte(address>>8);
    ade7858_spi_send_byte(address);
    switch (bytecount) {
        case 1: // read thanh ghi co 1 byte :  dread3
            dread1 = 0;
            dread2 = 0;
            dread3 = 0;
            dread4 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            break;
        case 2: // read thanh ghi co 2 byte :  dread2:dread3
            dread1 = 0;
            dread2 = 0;
            dread3 = ade7858_spi_get_byte();
            //delay_us(1);
            dread4 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            break;
        case 3: // read thanh ghi co 3 byte :  dread1:dread2:dread3
            dread1 = 0;
            dread2 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            dread3 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            dread4 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            break;
        case 4:
            dread1 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            dread2 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            dread3 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            dread4 = ade7858_spi_get_byte();
            //delay_us(1); //#asm("nop");//delay_us(1);
            break;
    }
    ade7858_cs_high(); 
    ade7858_sdi_output_high(); 
    ade7858_sclk_output_high();
}

uint8_t ade7858_read_1byte(uint16_t address)
{
    uint8_t value = 0;
    ade7858_read(address, 1);
    value = dread4;
    return value;
}

uint16_t ade7858_read_2byte(uint16_t address)
{
    uint16_t value = 0;
    ade7858_read(address, 2);
    value = dread4 + ((uint16_t)dread3 << 8);
    return value;
}

uint32_t ade7858_read_3byte(uint16_t address)
{
    uint32_t value = 0;
    ade7858_read(address, 3);
    value = dread4 + ((uint32_t)dread3 << 8) + ((uint32_t)dread2 << 16);
    return value;
}

uint32_t ade7858_read_4byte(uint16_t address)
{
    uint32_t value = 0;
    ade7858_read(address, 4);
    value = dread4 + ((uint32_t)dread3 << 8) + ((uint32_t)dread2 << 16) + ((uint32_t)dread1 << 24);
    return value;
}
void ade7858_spi_enable()
{
    //toggle CS three time to enable SPI mode
//    ade7858_cs_tgl();
//    delay_ms(1);
//    ade7858_cs_tgl();
//    delay_ms(1);
//    ade7858_cs_tgl();
//    delay_ms(1);
//    ade7858_cs_tgl();
//    delay_ms(1);
    
    ade7858_cs_high();
    delay_ms(1);
    ade7858_cs_low();
    delay_ms(1);
    ade7858_cs_high();
    delay_ms(1);
    ade7858_cs_low();
    delay_ms(1);
    ade7858_cs_high();
    delay_ms(1);
    ade7858_cs_low();
    delay_ms(1);
    
    ade7858_cs_high();
    
    //write dummy value to dummy address of ade7858
    ade7858_write_2byte(ADE7858_DUMMY_ADDR,0x0000);
    delay_ms(1);
    ade7858_write_2byte(ADE7858_DUMMY_ADDR,0x0000);
    delay_ms(1);
    ade7858_write_2byte(ADE7858_DUMMY_ADDR,0x0000);
    delay_ms(1);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_config
//  Created     : 26/02/2015, by thaodtn
//  Description : cau hinh cac thanh ghi hoat dong va do dem
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_config(void) 
{
    uint32_t tmp32;
    BOOL check;
    ade7878_go_normal_mode();
    delay_ms(2);
    ade7858_reset();
    delay_ms(30);
    ade7858_spi_enable();
#ifdef ADE7880_SUPPORT
    //PGA Gain
    check = ade7858_write_2byte(Gain,0x0000); //current Gain = 1, voltage gain = 1, neutral current gain = 1
    if(!check) return FALSE;
#endif
    /* <config> */
    check = ade7858_write_2byte(CONFIG,0x0000); //if use normal CT
    if(!check) return FALSE;
#ifdef USE_EXTREF
    check = ade7858_write_1byte(CONFIG2,0x01); // use external ref 1.2V (ADR)
    if(!check) return FALSE;
#else
    check = ade7858_write_1byte(CONFIG2,0x00); // use internal ref
    if(!check) return FALSE;
#endif
    /* <threshold> */
#if defined(ADE7858_SUPPORT)
    check = ade7858_write_4byte(WTHR1,THR1_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(WTHR0,THR0_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VATHR1,THR1_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VATHR0,THR0_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VARTHR1,THR1_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VARTHR0,THR0_C);
    if(!check) return FALSE;
#elif defined(ADE7880_SUPPORT)
    check = ade7858_write_1byte(WTHR, THR_C);
    if(!check) return FALSE;
    check = ade7858_write_1byte(VATHR, THR_C);
    if(!check) return FALSE;
    check = ade7858_write_1byte(VARTHR, THR_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VLEVEL, VLEVEL_C);
    if(!check) return FALSE;
    check = ade7858_write_4byte(VNOM, VNOM_C);
    if(!check) return FALSE;
#endif
    /* <RAM protect> */
    //ade7858_write_1byte(0xE7FE,0xAD);
    //ade7858_write_1byte(0xE7E3,0x80);
    /* <mode> */
#ifdef METER_3WIRE
    check = ade7858_write_1byte(ACCMODE,0x10);//bit[5:4] => CONSEL = 01;
#else
    check = ade7858_write_1byte(ACCMODE, ACCMODE_C);
#endif
    if(!check) return FALSE;
    check = ade7858_write_2byte(COMPMODE,COMPMODE_C);
    if(!check) return FALSE;
    check = ade7858_write_1byte(MMODE,0x16);
    if(!check) return FALSE;
    check = ade7858_write_1byte(LCYCMODE,0x78);
    if(!check) return FALSE;
    ade7858_angle_measure_select(ANGLE_UU); 
    /* <led pulse out> */
#if defined(METER_THAILAN) || defined(_DT03MRF_PCB_PIC_AS8_S_V10_) || defined(_DT03MRF_PCB_PIC_AMS_S_V10_)
    tmp32 = CF1_REACTIVE + CF2_ACTIVE + CF3_APPARENT;
#else
    tmp32 = CF1_ACTIVE + CF2_REACTIVE + CF3_APPARENT;
#endif
    check = ade7858_write_2byte(CFMODE,tmp32);
    if(!check) return FALSE;
#if defined(_ROUTER3P_) || defined(DT03M_5_10A)
    check = ade7858_write_2byte(CF1DEN ,METER_CONST_5000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF2DEN ,METER_CONST_5000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF3DEN ,METER_CONST_5000);
    if(!check) return FALSE;
#elif defined(DT03M_10_100A)
    check = ade7858_write_2byte(CF1DEN ,METER_CONST_1000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF2DEN ,METER_CONST_1000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF3DEN ,METER_CONST_1000);
    if(!check) return FALSE;
#elif defined(DT03M_1_12A)
    check = ade7858_write_2byte(CF1DEN ,METER_CONST_25000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF2DEN ,METER_CONST_25000);
    if(!check) return FALSE;
    check = ade7858_write_2byte(CF3DEN ,METER_CONST_25000);
    if(!check) return FALSE;
#endif
    /* <calib value> */
    check =  ade7858_set_calib_value_phaseA(FALSE);
    if(!check) return FALSE;
    check =  ade7858_set_calib_value_phaseB(FALSE);
    if(!check) return FALSE;
    check =  ade7858_set_calib_value_phaseC(FALSE);
    if(!check) return FALSE;
    check =  ade7858_set_calib_value_phaseN(FALSE);
    if(!check) return FALSE;
    /* <start DSP> */     
    check =  ade7858_write_2byte(RUN, START);
    if(!check) return FALSE;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_load_calib_value
//  Created     : 16/05/2016, by tuantm
//  Description : load all thong so can chinh tu eeprom
//  Input		: 
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_load_calib_value(void)
{
    //phase A
    ade7858.calib.pA.i_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.i_gain = AIGAIN_C; //default value
    ade7858.calib.pA.v_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.v_gain = AVGAIN_C; //default value
    ade7858.calib.pA.irms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.irms_gain = AIRMSOS_C; //default value
    ade7858.calib.pA.vrms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.vrms_gain = AVRMSOS_C; //default value
#if defined(ADE7858_SUPPORT)
    ade7858.calib.pA.watt_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.watt_gain = AWGAIN_C; //default value
    ade7858.calib.pA.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AVARGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.var_gain = AVARGAIN_C; //default value
    ade7858.calib.pA.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AVAGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.va_gain = AVAGAIN_C; //default value
#elif defined(ADE7880_SUPPORT)
    ade7858.calib.pA.p_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_AWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.p_gain = AWGAIN_C; //default value    
#endif
    ade7858.calib.pA.phase = (int16_t)eeprom_read_2byte(EEPROM_ADE78XX_CALIB_APHASE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.phase = APHASE_C; //default value
    ade7858.calib.pA.dips_scale = eeprom_read_4byte(EEPROM_ADE78XX_CALIB_A_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pA.dips_scale = A_P_SCALE_C;//default value
    restart_watchdog();
    //phase B
    ade7858.calib.pB.i_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.i_gain = BIGAIN_C; //default value
    ade7858.calib.pB.v_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.v_gain = BVGAIN_C; //default value
    ade7858.calib.pB.irms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.irms_gain = BIRMSOS_C; //default value
    ade7858.calib.pB.vrms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.vrms_gain = BVRMSOS_C; //default value
#if defined(ADE7858_SUPPORT)    
    ade7858.calib.pB.watt_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.watt_gain = BWGAIN_C; //default value
    ade7858.calib.pB.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BVARGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.var_gain = BVARGAIN_C; //default value
    ade7858.calib.pB.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BVAGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.va_gain = BVAGAIN_C; //default value
#elif defined(ADE7880_SUPPORT)
    ade7858.calib.pB.p_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_BWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.p_gain = BWGAIN_C; //default value
#endif    
    ade7858.calib.pB.phase = (int16_t)eeprom_read_2byte(EEPROM_ADE78XX_CALIB_BPHASE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.phase = BPHASE_C; //default value
    ade7858.calib.pB.dips_scale = eeprom_read_4byte(EEPROM_ADE78XX_CALIB_B_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pB.dips_scale = B_P_SCALE_C;//default value
    restart_watchdog();
    //phase C
    ade7858.calib.pC.i_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.i_gain = CIGAIN_C; //default value
    ade7858.calib.pC.v_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.v_gain = CVGAIN_C; //default value
    ade7858.calib.pC.irms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.irms_gain = CIRMSOS_C; //default value
    ade7858.calib.pC.vrms_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.vrms_gain = CVRMSOS_C; //default value
#if defined(ADE7858_SUPPORT)      
    ade7858.calib.pC.watt_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.watt_gain = CWGAIN_C; //default value
    ade7858.calib.pC.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CVARGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.var_gain = CVARGAIN_C; //default value
    ade7858.calib.pC.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CVAGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.va_gain = CVAGAIN_C; //default value
#elif defined(ADE7880_SUPPORT)
    ade7858.calib.pC.p_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE78XX_CALIB_CWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.p_gain = CWGAIN_C; //default value
#endif    
    ade7858.calib.pC.phase = (int16_t)eeprom_read_2byte(EEPROM_ADE78XX_CALIB_CPHASE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.phase = CPHASE_C; //default value
    ade7858.calib.pC.dips_scale = eeprom_read_4byte(EEPROM_ADE78XX_CALIB_C_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pC.dips_scale = C_P_SCALE_C;//default value
    restart_watchdog();
    //phase N
    ade7858.calib.pN.i_gain = eeprom_read_4byte(EEPROM_ADE78XX_CALIB_NIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pN.i_gain = NIGAIN_C;
    ade7858.calib.pN.irms_gain = eeprom_read_4byte(EEPROM_ADE78XX_CALIB_NIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7858.calib.pN.irms_gain = NIRMSOS_C;
    restart_watchdog();
    if(emeter.flags.eeprom_error) return FALSE;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_set_calib_value_phaseA
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_set_calib_value_phaseA(BOOL wr)
{
    BOOL check;
    //current => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AIGAIN,ade7858.calib.pA.i_gain,EEPROM_1);
    //voltage => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AVGAIN,ade7858.calib.pA.v_gain,EEPROM_1);
    //rms current offset
    check = ade7858_write_4byte(AIRMSOS, ade7858.calib.pA.irms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AIRMSGAIN,ade7858.calib.pA.irms_gain,EEPROM_1);
    //rms voltage offset
    check = ade7858_write_4byte(AVRMSOS, ade7858.calib.pA.vrms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AVRMSGAIN,ade7858.calib.pA.vrms_gain,EEPROM_1);
#if defined(ADE7858_SUPPORT)
    //watt
    check = ade7858_write_4byte(AWGAIN, ade7858.calib.pA.watt_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AWGAIN,ade7858.calib.pA.watt_gain,EEPROM_1);
    //var
    check = ade7858_write_4byte(AVARGAIN, ade7858.calib.pA.var_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AVARGAIN,ade7858.calib.pA.var_gain,EEPROM_1);
    //va
    check = ade7858_write_4byte(AVAGAIN, ade7858.calib.pA.va_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AVAGAIN,ade7858.calib.pA.va_gain,EEPROM_1);
#elif defined(ADE7880_SUPPORT)
    check = ade7858_write_4byte(APGAIN, ade7858.calib.pA.p_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_AWGAIN,ade7858.calib.pA.p_gain,EEPROM_1);
#endif
     //phase
    check = ade7858_write_2byte(APHCAL,ade7858.calib.pA.phase);
    if(!check) return FALSE;
    if(wr) eeprom_write_2byte(EEPROM_ADE78XX_CALIB_APHASE,ade7858.calib.pA.phase,EEPROM_1);
    //P display scale
    if(ade7858.calib.pA.dips_scale==0) ade7858.calib.pA.dips_scale = A_P_SCALE_C;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_A_P_SCALE,ade7858.calib.pA.dips_scale,EEPROM_1);
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_set_calib_value_phaseB
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_set_calib_value_phaseB(BOOL wr)
{
    BOOL check;
    //current => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BIGAIN,ade7858.calib.pB.i_gain,EEPROM_1);
    //voltage => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BVGAIN,ade7858.calib.pB.v_gain,EEPROM_1);
    //rms current offset
    check = ade7858_write_4byte(BIRMSOS, ade7858.calib.pB.irms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BIRMSGAIN,ade7858.calib.pB.irms_gain,EEPROM_1);
    //rms voltage offset
    check = ade7858_write_4byte(BVRMSOS, ade7858.calib.pB.vrms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BVRMSGAIN,ade7858.calib.pB.vrms_gain,EEPROM_1);
#if defined(ADE7858_SUPPORT)    
    //watt
    check = ade7858_write_4byte(BWGAIN, ade7858.calib.pB.watt_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BWGAIN,ade7858.calib.pB.watt_gain,EEPROM_1);
    //var
    check = ade7858_write_4byte(BVARGAIN, ade7858.calib.pB.var_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BVARGAIN,ade7858.calib.pB.var_gain,EEPROM_1);
    //va
    check = ade7858_write_4byte(BVAGAIN, ade7858.calib.pB.va_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BVAGAIN,ade7858.calib.pB.va_gain,EEPROM_1);
#elif defined(ADE7880_SUPPORT)
    check = ade7858_write_4byte(BPGAIN, ade7858.calib.pB.p_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_BWGAIN,ade7858.calib.pB.p_gain,EEPROM_1);
#endif    
     //phase
    check = ade7858_write_2byte(BPHCAL,ade7858.calib.pB.phase);
    if(!check) return FALSE;
    if(wr) eeprom_write_2byte(EEPROM_ADE78XX_CALIB_BPHASE,ade7858.calib.pB.phase,EEPROM_1);
    //P display scale
    if(ade7858.calib.pB.dips_scale==0) ade7858.calib.pB.dips_scale = B_P_SCALE_C;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_B_P_SCALE,ade7858.calib.pB.dips_scale,EEPROM_1);
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_set_calib_value_phaseC
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_set_calib_value_phaseC(BOOL wr)
{
    BOOL check;
    //current => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CIGAIN,ade7858.calib.pC.i_gain,EEPROM_1);
    //voltage => ade nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CVGAIN,ade7858.calib.pC.v_gain,EEPROM_1);
    //rms current offset
    check = ade7858_write_4byte(CIRMSOS, ade7858.calib.pC.irms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CIRMSGAIN,ade7858.calib.pC.irms_gain,EEPROM_1);
    //rms voltage offset
    check = ade7858_write_4byte(CVRMSOS, ade7858.calib.pC.vrms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CVRMSGAIN,ade7858.calib.pC.vrms_gain,EEPROM_1);
#if defined(ADE7858_SUPPORT)     
    //watt
    check = ade7858_write_4byte(CWGAIN, ade7858.calib.pC.watt_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CWGAIN,ade7858.calib.pC.watt_gain,EEPROM_1);
    //var
    check = ade7858_write_4byte(CVARGAIN, ade7858.calib.pC.var_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CVARGAIN,ade7858.calib.pC.var_gain,EEPROM_1);
    //va
    check = ade7858_write_4byte(CVAGAIN, ade7858.calib.pC.va_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CVAGAIN,ade7858.calib.pC.va_gain,EEPROM_1);
#elif defined(ADE7880_SUPPORT)
    check = ade7858_write_4byte(CPGAIN, ade7858.calib.pC.p_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_CWGAIN,ade7858.calib.pC.p_gain,EEPROM_1);
#endif    
     //phase
    check = ade7858_write_2byte(CPHCAL,ade7858.calib.pC.phase);
    if(!check) return FALSE;
    if(wr) eeprom_write_2byte(EEPROM_ADE78XX_CALIB_CPHASE,ade7858.calib.pC.phase,EEPROM_1);
     //P display scale
    if(ade7858.calib.pC.dips_scale==0) ade7858.calib.pC.dips_scale = C_P_SCALE_C;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_C_P_SCALE,ade7858.calib.pC.dips_scale,EEPROM_1);
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_set_calib_value_phaseN
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7858_set_calib_value_phaseN(BOOL wr)
{
    BOOL check;
    //current => nothing
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_NIGAIN,ade7858.calib.pN.i_gain,EEPROM_1);
    //rms current offset
    check = ade7858_write_4byte(NIRMSOS, ade7858.calib.pN.irms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_write_4byte(EEPROM_ADE78XX_CALIB_NIRMSGAIN,ade7858.calib.pN.irms_gain,EEPROM_1);
    return TRUE;
}
void ade7858_get_calib_value_phaseA(void)
{
    //current => nothing
    //voltage => nothing
    restart_watchdog();
    mcu_watchdog_toggle();
    //rms current offset
    ade7858.calib.pA.irms_gain = ade7858_read_4byte(AIRMSOS);
    //rms voltage offset
    ade7858.calib.pA.vrms_gain = ade7858_read_4byte(AVRMSOS);    
#if defined(ADE7858_SUPPORT)
    //watt
    ade7858.calib.pA.watt_gain = ade7858_read_4byte(AWGAIN);
    //var
    ade7858.calib.pA.var_gain = ade7858_read_4byte(AVARGAIN);
    //va
    ade7858.calib.pA.va_gain = ade7858_read_4byte(AVAGAIN);
#elif defined(ADE7880_SUPPORT)
    ade7858.calib.pA.p_gain = ade7858_read_4byte(APGAIN);
#endif
     //phase
    ade7858.calib.pA.phase = ade7858_read_2byte(APHCAL);
    restart_watchdog();
    mcu_watchdog_toggle();
    //dummy
}
void ade7858_get_calib_value_phaseB(void)
{
    //current => nothing
    //voltage => nothing
    restart_watchdog();
    mcu_watchdog_toggle();
    //rms current offset
    ade7858.calib.pB.irms_gain = ade7858_read_4byte(BIRMSOS);
    //rms voltage offset
    ade7858.calib.pB.vrms_gain = ade7858_read_4byte(BVRMSOS);
#if defined(ADE7858_SUPPORT)    
    //watt
    ade7858.calib.pB.watt_gain = ade7858_read_4byte(BWGAIN);
    //var
    ade7858.calib.pB.var_gain = ade7858_read_4byte(BVARGAIN);
    restart_watchdog();
    //va
    ade7858.calib.pB.va_gain = ade7858_read_4byte(BVAGAIN);
#elif defined(ADE7880_SUPPORT)  
    ade7858.calib.pB.p_gain = ade7858_read_4byte(BPGAIN);
#endif    
     //phase
    ade7858.calib.pB.phase = ade7858_read_2byte(BPHCAL);
    restart_watchdog();
    mcu_watchdog_toggle();
    //dummy
}
void ade7858_get_calib_value_phaseC(void)
{
    //current => nothing
    //voltage => nothing
    restart_watchdog();
    mcu_watchdog_toggle();
    //rms current offset
    ade7858.calib.pC.irms_gain = ade7858_read_4byte(CIRMSOS);
    //rms voltage offset
    ade7858.calib.pC.vrms_gain = ade7858_read_4byte(CVRMSOS);
#if defined(ADE7858_SUPPORT)    
    //watt
    ade7858.calib.pC.watt_gain = ade7858_read_4byte(CWGAIN);
    //var
    ade7858.calib.pC.var_gain = ade7858_read_4byte(CVARGAIN);
    //va
    ade7858.calib.pC.va_gain = ade7858_read_4byte(CVAGAIN);
#elif defined(ADE7880_SUPPORT) 
    ade7858.calib.pC.p_gain = ade7858_read_4byte(CPGAIN);
#endif    
     //phase
    ade7858.calib.pC.phase = ade7858_read_2byte(CPHCAL);
    //dummy
    restart_watchdog();
    mcu_watchdog_toggle();
}
void ade7858_get_calib_value_phaseN(void)
{
	ade7858.calib.pN.irms_gain = ade7858_read_4byte(NIRMSOS);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : void ade7858_calibration(char ph) 
//  Created     : 26/02/2015, by thaodtn
//  Description : can chinh tung pha
//  Input		: ch: phase duoc can chinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void ade7858_calibration(unsigned char ph) {
//todo
}
void ade7858_reset()
{
    ade_reset_dir_output();
    ade_reset_output_low();
    delay_ms(5);
    ade_reset_output_high();
}
/******************************************************************************/
void ade7858_init(void) {
    BOOL check;
    ade7858_spi_setup();   
    ade7858_pm0_dir_output();
    ade7858_pm1_dir_output();
    _ade_active_pulse_dir_input();
    _ade_reactive_pulse_dir_input();
    _ade_apparent_pulse_dir_input();
    check = ade7858_config();
    if(check == FALSE)
        ade7858.flags.register_error = TRUE;
    else
        ade7858.flags.register_error = FALSE;
    //ngat xung do dem
    _ade_active_pulse_interrupt_select();
    _ade_active_pulse_interrupt_high_to_low();
    _ade_active_pulse_interrupt_flag_clear();
    _ade_active_pulse_interrupt_enable();
    
    _ade_reactive_pulse_interrupt_select();
    _ade_reactive_pulse_interrupt_high_to_low();
    _ade_reactive_pulse_interrupt_flag_clear();
    _ade_reactive_pulse_interrupt_enable();
    
    _ade_apparent_pulse_interrupt_select();
    _ade_apparent_pulse_interrupt_high_to_low();
    _ade_apparent_pulse_interrupt_flag_clear();
    _ade_apparent_pulse_interrupt_enable();
}
////////////////////////////////////////////////////////////////////////////////
void ade7878_go_normal_mode() {
    ade7858_pm0_output_high();
    ade7858_pm1_output_low();
}
void ade7878_go_reduce_power_mode() {
    ade7858_pm0_output_low();
    ade7858_pm1_output_low();
}
void ade7878_go_low_power_mode() {
    ade7858_pm0_output_low();
    ade7858_pm1_output_high();
}
void ade7878_go_sleep_mode() {
    ade7858_pm0_output_high();
    ade7858_pm1_output_high();
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : 
//  Created     : //, by thaodtn
//  Description : 
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#ifdef debug
void measure_task(void) {
    if (emeter.flags.active_pulse == TRUE) {
        emeter.flags.active_pulse = FALSE;
        check_reverse_active_power();
        //if (emeter.active.power >= 0) //Watt thuan
        if (emeter.flags.reverse_active_power == FALSE) {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('+');
            uart_transmit_byte('A');
#endif
            emeter.active.forward.pulse_count++;
            if (emeter.active.forward.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.active.forward.pulse_count = 0;
                emeter.active.forward.value++;
                if (emeter.active.forward.value > KWH_WRAP) {
                    emeter.active.forward.value = 0;
                }
                //luu eeprom
                if (emeter.active.forward.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        } else // Watt nguoc
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('-');
            uart_transmit_byte('A');
#endif
            emeter.active.reverse.pulse_count++;
            if (emeter.active.reverse.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.active.reverse.pulse_count = 0;
                emeter.active.reverse.value++;
                if (emeter.active.reverse.value > KWH_WRAP) {
                    emeter.active.reverse.value = 0;
                }
                //luu eeprom
                if (emeter.active.reverse.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        }

    }
    if (emeter.flags.reactive_pulse == TRUE) {
        emeter.flags.reactive_pulse = FALSE;
        check_reverse_reactive_power();
        //if (emeter.reactive.power >= 0) // var thuan
        if (emeter.flags.reverse_reactive_power == FALSE) // Q > 0
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('+');
            uart_transmit_byte('R');
#endif
            emeter.reactive.forward.pulse_count++;
            if (emeter.reactive.forward.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.reactive.forward.pulse_count = 0;
                emeter.reactive.forward.value++;
                if (emeter.reactive.forward.value > KWH_WRAP) {
                    emeter.reactive.forward.value = 0;
                }
                if (emeter.flags.reverse_active_power == FALSE) // P > 0
                {
                    emeter.reactive.q1++;
                    if (emeter.reactive.q1 > KWH_WRAP) {
                        emeter.reactive.q1 = 0;
                    }
                } else // P < 0
                {
                    emeter.reactive.q2++;
                    if (emeter.reactive.q2 > KWH_WRAP) {
                        emeter.reactive.q2 = 0;
                    }
                }
                //luu eeprom
                if (emeter.reactive.forward.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }

        } else //var nguoc
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('-');
            uart_transmit_byte('R');
#endif
            emeter.reactive.reverse.pulse_count++;
            if (emeter.reactive.reverse.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.reactive.reverse.pulse_count = 0;
                emeter.reactive.reverse.value++;
                if (emeter.reactive.reverse.value > KWH_WRAP) {
                    emeter.reactive.reverse.value = 0;
                }
                if (emeter.flags.reverse_active_power == FALSE) // P > 0
                {
                    emeter.reactive.q4++;
                    if (emeter.reactive.q4 > KWH_WRAP) {
                        emeter.reactive.q4 = 0;
                    }
                } else // P < 0
                {
                    emeter.reactive.q3++;
                    if (emeter.reactive.q3 > KWH_WRAP) {
                        emeter.reactive.q3 = 0;
                    }
                }
                //luu eeprom
                if (emeter.reactive.reverse.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        }
    }
}
#endif
//****************************************************************

void measure_vrms(void) {
    long tmp;
    float tmp2;
    //tmp  = ade7858_read_3byte(AVRMS);
    //tmp2 = (long)(AVGAIN_C/CAL_POW);
    //ade7858.measure.vrmsA = (long)(tmp*(1+tmp2));
    // <voltage>
    tmp  = ade7858_read_3byte(AVRMS);
    tmp2 = (float) ((float)AVGAIN_C/(float)CAL_POW);
    ade7858.measure.vrmsA = (uint32_t)(tmp*(1+tmp2));
    
    tmp  = ade7858_read_3byte(BVRMS);
    tmp2 = (float) ((float)BVGAIN_C/(float)CAL_POW);
    ade7858.measure.vrmsB = (uint32_t)(tmp*(1+tmp2));
    
    tmp  = ade7858_read_3byte(CVRMS);
    tmp2 = (float) ((float)CVGAIN_C/(float)CAL_POW);
    ade7858.measure.vrmsC = (uint32_t)(tmp*(1+tmp2));
}

void measure_irms(void) {
    long tmp;
    float tmp2;
        // <current>
    tmp  = ade7858_read_3byte(AIRMS);
    tmp2 = (float) ((float)AIGAIN_C/(float)CAL_POW);
    ade7858.measure.irmsA = (uint32_t)(tmp*(1+tmp2));
    
    tmp  = ade7858_read_3byte(BIRMS);
    tmp2 = (float) ((float)BIGAIN_C/(float)CAL_POW);
    ade7858.measure.irmsB = (uint32_t)(tmp*(1+tmp2));
    
    tmp  = ade7858_read_3byte(CIRMS);
    tmp2 = (float) ((float)CIGAIN_C/(float)CAL_POW);
    ade7858.measure.irmsC = (uint32_t)(tmp*(1+tmp2));
}

void measure_activepower(void) {
    ade7858.measure.active_powerA = ade7858_read_4byte(AWATT);
    ade7858.measure.active_powerB = ade7858_read_4byte(BWATT);
    ade7858.measure.active_powerC = ade7858_read_4byte(CWATT);
}

void measure_reactivepower(void) {
#if defined(ADE7858_SUPPORT)
    ade7858.measure.reactive_powerA = ade7858_read_4byte(AVAR);
    ade7858.measure.reactive_powerB = ade7858_read_4byte(BVAR);
    ade7858.measure.reactive_powerC = ade7858_read_4byte(CVAR);
#elif defined(ADE7880_SUPPORT)
    ade7858.measure.reactive_powerA = ade7858_read_4byte(AFVAR);
    ade7858.measure.reactive_powerB = ade7858_read_4byte(BFVAR);
    ade7858.measure.reactive_powerC = ade7858_read_4byte(CFVAR);
#endif
}
void measure_apparentpower(void) {
    ade7858.measure.apparent_powerA = ade7858_read_4byte(AVA);
    ade7858.measure.apparent_powerB = ade7858_read_4byte(BVA);
    ade7858.measure.apparent_powerC = ade7858_read_4byte(CVA);
}

void measure_freq(void) {
    uint16_t tmp;
#if defined(ADE7858_SUPPORT)
    tmp = ade7858_read_2byte(PERIOD);
    ade7858.measure.freq = ((float)DIGITAL_FILTER_CLK / (float)(tmp + 1))*100;
#elif defined(ADE7880_SUPPORT)
    tmp = ade7858_read_2byte(APERIOD);
    ade7858.measure.freqA = ((float)DIGITAL_FILTER_CLK / (float)(tmp + 1))*100;
    tmp = ade7858_read_2byte(BPERIOD);
    ade7858.measure.freqB = ((float)DIGITAL_FILTER_CLK / (float)(tmp + 1))*100;
    tmp = ade7858_read_2byte(CPERIOD);
    ade7858.measure.freqC = ((float)DIGITAL_FILTER_CLK / (float)(tmp + 1))*100;
#endif
}
void measure_power_factor(void)
{
    ade7858.measure.power_factorA = ((float)ade7858.measure.active_powerA / (float)ade7858.measure.apparent_powerA)*1000;
    ade7858.measure.power_factorB = ((float)ade7858.measure.active_powerB / (float)ade7858.measure.apparent_powerB)*1000;
    ade7858.measure.power_factorC = ((float)ade7858.measure.active_powerC / (float)ade7858.measure.apparent_powerC)*1000;
}
void measure_line_cycles(void) {
    ade7858.measure.line_cycles =  ade7858_read_2byte(LINECYC);
}

void measure_temperature(void) {    
}

void check_reverse_active_power(void) {
    int32_t total;
    total = chan[0].readings.active_power + chan[1].readings.active_power
            + chan[2].readings.active_power;
    if(total >= 0)
        emeter.flags.reverse_active_power = FALSE;
    else
        emeter.flags.reverse_active_power = TRUE;
}

void check_reverse_reactive_power(void) {	
    int32_t total;
    total = chan[0].readings.reactive_power + chan[1].readings.reactive_power
            + chan[2].readings.reactive_power;
    if(total >= 0)
        emeter.flags.reverse_reactive_power = FALSE;
    else
        emeter.flags.reverse_reactive_power = TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_struct_calib
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7858_struct_calib(uint16_t address)
{
    switch(address)
    {
        // IGAIN
        case AIGAIN:
            ade7858.calib.pA.i_gain = ade7858.reg_uint32;
            break;                         
        case BIGAIN:
            ade7858.calib.pB.i_gain = ade7858.reg_uint32;
            break;    
        case CIGAIN:
            ade7858.calib.pC.i_gain = ade7858.reg_uint32;
            break; 
        case NIGAIN:
            ade7858.calib.pN.i_gain = ade7858.reg_uint32;
            break;                     
        // v_gain
        case AVGAIN:
            ade7858.calib.pA.v_gain = ade7858.reg_uint32;
            break;                         
        case BVGAIN:
            ade7858.calib.pB.v_gain = ade7858.reg_uint32;
            break;    
        case CVGAIN:
            ade7858.calib.pC.v_gain = ade7858.reg_uint32;
            break;   
        // irms_gain
        case AIRMSOS:
            ade7858.calib.pA.irms_gain = ade7858.reg_uint32;
            break;                         
        case BIRMSOS:
            ade7858.calib.pB.irms_gain = ade7858.reg_uint32;
            break;    
        case CIRMSOS:
            ade7858.calib.pC.irms_gain = ade7858.reg_uint32;
            break; 
        case NIRMSOS:
            ade7858.calib.pN.irms_gain = ade7858.reg_uint32;
            break;    
        // vrms_gain
        case AVRMSOS:
            ade7858.calib.pA.vrms_gain = ade7858.reg_uint32;
            break;                         
        case BVRMSOS:
            ade7858.calib.pB.vrms_gain = ade7858.reg_uint32;
            break;    
        case CVRMSOS:
            ade7858.calib.pC.vrms_gain = ade7858.reg_uint32;
            break;  
#if defined(ADE7858_SUPPORT)
        //watt_gain
        case AWGAIN:
            ade7858.calib.pA.watt_gain = ade7858.reg_uint32;
            break;                         
        case BWGAIN:
            ade7858.calib.pB.watt_gain = ade7858.reg_uint32;
            break;    
        case CWGAIN:
            ade7858.calib.pC.watt_gain = ade7858.reg_uint32;
            break;  
        // var_gain
        case AVARGAIN:
            ade7858.calib.pA.var_gain = ade7858.reg_uint32;
            break;                         
        case BVARGAIN:
            ade7858.calib.pB.var_gain = ade7858.reg_uint32;
            break;    
        case CVARGAIN:
            ade7858.calib.pC.var_gain = ade7858.reg_uint32;
            break; 
        //va_gain
        case AVAGAIN:
            ade7858.calib.pA.va_gain = ade7858.reg_uint32;
            break;                         
        case BVAGAIN:
            ade7858.calib.pB.va_gain = ade7858.reg_uint32;
            break;    
        case CVAGAIN:
            ade7858.calib.pC.va_gain = ade7858.reg_uint32;
            break; 
#elif defined(ADE7880_SUPPORT)
        case APGAIN:
            ade7858.calib.pA.p_gain = ade7858.reg_uint32;
            break;                         
        case BPGAIN:
            ade7858.calib.pB.p_gain = ade7858.reg_uint32;
            break;    
        case CPGAIN:
            ade7858.calib.pC.p_gain = ade7858.reg_uint32;
            break;  
#endif
        //phase
        case APHCAL:
            ade7858.calib.pA.phase = ade7858.reg_uint16;
            break;
        case BPHCAL:
            ade7858.calib.pB.phase = ade7858.reg_uint16;
            break;   
        case CPHCAL:
            ade7858.calib.pC.phase = ade7858.reg_uint16;
            break;                
    }
}
void ade7858_angle_measure_select(uint8_t type)
{
    uint16_t d_write = COMPMODE_C;
    switch(type)
    {
        case ANGLE_UU: //Vol-Vol
            d_write set (BIT_9);
            break;
        case ANGLE_II: //Current - current
            d_write set (BIT_10);
        case ANGLE_UI: //default: Vol-current
        default:
            break;
    }
    if(ade7858_write_2byte(COMPMODE,d_write))
        ade7858.angle_mode = type;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_process_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
                //68-len-cmd-subcmd-address[2]-data[4]-crc[1]-16 (write)
                //68-len-cmd-subcmd-address[2]-crc[1]-16 (read)
////////////////////////////////////////////////////////////////////////////////
void ade7858_write_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7858.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length)   
    comm_add_16bit(ade7858.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7858.reg_uint8  = comm_get_8bit(6,module);//_dtp_get_8bit       
            ade7858_write_1byte(ade7858.addr_reg,ade7858.reg_uint8);
            ade7858.reg_uint8 = 0;
            ade7858.reg_uint8 = ade7858_read_1byte(ade7858.addr_reg);
            comm_add_8bit(ade7858.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7858.reg_uint16 = comm_get_16bit(6,module);//_dtp_get_16bit  
            ade7858_write_2byte(ade7858.addr_reg,ade7858.reg_uint16);                        
            ade7858.reg_uint16 = 0;
            ade7858.reg_uint16 = ade7858_read_2byte(ade7858.addr_reg);    
            ade7858_struct_calib(ade7858.addr_reg);
            comm_add_16bit(ade7858.reg_uint16,module);//dtp_add_16bit           
            break;
        case 3:
        case 4:
            ade7858.reg_uint32 = comm_get_32bit(6,module);     // chu y : function nay cua thaodtn sai y do //_dtp_get_32bit                            
            ade7858_write_4byte(ade7858.addr_reg,ade7858.reg_uint32);
            ade7858.reg_uint32 = 0;
            ade7858.reg_uint32 = ade7858_read_4byte(ade7858.addr_reg);                         
            ade7858_struct_calib(ade7858.addr_reg);
            comm_add_32bit(ade7858.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_read_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : doc 1 thanh ghi ADE7858 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7858_read_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7858.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length) 
    comm_add_16bit(ade7858.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7858.reg_uint8 = ade7858_read_1byte(ade7858.addr_reg);   
            comm_add_8bit(ade7858.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7858.reg_uint16 = ade7858_read_2byte(ade7858.addr_reg);             
            ade7858_struct_calib(ade7858.addr_reg);
            comm_add_16bit(ade7858.reg_uint16,module);//dtp_add_16bit
        case 3:
        case 4:
            ade7858.reg_uint32 = ade7858_read_4byte(ade7858.addr_reg);                         
            ade7858_struct_calib(ade7858.addr_reg);
            comm_add_32bit(ade7858.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(_DT03M_RF_)
BOOL ade7858_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length)
{
    comm_add_8bit(phase,module);//dtp_add_8bit
    comm_add_8bit(length,module);//dtp_add_8bit
#elif defined(_ROUTER3P_)
BOOL ade7858_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module)
{
    uint8_t   phase = comm_get_8bit(3,module); //so byte//_dtp_get_8bit
    uint16_t  length = comm_get_8bit(4,module); //so byte//dtp_get_8bit
    comm_add_8bit(phase,module);//dtp_add_8bit
#endif
    BOOL result = TRUE;
// Tinh do dai du lieu de biet khung dung hay sai.
    int index_data = 0;
#if defined(_DT03M_RF_)
    #if defined(COMM_PROCESS_EMEC_CMD_SUPPORT)
    index_data = 7;
    #elif defined(COMM_PROCESS_DTP_CMD_SUPPORT)
    index_data = 6;
    #endif
#elif defined(_ROUTER3P_)
    index_data = 5;
#endif
    
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
            //length = sizeof(ade7858.calib);         // Nho kiem tra length
            if (length != sizeof(ade7858.calib)) return FALSE;      
            comm_get_buffer((uint8_t *)&ade7858.calib, index_data, 0, length, module); //_dtp_get_buffer
            if (ade7858_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
            if (ade7858_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;
            if (ade7858_set_calib_value_phaseC(TRUE) == FALSE) return FALSE;
            if (ade7858_set_calib_value_phaseN(TRUE) == FALSE) return FALSE;            
            ade7858_get_calib_value_phaseA();
            ade7858_get_calib_value_phaseB();
            ade7858_get_calib_value_phaseC();
            ade7858_get_calib_value_phaseN();
            // quanhdt Luu eerom
            comm_add_buffer((uint8_t *)&ade7858.calib,length,module);//dtp_add_buffer
            break;
        case PHASE_A:         // Phase A
            //length = sizeof(ade7858.calib.pA);         // Nho kiem tra length
            if (length != sizeof(ade7858.calib.pA)) return FALSE;        
            comm_get_buffer((uint8_t *)&ade7858.calib.pA, index_data, 0, length, module);//_dtp_get_buffer
            if (ade7858_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
            ade7858_get_calib_value_phaseA();
            // quanhdt Luu eerom            
            comm_add_buffer((uint8_t *)&ade7858.calib.pA,length,module);//dtp_add_buffer
            break;
        case PHASE_B:         // Phase B
            //length = sizeof(ade7858.calib.pB);         // Nho kiem tra length
            if (length != sizeof(ade7858.calib.pB)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7858.calib.pB, index_data, 0, length, module);//_dtp_get_buffer
            if (ade7858_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;
            ade7858_get_calib_value_phaseB();
            // quanhdt Luu eerom            
            comm_add_buffer((uint8_t *)&ade7858.calib.pB,sizeof(ade7858.calib.pB),module);//dtp_add_buffer
            break;
        case PHASE_C:         // Phase C
            //length = sizeof(ade7858.calib.pC);         // Nho kiem tra length
            if (length != sizeof(ade7858.calib.pC)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7858.calib.pC, index_data, 0, length, module);//_dtp_get_buffer
            if (ade7858_set_calib_value_phaseC(TRUE) == FALSE) return FALSE;
            ade7858_get_calib_value_phaseC();
            // quanhdt Luu eerom            
            comm_add_buffer((uint8_t *)&ade7858.calib.pC,sizeof(ade7858.calib.pC),module);//dtp_add_buffer
            break;
        case NEUTRAL:
            //length = sizeof(ade7858.calib.pN);         // Nho kiem tra length
            if (length != sizeof(ade7858.calib.pN)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7858.calib.pN, index_data, 0, length, module); //_dtp_get_buffer
            if (ade7858_set_calib_value_phaseN(TRUE) == FALSE) return FALSE;
            ade7858_get_calib_value_phaseN();
            // quanhdt Luu eerom
            comm_add_buffer((uint8_t *)&ade7858.calib.pN,sizeof(ade7858.calib.pN),module);//dtp_add_buffer
            break;
        default:
            return FALSE;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(_DT03M_RF_)
BOOL ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase)
{
#elif defined(_ROUTER3P_)
BOOL ade7858_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module)
{
uint8_t   phase = comm_get_8bit(3,module); //so byte    //_dtp_get_8bit
#endif
    comm_add_8bit(phase,module);//dtp_add_8bit
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
            ade7858_get_calib_value_phaseA();
            ade7858_get_calib_value_phaseB();
            ade7858_get_calib_value_phaseC();
            ade7858_get_calib_value_phaseN();            
            comm_add_buffer((uint8_t *)&ade7858.calib,sizeof(ade7858.calib),module);//dtp_add_buffer
            break;
        case PHASE_A:         // Phase A
            ade7858_get_calib_value_phaseA();
            comm_add_buffer((uint8_t *)&ade7858.calib.pA,sizeof(ade7858.calib.pA),module);//dtp_add_buffer
            break;
        case PHASE_B:         // Phase B
            ade7858_get_calib_value_phaseB();
            comm_add_buffer((uint8_t *)&ade7858.calib.pB,sizeof(ade7858.calib.pB),module);//dtp_add_buffer
            break;
        case PHASE_C:         // Phase C
            ade7858_get_calib_value_phaseC();
            comm_add_buffer((uint8_t *)&ade7858.calib.pC,sizeof(ade7858.calib.pC),module);//dtp_add_buffer
            break;
        case NEUTRAL:
            ade7858_get_calib_value_phaseN();
            comm_add_buffer((uint8_t *)&ade7858.calib.pN,sizeof(ade7858.calib.pN),module);  //dtp_add_buffer      
            break;
        default:
            return FALSE;
    }       
return TRUE;    
}

// <dtp get buffer, se move sang dtp.c>
////////////////////////////////////////////////////////////////////////////////
//  Function    : _dtp_get_8bit
//  Created     : 6/1/2015, by thaodtn
//  Description : lay gia tri 16bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//uint8_t _dtp_get_8bit(uint8_t index, uint8_t module)
//{
//    uint8_t value = 0;
//    switch (module)
//    {
//        case DTP_OPTICAL:
//        value = dtp_optical_buffer[index];
//        break;
//        case DTP_PC:
//        value = dtp_pc_buffer[index];
//        break;
//        case DTP_DEVICE:
//        value = dtp_device_buffer[index];
//        break;
//    }
//    return value;
//}

////////////////////////////////////////////////////////////////////////////////
//  Function    : dtp_get_16bit
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//uint16_t _dtp_get_16bit(uint8_t index, uint8_t module)
//{
//uint16_t value = 0;
//    switch (module)
//    {
//        case DTP_OPTICAL:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_optical_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_optical_buffer + index + 1);
//            break;
//        case DTP_PC:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_pc_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_pc_buffer + index + 1);
//            break;
//        case DTP_DEVICE:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_device_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_device_buffer + index + 1);
//            break;
//        default:
//            value = 0;
//            break;
//    }
//    return value;
//}

////////////////////////////////////////////////////////////////////////////////
//  Function    : _dtp_get_32bit
//  Created     : 
//  Description : lay gia tri 32bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//uint32_t _dtp_get_32bit(uint8_t index, uint8_t module)
//{
//uint32_t value;
//    switch (module)
//    {
//        case DTP_OPTICAL:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_optical_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_optical_buffer + index + 1);
//            *((uint8_t *)&value + 2) = *((uint8_t *)&dtp_optical_buffer + index + 2);
//            *((uint8_t *)&value + 3) = *((uint8_t *)&dtp_optical_buffer + index + 3);
//            break;
//        case DTP_PC:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_pc_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_pc_buffer + index + 1);
//            *((uint8_t *)&value + 2) = *((uint8_t *)&dtp_pc_buffer + index + 2);
//            *((uint8_t *)&value + 3) = *((uint8_t *)&dtp_pc_buffer + index + 3);            
//            break;
//        case DTP_DEVICE:
//            *((uint8_t *)&value) = *((uint8_t *)&dtp_device_buffer + index);
//            *((uint8_t *)&value + 1) = *((uint8_t *)&dtp_device_buffer + index + 1);
//            *((uint8_t *)&value + 2) = *((uint8_t *)&dtp_device_buffer + index + 2);
//            *((uint8_t *)&value + 3) = *((uint8_t *)&dtp_device_buffer + index + 3); 
//        break;
//        default:
//            value = 0;
//            break;
//    }
//    return value;
//}


////////////////////////////////////////////////////////////////////////////////
//  Function    : _dtp_get_32bit
//  Created     : 
//  Description : lay gia tri 32bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//void _dtp_get_buffer(uint8_t *source, uint8_t index, uint16_t length,uint8_t module)
//{
//    switch (module)
//    {
//        case DTP_OPTICAL:
//            memcpy(source,(uint8_t *)&dtp_optical_buffer + index,length);
//            break;
//        case DTP_PC:
//            memcpy(source,(uint8_t *)&dtp_pc_buffer + index,length);            
//            break;
//        case DTP_DEVICE:
//            memcpy(source,(uint8_t *)&dtp_device_buffer + index,length);
//        break;
//        default:
//            break;
//    }
//}

#endif //ADE7858_SUPPORT

