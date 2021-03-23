/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <io.h>

#include "..\library.h"

#ifndef _IHD_
#include <emeter-toolkit.h>
#endif

#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter\emeter-main.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter\emeter-main.h"
#endif
#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter\emeter-main.h"
#endif

#ifdef _DT01P_RF_
#include "..\..\dt01p80-rf\fw\emeter\emeter-main.h"
#include "..\..\dt01p80-rf\fw\emeter\emeter-hardware.h"
#endif

#include "ic24aa08.h"
#include "..\i2c\i2c.h"
#include "..\framework\crc.h"
#include "..\rf\cc1101.h"
#include "..\framework\delay.h"
#ifndef _IHD_
#include "../../../library/1-phase-measurement/measure-struct.h"
#endif
#include "..\communication\dtp.h"
#include "..\lcd\lcd-segment-b157m0742.h"
#include "..\lcd\emeter-display.h"
#include "..\rf\hu.h"
#ifdef INT_RTC_SUPPORT  //vinh
#include "..\rtc\rtc.h"
#include "..\framework\utc.h"
#endif


uint32_t tmp; //for copy
//-----  Prototype EEprom AA08------------------------------------
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_byte
//  Created     : 21/01/2015, by tuantm
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_byte(unsigned int address, uint8_t data)
{
	unsigned char addr_tam,addr_tam1;
	unsigned int crc8;
	//status;
    
	//if (token2 != write_ext_eeprom_token) {Disable_interrupts(GLOBAL); while(1) {} }
	//address
	addr_tam=(unsigned char)(address >>7);  //dia chi cao
	addr_tam1=(unsigned char)(address % 0x100); //dia chi thap
    
	i2c_start();
	i2c_write((0xa0|(addr_tam & 0x07))&0xfe);          // I2C address
	i2c_write(addr_tam1);       
	//data
	i2c_write(data);
	//crc
	crc8 = crc8bit_buffer(&data,1);
	i2c_write(crc8);
	i2c_stop();
	delay_ms(5);//delay_ms(10);
}
//doc 1 byte tu` EEPROM 24AA08
unsigned char ic24aa08_read_byte(unsigned int address)
{
	unsigned char data,addr_tam,addr_tam1;
	unsigned char crc8;
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);
    
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) & 0xFE);       // I2C address
	i2c_write(addr_tam1);
	delay_us(10);//delay_us(5);  //ack tuantm 10us
    
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) | 1);
	//data
	data=i2c_read(1); //0x09
	//crc
	crc8 =i2c_read(0);//0x05 => fail crc, crc dung A9
	i2c_stop();
	if (crc8 != crc8bit_buffer(&data,1)) flag_eeprom_error = 1;
	return(data);
}

// ghi EEPROM lie^n tuc -> tiet kiem thoi gian save EEPROM
// ghi data 2 byte tai dia chi? address
void ic24aa08_write_2byte(unsigned int address, int data)
{
	unsigned char addr_tam,addr_tam1;
	//unsigned char crc8;
	//int1 status;
	//if (token2 != write_int16_ext_eeprom_token) {Disable_interrupts(GLOBAL); while(1) {} }
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);
    
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) & 0xFE);
	i2c_write(addr_tam1);
	//data
	i2c_write(*(&data));
	i2c_write( *(&data+1));
	//crc
	//crc8 = crc8bit_buffer(&data,2);
	//i2c_write(crc8);
	i2c_stop();
	//delay_ms(7);
    
	/*status=1;
	while(status==1)
	{
	i2c_start();
	status=i2c_write(0xa0);
}*/
	delay_ms(5);
}
//doc lien tuc du~ lie.u 2 byte tu` 24AA08
int ic24aa08_read_2byte(unsigned int address)
{
	unsigned char addr_tam,addr_tam1;
	//unsigned int crc8;
	int data;
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	//i2c_write(addr_tam & 0x1F);
	i2c_write(addr_tam1);        
	//
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) |1);
	*(&data) = i2c_read(1);
	*(&data+1) = i2c_read(0);
	//crc8  = i2c_read(0);
    
	i2c_stop();
	//if (crc8 != crc8bit_buffer(&data,2)) flag_eeprom_error = 1;
	return(data);
	//addr_tam*0xFFFF +
}

