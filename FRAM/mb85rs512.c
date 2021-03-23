/*===========================================================================*/
/*
SOURCE_FILE:		mb85rs512.c
APPLICATION:		DT03M-RF
MODULE:				fram
ORIGINAL AUTHOR:	vuonglq
VERSION:			1.0
DATE:				25/07/2018

(C) 2013: COPCEMEC

All rights are reserved. Reproduction in whole or in part is
prohibited without the prior written consent of the copyright
owner. The information presented in this document does not
form part of any quotation or contract, is believed to be
accurate and reliable and may be changed without notice.
No liability will be accepted by the publisher for any
consequence of its use. Publication thereof does not convey
nor imply any license under patent- or other industrial or
intellectual property rights.
*/

/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#ifdef __MSP430__
#include <io.h>
#include <emeter-toolkit.h>
#endif
#include "..\library.h"
#ifndef UINT64_SUPPORT
#include "../../Elster-RF/FW/FWELSTERRFX/driver/uint64.h"
#endif

#if defined(_DT03M_RF_) && defined(__MSP430__)
#include "..\..\dt03m-rf\fw\emeter-main.h"
#endif
#if defined(_DT03M_RF_) && defined(__PIC24FJxxxGA410__)
#include "../../DT03M-RF/FW/main.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-main.h"
#endif
#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter-main.h"
#endif
#ifdef _DT01PP_RF_
#include "..\..\dt01pp\fw\emeter-main.h"
#include "..\..\dt01pp\fw\emeter-hardware.h"
#endif
#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#include "../../ROUTER/FW/hardware.h"
#endif
#ifdef _DCU_
#include "../../DCU/FW/main.h"
#include "../../DCU/FW/hardware.h"
#endif

#ifdef _LBS_
#include "../../LBS/FW/main.h"
#include "../../LBS/FW/hardware.h"
#endif

#ifdef _FCI_
#include "../../RemoteAlarm/FW/main.h"
#include "../../RemoteAlarm/FW/hardware.h"
#endif
#ifdef _RF_SLOT_
#include "../../Elster-RF/FW/FWELSTERRFX/main.h"
#include "../../Elster-RF/FW/FWELSTERRFX/driver/hardware.h"
#include "../../Elster-RF/FW/FWELSTERRFX/driver/fram.h"
#endif
#ifdef FRAM_SUPPORT
//#include "rs512.h"
#include "mb85rs512.h"
#include "..\spi\spi.h"
#include "..\framework\crc.h"
#include "..\framework\delay.h"
#include "../framework/macro.h"


/*===========================================================================*/
/* L O C A L   F U N C T I O N   P R O T O T Y P E S                         */
/*===========================================================================*/
//===========================================================================
//FUNCTION NAME:  rs512_chip_enable
//PROTOTYPE:      int rs512_chip_enable(unsigned char framIndex)
//INPUT:          framIndex: chon fram
//OUTPUT:         true/false
//DESCRIPTION:    active CS pin of fram
//NOTE:
//=============================================================================
int rs512_chip_enable(unsigned char framIndex)
{
    switch (framIndex)
    {
        #ifdef FRAM_1_SUPPORT
        case 1:
            fram_cs1_low();
            break;
        #endif
        #ifdef FRAM_2_SUPPORT
        case 2:
            fram_cs2_low();
            break;
        #endif
        #ifdef FRAM_3_SUPPORT
        case 3:
            fram_cs3_low();
            break;
        #endif
        #ifdef FRAM_4_SUPPORT
        case 4:
            fram_cs4_low();
            break;
        #endif
        default:
            return false;
    }
    return true;
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int rs512_chip_disable(unsigned char framIndex)
{
    switch (framIndex)
    {
        #ifdef FRAM_1_SUPPORT
        case 1:
            fram_cs1_high();
            break;
        #endif
        #ifdef FRAM_2_SUPPORT
        case 2:
            fram_cs2_high();
            break;
        #endif
        #ifdef FRAM_3_SUPPORT
        case 3:
            fram_cs3_high();
            break;
        #endif
        #ifdef FRAM_4_SUPPORT
        case 4:
            fram_cs4_high();
            break;
        #endif
        default:
            return false;
    }
    return true;
}

//===========================================================================
//FUNCTION NAME:  checksum
//PROTOTYPE:      unsigned char checksum(void *buf, int len)
//INPUT:          buf: bien con tro, tro toi bo dem can tinh checksum
//                len chieu dai bo dem
//OUTPUT:         ket qua checksum
//DESCRIPTION:
//NOTE:
//=============================================================================
//unsigned char checksum(void *buf, int len)
//{
//    unsigned char *st;
//    unsigned char sum;
//    sum = 0;
//    st = (unsigned char *) buf;
//    while (len-- > 0)
//    {
//        sum += *st++;
//    }
//    return((unsigned char) (sum+0x55));
//}

//===========================================================================
//FUNCTION NAME:  rs512_read_status
//PROTOTYPE:      unsigned char rs512_read_status(unsigned char framIndex)
//INPUT:          framIndex: chon fram
//OUTPUT:         byte status cua fram
//DESCRIPTION:
//NOTE:
//=============================================================================
unsigned char rs512_read_status(unsigned char framIndex)
{
    spi_disable();
    unsigned char status;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_RDSR);
    status = spi_get_byte();
    if (rs512_chip_disable(framIndex) == false) return false;
    return(status);
}

