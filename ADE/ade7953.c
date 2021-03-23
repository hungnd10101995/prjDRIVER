#if defined(__XC8)     
#include <xc.h> 
#else     
#include <htc.h> 
#endif
//#include <sg8v1.h>
#include <stdio.h>

#include "macro.h"
#include "main.h"

#ifdef _ADE7953_
//#include "spi_sg8v1.h"
#include "spi.h"
#include "ade7953.h"

#include "delay.h"
#include "uart.h"
#include "communication.h"
#include "eeprom.h"
#include "wdt.h"

ADE_STRUCT ade7953;

void ade7953_write(unsigned short address, unsigned char *buffer, unsigned char length)
{
    unsigned char i = 0;
    CS_L;
#if defined (_16F1947)
    spi_exchange_8bit(address >> 8);
    spi_exchange_8bit(address);
    spi_exchange_8bit(0x00);
    while (i < length)
    {
        spi_exchange_8bit(buffer[length - 1 - i]);
        i++;
    }
#elif defined(_18F86J94)
#ifndef SPI_SOFT
    spi_exchange_8bit(address >> 8);
    spi_exchange_8bit(address);
    spi_exchange_8bit(0x00);
    while (i < length)
    {
        spi_exchange_8bit(buffer[length - 1 - i]);
        i++;
    }
#else
    spi_write_byte(address >> 8);
    spi_write_byte(address);
    spi_write_byte(0x00);
    while (i < length)
    {
        spi_write_byte(buffer[length - 1 - i]);
        i++;
    }
#endif
#endif
    CS_H;
}
//
void ade7953_read(unsigned short address, unsigned char *buffer, unsigned char length)
{
    unsigned char i = 0;
    CS_L;
#if defined (_16F1947)
    spi_exchange_8bit(address >> 8);
    spi_exchange_8bit(address);
    spi_exchange_8bit(0x80);
    while (i < length) 
    {
        buffer[length - 1 - i] = spi_exchange_8bit(0);
        i++;
    }
#elif defined(_18F86J94)

#ifndef SPI_SOFT
    spi_exchange_8bit(address >> 8);
    spi_exchange_8bit(address);
    spi_exchange_8bit(0x80);
    while (i < length)
    {
        buffer[length - 1 - i] = spi_exchange_8bit(0);
        i++;
    }
#else
    spi_write_byte(address >> 8);
    spi_write_byte(address);
    spi_write_byte(0x80);
    while (i < length)
    {
        buffer[length - 1 - i] = spi_read_byte();
        i++;
    }
#endif

#endif
    CS_H;
    //CS_H;
}
long ade7953_read_32bit(unsigned short address)
{
	unsigned char ADE_Data[4]={0x00,0x00,0x00,0x00};
	long r;
	ade7953_read(address,ADE_Data,4);
	//
	*((unsigned char *)&r + 0) = ADE_Data[0];
	*((unsigned char *)&r + 1) = ADE_Data[1];
	*((unsigned char *)&r + 2) = ADE_Data[2];
	*((unsigned char *)&r + 3) = ADE_Data[3];
	//if(r > 0x7FFFFF)
	//r = 0xFFFFFF - r;
	return r;
}
unsigned long ade7953_read_24bit(unsigned short address)
{
	unsigned char ADE_Data[4]={0x00,0x00,0x00,0x00};
	unsigned long r;
	ade7953_read(address,ADE_Data,3);
	//
	*((unsigned char *)&r + 0) = ADE_Data[0];
	*((unsigned char *)&r + 1) = ADE_Data[1];
	*((unsigned char *)&r + 2) = ADE_Data[2];
	*((unsigned char *)&r + 3) = 0;
	//if(r > 0x7FFFFF)
	//r = 0xFFFFFF - r;
	return r;
}
unsigned short ade7953_read_16bit(unsigned short address)
{
	unsigned char ADE_Data[2];
	unsigned short r;
	ade7953_read(address,ADE_Data,2);
	//
	*((unsigned char *)&r + 0) = ADE_Data[0];
	*((unsigned char *)&r + 1) = ADE_Data[1];
	//if(r > 0x7FFF)
	//r = 0xFFFF - r;
	return r;
}
unsigned char ade7953_read_8bit(unsigned short address)
{
	unsigned char r=0;
	ade7953_read(address,r,1);
	return r;
}
BOOL ade7953_write_32bit(unsigned short address, unsigned long _data)
{
	unsigned char ADE_Data[4];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ADE_Data[1] = (unsigned char)(_data >> 8);
    ADE_Data[2] = (unsigned char)(_data >> 16);
    ADE_Data[3] = (unsigned char)(_data >> 32);
	ade7953_write(address,ADE_Data,4);
    //check data
    if(ade7953_read_32bit(address) != _data)
        return FALSE;
    return TRUE;
}
BOOL ade7953_write_24bit(unsigned short address, unsigned long _data)
{
	unsigned char ADE_Data[3];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ADE_Data[1] = (unsigned char)(_data >> 8);
    ADE_Data[2] = (unsigned char)(_data >> 16);
	ade7953_write(address,ADE_Data,3);
    //check data
    if(ade7953_read_32bit(address) != (_data & 0x00FFFFFF))
        return FALSE;
    return TRUE;
}

