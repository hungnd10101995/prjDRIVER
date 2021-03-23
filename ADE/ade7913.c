#if defined(__XC8)     
#include <xc.h> 
#else     
#include <htc.h> 
#endif
//#include <sg8v1.h>
#include <stdio.h>

#include "main.h"

#ifdef _ADE7913_

#include "macro.h"
//#include "spi_sg8v1.h"
#include "spi.h"
#include "ade7913.h"

#include "delay.h"
#include "uart.h"
#include "communication.h"
#ifdef EEPROM_SUPPORT
    #include "eeprom.h"
#endif
#ifdef FRAM_SUPPORT
    #include "../fram/fram.h"
#endif
#include "wdt.h"

ADE_STRUCT ade7913;

void ade7913_write(unsigned short address, unsigned char *buffer, unsigned char length)
{
    unsigned char i = 0;
    ade_cs_output_low();
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
//    spi_ade_write_byte(address >> 8);
//    spi_ade_write_byte(address);
    spi_ade_write_byte(address);
    while (i < length)
    {
        spi_ade_write_byte(buffer[length - 1 - i]);
        i++;
    }
#endif
#endif
    ade_cs_output_high();
}
//
void ade7913_read(unsigned short address, unsigned char *buffer, unsigned char length)
{
    unsigned char i = 0;
    ade_cs_output_low();
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
//    spi_ade_write_byte(address >> 8);
//      spi_ade_write_byte(address|0x20);
    spi_ade_write_byte(address);
    while (i < length) 
    {
        buffer[length - 1 - i] = spi_ade_read_byte();
        i++;
    }
#endif

#endif
    ade_cs_output_high();
    //CS_H;
}
unsigned long ade7913_read_24bit(unsigned short address)
{
	unsigned char ADE_Data[4]={0x00,0x00,0x00,0x00};
	unsigned long r;
	ade7913_read(address,ADE_Data,3);
	//
	*((unsigned char *)&r + 0) = ADE_Data[0];
	*((unsigned char *)&r + 1) = ADE_Data[1];
	*((unsigned char *)&r + 2) = ADE_Data[2];
	*((unsigned char *)&r + 3) = 0;
	//if(r > 0x7FFFFF)
	//r = 0xFFFFFF - r;
	return r;
}
unsigned short ade7913_read_16bit(unsigned short address)
{
	unsigned char ADE_Data[2];
	unsigned short r;
	ade7913_read(address,ADE_Data,2);
	//
	*((unsigned char *)&r + 0) = ADE_Data[0];
	*((unsigned char *)&r + 1) = ADE_Data[1];
	//if(r > 0x7FFF)
	//r = 0xFFFF - r;
	return r;
}
unsigned char ade7913_read_8bit(unsigned short address)
{
	unsigned char r=0;
	ade7913_read(address,r,1);
	return r;
}

unsigned char ade7913_clkout_ready(void)
{
	unsigned char result = FALSE;
    unsigned char counter = 0, i;
    //Chan CLKOUT cua ADE7913
    // xung clk 15.625us
    // Do do can than khi dung nop();
    for (i=0; i<10; i++)
    {
        //nop();
//        nop();
        if (ade_clkout_input() == 0) 
            counter++;
    }
    if (counter > 5) result = TRUE;
	return result;
}

BOOL ade7913_write_24bit(unsigned short address, unsigned long _data)
{
	unsigned char ADE_Data[3];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ADE_Data[1] = (unsigned char)(_data >> 8);
    ADE_Data[2] = (unsigned char)(_data >> 16);
	ade7913_write(address,ADE_Data,3);
    //check data
    if(ade7913_read_24bit(address) != (_data & 0x00FFFFFF))
        return FALSE;
    return TRUE;
}

