/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#if defined(__XC8)
    #include <xc.h>
#endif
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#ifdef __MSP430__
#include <io.h>
#include <emeter-toolkit.h>
#endif

#include "../devices.h"

#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter-main.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-main.h"
#endif
#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter-main.h"
#endif

#if defined(_DT01P_RF_) && defined(_PIC16XX1919Y_) //namhv 01/11/2019
#include "..\..\dt01p-rf\fw\emeter-main.h"
#include "..\..\dt01p-rf\fw\emeter-hardware.h"
#endif

#if defined(_DT01P_RF_) && defined(__MSP430__)
#include "..\..\dt01p80-rf\fw\emeter-main.h"
#include "..\..\dt01p80-rf\fw\emeter-hardware.h"
#endif
#if defined(_DT01P_RF_)  && defined(_18F86J94)
#include "..\..\dt01p80-rf\fw\main.h"
#include "..\..\dt01p80-rf\fw\hardware.h"
#endif

#include "24aa08.h"
#include "..\i2c\i2c.h"
#include "..\framework\crc.h"

#ifdef RF_SUPPORT
//#include "..\rf\cc1101.h"
#include "..\rf\rf.h"
#endif

#include "..\framework\delay.h"
#include "../framework/process.h"

#ifdef _TI_MEASURE_1P_
#include "../1-phase-measurement/measure-struct.h"
#include "../1-phase-measurement/measure-background.h"
#endif
#ifdef _ADE7953_
#include "../ade/ade-1ph-measure.h"
#endif
//#include "..\communication\dtp.h"

#if defined(_LCD_OST11248_)
#include "..\lcd\lcd-segment-ost11248.h"
#elif defined(_LCD_M0752_)
#include "..\lcd\lcd-segment-m0752.h"
#endif

#include "..\lcd\emeter-display.h"
//#ifdef INT_RTC_SUPPORT  //vinh
//#include "..\rtc\rtc.h"
//#include "..\framework\utc.h"
//#endif

#ifndef BILLING_PERIOD_SUPPORT
#warning "undef BILLING_RF_SUPPORT in 24aa08.c"
#endif

//#ifdef BILLING_RF_SUPPORT
//LATCH_VALUE empty_value()
//{
//    LATCH_VALUE EMPTY_VALUE;
//    EMPTY_VALUE.mUTC            = 0;
//    EMPTY_VALUE.mImport         = 0;
//    EMPTY_VALUE.mExport         = 0;
//    EMPTY_VALUE.mMaintain       = 0;
//    EMPTY_VALUE.mWarning        = 0;
//    EMPTY_VALUE.mTemper         = 0;
//    //EMPTY_VALUE.mCRC            = 0;        
//    EMPTY_VALUE.mCRC            = crc8bit_buffer((unsigned char*)&EMPTY_VALUE,15);
//    return EMPTY_VALUE;
//}
//
////LATCH_VALUE billing_data; //vinh
//#endif
//unsigned char eeprom_index;
////unsigned char EEPROM_page_copy;
//unsigned char eeprom_neutral_page;
uint32_t tmp; //for copy

////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_byte
//  Created     : 21/01/2015, by tuantm
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_byte(unsigned int address, uint8_t data,uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned int crc8;
	//status;
        eeprom_wp_output_low();
        delay_us(10);
	//if (token2 != write_ext_eeprom_token) {Disable_interrupts(GLOBAL); while(1) {} }
	//address
#if defined(AA08_SUPPORT)
	addr_tam=(unsigned char)(address >>7 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write((0xa0|(addr_tam & 0x07))&0xfe);          // I2C address
	i2c_write(addr_tam1);
#elif defined(AA32_SUPPORT)
	addr_tam=(unsigned char)(address / 0x100 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write(0xa0);          // I2C address
	i2c_write(addr_tam);
        i2c_write(addr_tam1);
#endif        
	//data
	i2c_write(data);
	//crc
	crc8 = crc8bit_buffer(&data,1);
	i2c_write(crc8);
	i2c_stop();
	delay_ms(6);//delay_ms(10);
        eeprom_wp_output_high();
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_3byte
//  Created     : 
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_3byte(unsigned int address, unsigned long data,uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
        eeprom_wp_output_low();
        delay_us(10);
	crc8 = crc8bit_buffer((unsigned char*)&data,3);
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned char)(address >> 7 );
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);
#elif defined(AA32_SUPPORT)
        addr_tam=(unsigned char)(address / 0x100 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write(0xa0);          // I2C address
	i2c_write(addr_tam);
        i2c_write(addr_tam1);
#endif
	i2c_write((unsigned char)data);
	i2c_write((unsigned char)(data>>8));
	i2c_write((unsigned char)(data>>16));
	i2c_write(crc8);
	i2c_stop();
	delay_ms(6);
        eeprom_wp_output_high();
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_4byte
//  Created     : 21/01/2015, by tuantm
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_4byte(unsigned int address, unsigned long data,uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
        eeprom_wp_output_low();
        delay_us(10);
	crc8 = crc8bit_buffer((unsigned char*)&data,4);
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned char)(address >> 7 );
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);
#elif defined(AA32_SUPPORT)
    addr_tam=(unsigned char)(address / 0x100 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write(0xa0);          // I2C address
	i2c_write(addr_tam);
        i2c_write(addr_tam1);
#endif
	i2c_write((unsigned char)data);
	i2c_write((unsigned char)(data>>8));
	i2c_write((unsigned char)(data>>16));
	i2c_write((unsigned char)(data>>24));
	i2c_write(crc8);
	i2c_stop();
	delay_ms(10);
        eeprom_wp_output_high();
}
//doc 1 byte tu` EEPROM 24AA08
unsigned char ic24aa08_read_byte(unsigned int address,uint8_t eepromIndex)
{
	unsigned char data,addr_tam,addr_tam1;
	unsigned char crc8;
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);

	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) & 0xFE);       // I2C address
	i2c_write(addr_tam1);
	delay_us(5);  //ack

	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) | 1 );