BOOL ade7953_write_16bit(unsigned short address, unsigned short _data)
{
	unsigned char ADE_Data[2];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ADE_Data[1] = (unsigned char)(_data >> 8);
	ade7953_write(address,ADE_Data,2);
    //check data
    if(ade7953_read_16bit(address) != _data)
        return FALSE;
    return TRUE;
}
BOOL ade7953_write_8bit(unsigned short address, unsigned char _data)
{
	unsigned char ADE_Data[1];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ade7953_write(address,ADE_Data,1);
    //check data
    if(ade7953_read_8bit(address) != _data)
        return FALSE;
    return TRUE;
}

//void ade7953_config(void)
//{
//    
//}

void ade7953_init(void)
{
#ifdef METER_DC
    ade7953_write_16bit(CONFIG, 0x8000);
    delay_ms(1);
    ade7953_write_8bit(PGA_V, 0x00); // gain = 1
    delay_ms(1);
    ade7953_write_8bit(PGA_IA, 0x04); // gain = 16
    delay_ms(1);
    ade7953_write_8bit(PGA_IB, 0x04); // gain = 1
    delay_ms(1);
    ade7953_write_16bit(CF1DEN, CFDEN_SHUNT); //1000 imp/kwh shunt 200u
    delay_ms(1);
    ade7953_write_16bit(CF2DEN, CFDEN_SHUNT); //1000 imp/kwh 
    delay_ms(1);    
#else
    ade7953_write_16bit(CONFIG, 0x8004);
    delay_ms(1);
    ade7953_write_8bit(PGA_V, 0x01); // gain = 2
    delay_ms(1);
    ade7953_write_8bit(PGA_IA, 0x04); // gain = 16
    delay_ms(1);
    ade7953_write_8bit(PGA_IB, 0x00); // gain = 1    
    delay_ms(1);
    ade7953_write_16bit(CF1DEN, CFDEN_SHUNT); //1600 imp/kwh shunt 200u
    delay_ms(1);
    ade7953_write_16bit(CF2DEN, CFDEN_CT); //1000 imp/kwh CT 1:2000 10 Ohm
    delay_ms(1);        
#endif
//    ade7953_write_16bit(CF2DEN, CFDEN_CT); //1000 imp/kwh CT 1:2000 10 Ohm
//    delay_ms(1);
    ade7953_write_16bit(ALT_OUTPUT, 0x000); //1600 imp/kwh shunt 200u
    delay_ms(1);
    ade7953_write_8bit(DISNOLOAD, 0);//0x07); //1600 imp/kwh shunt 200u
    delay_ms(1);
//    ade7953_write_32bit(IRQENA, 0x200); //1600 imp/kwh shunt 200u
//    delay_ms(1);
//    ade7953_write_32bit(AP_NOLOAD, 30000   ); //1600 imp/kwh shunt 200u
//    delay_ms(1);
//    ade7953_write_32bit(VAR_NOLOAD,30000); //1600 imp/kwh shunt 200u
//    delay_ms(1);
//    ade7953_write_8bit(DISNOLOAD, 0); //1600 imp/kwh shunt 200u
//    ade7953_write_32bit(IRQENB, 0x200); //1600 imp/kwh shunt 200u
//    delay_ms(1);
    wdt_restart();
//============= CONFIG thong so van hanh ==============================
    ade7953_write_32bit(VRMSOS, ade7953.calib.pA.vrms_gain);
    delay_us(10);
    ade7953_write_32bit(AIRMSOS, ade7953.calib.pA.irms_gain);
    delay_us(10);
    ade7953_write_32bit(AWGAIN, ade7953.calib.pA.watt_gain);
    delay_us(10);
    ade7953_write_32bit(AWATTOS, ade7953.calib.pA.watt_os);
    delay_us(10);
    ade7953_write_16bit(PHCALA, ade7953.calib.pA.phase);
    delay_us(10);
    ade7953_write_32bit(BIRMSOS, ade7953.calib.pB.irms_gain);
    delay_us(10);
    ade7953_write_32bit(BWGAIN, ade7953.calib.pB.watt_gain);
    delay_us(10);
    ade7953_write_32bit(BWATTOS, ade7953.calib.pB.watt_os);
    delay_us(10);
    ade7953_write_16bit(PHCALB, ade7953.calib.pB.phase);
    delay_us(10);
//========= VAR ========================
    ade7953_write_32bit(AVARGAIN, ade7953.calib.pA.var_gain);
    delay_us(10);
    ade7953_write_32bit(AVAROS, ade7953.calib.pA.watt_os);
    delay_us(10);
    ade7953_write_32bit(BVARGAIN, ade7953.calib.pB.var_gain);
    delay_us(10);
    ade7953_write_32bit(BVAROS, ade7953.calib.pB.watt_os);
    delay_us(10);
//========= VA ========================    
    ade7953_write_32bit(AVAGAIN, ade7953.calib.pA.va_gain);
    delay_us(10);
    ade7953_write_32bit(AVAOS, ade7953.calib.pA.watt_os);
    delay_us(10);
    ade7953_write_32bit(BVAGAIN, ade7953.calib.pB.va_gain);
    delay_us(10);
    ade7953_write_32bit(BVAOS, ade7953.calib.pB.watt_os);
    delay_us(10);
//========================================    
    ade7953_read_32bit(RSTIRQSTATA);
    delay_us(10);
    ade7953_read_32bit(RSTIRQSTATB);
    delay_us(10);
//    ade7953_write_16bit(CFMODE, CF1_CHANEL_A);//0x0040);
    ade7953_write_16bit(CFMODE, CF1_CHANEL_B);//0x0040);
    delay_us(10);
    wdt_restart();
}