BOOL ade7913_write_16bit(unsigned short address, unsigned short _data)
{
	unsigned char ADE_Data[2];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ADE_Data[1] = (unsigned char)(_data >> 8);
	ade7913_write(address,ADE_Data,2);
    //check data
    if(ade7913_read_16bit(address) != _data)
        return FALSE;
    return TRUE;
}
BOOL ade7913_write_8bit(unsigned short address, unsigned char _data)
{
	unsigned char ADE_Data[1];
	ADE_Data[0] = (unsigned char)_data;		  // MPL
	ade7913_write(address,ADE_Data,1);
    //check data
    if(ade7913_read_8bit(address) != _data)
        return FALSE;
    return TRUE;
}

void ade7913_ready(void) 
{
//    uint8_t STATUS0_DATA = 0b11111111;
    uint8_t STATUS0[1];
    STATUS0[0] = 0b11111111;
    uint8_t nTry = 0, nMaxWriteTry = 100;
    uint8_t error = FALSE;
    do 
    {
        //    readMultBytesADE7913(STATUS0_READ, STATUS0, 1);
        error = ade7913_write_8bit(STATUS0_READ, STATUS0);
        if (error == FALSE) 
        {
            error = ade7913_write_8bit(STATUS0_READ, STATUS0);
            if (error == FALSE) 
            {
                error = ade7913_write_8bit(STATUS0_READ, STATUS0);
            }
        }
        nTry++;
    } while ((STATUS0[0] >> 0 & 1) && nTry < nMaxWriteTry);

    // Check if bit succusfully cleared
    if ((STATUS0[0] >> 0 & 1)) 
    {
        //error ADE7913
        while (TRUE) 
        {}; // LOOP forever  on failure
    }
    else 
    {
        //Serial.print("RESET_ON bit cleared, nTry: "); Serial.println(nTry);
    }
}
void ade7913_write_configure(void) 
{
//  writeADE7913(LOCK_KEY_WRITE, UNLOCK_BYTE);
  uint8_t error;
  error = ade7913_read_8bit(STATUS0_READ);
  error = ade7913_write_8bit(LOCK_KEY_WRITE, UNLOCK_BYTE);
  error = ade7913_write_8bit(CONFIG_WRITE, 0b00001000);
  error = ade7913_read_8bit(CONFIG_READ);
  error = ade7913_write_8bit(EMI_CTRL_WRITE, 0b01010101);
  error = ade7913_read_8bit(EMI_CTRL_READ);
  error = ade7913_write_8bit(LOCK_KEY_WRITE, LOCK_BYTE);
  error = ade7913_write_8bit(LOCK_KEY_WRITE, UNLOCK_BYTE);
}



//void ade7913_config(void)
//{
//    
//}

void ade7913_init(void)
{
//    ade7913_write_16bit(CONFIG, 0x8000);
//    delay_ms(1);
//    wdt_restart();
//============= CONFIG thong so van hanh ==============================
//    wdt_restart();
}