#elif defined(AA32_SUPPORT)
        addr_tam=(unsigned char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);

	i2c_start();
	i2c_write(0xa0);       // I2C address
	i2c_write(addr_tam);
        i2c_write(addr_tam1);
	delay_us(5);  //ack

	i2c_start();
	i2c_write(0xa1);
#endif
	//data
	data=i2c_read(1);
	//crc
	crc8 =i2c_read(0);
	i2c_stop();
        emeter.flags.eeprom_error = 0;
	if (crc8 != crc8bit_buffer(&data,1)) 
        {
            emeter.flags.eeprom_error = 1;
        }
	return(data);

}



// ghi EEPROM lie^n tuc -> tiet kiem thoi gian save EEPROM
// ghi data 2 byte tai dia chi? address
void ic24aa08_write_2byte(unsigned int address, int data,uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
	//int1 status;
	//if (token2 != write_int16_ext_eeprom_token) {Disable_interrupts(GLOBAL); while(1) {} }
	//address
        eeprom_wp_output_low();
        delay_us(10);
#if defined(AA08_SUPPORT)
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);

	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) & 0xFE);
	i2c_write(addr_tam1);
#elif defined(AA32_SUPPORT)
        addr_tam=(unsigned char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);

	i2c_start();
	i2c_write(0xa0);
        i2c_write(addr_tam);
	i2c_write(addr_tam1);
#endif
	//data
	i2c_write(*(&data));
	i2c_write( *(&data+1));
	//crc
	crc8 = crc8bit_buffer(&data,2);
	i2c_write(crc8);

	i2c_stop();
	//delay_ms(7);

	/*status=1;
	while(status==1)
	{
	i2c_start();
	status=i2c_write(0xa0);
}*/
	delay_ms(6);
        eeprom_wp_output_high();
}


//doc lien tuc du~ lie.u 2 byte tu` 24AA08

int ic24aa08_read_2byte(unsigned int address, uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned int crc8;
	int data;
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned char)(address >> 7 );
	addr_tam1=(unsigned char)(address % 0x100);
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	//i2c_write(addr_tam & 0x1F );
	i2c_write(addr_tam1);        
	//
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) |1);
#elif defined(AA32_SUPPORT)
        addr_tam=(unsigned char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);
	i2c_start();
	i2c_write(0xa0);
	i2c_write(addr_tam);
	i2c_write(addr_tam1);        
	//
	i2c_start();
	i2c_write(0xa1);