//void ade7953_get_vrms(void)
//{
//    unsigned long vrms_read = ade7953_read_32bit(VRMS);
//    emeter.vrms = ((double)vrms_read/(double)VRMS_REF)*2200;
//}
//
//void ade7953_get_irms_a(void)
//{
//    unsigned long irms_a_read = ade7953_read_32bit(IRMSA);
////    emeter.phase.irms=((float)irms_a_read/(float)IRMSA_REF)*500;
//    emeter.phase.irms=((double)irms_a_read/(double)IRMSA_REF)*500;
//}
//
//void ade7953_get_irms_b(void)
//{
//    unsigned long irms_b_read = ade7953_read_32bit(IRMSB);
//    emeter.neutral.irms= ((double)irms_b_read/(double)IRMSB_REF)*500;
//}
//void ade7953_get_awatt(void)
//{
//    unsigned long awatt_read = ade7953_read_32bit(AWATT);
//    if (awatt_read > 0x7FFFFF)
//        emeter.phase.watt= -(((double)(0xFFFFFF-awatt_read)*10/( double)AWATT_REF)*1100);
//    else
//        emeter.phase.watt= (( double)(awatt_read)*10/( double)AWATT_REF)*1100;
//    //return ;
//}
//void ade7953_get_bwatt(void)
//{
//    unsigned long awatt_read = ade7953_read_32bit(BWATT);
//    if (awatt_read > 0x7FFFFF)
//        emeter.neutral.watt= -(((double)(0xFFFFFF-awatt_read)*10/( double)BWATT_REF)*1100);
//    else
//        emeter.neutral.watt= (( double)(awatt_read)*10/( double)BWATT_REF)*1100;
//    return ;
//} 
//void ade7953_get_pfa(void)
//{
//    unsigned short awatt_read = ade7953_read_16bit(PFA);
//    if (awatt_read > 0x7FFF)
//        emeter.phase.pf= -(((double)(0xFFFF-awatt_read)/( double)PFA_REF)*100);
//    else
//        emeter.phase.pf= (( double)(awatt_read)/( double)PFA_REF)*100;
//    //return ;
//}
//void ade7953_get_pfb(void)
//{
//    unsigned short awatt_read = ade7953_read_16bit(PFB);
//    if (awatt_read > 0x7FFF)
//        emeter.neutral.pf= -(((double)(0xFFFF-awatt_read)/( double)PFB_REF)*100);
//    else
//        emeter.neutral.pf= (( double)(awatt_read)/( double)PFB_REF)*100;
//    return ;
//} 