void ade7913_check_param(void)
{
    
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_struct_calib
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7913_struct_calib(uint16_t address)
{
//    switch(address)
//    {
//        // IGAIN
//        case AIGAIN:
//            ade7913.calib.pA.i_gain = ade7913.reg_uint32;
//            break;                         
//        case BIGAIN:
//            ade7913.calib.pB.i_gain = ade7913.reg_uint32;
//            break;                  
//        // v_gain
//        case AVGAIN:
//            ade7913.calib.pA.v_gain = ade7913.reg_uint32;
//            break;                         
//        case BVGAIN:
//            ade7913.calib.pB.v_gain = ade7913.reg_uint32;
//            break;    
//        // irms_gain
//        case AIRMSOS:
//            ade7913.calib.pA.irms_gain = ade7913.reg_uint32;
//            break;                         
//        case BIRMSOS:
//            ade7913.calib.pB.irms_gain = ade7913.reg_uint32;
//            break;     
//        // vrms_gain
//        case VRMSOS:
//            ade7913.calib.pA.vrms_gain = ade7913.reg_uint32;
//            break;                         
//        //watt_gain
//        case AWGAIN:
//            ade7913.calib.pA.watt_gain = ade7913.reg_uint32;
//            break;                         
//        case BWGAIN:
//            ade7913.calib.pB.watt_gain = ade7913.reg_uint32;
//            break;    
//        // var_gain
//        case AVARGAIN:
//            ade7913.calib.pA.var_gain = ade7913.reg_uint32;
//            break;                         
//        case BVARGAIN:
//            ade7913.calib.pB.var_gain = ade7913.reg_uint32;
//            break;    
//        //va_gain
//        case AVAGAIN:
//            ade7913.calib.pA.va_gain = ade7913.reg_uint32;
//            break;                         
//        case BVAGAIN:
//            ade7913.calib.pB.va_gain = ade7913.reg_uint32;
//            break;    
//        //phase
//        case PHCALA:
//            ade7913.calib.pA.phase = ade7913.reg_uint16;
//            break;
//        case PHCALB:
//            ade7913.calib.pB.phase = ade7913.reg_uint16;
//            break;                 
//    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_set_calib_value_phaseA
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7913_set_calib_value_phaseA(BOOL wr)
{
//    BOOL check;
    //current => ade nothing
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,ade7913.calib.pA.i_ref,EEPROM_1);
//    voltage => ade nothing
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AVGAIN,ade7913.calib.pA.v_ref,EEPROM_1);
    //rms current offset
//    check = ade7913_write_24bit(BIRMSOS, ade7913.calib.pA.irms_gain);
//    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AIRMSGAIN,ade7913.calib.pA.irms_offset,EEPROM_1);
    //rms voltage offset
//    check = ade7913_write_24bit(VRMSOS, ade7913.calib.pA.vrms_gain);
//    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AVRMSGAIN,ade7913.calib.pA.vrms_offset,EEPROM_1);
    //watt
//    check = ade7913_write_24bit(BWGAIN, ade7913.calib.pA.watt_gain);
//    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AWGAIN,ade7913.calib.pA.irms_ref,EEPROM_1);
    //var
//    check = ade7913_write_24bit(BVARGAIN, ade7913.calib.pA.var_gain);
//    if(!check) return FALSE;
////    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_AVARGAIN,ade7913.calib.pA.var_gain,EEPROM_1);
//    //va
//    check = ade7913_write_24bit(BVAGAIN, ade7913.calib.pA.va_gain);
//    if(!check) return FALSE;
////    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_AVAGAIN,ade7913.calib.pA.va_gain,EEPROM_1);
//    // phase
//    check = ade7913_write_16bit(PHCALB,ade7913.calib.pA.phase);
//    if(!check) return FALSE;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_APHASE,(unsigned long)ade7913.calib.pA.phase,EEPROM_1);
//    //P display scale
//    if(ade7913.calib.pA.dips_scale==0) ade7913.calib.pA.dips_scale = A_P_SCALE_C;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_A_P_SCALE,ade7913.calib.pA.dips_scale,EEPROM_1);
//    //watt offset
//    check = ade7913_write_24bit(BWATTOS, ade7913.calib.pA.watt_os);
//    if(!check) return FALSE;
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AWATTOS,ade7913.calib.pA.vrms_ref,EEPROM_1);
//    // VAR offset
//    check = ade7913_write_24bit(BVAROS, ade7913.calib.pA.watt_os);
//    if(!check) return FALSE;
//    // VA offset
//    check = ade7913_write_24bit(BVAOS, ade7913.calib.pA.watt_os);
//    if(!check) return FALSE;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_set_calib_value_phaseB
//  Created     : 16/05/2016, by tuantm
//  Description : can chinh tung pha
//  Input		: wr: cho phep ghi vao eeprom hay ko
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7913_set_calib_value_phaseB(BOOL wr)
{
//    BOOL check;
////    //current => ade nothing
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BIGAIN,ade7913.calib.pB.i_gain,EEPROM_1);
//    //voltage => ade nothing
//    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVGAIN,ade7913.calib.pB.v_gain,EEPROM_1);
//    //rms current offset
//    check = ade7913_write_24bit(AIRMSOS, ade7913.calib.pB.irms_gain);
//    if(!check) return FALSE;
    #if defined (EV_CHARGER_PCB_V13) || defined (EV_CHARGER_PCB_V14)