//===========================================================================
//FUNCTION NAME:  rs512_write_status
//PROTOTYPE:      int rs512_write_status(char status, unsigned char framIndex)
//INPUT:          status: gia tri can ghi
//                framIndex: chon fram
//OUTPUT:         true: thanh cong
//DESCRIPTION:    ghi gia tri status vao fram
//NOTE:
//=============================================================================
int rs512_write_status(char status, unsigned char framIndex)
{
    spi_disable();
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WRSR);
    spi_send_byte(status);
    if (rs512_chip_disable(framIndex) == false) return false;
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_erase
//PROTOTYPE:      int rs512_erase(unsigned char framIndex)
//INPUT:          framIndex: chon fram
//OUTPUT:         true: thanh cong
//DESCRIPTION:    xoa toan bo fram
//NOTE:
//=============================================================================
//int rs512_erase(unsigned char framIndex)
//{
//    spi_disable();
//    if (rs512_chip_enable(framIndex) == false) return false;
//    spi_send_byte(RS512_WREN);
//    if (rs512_chip_disable(framIndex) == false) return false;
//    delay_us(20);
//    if (rs512_chip_enable(framIndex) == false) return false;
//    spi_send_byte(CHIP_ERASE);
//    if (rs512_chip_disable(framIndex) == false) return false;
//    rs512_check_write_complete(framIndex);    
//    return true;
//}