unsigned char ade7953_check_arevp(void)
{ 
    unsigned long sign;
	sign = ade7953_read_32bit(ACCMODE);
	if (sign & APSIGN_A_BIT)
    {
		return TRUE;
    }
	else
    {
		return FALSE;
    }
}
unsigned char ade7953_check_brevp(void)
{
	unsigned long sign;
	sign = ade7953_read_32bit(ACCMODE);
	if (sign & APSIGN_B_BIT)
    {
		return TRUE;
    }
	else
    {
		return FALSE;
    }
}
void ade7953_check_param(void)
{
//    if (ade7953_read_32bit(VRMSOS) != VRMSOS_REF)       ade7953_write_32bit(VRMSOS, VRMSOS_REF);
//    delay_us(10);
//    if (ade7953_read_32bit(AIRMSOS) != AIRMSOS_REF)     ade7953_write_32bit(AIRMSOS, AIRMSOS_REF);
//    delay_us(10);
//    if (ade7953_read_32bit(AWGAIN) != AWGAIN_REF)       ade7953_write_32bit(AWGAIN, AWGAIN_REF);
//    delay_us(10);
//    if (ade7953_read_32bit(AWATTOS) != AWATTOS_REF)     ade7953_write_32bit(AWATTOS, AWATTOS_REF);
//    delay_us(10);
//    if (ade7953_read_16bit(PHCALA) != PHCALA_REF)       ade7953_write_16bit(PHCALA, PHCALA_REF);
//    delay_us(10);
//    if (ade7953_read_32bit(BIRMSOS) != BIRMSOS_REF)     ade7953_write_32bit(BIRMSOS, BIRMSOS_REF);
//    delay_ms(1);
//    if (ade7953_read_32bit(BWGAIN) != BWGAIN_REF)       ade7953_write_32bit(BWGAIN, BWGAIN_REF);
//    delay_us(10);
//    if (ade7953_read_32bit(BWATTOS) != BWATTOS_REF)     ade7953_write_32bit(BWATTOS, BWATTOS_REF);
//    delay_us(10);
//    if (ade7953_read_16bit(PHCALB) != PHCALB_REF)       ade7953_write_16bit(PHCALB, PHCALB_REF);
//    delay_us(10);
//    //ade7953_write_32bit(AVARGAIN, 0x4D4BA7);
//    //delay_ms(1);
//    //ade7953_write_32bit(AVAROS, 0x003665);
//    //delay_ms(1);*/
//    
//    ade7953_write_16bit(CFMODE, 0x0040);
//    delay_ms(1);
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_struct_calib
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7953_struct_calib(uint16_t address)
{
    switch(address)
    {
        // IGAIN
        case AIGAIN:
            ade7953.calib.pA.i_gain = ade7953.reg_uint32;
            break;                         
        case BIGAIN:
            ade7953.calib.pB.i_gain = ade7953.reg_uint32;
            break;    
//        case CIGAIN:
//            ade7953.calib.pC.i_gain = ade7953.reg_uint32;
//            break; 
//        case NIGAIN:
//            ade7953.calib.pN.i_gain = ade7953.reg_uint32;
//            break;                     
        // v_gain
        case AVGAIN:
            ade7953.calib.pA.v_gain = ade7953.reg_uint32;
            break;                         
        case BVGAIN:
            ade7953.calib.pB.v_gain = ade7953.reg_uint32;
            break;    
//        case CVGAIN:
//            ade7953.calib.pC.v_gain = ade7953.reg_uint32;
//            break;   
        // irms_gain
        case AIRMSOS:
            ade7953.calib.pA.irms_gain = ade7953.reg_uint32;
            break;                         
        case BIRMSOS:
            ade7953.calib.pB.irms_gain = ade7953.reg_uint32;
            break;    
//        case CIRMSOS:
//            ade7953.calib.pC.irms_gain = ade7953.reg_uint32;
//            break; 
//        case NIRMSOS:
//            ade7953.calib.pN.irms_gain = ade7953.reg_uint32;
//            break;    
        // vrms_gain
        case VRMSOS:
            ade7953.calib.pA.vrms_gain = ade7953.reg_uint32;
            break;                         
//        case BVRMSOS:
//            ade7953.calib.pB.vrms_gain = ade7953.reg_uint32;
//            break;    
//        case CVRMSOS:
//            ade7953.calib.pC.vrms_gain = ade7953.reg_uint32;
//            break;  
        //watt_gain
        case AWGAIN:
            ade7953.calib.pA.watt_gain = ade7953.reg_uint32;
            break;                         
        case BWGAIN:
            ade7953.calib.pB.watt_gain = ade7953.reg_uint32;
            break;    
//        case CWGAIN:
//            ade7953.calib.pC.watt_gain = ade7953.reg_uint32;
//            break;  
        // var_gain
        case AVARGAIN:
            ade7953.calib.pA.var_gain = ade7953.reg_uint32;
            break;                         
        case BVARGAIN:
            ade7953.calib.pB.var_gain = ade7953.reg_uint32;
            break;    
//        case CVARGAIN:
//            ade7953.calib.pC.var_gain = ade7953.reg_uint32;
//            break; 
        //va_gain
        case AVAGAIN:
            ade7953.calib.pA.va_gain = ade7953.reg_uint32;
            break;                         
        case BVAGAIN:
            ade7953.calib.pB.va_gain = ade7953.reg_uint32;
            break;    
//        case CVAGAIN:
//            ade7953.calib.pC.va_gain = ade7953.reg_uint32;
//            break; 
                    
        //phase
        case PHCALA:
            ade7953.calib.pA.phase = ade7953.reg_uint16;
            break;
        case PHCALB:
            ade7953.calib.pB.phase = ade7953.reg_uint16;
            break;   
//        case CPHCAL:
//            ade7953.calib.pC.phase = ade7953.reg_uint16;
//            break;                
    }
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_set_calib_value_phaseA
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7953_set_calib_value_phaseA(BOOL wr)
{
    BOOL check;
    //current => ade nothing
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,ade7953.calib.pA.i_gain,EEPROM_1);
//    voltage => ade nothing
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AVGAIN,ade7953.calib.pA.v_gain,EEPROM_1);
    //rms current offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BIRMSOS, ade7953.calib.pA.irms_gain);
    #else
    check = ade7953_write_32bit(AIRMSOS, ade7953.calib.pA.irms_gain);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AIRMSGAIN,ade7953.calib.pA.irms_gain,EEPROM_1);
    //rms voltage offset
    check = ade7953_write_32bit(VRMSOS, ade7953.calib.pA.vrms_gain);
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AVRMSGAIN,ade7953.calib.pA.vrms_gain,EEPROM_1);
    //watt
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BWGAIN, ade7953.calib.pA.watt_gain);
    #else
    check = ade7953_write_32bit(AWGAIN, ade7953.calib.pA.watt_gain);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AWGAIN,ade7953.calib.pA.watt_gain,EEPROM_1);
    //var
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BVARGAIN, ade7953.calib.pA.var_gain);
    #else
    check = ade7953_write_32bit(AVARGAIN, ade7953.calib.pA.var_gain);
    #endif
    if(!check) return FALSE;
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_AVARGAIN,ade7953.calib.pA.var_gain,EEPROM_1);
    //va
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BVAGAIN, ade7953.calib.pA.va_gain);
    #else
    check = ade7953_write_32bit(AVAGAIN, ade7953.calib.pA.va_gain);
    #endif
    if(!check) return FALSE;
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_AVAGAIN,ade7953.calib.pA.va_gain,EEPROM_1);
    // phase
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_16bit(PHCALB,ade7953.calib.pA.phase);
    #else
    check = ade7953_write_16bit(PHCALA,ade7953.calib.pA.phase);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_APHASE,(unsigned long)ade7953.calib.pA.phase,EEPROM_1);
    //P display scale
    if(ade7953.calib.pA.dips_scale==0) ade7953.calib.pA.dips_scale = A_P_SCALE_C;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_A_P_SCALE,ade7953.calib.pA.dips_scale,EEPROM_1);
    //watt offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BWATTOS, ade7953.calib.pA.watt_os);
    #else
    check = ade7953_write_32bit(AWATTOS, ade7953.calib.pA.watt_os);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AWATTOS,ade7953.calib.pA.watt_os,EEPROM_1);
    // VAR offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BVAROS, ade7953.calib.pA.watt_os);
    #else
    check = ade7953_write_32bit(AVAROS, ade7953.calib.pA.watt_os);
    #endif
    if(!check) return FALSE;
    // VA offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(BVAOS, ade7953.calib.pA.watt_os);
    #else
    check = ade7953_write_32bit(AVAOS, ade7953.calib.pA.watt_os);
    #endif
    if(!check) return FALSE;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_set_calib_value_phaseB
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7953_set_calib_value_phaseB(BOOL wr)
{
    BOOL check;
//    //current => ade nothing
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BIGAIN,ade7953.calib.pB.i_gain,EEPROM_1);
//    //voltage => ade nothing
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVGAIN,ade7953.calib.pB.v_gain,EEPROM_1);
    //rms current offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AIRMSOS, ade7953.calib.pB.irms_gain);
    #else
    check = ade7953_write_32bit(BIRMSOS, ade7953.calib.pB.irms_gain);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,ade7953.calib.pB.irms_gain,EEPROM_1);