//  if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,ade7913.calib.pB.irms_gain,EEPROM_1);
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,ade7913.calib.pB.vrms_offset,EEPROM_1);
//    //watt
//    check = ade7913_write_24bit(AWGAIN, ade7913.calib.pB.watt_gain);
//    if(!check) return FALSE;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,ade7913.calib.pB.watt_gain,EEPROM_1);
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,ade7913.calib.pB.vrms_ref,EEPROM_1);
    #endif
//    //var
//    check = ade7913_write_24bit(AVARGAIN, ade7913.calib.pB.var_gain);
//    if(!check) return FALSE;
////    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVARGAIN,ade7913.calib.pB.var_gain,EEPROM_1);
//    //va
//    check = ade7913_write_24bit(AVAGAIN, ade7913.calib.pB.va_gain);
//    if(!check) return FALSE;
////    if(wr) eeprom_write_4byte(EEPROM_ADE79XX_CALIB_BVAGAIN,ade7913.calib.pB.va_gain,EEPROM_1);
//     //phase
//    check = ade7913_write_16bit(PHCALA,ade7913.calib.pB.phase);
//    if(!check) return FALSE;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BPHASE,(unsigned long)ade7913.calib.pB.phase,EEPROM_1);
//    //P display scale
//    if(ade7913.calib.pB.dips_scale==0) ade7913.calib.pB.dips_scale = B_P_SCALE_C;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_B_P_SCALE,ade7913.calib.pB.dips_scale,EEPROM_1);
//    //watt offset
//    check = ade7913_write_24bit(AWATTOS, ade7913.calib.pB.watt_os);
//    if(!check) return FALSE;
//    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,ade7913.calib.pB.watt_os,EEPROM_1);
    #if defined (EV_CHARGER_PCB_V13) || defined (EV_CHARGER_PCB_V14)
    if(wr) eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,ade7913.calib.pB.v_ref,EEPROM_1);
    #endif
//    // VAR offset
//    check = ade7913_write_24bit(AVAROS, ade7913.calib.pB.watt_os);
//    if(!check) return FALSE;
//    // VA offset
//    check = ade7913_write_24bit(AVAOS, ade7913.calib.pB.watt_os);
//    if(!check) return FALSE;
    return TRUE;
}

void ade7913_get_calib_value_phaseA(void)
{
    //current => nothing
    //voltage => nothing
    //rms current offset
//    ade7913.calib.pA.irms_gain = 0;//ade7913_read_32bit(BIRMSOS);
//    //rms voltage offset
//    ade7913.calib.pA.vrms_gain = 0;//ade7913_read_32bit(VRMSOS);
//    wdt_restart();
//    //watt
//    ade7913.calib.pA.watt_gain = 0;//ade7913_read_32bit(BWGAIN);
//    //var
//    ade7913.calib.pA.var_gain = 0;//ade7913_read_32bit(BVARGAIN);
//    wdt_restart();
//    //va
//    ade7913.calib.pA.va_gain = 0;//ade7913_read_32bit(BVAGAIN);
//     //phase
//    ade7913.calib.pA.phase = 0;//ade7913_read_16bit(PHCALB);
//    // watt os
//    wdt_restart();
//    ade7913.calib.pA.watt_os = 0;//ade7913_read_32bit(BWATTOS);
//    //dummy
}