#endif
	*(&data) = i2c_read(1);
	*(&data+1) = i2c_read(1);
	crc8  = i2c_read(0);

	i2c_stop();
	if (crc8 != crc8bit_buffer(&data,2) ) 
    {
        emeter.flags.eeprom_error = 1;
    }
    else 
    {
        emeter.flags.eeprom_error = 0;
    }
	return(data);
	//addr_tam*0xFFFF +
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_read_3byte
//  Created     : 04/11/2015, by vinh
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
unsigned long ic24aa08_read_3byte(unsigned int address, uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned long data = 0;
	unsigned char crc8;
	unsigned char tam;
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned  char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) | 1);
#elif defined(AA32_SUPPORT)
        //address
	addr_tam=(unsigned  char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write(0xa0);
        i2c_write(addr_tam);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write(0xa1);
#endif        
	//for (i=0;i<3;i++)   *(&data+i) = i2c_read(1);// (i==3)?0:1 );
	tam = i2c_read(1) ;
	data = tam;
	tam = i2c_read(1);
	data = data + (unsigned int)tam*0x100;
	tam = i2c_read(1);
	data = data + (unsigned long)tam * 0x10000;
	// *(&data+3) = 0;

	//unsigned char *p_data;
	//p_data = (unsigned char *)&data;
	//for (i = 0;  i < 3;  i++)
	//*(p_data+i) = i2c_read(1);

	crc8 =i2c_read(0);
	i2c_stop();
        emeter.flags.eeprom_error = 0;
	if (crc8 != crc8bit_buffer((unsigned char*)&data,4) )
        {
            emeter.flags.eeprom_error = 1;
        }
	return(data);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_read_buffer
//  Created     : 04/11/2015, by vinh
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//doc lien tuc du~ lie.u 4 byte tu` 24AA08
unsigned long ic24aa08_read_4byte(unsigned int address, uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned long data;
	unsigned char crc8;
	unsigned char tam;
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned  char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) | 1);
#elif defined(AA32_SUPPORT)
        //address
	addr_tam=(unsigned  char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write(0xa0);
        i2c_write(addr_tam);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write(0xa1);
#endif        
	//for (i=0;i<3;i++)   *(&data+i) = i2c_read(1);// (i==3)?0:1 );
	tam = i2c_read(1) ;
	data = tam;
	tam = i2c_read(1);
	data = data + (unsigned int)tam*0x100;
	tam = i2c_read(1);
	data = data + (unsigned long)tam * 0x10000;
	tam = i2c_read(1);
	data = data + (unsigned long)tam * 0x1000000;
	// *(&data+3) = 0;

	//unsigned char *p_data;
	//p_data = (unsigned char *)&data;
	//for (i = 0;  i < 3;  i++)
	//*(p_data+i) = i2c_read(1);

	crc8 =i2c_read(0);
	i2c_stop();
        emeter.flags.eeprom_error = 0;
	if (crc8 != crc8bit_buffer((unsigned char*)&data,4) )
    { 
        emeter.flags.eeprom_error = 1;
    }
	return(data);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_read_buffer
//  Created     : 04/11/2015, by vinh
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_read_buffer(unsigned int address,unsigned char *buffer_data, uint8_t length, uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	//unsigned long data;
	unsigned char crc8;
	//unsigned char tam;
#if defined(AA08_SUPPORT)
	//address
	addr_tam=(unsigned  char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) | 1);
#elif defined(AA32_SUPPORT)
        //address
	addr_tam=(unsigned  char)(address / 0x100);
	addr_tam1=(unsigned char)(address % 0x100);

	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write(0xa0);
        i2c_write(addr_tam);
	i2c_write(addr_tam1);

	delay_us(5);  //ack
	//data
	i2c_start();
	i2c_write(0xa1);
        if(length>32) length = 32;
#endif        
	//for (i=0;i<3;i++)   *(&data+i) = i2c_read(1);// (i==3)?0:1 );
        for(unsigned char n=0; n < length; n++)
            *(buffer_data+n) = i2c_read(1);
	crc8 = i2c_read(0);
	i2c_stop();
    emeter.flags.eeprom_error = 0;
    if (crc8 != crc8bit_buffer(buffer_data,length)) 
    {
        emeter.flags.eeprom_error = 1;
    }
	//return(data);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_buffer
//  Created     : 04/11/2015, by vinh
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_buffer(unsigned int address, uint8_t *buffer, uint8_t length, uint8_t eepromIndex)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
	//status;
        eeprom_wp_output_low();
        delay_us(10);
	//if (token2 != write_ext_eeprom_token) {Disable_interrupts(GLOBAL); while(1) {} }
	//address
#if defined(AA08_SUPPORT)
	addr_tam=(unsigned char)(address >>7 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write((0xa0|(addr_tam & 0x07))&0xfe);          // I2C address
	i2c_write(addr_tam1);
        if(length > 16) length =16;
#elif defined(AA32_SUPPORT)
	addr_tam=(unsigned char)(address / 0x100 );  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap

	i2c_start();
	i2c_write(0xa0);          // I2C address
	i2c_write(addr_tam);
        i2c_write(addr_tam1);
        if(length > 32) length =32;
#endif        
	//data
        for(unsigned char n=0 ; n < length; n++)    
            i2c_write(*(buffer+n));
	//crc
	crc8 = crc8bit_buffer(buffer,length);
	i2c_write(crc8);
	i2c_stop();
	delay_ms(10);
        eeprom_wp_output_high();
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : eeprom_24aa08_write_byte
//  Created     : 28/10/2015, by vinh
//  Description : 
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
unsigned char eeprom_24aa08_write_byte(uint16_t addr, uint8_t value, uint8_t number_check)
{
    unsigned char value8_check;
    for(unsigned char number=0; number<number_check; number++)
    {
        restart_wdt();
        ic24aa08_write_byte(addr, value, EEPROM_1);
        emeter.flags.eeprom_error  = 0;
        value8_check = ic24aa08_read_byte(addr,EEPROM_1);
        if((value8_check != value)||(emeter.flags.eeprom_error == 1))
        {
            if(number == number_check-1)
            {
                emeter.flags.eeprom_error  = 0;
                return 1;
            }
            delay_ms(5);
        }
        else
            number = number_check;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : eeprom_24aa08_write_2byte
//  Created     : 28/10/2015, by vinh
//  Description : 
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//unsigned char eeprom_24aa08_write_2byte(uint16_t addr, uint16_t value, uint8_t number_check)
//{
//    uint16_t value16_test;
//    for(unsigned char number = 0; number<number_check; number++)
//    {
//        restart_watchdog();
//        ic24aa08_write_2byte(addr, value, EEPROM_1);
//        emeter.flags.eeprom_error = 0;
//        value16_test = ic24aa08_read_2byte(addr, EEPROM_1);
//        if((value != value16_test)||(emeter.flags.eeprom_error == 1))
//        {
//            if(number == number_check-1)
//            {
//                emeter.flags.eeprom_error = 0;
//                return 1;
//            }
//            delay_ms(5);
//        }
//        else
//            number = number_check;
//    }
//    return 0;
//}
////////////////////////////////////////////////////////////////////////////////
//  Function    : eeprom_24aa08_write_4byte
//  Created     : 28/10/2015, by vinh
//  Description : 
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
unsigned char eeprom_24aa08_write_4byte(uint16_t addr, uint32_t value, uint8_t number_check)
{
    uint32_t value32_test;
    for(unsigned char number = 0; number<number_check; number++)
    {
        restart_wdt();
        ic24aa08_write_4byte(addr, value, EEPROM_1);
        emeter.flags.eeprom_error  = 0;
        value32_test = ic24aa08_read_4byte(addr,EEPROM_1);
        if((value != value32_test)||(emeter.flags.eeprom_error  == 1))
        {
            if(number == number_check-1)
            {
                emeter.flags.eeprom_error  = 0;
                return 1;
            }
            delay_ms(5);
        }
        else
            number = number_check;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : eeprom_24aa08_write_byte
//  Created     : 28/10/2015, by vinh
//  Description : 
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(ENERGY_5_INT_1_DECIMAL_SUPPORT)
unsigned char eeprom_24aa08_get_index(uint32_t value_data)
{
    uint8_t index_tmp8 = 0;
    index_tmp8 = value_data/REGION_VALUE;
    if (index_tmp8 >= REGION_BACKUP_MAX) 
        index_tmp8 = REGION_BACKUP_MAX - 1;
    return index_tmp8;
}
#endif
//////////////////////////////////
//  Function    : eeprom_load_parameters
//  Created     : 28/07/2014, by tuantm
//  Description : load lai gia tri cua cac thanh ghi khi reset lai
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_load_parameters(void)
{        
    _DINT();
//            eeprom_wp_dir_output();
//            eeprom_power_high();
    emeter.flags.eeprom_error  = 0;
    //          emeter.energy_reg_error = 0;
    eepromError.errorRegs = 0;
    eepromError.errorRegs_neutral = 0;    
    ////// so lan lap trinh
    delay_ms(5);
    emeter.progtimes = ic24aa08_read_byte(ADDR_PROG_TIMES,EEPROM_1);
    if (emeter.flags.eeprom_error  == 1)
    {
        emeter.flags.eeprom_error  = 0;
        emeter.progtimes = ic24aa08_read_byte(ADDR_PROG_TIMES+2,EEPROM_1);
    }       
    ///////// pulse count
    delay_ms(5);
    emeter.active.forward.pulse_count =ic24aa08_read_byte(EEPROM_KWH_PULSE_COUNTER_ADDR,EEPROM_1);
    if (emeter.flags.eeprom_error  == 1)
    {
        emeter.flags.eeprom_error  = 0;
        emeter.active.forward.pulse_count=0;
    }
    delay_ms(5);
    emeter.active.reverse.pulse_count = ic24aa08_read_byte(EEPROM_KWH_PULSE_REVERSE_COUNTER_ADDR,EEPROM_1);
    if (emeter.flags.eeprom_error  == 1)
    {
        emeter.flags.eeprom_error  = 0;
        emeter.active.reverse.pulse_count=0;
    }
    ///////////////////////////////////////////////////////          
    /*   emeter.id    *///////////////////////
    delay_ms(5);
    emeter.id = ic24aa08_read_4byte(EEPROM_METER_ID,EEPROM_1);
    if (emeter.flags.eeprom_error  == 1)
    {
        emeter.flags.eeprom_error  = 0;
        delay_ms(5);
        emeter.id  = ic24aa08_read_4byte(EEPROM_METER_ID_BACKUP, EEPROM_1);//(ADDR_METER_ID+BACKUP_OFFSET,EEPROM_1);
        if(emeter.flags.eeprom_error  == 1)
        {
            emeter.flags.eeprom_error = 0;                      
            delay_ms(5);
            emeter.id  = ic24aa08_read_4byte(EEPROM_METER_ID+5,EEPROM_1);            
            if(emeter.flags.eeprom_error == 1)
            {                
                emeter.flags.eeprom_error = 0; //namhv 29/10/2019 thay cho flag_eeprom_error = 0;
                emeter.flags.error_id = TRUE;//namhv 28/10/2019 eepromError.flags.id = TRUE; 
                //                      emeter.energy_reg_error |= EMETER_ID_ERR;
            }
            else
            {
                delay_ms(5);              
                ic24aa08_write_4byte(EEPROM_METER_ID_BACKUP,emeter.id,EEPROM_1);
                eeprom_24aa08_write_4byte(EEPROM_METER_ID,emeter.id,3);
            }
        }
        else
        {
            delay_ms(5);            
            eeprom_24aa08_write_4byte(EEPROM_METER_ID,emeter.id,3);
        }          
    }
    //          if(emeter.energy_reg_error & EMETER_ID_ERR)
    if(emeter.flags.error_id == TRUE)//namhv 28/10/2019 if(eepromError.flags.id == TRUE)
    {
        //              emeter.energy_reg_error &= ~EMETER_ID_ERR;
        emeter.flags.error_id = FALSE;//namhv 28/10/2019 eepromError.flags.id = FALSE;
        restart_wdt();
        delay_ms(500);
        restart_wdt();
        emeter.id = ic24aa08_read_4byte(EEPROM_METER_ID,EEPROM_1);
        if (emeter.flags.eeprom_error == 1)
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.id  = ic24aa08_read_4byte(EEPROM_METER_ID_BACKUP, EEPROM_1);//(ADDR_METER_ID+BACKUP_OFFSET,EEPROM_1);
            if(emeter.flags.eeprom_error == 1)
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.id  = ic24aa08_read_4byte(EEPROM_METER_ID+5,EEPROM_1);
                if(emeter.flags.eeprom_error == 1)
                {
                    emeter.flags.eeprom_error = 0;
                    emeter.flags.error_id = TRUE;//namhv 28/10/2019 eepromError.flags.id = TRUE;
                    //                          emeter.energy_reg_error |= EMETER_ID_ERR;
                }
                else
                {
                    delay_ms(5);                  
                    ic24aa08_write_4byte(EEPROM_METER_ID_BACKUP,emeter.id,EEPROM_1);
                    eeprom_24aa08_write_4byte(EEPROM_METER_ID,emeter.id,3);
                }
            }
            else
            {
                delay_ms(5);
                eeprom_24aa08_write_4byte(EEPROM_METER_ID,emeter.id,3);
            }          
        }
    }
    if(emeter.flags.error_id == TRUE) emeter.id = 0xFFFFFFFF; //namhv 28/10/2019 if (eepromError.flags.id == TRUE) emeter.id = 0xFFFFFFFF;
#ifdef RF_SUPPORT
//    cc1101.address = *(&emeter.id);
//    cc1101_write_reg(CC1101_ADDR, cc1101.address); 
    rf_set_address((unsigned char)emeter.id);
#endif
    ////////////////////////////////////////////////////////////////////////////
    /*   load thong so cho neutral    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef NEUTRAL_ENEGRY_SUPPORT
    delay_ms(5);
    eeprom_neutral_page = (unsigned char)ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE,EEPROM_1);
    if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) ) // !!!
    {
        emeter.flags.eeprom_error = 0;                                      //Page_Backup_offset
        delay_ms(5);
        eeprom_neutral_page = ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE+2,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) )            
        {            
            emeter.flags.eeprom_error = 0;   
            delay_ms(5);
            eeprom_neutral_page = ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE+4,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) )               
            {
                emeter.flags.eeprom_error = 0;
                eepromError.flag.eepromIndex_neutral = TRUE;
                //                      emeter.energy_reg_error |= EEPROM_NEUTRAL_PAGE_ERR;
            }
            else
            {
                delay_ms(5);
                ic24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE+2,eeprom_neutral_page,EEPROM_1);
                eeprom_24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE,eeprom_neutral_page,3);
            }
        }
        else
        {
            delay_ms(5);
            eeprom_24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE,eeprom_neutral_page,3);
        }
    }
    //          if(emeter.energy_reg_error & EEPROM_NEUTRAL_PAGE_ERR)
    if(eepromError.flag.eepromIndex_neutral == TRUE)
    {
        //              emeter.energy_reg_error &= ~EEPROM_NEUTRAL_PAGE_ERR;
        eepromError.flag.eepromIndex_neutral = FALSE;
        restart_wdt();
        delay_ms(500);
        restart_wdt();
        eeprom_neutral_page = (unsigned char)ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) ) // !!!
        {
            emeter.flags.eeprom_error = 0;                                      //Page_Backup_offset
            delay_ms(5);
            eeprom_neutral_page = ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE+2,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) )                
            {
                emeter.flags.eeprom_error = 0;   
                delay_ms(5);
                eeprom_neutral_page = ic24aa08_read_byte(ADDR_NEUTRAL_EEPROM_PAGE+4,EEPROM_1);
                if ((emeter.flags.eeprom_error == 1) || ((eeprom_neutral_page-ADDR_NEUTRAL_PAGE_START) >=EEPROM_PAGE_MAX) )
                {
                    emeter.flags.eeprom_error = 0;
                    eepromError.flag.eepromIndex_neutral = TRUE;
                    //                          emeter.energy_reg_error |= EEPROM_NEUTRAL_PAGE_ERR;
                }
                else
                {
                    delay_ms(5);                 
                    ic24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE+2,eeprom_neutral_page,EEPROM_1);
                    eeprom_24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE,eeprom_neutral_page,3);
                }
            }
            else
            {
                delay_ms(5);
                eeprom_24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE,eeprom_neutral_page,3);
            }
        }
    }
    //          if((emeter.energy_reg_error & EEPROM_NEUTRAL_PAGE_ERR)==0)
    if(eepromError.flag.eepromIndex_neutral == FALSE)
    {    
        emeter.activeNeutral.forward.address =  16*(unsigned int)eeprom_neutral_page;
        //emeter.activeNeutral.copy.address  = emeter.activeNeutral.forward.address;
        delay_ms(5);
        emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address+BACKUP_OFFSET,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))                
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address+5,EEPROM_1);
                //20120722  check loi CRC lan doc thu 3
                if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))
                {
                    emeter.flags.eeprom_error = 0;
                    eepromError.flag.fwActiveReg_neutral = TRUE;
                    //                          emeter.energy_reg_error |= KWH_NEUTRAL_FORWARD_ERR;
                    
                }
            }
        }
        //              if(emeter.energy_reg_error & KWH_NEUTRAL_FORWARD_ERR)
        if(eepromError.flag.fwActiveReg_neutral == TRUE)
        {
            //                  emeter.energy_reg_error &= ~KWH_NEUTRAL_FORWARD_ERR;
            eepromError.flag.fwActiveReg_neutral = FALSE;
            emeter.activeNeutral.forward.address =  16*(unsigned int)eeprom_neutral_page;
            restart_wdt();
            delay_ms(500);
            restart_wdt();
            emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))                
            {
                
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address+BACKUP_OFFSET,EEPROM_1);
                if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))
                {
                    emeter.flags.eeprom_error = 0;
                    delay_ms(5);
                    emeter.activeNeutral.forward.value = ic24aa08_read_4byte(emeter.activeNeutral.forward.address+5,EEPROM_1);
                    //20120722  check loi CRC lan doc thu 3
                    if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.forward.value > KWH_WRAP))
                    {
                        emeter.flags.eeprom_error = 0;
                        eepromError.flag.fwActiveReg_neutral = TRUE;
                        //                              emeter.energy_reg_error |= KWH_NEUTRAL_FORWARD_ERR;
                        
                    }
                }
            }
        }
    }
    else
    {
        eepromError.flag.fwActiveReg_neutral = TRUE;
        //              emeter.energy_reg_error |= KWH_NEUTRAL_FORWARD_ERR;
    }
    //emeter.activeNeutral.copy.value = emeter.activeNeutral.forward.value;
    //lcd_display_item(DISPLAY_ITEM[0]);  //tuantm
    delay_ms(5);    
    emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE,EEPROM_1);
    if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))
    {
        emeter.flags.eeprom_error = 0;
        delay_ms(5);
        emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE+BACKUP_OFFSET,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE+5,EEPROM_1);
            //20120722  check loi CRC lan doc thu 3
            if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))
            {
                emeter.flags.eeprom_error = 0;
                eepromError.flag.rvActiveReg_neutral = TRUE;
                //                      emeter.energy_reg_error |= KWH_NEUTRAL_REVERSE_ERR;
            }
        }
    }
    //          if(emeter.energy_reg_error & KWH_NEUTRAL_REVERSE_ERR)
    if(eepromError.flag.rvActiveReg_neutral == TRUE)
    {
        //              emeter.energy_reg_error &= ~KWH_NEUTRAL_REVERSE_ERR;
        eepromError.flag.rvActiveReg_neutral = FALSE;
        restart_wdt();
        delay_ms(500);
        restart_wdt();
        emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE+BACKUP_OFFSET,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))
                
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.activeNeutral.reverse.value = ic24aa08_read_4byte(ADDR_KWH_NEUTRAL_REVERSE+5,EEPROM_1);
                //20120722  check loi CRC lan doc thu 3
                if ((emeter.flags.eeprom_error == 1)||(emeter.activeNeutral.reverse.value > KWH_WRAP))
                {
                    emeter.flags.eeprom_error = 0;
                    eepromError.flag.rvActiveReg_neutral = TRUE;
                    //                          emeter.energy_reg_error |= KWH_NEUTRAL_REVERSE_ERR;
                }
            }
        }
    }
    emeter.activeNeutral.forward.pulse_count =ic24aa08_read_byte(ADDR_KWH_NEUTRAL_PULSE_COUNT,EEPROM_1);
    if (emeter.flags.eeprom_error == 1)
    {
        emeter.flags.eeprom_error = 0;
        emeter.activeNeutral.forward.pulse_count=0;
    }
    emeter.activeNeutral.reverse.pulse_count = ic24aa08_read_byte(ADDR_KWH_NEUTRAL_REVERSE_PULSE_COUNT,EEPROM_1);
    if (emeter.flags.eeprom_error == 1)
    {
        emeter.flags.eeprom_error = 0;
        emeter.activeNeutral.reverse.pulse_count=0;
    }
#endif
    ////////////////////////////////////////////////////////////////////////////
    /*   load thong so cho live    */
    ////////////////////////////////////////////////////////////////////////////
    delay_ms(5);
    eeprom_index = (unsigned char)ic24aa08_read_byte(ADDR_EEPROM_PAGE, EEPROM_1);
    if ((emeter.flags.eeprom_error == 1) || (eeprom_index >=EEPROM_PAGE_MAX) ) // !!!
    {
        emeter.flags.eeprom_error = 0;                                      //Page_Backup_offset
        delay_ms(5);
        eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE+2,EEPROM_1);
        if((emeter.flags.eeprom_error == 1)|| (eeprom_index >=EEPROM_PAGE_MAX))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE+4,EEPROM_1);
            if((emeter.flags.eeprom_error == 1)|| (eeprom_index >=EEPROM_PAGE_MAX))                
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE_BACKUP,EEPROM_1);
                if((emeter.flags.eeprom_error == 1)|| (eeprom_index >=EEPROM_PAGE_MAX))
                {
                    emeter.flags.eeprom_error = 0;
                    eepromError.flags.eepromIndex = TRUE;
                    //                          emeter.energy_reg_error |= EEPROM_INDEX_ERR;
                }
                else
                {
                    delay_ms(5);
                    ic24aa08_write_byte(ADDR_EEPROM_PAGE+2,eeprom_index,EEPROM_1);
                    ic24aa08_write_byte(ADDR_EEPROM_PAGE+4,eeprom_index,EEPROM_1);
                    eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
                }                
            }
            else
            {
                delay_ms(5);
                ic24aa08_write_byte(ADDR_EEPROM_PAGE+2,eeprom_index,EEPROM_1);
                eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
            }
        }
        else
        {
            delay_ms(5);
            eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
        }
        //20080930
    }
    //          if(emeter.energy_reg_error & EEPROM_INDEX_ERR)
    if(eepromError.flags.eepromIndex == TRUE)
    {
        eepromError.flags.eepromIndex = FALSE;
        //              emeter.energy_reg_error &= ~EEPROM_INDEX_ERR;
        restart_wdt();
        delay_ms(500);
        restart_wdt();
        eeprom_index = (unsigned char)ic24aa08_read_byte(ADDR_EEPROM_PAGE, EEPROM_1);
        if ((emeter.flags.eeprom_error == 1) || (eeprom_index >=EEPROM_PAGE_MAX) ) // !!!
        {
            emeter.flags.eeprom_error = 0;                                      //Page_Backup_offset
            delay_ms(5);
            eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE+2,EEPROM_1);
            if((emeter.flags.eeprom_error == 1) || (eeprom_index >=EEPROM_PAGE_MAX) )                
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE+4,EEPROM_1);
                if((emeter.flags.eeprom_error == 1) || (eeprom_index >=EEPROM_PAGE_MAX))                    
                {
                    emeter.flags.eeprom_error = 0;
                    delay_ms(5);
                    eeprom_index = ic24aa08_read_byte(ADDR_EEPROM_PAGE_BACKUP,EEPROM_1);
                    if((emeter.flags.eeprom_error == 1) || (eeprom_index >=EEPROM_PAGE_MAX))
                    {
                        emeter.flags.eeprom_error = 0;
                        eepromError.flags.eepromIndex = TRUE;
                        //                              emeter.energy_reg_error |= EEPROM_INDEX_ERR;
                    }
                    else
                    {
                        delay_ms(5);
                        ic24aa08_write_byte(ADDR_EEPROM_PAGE+2,eeprom_index,EEPROM_1);
                        ic24aa08_write_byte(ADDR_EEPROM_PAGE+4,eeprom_index,EEPROM_1);
                        eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
                    }                    
                }
                else
                {
                    delay_ms(5);
                    ic24aa08_write_byte(ADDR_EEPROM_PAGE+2,eeprom_index,EEPROM_1);
                    eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
                }
            }
            else
            {
                delay_ms(5);
                eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
            }
            //20080930
        }
    }
    //          if((emeter.energy_reg_error & EEPROM_INDEX_ERR)==0)
    if(eepromError.flags.eepromIndex == FALSE)
    {
        emeter.active.forward.address =  16*(unsigned int)eeprom_index;
        tmp	   = emeter.active.forward.address;
        ////////////////////////////////////////////////////////////////////////////
        delay_ms(5);
        emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address+BACKUP_KWH_OFFSET,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address+5,EEPROM_1);
                //20120722  check loi CRC lan doc thu 3
                if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))
                {
                    emeter.flags.eeprom_error = 0;
                    eepromError.flags.fwActiveReg = TRUE;
                    //                          emeter.energy_reg_error |= KWH_FORWARD_ERR;
                }
            }
        }
        //              if(emeter.energy_reg_error & KWH_FORWARD_ERR)
        if(eepromError.flags.fwActiveReg == TRUE)
        {
            //                  emeter.energy_reg_error &= ~KWH_FORWARD_ERR;
            eepromError.flags.fwActiveReg = FALSE;
            emeter.active.forward.address =  16*(unsigned int)eeprom_index;
            restart_wdt();
            delay_ms(500);
            restart_wdt();
            emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))                
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address+BACKUP_KWH_OFFSET,EEPROM_1);
                if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))
                    
                {
                    emeter.flags.eeprom_error = 0;
                    delay_ms(5);
                    emeter.active.forward.value = ic24aa08_read_4byte(emeter.active.forward.address+5,EEPROM_1);
                    //20120722  check loi CRC lan doc thu 3
                    if ((emeter.flags.eeprom_error == 1)||(emeter.active.forward.value > KWH_WRAP))
                    {
                        emeter.flags.eeprom_error = 0;
                        eepromError.flags.fwActiveReg = TRUE;
                        //                              emeter.energy_reg_error |= KWH_FORWARD_ERR;
                    }
                }
            }
        }
    }
    else
    {
        eepromError.flags.fwActiveReg = TRUE;
        //              emeter.energy_reg_error |= KWH_FORWARD_ERR;
    }
    //emeter.active.copy.value = emeter.active.forward.value;
    //lcd_display_item(DISPLAY_ITEM[0]);  //tuantm
    ////////////////////////////////////////////////////////////////////////////
    delay_ms(5);
    emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE,EEPROM_1);
    if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))        
    {
        emeter.flags.eeprom_error = 0;
        delay_ms(5);
        emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE_BACKUP, EEPROM_1);//(ADDR_KWH_REVERSE+BACKUP_OFFSET,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE+5,EEPROM_1);
            //20120722  check loi CRC lan doc thu 3
            if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))
            {
                emeter.flags.eeprom_error = 0;
                //                      emeter.energy_reg_error |= KWH_REVERSE_ERR;
                eepromError.flags.rvActiveReg = TRUE;
            }
        }
    }
    //          if(emeter.energy_reg_error & KWH_REVERSE_ERR)
    if(eepromError.flags.rvActiveReg == TRUE)
    {
        //              emeter.energy_reg_error &= ~KWH_REVERSE_ERR;
        eepromError.flags.rvActiveReg = FALSE;
        restart_wdt();
        delay_ms(500);
        restart_wdt();
        emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE,EEPROM_1);
        if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))            
        {
            emeter.flags.eeprom_error = 0;
            delay_ms(5);
            emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE_BACKUP, EEPROM_1);//(ADDR_KWH_REVERSE+BACKUP_OFFSET,EEPROM_1);
            if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))
            {
                emeter.flags.eeprom_error = 0;
                delay_ms(5);
                emeter.active.reverse.value = ic24aa08_read_4byte(ADDR_KWH_REVERSE+5,EEPROM_1);
                //20120722  check loi CRC lan doc thu 3
                if ((emeter.flags.eeprom_error == 1)||(emeter.active.reverse.value > KWH_WRAP))
                {
                    emeter.flags.eeprom_error = 0;
                    //                          emeter.energy_reg_error |= KWH_REVERSE_ERR;
                    eepromError.flags.rvActiveReg = TRUE;
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////
//  Function : main
//  Comment  : tang 1kwh thi luu vao EEPROM mot lan
//  Input   :   None
//  Output  :   None
/////////////////////////////////////////////////////////////////////
void eeprom_data_save_task(void)
{
//	if(emeter.task.save_data == 0) return;
    uint8_t index_temp;
#ifdef RF_SUPPORT
    if(((emeter.task.save_data&0x0F)== 0)||(rf.status == TX)||(rf.flags.received == TRUE)) return;
#else
    if((emeter.task.save_data&0x0F)== 0) return;
#endif
    eeprom_wp_dir_output();
    eeprom_power_high();//namhv: khong ro~
    _DINT();
#ifdef RF_SUPPORT
    #ifdef CC1101_MODULE_SUPPORT
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(2);
    cc1101_cmd_strobe(CC1101_SPWD);   
    #endif
#endif
////////////////////// KWH FORWARD /////////////////////////////
    if(emeter.task.save_data & FLAG_SAVE_DATA_FORWARD)
    {
        emeter.task.save_data &= ~FLAG_SAVE_DATA_FORWARD;
//        if((emeter.energy_reg_error & KWH_FORWARD_ERR)==0)
        if(eepromError.flags.fwActiveReg == FALSE)
        {
            #if defined(_DT01P_RF_) && defined(ENERGY_5_INT_1_DECIMAL_SUPPORT)
            index_temp = eeprom_24aa08_get_index(emeter.active.forward.value);
            if (index_temp != eeprom_index)
            {
                eeprom_index = index_temp;
            #else
            if (*((unsigned char*)&emeter.active.forward.value+3) != eeprom_index)
            {
                eeprom_index = *((unsigned char*)&emeter.active.forward.value+3);    
            #endif
                _DINT();
                ic24aa08_write_byte(ADDR_EEPROM_PAGE+2,eeprom_index,EEPROM_1);
                ic24aa08_write_byte(ADDR_EEPROM_PAGE+4,eeprom_index,EEPROM_1);
                eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE,eeprom_index,3);
                delay_ms(5);
                eeprom_24aa08_write_byte(ADDR_EEPROM_PAGE_BACKUP,eeprom_index,3);
                emeter.active.forward.address  =  16*(unsigned int)eeprom_index;
                tmp  = emeter.active.forward.address;
            }
            _DINT();
            ic24aa08_write_4byte(emeter.active.forward.address+5,emeter.active.forward.value,EEPROM_1);
            ic24aa08_write_4byte(emeter.active.forward.address+BACKUP_KWH_OFFSET,emeter.active.forward.value,EEPROM_1);
            eeprom_24aa08_write_4byte(emeter.active.forward.address, emeter.active.forward.value,3);
            delay_ms(3);
            //_EINT();
        }
    }
    ////////////////////// KWH REVERSE ////////////////////////////////          
    if(emeter.task.save_data & FLAG_SAVE_DATA_REVERSE)
    {
        emeter.task.save_data &= ~FLAG_SAVE_DATA_REVERSE;
//        if((emeter.energy_reg_error & KWH_REVERSE_ERR)==0)
        if(eepromError.flags.rvActiveReg == FALSE)
        {
            _DINT();
            ic24aa08_write_4byte(ADDR_KWH_REVERSE+5,emeter.active.reverse.value,EEPROM_1);
            ic24aa08_write_4byte(ADDR_KWH_REVERSE_BACKUP,emeter.active.reverse.value,EEPROM_1);
            eeprom_24aa08_write_4byte(ADDR_KWH_REVERSE,emeter.active.reverse.value,3);
            delay_ms(3);
        }
    }
    ///////////////////////////////////////////////////////////////////////////
    #ifdef NEUTRAL_ENEGRY_SUPPORT
    ////////////////////// KWH FORWARD NEUTRAL ////////////////////////////
    if(emeter.task.save_data & FLAG_SAVE_NEUTRAL_DATA_FORWARD)
    {
        emeter.task.save_data &= ~FLAG_SAVE_NEUTRAL_DATA_FORWARD;
//        if((emeter.energy_reg_error & KWH_NEUTRAL_FORWARD_ERR)==0)
        if(eepromError.flag.fwActiveReg_neutral == FALSE)
        {
            if ((*((unsigned char*)&emeter.activeNeutral.forward.value+3)+ADDR_NEUTRAL_PAGE_START) != eeprom_neutral_page)
            {
                eeprom_neutral_page = *((unsigned char*)&emeter.activeNeutral.forward.value+3)+ADDR_NEUTRAL_PAGE_START;
                _DINT();

                //if (token1 != 3) {Disable_interrupts(GLOBAL); while(1); }
                ic24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE+2,eeprom_neutral_page,EEPROM_1);
                ic24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE+4,eeprom_neutral_page,EEPROM_1);
                eeprom_24aa08_write_byte(ADDR_NEUTRAL_EEPROM_PAGE,eeprom_neutral_page,3);
                emeter.activeNeutral.forward.address  =  16*(unsigned int)eeprom_neutral_page;
            }
            _DINT();
            ic24aa08_write_4byte(emeter.activeNeutral.forward.address+5,emeter.activeNeutral.forward.value,EEPROM_1);
            ic24aa08_write_4byte(emeter.activeNeutral.forward.address+BACKUP_OFFSET,emeter.activeNeutral.forward.value,EEPROM_1);
            eeprom_24aa08_write_4byte(emeter.activeNeutral.forward.address, emeter.activeNeutral.forward.value,3);
            delay_ms(3);
            //_EINT();
        }
    }
    ////////////////////// KWH REVERSE NEUTRAL ///////////////////////////////
    if(emeter.task.save_data & FLAG_SAVE_NEUTRAL_DATA_REVERSE)
    {
        emeter.task.save_data &= ~FLAG_SAVE_NEUTRAL_DATA_REVERSE;
//        if((emeter.energy_reg_error & KWH_NEUTRAL_REVERSE_ERR)==0)
        if(eepromError.flag.rvActiveReg_neutral == FALSE)
        {
            _DINT();
            ic24aa08_write_4byte(ADDR_KWH_NEUTRAL_REVERSE+5,emeter.activeNeutral.reverse.value,EEPROM_1);
            ic24aa08_write_4byte(ADDR_KWH_NEUTRAL_REVERSE+BACKUP_OFFSET,emeter.activeNeutral.reverse.value,EEPROM_1);
            eeprom_24aa08_write_4byte(ADDR_KWH_NEUTRAL_REVERSE,emeter.activeNeutral.reverse.value,3);
            delay_ms(3);
        }
    }
    #endif
#ifdef RF_SUPPORT
    rf.status = RX;
    rf_sts_to_rx();
    rf_gdo2_interrupt_flag_clear();
    rf_gdo2_interrupt_enable();
#endif
    _EINT();		
}

//