//    //rms voltage offset
//    check = ade7953_write_32bit(BVRMSOS, ade7953.calib.pB.vrms_gain);
//    if(!check) return FALSE;
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVRMSGAIN,ade7953.calib.pB.vrms_gain,EEPROM_1);
    //watt
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AWGAIN, ade7953.calib.pB.watt_gain);
    #else
    check = ade7953_write_32bit(BWGAIN, ade7953.calib.pB.watt_gain);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,ade7953.calib.pB.watt_gain,EEPROM_1);
    //var
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AVARGAIN, ade7953.calib.pB.var_gain);
    #else
    check = ade7953_write_32bit(BVARGAIN, ade7953.calib.pB.var_gain);
    #endif
    if(!check) return FALSE;
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVARGAIN,ade7953.calib.pB.var_gain,EEPROM_1);
    //va
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AVAGAIN, ade7953.calib.pB.va_gain);
    #else
    check = ade7953_write_32bit(BVAGAIN, ade7953.calib.pB.va_gain);
    #endif
    if(!check) return FALSE;
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVAGAIN,ade7953.calib.pB.va_gain,EEPROM_1);
     //phase
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_16bit(PHCALA,ade7953.calib.pB.phase);
    #else
    check = ade7953_write_16bit(PHCALB,ade7953.calib.pB.phase);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BPHASE,(unsigned long)ade7953.calib.pB.phase,EEPROM_1);
    //P display scale
    if(ade7953.calib.pB.dips_scale==0) ade7953.calib.pB.dips_scale = B_P_SCALE_C;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_B_P_SCALE,ade7953.calib.pB.dips_scale,EEPROM_1);
    //watt offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AWATTOS, ade7953.calib.pB.watt_os);
    #else
    check = ade7953_write_32bit(BWATTOS, ade7953.calib.pB.watt_os);
    #endif
    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,ade7953.calib.pB.watt_os,EEPROM_1);
    // VAR offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AVAROS, ade7953.calib.pB.watt_os);
    #else
    check = ade7953_write_32bit(BVAROS, ade7953.calib.pB.watt_os);
    #endif
    if(!check) return FALSE;
    // VA offset
    #ifdef MEASUREMENT_CHANEL_B
    check = ade7953_write_32bit(AVAOS, ade7953.calib.pB.watt_os);
    #else
    check = ade7953_write_32bit(BVAOS, ade7953.calib.pB.watt_os);
    #endif
    if(!check) return FALSE;
    return TRUE;
}