void ade7913_get_calib_value_phaseB(void)
{
    //current => nothing
    //voltage => nothing
    //rms current offset
//    ade7913.calib.pB.irms_gain = 0;//ade7913_read_32bit(AIRMSOS);
//    //rms voltage offset
//    ade7913.calib.pB.vrms_gain = 0;//ade7913_read_32bit(VRMSOS);
//    wdt_restart();
//    //watt
//    ade7913.calib.pB.watt_gain = 0;//ade7913_read_32bit(AWGAIN);
//    //var
//    ade7913.calib.pB.var_gain = 0;//ade7913_read_32bit(AVARGAIN);
//    wdt_restart();
//    //va
//    ade7913.calib.pB.va_gain = 0;//ade7913_read_32bit(AVAGAIN);
//     //phase
//    ade7913.calib.pB.phase = 0;//ade7913_read_16bit(PHCALA);
//    wdt_restart();
//    // watt os
//    ade7913.calib.pB.watt_os = 0;//ade7913_read_32bit(AWATTOS);
//    //dummy
}

//void ade7913_angle_measure_select(uint8_t type)
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
//    if(ade7913_write_16bit(COMPMODE,d_write))
//        ade7913.angle_mode = type;
//}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_process_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : lay gia tri 16bit tu bo dem
//  Input		: index: vi tri bat dau tinh
//  Output      :
                //68-len-cmd-subcmd-address[2]-data[4]-crc[1]-16 (write)
                //68-len-cmd-subcmd-address[2]-crc[1]-16 (read)