//doc lien tuc du~ lie.u 4 byte tu` 24AA08
unsigned long ic24aa08_read_4byte(unsigned int address)
{
	unsigned char addr_tam,addr_tam1;
	unsigned long data;
	unsigned char crc8;
	unsigned char tam;
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
	//for (i=0;i<3;i++)   *(&data+i) = i2c_read(1);// (i==3)?0:1);
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
	if (crc8 != crc8bit_buffer((unsigned char*)&data,4)) flag_eeprom_error = 1;
	return(data);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_4byte
//  Created     : 21/01/2015, by tuantm
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_4byte(unsigned int address, unsigned long data)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
	crc8 = crc8bit_buffer((unsigned char*)&data,4);
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);
    
	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);
	i2c_write((unsigned char)data);
	i2c_write((unsigned char)(data>>8));
	i2c_write((unsigned char)(data>>16));
	i2c_write((unsigned char)(data>>24));
	i2c_write(crc8);
	i2c_stop();
	delay_ms(10);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_read_5byte
//  Created     : 25/08/2015, by tuantm
//  Description : 
//  Input		:  
//  Output      :  
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_read_5byte_rtc(unsigned int address, uint8_t index)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char data[5];
	unsigned char crc8;
	unsigned char i;
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
	for (i=0;i<5;i++)
        data[i] = i2c_read(1);
	// *(&data+3) = 0;
	//unsigned char *p_data;
	//p_data = (unsigned char *)&data;
	//for (i = 0;  i < 3;  i++)
	//*(p_data+i) = i2c_read(1);
	crc8 =i2c_read(0);
	i2c_stop();
	if (crc8 != crc8bit_buffer((unsigned char*)&data,5)) 
        flag_eeprom_error = 1;
    if(flag_eeprom_error == 1)
    {
        ihd.data.rtc[index].hour = 0xff;
        ihd.data.rtc[index].min = 0xff;
        ihd.data.rtc[index].day = 0xff;
        ihd.data.rtc[index].month = 0xff;
        ihd.data.rtc[index].year = 0xff;
    }
    else
    {
        ihd.data.rtc[index].hour = data[0];
        ihd.data.rtc[index].min = data[1];
        ihd.data.rtc[index].day = data[2];
        ihd.data.rtc[index].month = data[3];
        ihd.data.rtc[index].year = data[4];
    }
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : ic24aa08_write_5byte
//  Created     : 21/01/2015, by tuantm
//  Description :
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_write_5yte_rtc(unsigned int address)
{
	unsigned char addr_tam,addr_tam1;
	unsigned char crc8;
    unsigned char data[5] ={rtc_int.hour, rtc_int.minute, rtc_int.day,
    rtc_int.month, rtc_int.year};
	crc8 = crc8bit_buffer((unsigned char*)&data,5);
	//address
	addr_tam=(unsigned char)(address >> 7);
	addr_tam1=(unsigned char)(address % 0x100);
    
	//while(!ext_eeprom_ready());
	i2c_start();
	i2c_write((0xa0 | (addr_tam & 0x07)) &0xFE);
	i2c_write(addr_tam1);
	i2c_write(data[0]);
	i2c_write(data[1]);
	i2c_write(data[2]);
	i2c_write(data[3]);
    i2c_write(data[4]);
	i2c_write(crc8);
	i2c_stop();
	delay_ms(10);
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : eeprom_load_parameters
//  Created     : 28/07/2014, by tuantm
//  Description : load lai gia tri cua cac thanh ghi khi reset lai
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void ic24aa08_load_parameters(void)
{        
    uint8_t i, success_count_max;
    uint32_t kwh_imp_tmp, kwh_exp_tmp;
    uint32_t para1, para2, para3, para4, para5, para6, para7, para8, para9, para10;
    _DINT();
    flag_eeprom_error = 0;
    //load chager id index
    delay_ms(5);
    ihd.data.change_id_index  = ic24aa08_read_byte(IHD_NEW_ID_INDEX_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        //ihd.epprom_error = 1;
        flag_eeprom_error = 0;
        ihd.data.change_id_index = 0xFF;
    }
    //load billing day
    delay_ms(5);
    ihd.data.billing_day = ic24aa08_read_byte(IHD_BILLING_DAY_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 2;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.data.billing_day = ic24aa08_read_byte(IHD_BILLING_DAY_ADDRESS+2); //backup
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 3;
        flag_eeprom_error = 0;
        ihd.data.billing_day = 5; //mac dinh ngay 5 hang thang
    }
    //load billing hour
    delay_ms(5);
    ihd.data.billing_hour = ic24aa08_read_byte(IHD_BILLING_HOUR_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        flag_eeprom_error = 0;
        ihd.data.billing_hour = ic24aa08_read_byte(IHD_BILLING_HOUR_ADDRESS + 2);
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 4;
        flag_eeprom_error = 0;
        ihd.data.billing_hour = 12;//mac dinh 12h
    }
    //load billing index
    delay_ms(5);
    ihd.data.billing_index = ic24aa08_read_byte(IHD_DATA_BI_INDEX_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 5;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.data.billing_index = ic24aa08_read_byte(IHD_DATA_BI_INDEX_ADDRESS+2); //backup
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 6;
        flag_eeprom_error = 0;
        ihd.data.billing_index = 0xFF; //ko co
    }
    //load full history flag
    delay_ms(5);
    ihd.data.full_bi_his = ic24aa08_read_byte(IHD_INDEX_BI_FULL_ADDRESS);
    if(flag_eeprom_error == 1)
    {
        flag_eeprom_error= 0;
        delay_ms(5);
        ihd.data.full_bi_his = ic24aa08_read_byte(IHD_INDEX_BI_FULL_ADDRESS+2);
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 7;
        flag_eeprom_error = 0;
        ihd.data.full_bi_his = 0xFF;
    }
    //load id meter, meter type
    delay_ms(5);
    ihd.meter_id = ic24aa08_read_4byte(IHD_METER_ID_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 8;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.meter_id = ic24aa08_read_4byte(IHD_METER_ID_ADDRESS+5); //backup
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 9;
        flag_eeprom_error = 0;
        ihd.meter_id = 15500001;//@@
    }
    //load meter type
    delay_ms(5);
    ihd.meter_type = ic24aa08_read_byte(IHD_METER_TYPE_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 10;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.meter_type = ic24aa08_read_byte(IHD_METER_TYPE_ADDRESS+2);//backup 1
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 11;
        flag_eeprom_error = 0;
        ihd.meter_type = DT01PRF;//@@
    }
    //load id ihd
    delay_ms(5);
    ihd.id = ic24aa08_read_4byte(IHD_ID_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 12;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.id = ic24aa08_read_4byte(IHD_ID_ADDRESS+5); //backup
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 13;
        flag_eeprom_error = 0;
        ihd.id = 1500001;//@@
    }
    //load index
    delay_ms(5);
    ihd.data.index = ic24aa08_read_byte(IHD_DATA_INDEX_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 14;
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.data.index = ic24aa08_read_byte(IHD_DATA_INDEX_ADDRESS+2);//backup 1
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 15;
        flag_eeprom_error = 0;
        ihd.data.index = EMPTY_DATA_INDEX;//bo di het ta lam lai tu dau
    }
    //load full history flag
    delay_ms(5);
    ihd.data.full_his = ic24aa08_read_byte(IHD_INDEX_FULL_ADDRESS);
    if (flag_eeprom_error == 1)
    {
        flag_eeprom_error = 0;
        delay_ms(5);
        ihd.data.full_his = ic24aa08_read_byte(IHD_INDEX_FULL_ADDRESS+2);
    }
    if (flag_eeprom_error == 1)
    {
        ihd.epprom_error = 16;
        flag_eeprom_error = 0;
        ihd.data.full_his = 0xFF;
    }
    if((ihd.meter_type == DT03MRF)||(ihd.meter_type == ELSTER_RF))
        ihd.data.rf_record_max = (RF_RECORD_MAX + BILLING_RECORD_MAX) / 5;
    else
        ihd.data.rf_record_max = RF_RECORD_MAX;
    //config
    if(ihd.config.dt03m_full_tariff)
    {
        cmd_dt03m[0] = CMD_READ_T1;
        cmd_dt03m[1] = CMD_READ_T2;
        cmd_dt03m[2] = CMD_READ_T3;
        cmd_dt03m[3] = CMD_READ_T4;
        cmd_dt03m[4] = CMD_READ_T5;
        cmd_dt03m[5] = CMD_READ_T6;
    }
    else
    {
        cmd_dt03m[0] = CMD_READ_IMPORT_KWH;
        cmd_dt03m[1] = CMD_READ_T1;
        cmd_dt03m[2] = CMD_READ_T2;
        cmd_dt03m[3] = CMD_READ_T3;
        cmd_dt03m[4] = CMD_READ_IMPORT_KVARH;
        //chieu nghich
        cmd_dt03m[5] = CMD_READ_EXPORT_KWH;   
        cmd_dt03m[6] = CMD_READ_T4;
        cmd_dt03m[7] = CMD_READ_T5;
        cmd_dt03m[8] = CMD_READ_T6;
        cmd_dt03m[9] = CMD_READ_EXPORT_KVARH;
    }
    switch(ihd.meter_type)
    {
    case DT01P0RF:
        ihd.flags.listen = TRUE;
        break;
    case DT01PRF:
        check_meter_mesh();
        break;
    case DT01P80RF:
    case DT03PRF:
        if(ihd.config.dt01p_full_para)
        {
            cmd_dt01p[0] = HHU_CMD_READALL;
            cmd_dt01p[1] = 0;
            cmd_dt01p[2] = 0;
            cmd_dt01p[3] = 0;
            cmd_dt01p[4] = 0;
            cmd_dt01p[5] = 0;
        }
        else
        {
            cmd_dt01p[0] = HHU_CMD_READALL;
            cmd_dt01p[1] = CMD_READ_PHASE_A;
            cmd_dt01p[2] = 0;
            cmd_dt01p[3] = 0;
            cmd_dt01p[4] = 0;
            cmd_dt01p[5] = 0;
        }
        break;
    case DT03MRF:
    case ELSTER_RF:
        break;
    default:
        break;
    } 
    if((ihd.meter_type == DT03MRF) || (ihd.meter_type == ELSTER_RF))
    {
        ihd.flags.dtm = 1;
        ihd.data.billing_index = 0xFF;
        ihd.data.full_bi_his = 0xFF;
        //load all data - manual read
        if(ihd.data.index < (RF_RECORD_MAX+ BILLING_RECORD_MAX)) 
        {
            if(ihd.data.full_his == FULL_DATA_HIS)
                success_count_max = ihd.data.rf_record_max - 1;
            else
                success_count_max = ihd.data.index;
            for(i = 0; i<= success_count_max; i++)
            {
                //data
                delay_ms(5);
                para1 = ic24aa08_read_4byte(16*5*i);
                delay_ms(5);
                para2 = ic24aa08_read_4byte(16*5*i + 16);
                delay_ms(5);
                para3 = ic24aa08_read_4byte(16*5*i + 32);
                delay_ms(5);
                para4 = ic24aa08_read_4byte(16*5*i + 48);
                delay_ms(5);
                para5 = ic24aa08_read_4byte(16*5*i + 64);
                
                delay_ms(5);
                para6 = ic24aa08_read_4byte(16*5*i + 5);
                delay_ms(5);
                para7 = ic24aa08_read_4byte(16*5*i + 5 + 16);
                delay_ms(5);
                para8 = ic24aa08_read_4byte(16*5*i + 5 + 32);
                delay_ms(5);
                para9 = ic24aa08_read_4byte(16*5*i + 5 + 48);
                delay_ms(5);
                para10 = ic24aa08_read_4byte(16*5*i + 5 + 64);
                delay_ms(5);
                //rtc
                ic24aa08_read_5byte_rtc(16*5*i + 10, 5*i);
                restart_watchdog();
                if(flag_eeprom_error == 0) //neu khong co loi thi cho vao bie'n
                {
                    ihd.data.kwh_import[5*i] = para1;
                    ihd.data.kwh_import[5*i+1] = para2;
                    ihd.data.kwh_import[5*i+2] = para3;
                    ihd.data.kwh_import[5*i+3] = para4;
                    ihd.data.kwh_import[5*i+4] = para5;
                    
                    ihd.data.kwh_export[5*i] = para6;
                    ihd.data.kwh_export[5*i+1] = para7;
                    ihd.data.kwh_export[5*i+2] = para8;
                    ihd.data.kwh_export[5*i+3] = para9;
                    ihd.data.kwh_export[5*i+4] = para10;
                }
                else
                {
                    ihd.epprom_error = 18;
                    flag_eeprom_error = 0;
                    ihd.data.kwh_import[5*i] = 0xFFFFFFFF;
                    ihd.data.kwh_import[5*i+1] = 0xFFFFFFFF;
                    ihd.data.kwh_import[5*i+2] = 0xFFFFFFFF;
                    ihd.data.kwh_import[5*i+3] = 0xFFFFFFFF;
                    ihd.data.kwh_import[5*i+4] = 0xFFFFFFFF;
                    
                    ihd.data.kwh_export[5*i] = 0xFFFFFFFF;
                    ihd.data.kwh_export[5*i+1] = 0xFFFFFFFF;
                    ihd.data.kwh_export[5*i+2] = 0xFFFFFFFF;
                    ihd.data.kwh_export[5*i+3] = 0xFFFFFFFF;
                    ihd.data.kwh_export[5*i+4] = 0xFFFFFFFF;
                }
            }
        }
        else
            ihd.data.index = EMPTY_DATA_INDEX;
    }
    else
    {
        ihd.flags.dtm = 0;
        //load all data - manual read
        if(ihd.data.index < RF_RECORD_MAX) 
        {
            if(ihd.data.full_his == FULL_DATA_HIS)
                success_count_max = ihd.data.rf_record_max - 1;
            else
                success_count_max = ihd.data.index;
            for(i = 0; i<= success_count_max; i++)
            {
                kwh_imp_tmp = 
                    ic24aa08_read_4byte(IHD_DATA_START_ADDRESS + 16*i);
                delay_ms(5);
                kwh_exp_tmp = 
                    ic24aa08_read_4byte(IHD_DATA_START_ADDRESS + 16*i + 5);
                delay_ms(5);
                ic24aa08_read_5byte_rtc(IHD_DATA_START_ADDRESS + 16*i + 10, i + BILLING_RECORD_MAX);
                restart_watchdog();
                if(flag_eeprom_error == 0) //neu khong co loi thi cho vao bie'n
                {
                    ihd.data.kwh_import[i + BILLING_RECORD_MAX] = kwh_imp_tmp;
                    ihd.data.kwh_export[i + BILLING_RECORD_MAX] = kwh_exp_tmp;
                }
                else
                {
                    ihd.epprom_error = 19;
                    flag_eeprom_error = 0;
                    ihd.data.kwh_import[i + BILLING_RECORD_MAX] = 0xFFFFFFFF;
                    ihd.data.kwh_export[i + BILLING_RECORD_MAX] = 0xFFFFFFFF; 
                }
            }
        }
        else
            ihd.data.index = EMPTY_DATA_INDEX;
        //load all data - auto read
        if(ihd.data.billing_index < BILLING_RECORD_MAX) 
        {
            if(ihd.data.full_bi_his == FULL_DATA_HIS)
                success_count_max = BILLING_RECORD_MAX - 1;
            else
                success_count_max = ihd.data.billing_index;
            for(i = 0; i<= success_count_max; i++)
            {
                delay_ms(5);
                kwh_imp_tmp = 
                    ic24aa08_read_4byte(IHD_BILLING_DATA_START_ADDRESS + 16*i);
                delay_ms(5);
                kwh_exp_tmp = 
                    ic24aa08_read_4byte(IHD_BILLING_DATA_START_ADDRESS + 16*i + 5);
                delay_ms(5);
                ic24aa08_read_5byte_rtc(IHD_BILLING_DATA_START_ADDRESS + 16*i + 10, i);
                restart_watchdog();
                if(flag_eeprom_error == 0) //neu khong co loi thi cho vao bie'n
                {
                    ihd.data.kwh_import[i] = kwh_imp_tmp;
                    ihd.data.kwh_export[i] = kwh_exp_tmp;
                }
                else
                {
                    ihd.epprom_error = 20;
                    flag_eeprom_error = 0;
                    ihd.data.kwh_import[i] = 0xFFFFFFFF;
                    ihd.data.kwh_export[i] = 0xFFFFFFFF;
                }
            }
        }
        else
            ihd.data.billing_index = EMPTY_DATA_INDEX;
    }
}

/////////////////////////////////////////////////////////////////////
//  Function : main
//  Comment  : tang 1kwh thi luu vao EEPROM mot lan
//  Input   :   None
//  Output  :   None
/////////////////////////////////////////////////////////////////////
void eeprom_data_save_task(void)
{
	if(ihd.task.save_data == 0) return;
}