void ade7953_get_calib_value_phaseA(void)
{
    //current => nothing
    //voltage => nothing
    //rms current offset
    #ifdef MEASUREMENT_CHANEL_B
    ade7953.calib.pA.irms_gain = ade7953_read_32bit(BIRMSOS);
    //rms voltage offset
    ade7953.calib.pA.vrms_gain = ade7953_read_32bit(VRMSOS);
    wdt_restart();
    //watt
    ade7953.calib.pA.watt_gain = ade7953_read_32bit(BWGAIN);
    //var
    ade7953.calib.pA.var_gain = ade7953_read_32bit(BVARGAIN);
    wdt_restart();
    //va
    ade7953.calib.pA.va_gain = ade7953_read_32bit(BVAGAIN);
     //phase
    ade7953.calib.pA.phase = ade7953_read_16bit(PHCALB);
    // watt os
    wdt_restart();
    ade7953.calib.pA.watt_os = ade7953_read_32bit(BWATTOS);
    #else
    ade7953.calib.pA.irms_gain = ade7953_read_32bit(AIRMSOS);
    //rms voltage offset
    ade7953.calib.pA.vrms_gain = ade7953_read_32bit(VRMSOS);
    wdt_restart();
    //watt
    ade7953.calib.pA.watt_gain = ade7953_read_32bit(AWGAIN);
    //var
    ade7953.calib.pA.var_gain = ade7953_read_32bit(AVARGAIN);
    wdt_restart();
    //va
    ade7953.calib.pA.va_gain = ade7953_read_32bit(AVAGAIN);
     //phase
    ade7953.calib.pA.phase = ade7953_read_16bit(PHCALA);
    // watt os
    wdt_restart();
    ade7953.calib.pA.watt_os = ade7953_read_32bit(AWATTOS);
    #endif
    //dummy
}

void ade7953_get_calib_value_phaseB(void)
{
    //current => nothing
    //voltage => nothing
    //rms current offset
    #ifdef MEASUREMENT_CHANEL_B
    ade7953.calib.pB.irms_gain = ade7953_read_32bit(AIRMSOS);
    //rms voltage offset
    ade7953.calib.pB.vrms_gain = ade7953_read_32bit(VRMSOS);
    wdt_restart();
    //watt
    ade7953.calib.pB.watt_gain = ade7953_read_32bit(AWGAIN);
    //var
    ade7953.calib.pB.var_gain = ade7953_read_32bit(AVARGAIN);
    wdt_restart();
    //va
    ade7953.calib.pB.va_gain = ade7953_read_32bit(AVAGAIN);
     //phase
    ade7953.calib.pB.phase = ade7953_read_16bit(PHCALA);
    wdt_restart();
    // watt os
    ade7953.calib.pB.watt_os = ade7953_read_32bit(AWATTOS);
    #else
    ade7953.calib.pB.irms_gain = ade7953_read_32bit(BIRMSOS);
    //rms voltage offset
    ade7953.calib.pB.vrms_gain = ade7953_read_32bit(VRMSOS);
    wdt_restart();
    //watt
    ade7953.calib.pB.watt_gain = ade7953_read_32bit(BWGAIN);
    //var
    ade7953.calib.pB.var_gain = ade7953_read_32bit(BVARGAIN);
    wdt_restart();
    //va
    ade7953.calib.pB.va_gain = ade7953_read_32bit(BVAGAIN);
     //phase
    ade7953.calib.pB.phase = ade7953_read_16bit(PHCALB);
    wdt_restart();
    // watt os
    ade7953.calib.pB.watt_os = ade7953_read_32bit(BWATTOS);
    #endif
    //dummy
}

//void ade7953_angle_measure_select(uint8_t type)
//{
//    uint16_t d_write = COMPMODE_C;
//    switch(type)
//    {
//        case ANGLE_UU: //Vol-Vol
//            d_write set (BIT_9);
//            break;
//        case ANGLE_II: //Current - current
//            d_write set (BIT_10);
//        case ANGLE_UI: //default: Vol-current
//        default:
//            break;
//    }
//    if(ade7953_write_16bit(COMPMODE,d_write))
//        ade7953.angle_mode = type;
//}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_process_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
                //68-len-cmd-subcmd-address[2]-data[4]-crc[1]-16 (write)
                //68-len-cmd-subcmd-address[2]-crc[1]-16 (read)