////////////////////////////////////////////////////////////////////////////////
void ade7913_write_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7913.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length)   
    comm_add_16bit(ade7913.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7913.reg_uint8  = comm_get_8bit(6,module);//_dtp_get_8bit       
            ade7913_write_8bit(ade7913.addr_reg,ade7913.reg_uint8);
            ade7913.reg_uint8 = 0;
            ade7913.reg_uint8 = ade7913_read_8bit(ade7913.addr_reg);
            comm_add_8bit(ade7913.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7913.reg_uint16 = comm_get_16bit(6,module);//_dtp_get_16bit  
            ade7913_write_16bit(ade7913.addr_reg,ade7913.reg_uint16);                        
            ade7913.reg_uint16 = 0;
            ade7913.reg_uint16 = ade7913_read_16bit(ade7913.addr_reg);    
            ade7913_struct_calib(ade7913.addr_reg);
            comm_add_16bit(ade7913.reg_uint16,module);//dtp_add_16bit           
            break;
        case 3:
        case 4:
            ade7913.reg_uint32 = comm_get_32bit(6,module);     // chu y : function nay cua thaodtn sai y do //_dtp_get_32bit                            
            ade7913_write_24bit(ade7913.addr_reg,ade7913.reg_uint32);
            ade7913.reg_uint32 = 0;
            ade7913.reg_uint32 = ade7913_read_24bit(ade7913.addr_reg);                         
            ade7913_struct_calib(ade7913.addr_reg);
            comm_add_32bit(ade7913.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_read_SUBCMD_RW_ADE_REGISTER
//  Created     : 
//  Description : doc 1 thanh ghi ADE7858 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ade7913_read_SUBCMD_RW_ADE_REGISTER(uint8_t module)
{
uint8_t index = 0;
//uint8_t result = 1;
    ade7913.addr_reg = comm_get_16bit(3,module); //dia chi//_dtp_get_16bit
    index = comm_get_8bit(5,module); //so byte//_dtp_get_8bit
    //address(2byte) + length (1byte) + data(length) 
    comm_add_16bit(ade7913.addr_reg,module);        // dtp_add_16bit
    //dtp_add_8bit(index,module);                     //
    switch(index)
    {
        case 1:
            ade7913.reg_uint8 = ade7913_read_8bit(ade7913.addr_reg);   
            comm_add_8bit(ade7913.reg_uint8,module);//dtp_add_8bit
            break;
        case 2:
            ade7913.reg_uint16 = ade7913_read_16bit(ade7913.addr_reg);             
            ade7913_struct_calib(ade7913.addr_reg);
            comm_add_16bit(ade7913.reg_uint16,module);//dtp_add_16bit
        case 3:
        case 4:
            ade7913.reg_uint32 = ade7913_read_24bit(ade7913.addr_reg);                         
            ade7913_struct_calib(ade7913.addr_reg);
            comm_add_32bit(ade7913.reg_uint32,module);//dtp_add_32bit
            break;
        default: 
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_write_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
BOOL ade7913_write_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase, uint8_t length)
{
    BOOL result = TRUE;
    comm_add_8bit(phase,module);//dtp_add_8bit
    comm_add_8bit(length,module);//dtp_add_8bit
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
            //length = sizeof(ade7913.calib);         // Nho kiem tra length
            if (length != sizeof(ade7913.calib)) return FALSE;      
            comm_get_buffer((uint8_t *)&ade7913.calib, 7, 0, length, module); //_dtp_get_buffer
            if (ade7913_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
            if (ade7913_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;       
//            ade7913_get_calib_value_phaseA();
//            ade7913_get_calib_value_phaseB();
            comm_add_buffer((uint8_t *)&ade7913.calib,length,module);//dtp_add_buffer
            break;
        case PHASE_A:         // Phase A
        case LIVE:
            //length = sizeof(ade7913.calib.pA);         // Nho kiem tra length
            if (length != sizeof(ade7913.calib.pA)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7913.calib.pA, 7, 0, length, module);//_dtp_get_buffer
            if (ade7913_set_calib_value_phaseA(TRUE) == FALSE) return FALSE;
//            ade7913_get_calib_value_phaseA();     
            comm_add_buffer((uint8_t *)&ade7913.calib.pA,length,module);//dtp_add_buffer
            break;
        case PHASE_B:         // Phase B
        case NEUTRAL:
            //length = sizeof(ade7913.calib.pB);         // Nho kiem tra length
            if (length != sizeof(ade7913.calib.pB)) return FALSE;
            comm_get_buffer((uint8_t *)&ade7913.calib.pB, 7, 0, length, module);//_dtp_get_buffer
            if (ade7913_set_calib_value_phaseB(TRUE) == FALSE) return FALSE;
//            ade7913_get_calib_value_phaseB();          
            comm_add_buffer((uint8_t *)&ade7913.calib.pB,sizeof(ade7913.calib.pB),module);//dtp_add_buffer
            break;
        default:
            return FALSE;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_read_SUBCMD_RW_ADE_CALIB_VARIABLE
//  Created     : 
//  Description : 
//  Input		: address: dia chi thanh ghi ADE7858
//  Output      :
////////////////////////////////////////////////////////////////////////////////
BOOL ade7913_read_SUBCMD_RW_ADE_CALIB_VARIABLE(uint8_t module, uint8_t phase)
{
    comm_add_8bit(phase,module);//dtp_add_8bit
    switch (phase)
    {
        case PHASE_TOTAL:         // Total
//            ade7913_get_calib_value_phaseA();
//            ade7913_get_calib_value_phaseB();      
            comm_add_buffer((uint8_t *)&ade7913.calib,sizeof(ade7913.calib),module);//dtp_add_buffer
            break;
        case PHASE_A:         // kenh pha
        case LIVE:
//            ade7913_get_calib_value_phaseA();
            comm_add_buffer((uint8_t *)&ade7913.calib.pA,sizeof(ade7913.calib.pA),module);//dtp_add_buffer
            break;
        case PHASE_B:         // kenh trung tinh
        case NEUTRAL:
//            ade7913_get_calib_value_phaseB();
            comm_add_buffer((uint8_t *)&ade7913.calib.pB,sizeof(ade7913.calib.pB),module);//dtp_add_buffer
            break;
        default:
            return FALSE;
    }       
return TRUE;    
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7913_load_calib_value
//  Created     : 16/05/2016, by tuantm
//  Description : load all thong so can chinh tu eeprom
//  Input		: 
//  Output      : 
////////////////////////////////////////////////////////////////////////////////
BOOL ade7913_load_calib_value(void)
{
    //phase A
//    eeprom_save_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,9870,EEPROM_1);
    //================ Gia tri In====================//
    ade7913.calib.pA.i_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIGAIN,EEPROM_1); 
    if(emeter.flags.eeprom_error) ade7913.calib.pA.i_ref = CURRENT_CALIBRATION; //default value
    //================ Gia tri Vn====================//
    ade7913.calib.pA.v_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pA.v_ref = VOLTAGE_CALIBRATION; //default value
    //================ Gia tri can chinh CURRENT tai 0A====================//
    ade7913.calib.pA.irms_offset = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pA.irms_offset = IRMSA_REF_OFFSET; //default value
    //================ Gia tri can chinh VOLTAGE tai 0V====================//
    ade7913.calib.pA.vrms_offset = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AVRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pA.vrms_offset = VARMS_REF_OFFSET; //default value
    //================ Gia tri can chinh CURRENT tai In====================//
    ade7913.calib.pA.irms_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pA.irms_ref = IRMSA_REF; //default value
//    // === VAR ================
//    ade7913.calib.pA.var_gain = ade7913.calib.pA.watt_gain;
//    // === VA ==================
//    ade7913.calib.pA.va_gain = ade7913.calib.pA.watt_gain;
//    // ====
//    ade7913.calib.pA.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_APHASE,EEPROM_1);
//    if(emeter.flags.eeprom_error) ade7913.calib.pA.phase = PHCALB_REF; //default value
//    ade7913.calib.pA.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_A_P_SCALE,EEPROM_1);
//    if(emeter.flags.eeprom_error) ade7913.calib.pA.dips_scale = B_P_SCALE_C;//default value
    //================ Gia tri can chinh CURRENT tai Vn====================//
    ade7913.calib.pA.vrms_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_AWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pA.vrms_ref = VARMS_REF; //default value
//    wdt_restart();
//    //phase NEUTRAL
//    ade7913.calib.pB.i_gain = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIGAIN,EEPROM_1);
//    #ifdef EEPROM_25AA1024
//    if(emeter.flags.eeprom_error) ade7913.calib.pB.i_gain = AIGAIN_REF; //default value
//    #else
//        if(emeter.flags.eeprom_error) ade7913.calib.pB.i_gain = AIGAIN_REF; //default value
//    #endif
#if defined (EV_CHARGER_PCB_V13) || defined (EV_CHARGER_PCB_V14)
    //================ Gia tri can chinh VOLTAGE-2 tai 0V====================//
    ade7913.calib.pB.vrms_offset = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BIRMSGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pB.vrms_offset = VBRMS_REF_OFFSET; //default value
//    ade7913.calib.pB.vrms_gain = ade7913.calib.pA.vrms_gain;
    //================ Gia tri can chinh VOLTAGE-2 tai Un====================//
    ade7913.calib.pB.vrms_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWGAIN,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pB.vrms_ref = VBRMS_REF; //default value
    //================ Gia tri Vn====================//
    ade7913.calib.pB.v_ref = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,EEPROM_1);
    if(emeter.flags.eeprom_error) ade7913.calib.pB.v_ref = VOLTAGE_CALIBRATION; //default value
#endif
//    ade7913.calib.pB.var_gain = ade7913.calib.pB.watt_gain;
//
//    ade7913.calib.pB.va_gain = ade7913.calib.pB.watt_gain;
//
//    ade7913.calib.pB.phase = (int16_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BPHASE,EEPROM_1);
//    if(emeter.flags.eeprom_error) ade7913.calib.pB.phase = PHCALA_REF; //default value
//    ade7913.calib.pB.dips_scale = eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_B_P_SCALE,EEPROM_1);
//    if(emeter.flags.eeprom_error) ade7913.calib.pB.dips_scale = A_P_SCALE_C;//default value
//    ade7913.calib.pB.watt_os = (int32_t)eeprom_load_param_ade(EEPROM_ADE79XX_CALIB_BWATTOS,EEPROM_1);
//    if(emeter.flags.eeprom_error) ade7913.calib.pB.watt_os = AWATTOS_REF; //default value
    wdt_restart();    
    if(emeter.flags.eeprom_error) return FALSE;
    return TRUE;
}
#endif