void rs512_check_write_complete(unsigned char framIndex)
{
#ifdef CHECK_WIP_BIT
    uint8_t status = 0;
    status = rs512_read_status(framIndex);
    uint8_t i = 0;
    while ((i<50)&&(status & 0x01)) //qua 10ms ma ko ghi xong thi huy
    {
        i++;
        status = rs512_read_status(framIndex);
        delay_us(200);
    }
    if (i >= 50)
       i++; //debug
#else
    delay_ms(T_wc);
#endif
}
//==============================================================================
//FUNCTION NAME:  rs512_write_byte
//PROTOTYPE:      int rs512_write_byte(uint32_t addr, uint8_t value, uint8_t framIndex)
//INPUT:          addr: dia chi bat dau
//                value: gia tri can ghi
//                framIndex: chon fram
//OUTPUT:         true: ghi thanh cong
//DESCRIPTION:    ghi gia tri 1 byte va 1 byte crc vao fram
//NOTE:
//==============================================================================
int rs512_write_byte(uint32_t addr, uint8_t value, uint8_t framIndex)
{
    spi_disable();
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(value);
    spi_send_byte(crc8bit_buffer(&value,1));
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_read_byte
//PROTOTYPE:      unsigned char rs512_read_byte(uint32_t addr,uint8_t framIndex)
//INPUT:          addr: dia chi bat dau
//                framIndex: chon fram de doc
//OUTPUT:         gia tri tai dia chi can doc
//DESCRIPTION:
//NOTE:
//=============================================================================
unsigned char rs512_read_byte(uint32_t addr,uint8_t framIndex)
{
    spi_disable();
    unsigned char buffer[2];
    unsigned char value = 0;
    if (rs512_chip_enable(framIndex) == false) return 0;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    
    buffer[0] = spi_get_byte();
    buffer[1] = spi_get_byte();//byte crc
    if (rs512_chip_disable(framIndex) == false) return 0;
    if (crc8bit_buffer(buffer,2) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    value = buffer[0];
   return(value);//buffer[0]
}

//===========================================================================
//FUNCTION NAME:  rs512_write_2byte
//PROTOTYPE:      int rs512_write_2byte(uint32_t addr, uint16_t value, uint8_t framIndex)
//INPUT:          addr: dia chi can ghi
//                value: gia tri 2 byte can ghi
//                framIndex: chon fram
//OUTPUT:         true: ghi thanh cong
//DESCRIPTION:    ghi gia tri 2 byte va 1 byte crc vao fram
//NOTE:
//=============================================================================
int rs512_write_2byte(uint32_t addr, uint16_t value, uint8_t framIndex)
{
    spi_disable();
    uint16_t data = value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(crc8bit_buffer((unsigned char *)&data,2));
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_read_2byte
//PROTOTYPE:      uint16_t rs512_read_2byte(uint32_t addr,uint8_t framIndex)
//INPUT:          addr: dia chi can doc
//                framIndex: chon fram
//OUTPUT:         gia tri 2 byte can doc
//DESCRIPTION:
//NOTE:
//=============================================================================
uint16_t rs512_read_2byte(uint32_t addr, uint8_t framIndex)
{
    spi_disable();
    unsigned char buffer[3];
    uint16_t *data;
    if (rs512_chip_enable(framIndex) == false) return 0;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();//byte cao
    buffer[2] = spi_get_byte();//byte crc
    if (rs512_chip_enable(framIndex) == false) return 0;
    if (crc8bit_buffer(buffer,3) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    data = (uint16_t *)buffer;
    return(*data);
}

//===========================================================================
//FUNCTION NAME:  rs512_write_3byte
//PROTOTYPE:      int rs512_write_3byte(uint32_t addr, uint32_t value,
//                                                    unsigned char framIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:    ghi gia tri 3 byte va 1 byte crc vao fram
//=============================================================================
int rs512_write_3byte(uint32_t addr, uint32_t value,uint8_t framIndex)
{
    spi_disable();
    uint32_t data = value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(data >> 16);

    spi_send_byte(crc8bit_buffer(&data,3));
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_read_3byte
//PROTOTYPE:      uint32_t rs512_read_3byte(uint32_t addr,uint8_t framIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint32_t rs512_read_3byte(uint32_t addr, uint8_t framIndex)
{
    spi_disable();
    unsigned char buffer[4];
    uint32_t *data;
    data = 0;
    if (rs512_chip_enable(framIndex) == false) return 0;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();
    buffer[2] = spi_get_byte();//byte cao
    buffer[3] = spi_get_byte();//byte crc
    if (rs512_chip_disable(framIndex) == false) return 0;
    if (crc8bit_buffer(buffer,4) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    buffer[3] = 0;
    data = (unsigned long *)buffer;
    return(*data);
}

//===========================================================================
//FUNCTION NAME:  rs512_write_4byte
//PROTOTYPE:      int rs512_write_4byte(uint32_t addr, uint32_t value, uint8_t framIndex)
//INPUT:          addr: dia chi can doc
//                value: gia tri can ghi
//                framIndex: chon fram
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int rs512_write_4byte(uint32_t addr, uint32_t value, uint8_t framIndex)
{
    spi_disable();
    uint32_t data = value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);

    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(data >> 16);
    spi_send_byte(data >> 24);

    spi_send_byte(crc8bit_buffer(&data,4));
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_read_4byte
//PROTOTYPE:      uint32_t rs512_read_4byte(uint32_t addr, uint8_t framIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint32_t rs512_read_4byte(uint32_t addr, uint8_t framIndex)
{
    spi_disable();
    unsigned char buffer[5];
    uint32_t *data;
    if (rs512_chip_enable(framIndex) == false) return 0;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();
    buffer[2] = spi_get_byte();
    buffer[3] = spi_get_byte();//byte cao
    buffer[4] = spi_get_byte();//byte crc
    if (rs512_chip_disable(framIndex) == false) return 0;
    if (crc8bit_buffer(buffer,5) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    data = (unsigned long *)buffer;
    return(*data);
}
#ifndef _RF_SLOT_ 
//===========================================================================
//FUNCTION NAME:  rs512_write_8byte
//PROTOTYPE:      int rs512_write_8byte(uint32_t addr, uint64_t value, uint8_t framIndex)
//INPUT:          addr: dia chi can doc
//                value: gia tri can ghi
//                framIndex: chon fram
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int rs512_write_8byte(uint32_t addr, uint64_t value, uint8_t framIndex)
{
    spi_disable();
    uint64_t data = value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);

    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte((uint8_t)addr >> 8);
    spi_send_byte((uint8_t)addr);

    spi_send_byte((uint8_t)data);
    spi_send_byte((uint8_t)(data >> 8));
    spi_send_byte((uint8_t)(data >> 16));
    spi_send_byte((uint8_t)(data >> 24));
    spi_send_byte((uint8_t)(data >> 32));
    spi_send_byte((uint8_t)(data >> 40));
    spi_send_byte((uint8_t)(data >> 48));
    spi_send_byte((uint8_t)(data >> 56));

    spi_send_byte(crc8bit_buffer(&data,8));
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  rs512_read_8byte
//PROTOTYPE:      uint64_t rs512_read_8byte(uint32_t addr, uint8_t framIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint64_t rs512_read_8byte(uint32_t addr, uint8_t framIndex)
{
    spi_disable();
    unsigned char buffer[9];
    uint64_t *data;
    if (rs512_chip_enable(framIndex) == false) return 0;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();
    buffer[2] = spi_get_byte();
    buffer[3] = spi_get_byte();
    buffer[4] = spi_get_byte();
    buffer[5] = spi_get_byte();
    buffer[6] = spi_get_byte();
    buffer[7] = spi_get_byte();//byte cao
    buffer[8] = spi_get_byte();//byte crc
    if (rs512_chip_disable(framIndex) == false) return 0;
    if (crc8bit_buffer(buffer,9) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif        
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    data = (uint64_t *)buffer;
    return(*data);
}
#endif
//===========================================================================
//FUNCTION NAME:  rs512_write_burst_pages
//PROTOTYPE:      int rs512_write_burst_pages(uint32_t addr,void *value,
//                                            unsigned long len,uint8_t framIndex)
//INPUT:          addr: dia chi can luu
//                *value: con tro toi bo dem can luu
//                len: chieu dai bo dem can luu
//                framIndex: chon fram
//OUTPUT:
//DESCRIPTION:    ghi chuoi nhieu byte, co the ghi sang trang khac cua fram
//NOTE:
//=============================================================================
int rs512_write_burst_pages(uint32_t addr, void *value, uint32_t len, uint8_t framIndex)
{
    spi_disable();
    int i;
    uint8_t *data;
    uint8_t *q;
    q = (uint8_t *) value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    //ghi data
    data = q;
    for (i = 0; i < len+1; i++)
    {
        if (i < len) spi_send_byte(*data++);
        else 
        {
            //q = (uint8_t *) value;
            spi_send_byte(crc8bit_buffer((void *)q,len));
        }
    }
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    
    return true;
}

////eeprom, phan page
//int rs512_write_burst_pages(uint32_t addr, void *value, uint32_t len, uint8_t framIndex)
//{
//    spi_disable();
//    int i;
//    uint8_t *data;
//    unsigned long section_len, size;
//    uint8_t *q;
//
//    q = (uint8_t *) value;
//    size = len;
//    // If the write spreads across pages in the EEPROM, we need to split the write
//    //   into sections.
//    while (len > 0)
//    {
//        if (addr + len > ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)))
//        {
//            section_len = ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)) - addr;
//        }
//        else
//        {
//            section_len = len;
//        }
//
//        if (rs512_chip_enable(framIndex) == false) return false;
//        spi_send_byte(RS512_WREN);
//        //        ________
//        //       /        \           <== CS# must be driven high before
//        //______/          \_________     the instruction is executed
//        if (rs512_chip_disable(framIndex) == false) return false;
//        delay_us(20);
//
//        if (rs512_chip_enable(framIndex) == false) return false;
//        spi_send_byte(RS512_PROGRAM);
//#ifdef FRAM_RS1024 
//    spi_send_byte(addr >> 16);
//#endif
//        spi_send_byte(addr >> 8);
//        spi_send_byte(addr);
//
//        data = q;
//        for (i = 0; i < section_len; i++)
//        {
//            spi_send_byte(*data++);
//        }
//        len -= section_len;
//        addr += section_len;
//        q += section_len;
//        if (rs512_chip_disable(framIndex) == false) return false;
//        //delay_ms(T_wc);
//		rs512_check_write_complete(framIndex);
//        if (len == 0)
//        {
//            q = (uint8_t *) value;
//            if (rs512_chip_enable(framIndex) == false) return false;
//            spi_send_byte(RS512_WREN);
//            //        ________
//            //       /        \           <== CS# must be driven high before
//            //______/          \_________     the instruction is executed
//            if (rs512_chip_disable(framIndex) == false) return false;
//            delay_us(20);
//
//            if (rs512_chip_enable(framIndex) == false) return false;
//            spi_send_byte(RS512_PROGRAM);
//            
//            #ifdef FRAM_RS1024 
//                spi_send_byte(addr >> 16);
//            #endif
//            spi_send_byte(addr >> 8);
//            spi_send_byte(addr);
//            spi_send_byte(crc8bit_buffer((void *)q,size));
//            if (rs512_chip_disable(framIndex) == false) return false;
//            //delay_ms(T_wc);
//			rs512_check_write_complete(framIndex);
//        }
//    }
//    return true;
//}
//===========================================================================
//FUNCTION NAME:  rs512_write_burst_pages_nocrc
//PROTOTYPE:      int rs512_write_burst_pages_nocrc(uint32_t addr,void *value,
//                                            unsigned long len,uint8_t framIndex)
//INPUT:          addr: dia chi can luu
//                *value: con tro toi bo dem can luu
//                len: chieu dai bo dem can luu
//                framIndex: chon fram
//OUTPUT:
//DESCRIPTION:    ghi chuoi nhieu byte, co the ghi sang trang khac cua fram
//NOTE:
//=============================================================================
int rs512_write_burst_pages_nocrc(uint32_t addr, void *value, uint32_t len, uint8_t framIndex)
{
    spi_disable();
    int i;
    uint8_t *data;
    uint8_t *q;
    q = (uint8_t *) value;
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (rs512_chip_disable(framIndex) == false) return false;
    delay_us(20);
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_PROGRAM);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    //ghi data
    data = q;
    for (i = 0; i < len; i++)
    {
        spi_send_byte(*data++);
    }
    if (rs512_chip_disable(framIndex) == false) return false;
    //delay_ms(T_wc);
    rs512_check_write_complete(framIndex);
    
    return true;
}

//eeprom, phan page
//int rs512_write_burst_pages_nocrc(uint32_t addr, void *value, uint32_t len, uint8_t framIndex)
//{
//    spi_disable();
//    int i;
//    uint8_t *data;
//    unsigned long section_len, size;
//    uint8_t *q;
//
//    q = (uint8_t *) value;
//    size = len;
//    // If the write spreads across pages in the EEPROM, we need to split the write
//    //   into sections.
//    while (len > 0)
//    {
//        if (addr + len > ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)))
//        {
//            section_len = ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)) - addr;
//        }
//        else
//        {
//            section_len = len;
//        }
//
//        if (rs512_chip_enable(framIndex) == false) return false;
//        spi_send_byte(RS512_WREN);
//        //        ________
//        //       /        \           <== CS# must be driven high before
//        //______/          \_________     the instruction is executed
//        if (rs512_chip_disable(framIndex) == false) return false;
//        delay_us(20);
//
//        if (rs512_chip_enable(framIndex) == false) return false;
//        spi_send_byte(RS512_PROGRAM);
//        #ifdef FRAM_RS1024 
//            spi_send_byte(addr >> 16);
//        #endif
//        spi_send_byte(addr >> 8);
//        spi_send_byte(addr);
//
//        data = q;
//        for (i = 0; i < section_len; i++)
//        {
//            spi_send_byte(*data++);
//        }
//        len -= section_len;
//        addr += section_len;
//        q += section_len;
//        if (rs512_chip_disable(framIndex) == false) return false;
//        //delay_ms(T_wc);
//		rs512_check_write_complete(framIndex);
////        if (len == 0)
////        {
////            q = (uint8_t *) value;
////            if (aa1024_chip_enable(framIndex) == false) return false;
////            spi_send_byte(AA512_WREN);
////            //        ________
////            //       /        \           <== CS# must be driven high before
////            //______/          \_________     the instruction is executed
////            if (aa1024_chip_disable(framIndex) == false) return false;
////            delay_us(20);
////
////            if (aa1024_chip_enable(framIndex) == false) return false;
////            spi_send_byte(AA512_PROGRAM);
////            #if (EEPROM_25AA1024)
////            spi_send_byte(addr >> 16);
////            #endif
////            spi_send_byte(addr >> 8);
////            spi_send_byte(addr);
////            spi_send_byte(crc8bit_buffer((void *)q,size));
////            if (aa1024_chip_disable(framIndex) == false) return false;
////            //delay_ms(T_wc);
////			aa1024_check_write_complete(framIndex);
////        }
//    }
//    return true;
//}
//===========================================================================
//FUNCTION NAME:  rs512_read_burst
//PROTOTYPE:      int rs512_read_burst(uint32_t addr,void *value,uint32_t len, uint8_t framIndex)
//INPUT:          addr: dia chi can doc fram
//                *value: con tro toi bo dem can doc
//                len: chieu dai bo dem can doc
//                framIndex: chon fram
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int rs512_read_burst(uint32_t addr, void *value, uint32_t len, uint8_t framIndex)
{
    spi_disable();
    uint8_t *data;
    uint8_t temp8;
    int i;
    data = (uint8_t *) value;
    spi_disable();
    NOP();
    if (rs512_chip_enable(framIndex) == false) return false;
    spi_send_byte(RS512_READ);
#ifdef FRAM_RS1024 
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    for (i = 0; i < len; i++)
    {
        *data++ = spi_get_byte();
    }
    temp8 = *data;//lay lai byte nay truoc khi doc crc
    *data++ = spi_get_byte();//byte crc
    data = (uint8_t *) value; //data=data-len;//lay lai dia chi offset

    if (rs512_chip_disable(framIndex) == false)
    {
        *(data + len) = temp8; //gan lai byte bi crc luu de len
        return false;
    }
    if (crc8bit_buffer((void *)data,len+1) != 0)
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 1;
#else
        emeter.flags.eeprom_error = 1;
#endif
    }
    else
    {
#ifdef _RF_SLOT_
        _eeprom.flags.error = 0;
#else
        emeter.flags.eeprom_error = 0;
#endif
    }
    *(data + len) = temp8; //gan lai byte bi crc luu de len
    return 1;
}

void rs512_sleep_mode(unsigned char framIndex)
{
    spi_disable();
    if (rs512_chip_enable(framIndex) == 0) return;
    spi_send_byte(RS512_SLEEP);
    if (rs512_chip_disable(framIndex) == 0) return;
    return;
}

//void rs512_wake_up(uint8_t framIndex)
//{
//    spi_disable();
//    uint32_t addr = 0x00000001;//dummy addr
//    uint8_t data;
//    rs512_chip_enable(framIndex);
//    spi_send_byte(RS512_WAKEUP);
//
////#ifdef EEPROM_25AA1024
////    spi_send_byte(addr >> 16);
////#endif
//    spi_send_byte(addr >> 8);
//    spi_send_byte(addr);
//
//    data = spi_get_byte();
//    //delay_us(100);
//    rs512_chip_disable(framIndex);
//    delay_ms(100);
//    //test UART1_send_byte(data);
//    return;
//}
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
		-
		-
*/

#endif //EEPROM_SUPPORT