////////////////////////////////////////////////////////////////////////////////
void ade7953_write_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7953.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length)   
    comm_add_16bit(ade7953.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7953.reg_uint8  = comm_get_8bit(6,module);//_dtp_get_8bit       
            ade7953_write_8bit(ade7953.addr_reg,ade7953.reg_uint8);
            ade7953.reg_uint8 = 0;
            ade7953.reg_uint8 = ade7953_read_8bit(ade7953.addr_reg);
            comm_add_8bit(ade7953.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7953.reg_uint16 = comm_get_16bit(6,module);//_dtp_get_16bit  
            ade7953_write_16bit(ade7953.addr_reg,ade7953.reg_uint16);                        
            ade7953.reg_uint16 = 0;
            ade7953.reg_uint16 = ade7953_read_16bit(ade7953.addr_reg);    
            ade7953_struct_calib(ade7953.addr_reg);
            comm_add_16bit(ade7953.reg_uint16,module);//dtp_add_16bit           
            break;
        case 3:
        case 4:
            ade7953.reg_uint32 = comm_get_32bit(6,module);     // chu y : function nay cua thaodtn sai y do //_dtp_get_32bit                            
            ade7953_write_32bit(ade7953.addr_reg,ade7953.reg_uint32);
            ade7953.reg_uint32 = 0;
            ade7953.reg_uint32 = ade7953_read_32bit(ade7953.addr_reg);                         
            ade7953_struct_calib(ade7953.addr_reg);
            comm_add_32bit(ade7953.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_read_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : doc 1 thanh ghi ADE7858 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7953_read_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7953.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length) 
    comm_add_16bit(ade7953.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7953.reg_uint8 = ade7953_read_8bit(ade7953.addr_reg);   
            comm_add_8bit(ade7953.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7953.reg_uint16 = ade7953_read_16bit(ade7953.addr_reg);             
            ade7953_struct_calib(ade7953.addr_reg);
            comm_add_16bit(ade7953.reg_uint16,module);//dtp_add_16bit
        case 3:
        case 4:
            ade7953.reg_uint32 = ade7953_read_32bit(ade7953.addr_reg);                         
            ade7953_struct_calib(ade7953.addr_reg);
            comm_add_32bit(ade7953.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_read_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//BOOL ade7953_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module)
BOOL ade7953_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length)
{
    BOOL result = TRUE;
    comm_add_8bit(phase,module);//dtp_add_8bit
    comm_add_8bit(length,module);//dtp_add_8bit
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
            //length = sizeof(ade7953.calib);         // Nho kiem tra length
            if (length != sizeof(ade7953.calib)) return FALSE;      
            comm_get_buffer((uint8_t *)&ade7953.calib, 7, 0, length, module); //_dtp_get_buffer
            if (ade7953_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
            if (ade7953_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;       
            ade7953_get_calib_value_phaseA();
            ade7953_get_calib_value_phaseB();
            comm_add_buffer((uint8_t *)&ade7953.calib,length,module);//dtp_add_buffer
            break;
        case PHASE_A:         // Phase A
        case LIVE:
            //length = sizeof(ade7953.calib.pA);         // Nho kiem tra length
            if (length != sizeof(ade7953.calib.pA)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7953.calib.pA, 7, 0, length, module);//_dtp_get_buffer
            if (ade7953_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
            ade7953_get_calib_value_phaseA();     
            comm_add_buffer((uint8_t *)&ade7953.calib.pA,length,module);//dtp_add_buffer
            break;
        case PHASE_B:         // Phase B
        case NEUTRAL:
            //length = sizeof(ade7953.calib.pB);         // Nho kiem tra length
            if (length != sizeof(ade7953.calib.pB)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7953.calib.pB, 7, 0, length, module);//_dtp_get_buffer
            if (ade7953_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;
            ade7953_get_calib_value_phaseB();          
            comm_add_buffer((uint8_t *)&ade7953.calib.pB,sizeof(ade7953.calib.pB),module);//dtp_add_buffer
            break;
        default:
            return FALSE;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_read_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
BOOL ade7953_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase)
{
    comm_add_8bit(phase,module);//dtp_add_8bit
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
            ade7953_get_calib_value_phaseA();
            ade7953_get_calib_value_phaseB();      
            comm_add_buffer((uint8_t *)&ade7953.calib,sizeof(ade7953.calib),module);//dtp_add_buffer
            break;
        case PHASE_A:         // kenh pha
        case LIVE:
            ade7953_get_calib_value_phaseA();
            comm_add_buffer((uint8_t *)&ade7953.calib.pA,sizeof(ade7953.calib.pA),module);//dtp_add_buffer
            break;
        case PHASE_B:         // kenh trung tinh
        case NEUTRAL:
            ade7953_get_calib_value_phaseB();
            comm_add_buffer((uint8_t *)&ade7953.calib.pB,sizeof(ade7953.calib.pB),module);//dtp_add_buffer
            break;
        default:
            return FALSE;
    }       
return TRUE;    
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7953_load_calib_value
//  Created     : 16/05/2016, by tuantm
//  Description : load all thong so can chinh tu eeprom
//  Input		: 
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7953_load_calib_value(void)
{
#ifdef MEASUREMENT_CHANEL_B
    //phase A
    ade7953.calib.pA.i_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.i_gain = BIGAIN_REF; //default value
    ade7953.calib.pA.v_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.v_gain = VGAIN_REF; //default value
    
    ade7953.calib.pA.irms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.irms_gain = BIRMSOS_REF; //default value 
    ade7953.calib.pA.vrms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.vrms_gain = VRMSOS_REF; //default value
    ade7953.calib.pA.watt_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.watt_gain = BWGAIN_REF; //default value
    
    ade7953.calib.pA.var_gain = ade7953.calib.pA.watt_gain;
//    ade7953.calib.pA.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_AVARGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pA.var_gain = AVARGAIN_C; //default value
    ade7953.calib.pA.va_gain = ade7953.calib.pA.watt_gain;
//    ade7953.calib.pA.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_AVAGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pA.va_gain = AVAGAIN_C; //default value
    ade7953.calib.pA.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_APHASE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.phase = PHCALB_REF; //default value
    ade7953.calib.pA.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_A_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.dips_scale = B_P_SCALE_C;//default value
    ade7953.calib.pA.watt_os = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.watt_os = BWATTOS_REF; //default value
    wdt_restart();
    //phase NEUTRAL
    ade7953.calib.pB.i_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.i_gain = AIGAIN_REF; //default value
//    ade7953.calib.pB.v_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.v_gain = BVGAIN_C; //default value
    ade7953.calib.pB.irms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.irms_gain = AIRMSOS_REF; //default value
    ade7953.calib.pB.vrms_gain = ade7953.calib.pA.vrms_gain;

    ade7953.calib.pB.watt_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.watt_gain = AWGAIN_REF; //default value
    
    ade7953.calib.pB.var_gain = ade7953.calib.pB.watt_gain;
//    ade7953.calib.pB.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVARGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.var_gain = BVARGAIN_C; //default value
    ade7953.calib.pB.va_gain = ade7953.calib.pB.watt_gain;
//    ade7953.calib.pB.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVAGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.va_gain = BVAGAIN_C; //default value
    ade7953.calib.pB.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BPHASE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.phase = PHCALA_REF; //default value
    ade7953.calib.pB.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_B_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.dips_scale = A_P_SCALE_C;//default value
    ade7953.calib.pB.watt_os = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.watt_os = AWATTOS_REF; //default value
    wdt_restart();    
#else
    //phase A
    ade7953.calib.pA.i_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.i_gain = AIGAIN_REF; //default value
    ade7953.calib.pA.v_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.v_gain = VGAIN_REF; //default value
    
    ade7953.calib.pA.irms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.irms_gain = AIRMSOS_REF; //default value 
    ade7953.calib.pA.vrms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.vrms_gain = VRMSOS_REF; //default value
    ade7953.calib.pA.watt_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.watt_gain = AWGAIN_REF; //default value
    
    ade7953.calib.pA.var_gain = ade7953.calib.pA.watt_gain;
//    ade7953.calib.pA.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_AVARGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pA.var_gain = AVARGAIN_C; //default value
    ade7953.calib.pA.va_gain = ade7953.calib.pA.watt_gain;
//    ade7953.calib.pA.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_AVAGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pA.va_gain = AVAGAIN_C; //default value
    ade7953.calib.pA.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_APHASE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.phase = PHCALA_REF; //default value
    ade7953.calib.pA.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_A_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.dips_scale = A_P_SCALE_C;//default value
    ade7953.calib.pA.watt_os = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pA.watt_os = AWATTOS_REF; //default value
    wdt_restart();
    //phase NEUTRAL
    ade7953.calib.pB.i_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.i_gain = BIGAIN_REF; //default value
//    ade7953.calib.pB.v_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.v_gain = BVGAIN_C; //default value
    ade7953.calib.pB.irms_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.irms_gain = BIRMSOS_REF; //default value
    ade7953.calib.pB.vrms_gain = ade7953.calib.pA.vrms_gain;

    ade7953.calib.pB.watt_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.watt_gain = BWGAIN_REF; //default value
    
    ade7953.calib.pB.var_gain = ade7953.calib.pB.watt_gain;
//    ade7953.calib.pB.var_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVARGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.var_gain = BVARGAIN_C; //default value
    ade7953.calib.pB.va_gain = ade7953.calib.pB.watt_gain;
//    ade7953.calib.pB.va_gain = (int32_t)eeprom_read_4byte(EEPROM_ADE79XX_CALIB_BVAGAIN,EEPROM_1);
//    if(emeter.flags.eeprom_err) ade7953.calib.pB.va_gain = BVAGAIN_C; //default value
    ade7953.calib.pB.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BPHASE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.phase = PHCALB_REF; //default value
    ade7953.calib.pB.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_B_P_SCALE,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.dips_scale = B_P_SCALE_C;//default value
    ade7953.calib.pB.watt_os = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_err) ade7953.calib.pB.watt_os = BWATTOS_REF; //default value
    wdt_restart();
#endif
    if(emeter.flags.eeprom_err) return FALSE;
    return TRUE;
}

#endif //_ADE7953_