/*===========================================================================*/
/*
SOURCE_FILE:		aa1024.c
APPLICATION:		DT03M-RF
MODULE:				eeprom
ORIGINAL AUTHOR:	thaodtn
VERSION:			1.0
DATE:				25/10/2013

(C) 2013: CPC IT

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

// Header prjEMETERS
#include "devices.h"
#include "main.h"


// Header prjLIB
#include "crc.h"
#include "delay.h"
#include "macro.h"
#include "eeprom.h"

// Header prjDRVIER
#include "aa1024.h"
#include "spi.h"

#ifndef UINT64_SUPPORT
#include "../../Elster-RF/FW/FWELSTERRFX/driver/uint64.h"
#endif

#ifdef EEPROM_SUPPORT

/*===========================================================================*/
/* L O C A L   F U N C T I O N   P R O T O T Y P E S                         */
/*===========================================================================*/
//===========================================================================
//FUNCTION NAME:  aa1024_chip_enable
//PROTOTYPE:      int aa1024_chip_enable(unsigned char eepromIndex)
//INPUT:          eepromIndex: chon eeprom
//OUTPUT:         true/false
//DESCRIPTION:    active CS pin of eeprom
//NOTE:
//=============================================================================
int aa1024_chip_enable(unsigned char eepromIndex)
{
    switch (eepromIndex)
    {
        #ifdef EEPROM_1_SUPPORT
        case 1:
            eeprom_1_select(); // eeprom_cs1_low(); Vuonglq Quy hoach lai mode SPI
            break;
        #endif
        #ifdef EEPROM_2_SUPPORT
        case 2:
            eeprom_2_select(); // eeprom_cs2_low(); Vuonglq Quy hoach lai mode SPI
            break;
        #endif
        #ifdef EEPROM_3_SUPPORT
        case 3:
            eeprom_3_select(); // eeprom_cs3_low(); Vuonglq Quy hoach lai mode SPI
            break;
        #endif
        #ifdef EEPROM_4_SUPPORT
        case 4:
            eeprom_4_select(); //  eeprom_cs4_low(); Vuonglq Quy hoach lai mode SPI
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
int aa1024_chip_disable(unsigned char eepromIndex)
{
    switch (eepromIndex)
    {
        #ifdef EEPROM_1_SUPPORT
        case 1:
            eeprom_1_de_select(); // eeprom_cs1_high(); Vuonglq 
            break;
        #endif
        #ifdef EEPROM_2_SUPPORT
        case 2:
            eeprom_2_de_select(); // eeprom_cs2_high(); Vuonglq
            break;
        #endif
        #ifdef EEPROM_3_SUPPORT
        case 3:
             eeprom_3_de_select(); // eeprom_cs3_high(); Vuonglq
            break;
        #endif
        #ifdef EEPROM_4_SUPPORT
        case 4:
             eeprom_4_de_select(); // eeprom_cs4_high(); Vuonglq
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
unsigned char checksum(void *buf, int len)
{
    unsigned char *st;
    unsigned char sum;
    sum = 0;
    st = (unsigned char *) buf;
    while (len-- > 0)
    {
        sum += *st++;
    }
    return((unsigned char) (sum+0x55));
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_status
//PROTOTYPE:      unsigned char aa1024_read_status(unsigned char eepromIndex)
//INPUT:          eepromIndex: chon eeprom
//OUTPUT:         byte status cua eeprom
//DESCRIPTION:
//NOTE:
//=============================================================================
unsigned char aa1024_read_status(unsigned char eepromIndex)
{
    spi_disable();
    unsigned char status;
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_RDSR);
    status = spi_get_byte();
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    return(status);
}

//===========================================================================
//FUNCTION NAME:  aa1024_write_status
//PROTOTYPE:      int aa1024_write_status(char status, unsigned char eepromIndex)
//INPUT:          status: gia tri can ghi
//                eepromIndex: chon eeprom
//OUTPUT:         true: thanh cong
//DESCRIPTION:    ghi gia tri status vao eeprom
//NOTE:
//=============================================================================
int aa1024_write_status(char status, unsigned char eepromIndex)
{
    spi_disable();
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WRSR);
    spi_send_byte(status);
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_erase
//PROTOTYPE:      int aa1024_erase(unsigned char eepromIndex)
//INPUT:          eepromIndex: chon eeprom
//OUTPUT:         true: thanh cong
//DESCRIPTION:    xoa toan bo eeprom
//NOTE:
//=============================================================================
int aa1024_erase(unsigned char eepromIndex)
{
    spi_disable();
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(CHIP_ERASE);
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    aa1024_check_write_complete(eepromIndex);    
    return true;
}

void aa1024_check_write_complete(unsigned char eepromIndex)
{
#ifdef CHECK_WIP_BIT
    uint8_t status = 0;
    status = aa1024_read_status(eepromIndex);
    uint8_t i = 0;
    while ((i<50)&&(status & 0x01)) //qua 10ms ma ko ghi xong thi huy
    {
        i++;
        status = aa1024_read_status(eepromIndex);
        delay_us(200);
    }
    if (i >= 50)
       i++; //debug
#else
    delay_ms(T_wc);
#endif
}
//==============================================================================
//FUNCTION NAME:  aa1024_write_byte
//PROTOTYPE:      int aa1024_write_byte(uint32_t addr, uint8_t value, uint8_t eepromIndex)
//INPUT:          addr: dia chi bat dau
//                value: gia tri can ghi
//                eepromIndex: chon eeprom
//OUTPUT:         true: ghi thanh cong
//DESCRIPTION:    ghi gia tri 1 byte va 1 byte crc vao eeprom
//NOTE:
//==============================================================================
int aa1024_write_byte(uint32_t addr, uint8_t value, uint8_t eepromIndex)
{
    spi_disable();
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_PROGRAM);

#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(value);
    spi_send_byte(crc8bit_buffer(&value,1));
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    //delay_ms(T_wc);
    aa1024_check_write_complete(eepromIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_byte
//PROTOTYPE:      unsigned char aa1024_read_byte(uint32_t addr,uint8_t eepromIndex)
//INPUT:          addr: dia chi bat dau
//                eepromIndex: chon eeprom de doc
//OUTPUT:         gia tri tai dia chi can doc
//DESCRIPTION:
//NOTE:
//=============================================================================
unsigned char aa1024_read_byte(uint32_t addr,uint8_t eepromIndex)
{
    spi_disable();
    unsigned char buffer[2];
    unsigned char value = 0;
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
    spi_send_byte(AA512_READ);

#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    buffer[0] = spi_get_byte();
    buffer[1] = spi_get_byte();//byte crc
    if (aa1024_chip_disable(eepromIndex) == false) return 0;
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
//FUNCTION NAME:  aa1024_write_2byte
//PROTOTYPE:      int aa1024_write_2byte(uint32_t addr, uint16_t value, uint8_t eepromIndex)
//INPUT:          addr: dia chi can ghi
//                value: gia tri 2 byte can ghi
//                eepromIndex: chon eeprom
//OUTPUT:         true: ghi thanh cong
//DESCRIPTION:    ghi gia tri 2 byte va 1 byte crc vao eeprom
//NOTE:
//=============================================================================
int aa1024_write_2byte(uint32_t addr, uint16_t value, uint8_t eepromIndex)
{
    spi_disable();
    uint16_t data = value;
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_PROGRAM);

#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(crc8bit_buffer((unsigned char *)&data,2));
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    //delay_ms(T_wc);
    aa1024_check_write_complete(eepromIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_2byte
//PROTOTYPE:      uint16_t aa1024_read_2byte(uint32_t addr,uint8_t eepromIndex)
//INPUT:          addr: dia chi can doc
//                eepromIndex: chon eeprom
//OUTPUT:         gia tri 2 byte can doc
//DESCRIPTION:
//NOTE:
//=============================================================================
uint16_t aa1024_read_2byte(uint32_t addr, uint8_t eepromIndex)
{
    spi_disable();
    unsigned char buffer[3];
    uint16_t *data;
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
    spi_send_byte(AA512_READ);

#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);
    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();//byte cao
    buffer[2] = spi_get_byte();//byte crc
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
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
//FUNCTION NAME:  aa1024_write_3byte
//PROTOTYPE:      int aa1024_write_3byte(uint32_t addr, uint32_t value,
//                                                    unsigned char eepromIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:    ghi gia tri 3 byte va 1 byte crc vao eeprom
//NOTE:
//=============================================================================
int aa1024_write_3byte(uint32_t addr, uint32_t value,uint8_t eepromIndex)
{
    spi_disable();
    uint32_t data = value;
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);

    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_PROGRAM);
#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(data >> 16);

    spi_send_byte(crc8bit_buffer(&data,3));
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    //delay_ms(T_wc);
    aa1024_check_write_complete(eepromIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_3byte
//PROTOTYPE:      uint32_t aa1024_read_3byte(uint32_t addr,uint8_t eepromIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint32_t aa1024_read_3byte(uint32_t addr, uint8_t eepromIndex)
{
    spi_disable();
    unsigned char buffer[4];
    uint32_t *data;
    data = 0;
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
    spi_send_byte(AA512_READ);
#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();
    buffer[2] = spi_get_byte();//byte cao
    buffer[3] = spi_get_byte();//byte crc
    if (aa1024_chip_disable(eepromIndex) == false) return 0;
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
//FUNCTION NAME:  aa1024_write_4byte
//PROTOTYPE:      int aa1024_write_4byte(uint32_t addr, uint32_t value, uint8_t eepromIndex)
//INPUT:          addr: dia chi can doc
//                value: gia tri can ghi
//                eepromIndex: chon eeprom
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int aa1024_write_4byte(uint32_t addr, uint32_t value, uint8_t eepromIndex)
{
    spi_disable();
    uint32_t data = value;
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);

    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_PROGRAM);
#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    spi_send_byte(data);
    spi_send_byte(data >> 8);
    spi_send_byte(data >> 16);
    spi_send_byte(data >> 24);

    spi_send_byte(crc8bit_buffer(&data,4));
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    //delay_ms(T_wc);
    aa1024_check_write_complete(eepromIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_4byte
//PROTOTYPE:      uint32_t aa1024_read_4byte(uint32_t addr, uint8_t eepromIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint32_t aa1024_read_4byte(uint32_t addr, uint8_t eepromIndex)
{
    spi_disable();
    unsigned char buffer[5];
    uint32_t *data;
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
    spi_send_byte(AA512_READ);
#ifdef EEPROM_25AA1024
    spi_send_byte(addr >> 16);
#endif
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    buffer[0] = spi_get_byte();//byte thap
    buffer[1] = spi_get_byte();
    buffer[2] = spi_get_byte();
    buffer[3] = spi_get_byte();//byte cao
    buffer[4] = spi_get_byte();//byte crc
    if (aa1024_chip_disable(eepromIndex) == false) return 0;
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
//FUNCTION NAME:  aa1024_write_8byte
//PROTOTYPE:      int aa1024_write_8byte(uint32_t addr, uint64_t value, uint8_t eepromIndex)
//INPUT:          addr: dia chi can doc
//                value: gia tri can ghi
//                eepromIndex: chon eeprom
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int aa1024_write_8byte(uint32_t addr, uint64_t value, uint8_t eepromIndex)
{
    spi_disable();
    uint64_t data = value;
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_WREN);
    //        ________
    //       /        \           <== CS# must be driven high before
    //______/          \_________     the instruction is executed
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    delay_us(20);

    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_PROGRAM);
#ifdef EEPROM_25AA1024
    spi_send_byte((uint8_t)(addr >> 16));
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
    if (aa1024_chip_disable(eepromIndex) == false) return false;
    //delay_ms(T_wc);
    aa1024_check_write_complete(eepromIndex);
    return true;
}

//===========================================================================
//FUNCTION NAME:  aa1024_read_8byte
//PROTOTYPE:      uint64_t aa1024_read_8byte(uint32_t addr, uint8_t eepromIndex)
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
uint64_t aa1024_read_8byte(uint32_t addr, uint8_t eepromIndex)
{
    spi_disable();
    unsigned char buffer[9];
    uint64_t *data;
    if (aa1024_chip_enable(eepromIndex) == false) return 0;
    spi_send_byte(AA512_READ);
#ifdef EEPROM_25AA1024
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
    if (aa1024_chip_disable(eepromIndex) == false) return 0;
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
//FUNCTION NAME:  aa1024_write_burst_pages
//PROTOTYPE:      int aa1024_write_burst_pages(uint32_t addr,void *value,
//                                            unsigned long len,uint8_t eepromIndex)
//INPUT:          addr: dia chi can luu
//                *value: con tro toi bo dem can luu
//                len: chieu dai bo dem can luu
//                eepromIndex: chon eeprom
//OUTPUT:
//DESCRIPTION:    ghi chuoi nhieu byte, co the ghi sang trang khac cua eeprom
//NOTE:
//=============================================================================
int aa1024_write_burst_pages(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex)
{
    spi_disable();
    int i;
    uint8_t *data;
    unsigned long section_len, size;
    uint8_t *q;

    q = (uint8_t *) value;
    size = len;
    // If the write spreads across pages in the EEPROM, we need to split the write
    //   into sections.
    while (len > 0)
    {
        if (addr + len > ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)))
        {
            section_len = ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)) - addr;
        }
        else
        {
            section_len = len;
        }

        if (aa1024_chip_enable(eepromIndex) == false) return false;
        spi_send_byte(AA512_WREN);
        //        ________
        //       /        \           <== CS# must be driven high before
        //______/          \_________     the instruction is executed
        if (aa1024_chip_disable(eepromIndex) == false) return false;
        delay_us(20);

        if (aa1024_chip_enable(eepromIndex) == false) return false;
        spi_send_byte(AA512_PROGRAM);
#ifdef EEPROM_25AA1024
        spi_send_byte(addr >> 16);
#endif
        spi_send_byte(addr >> 8);
        spi_send_byte(addr);

        data = q;
        for (i = 0; i < section_len; i++)
        {
            spi_send_byte(*data++);
        }
        len -= section_len;
        addr += section_len;
        q += section_len;
        if (aa1024_chip_disable(eepromIndex) == false) return false;
        //delay_ms(T_wc);
		aa1024_check_write_complete(eepromIndex);
        if (len == 0)
        {
            q = (uint8_t *) value;
            if (aa1024_chip_enable(eepromIndex) == false) return false;
            spi_send_byte(AA512_WREN);
            //        ________
            //       /        \           <== CS# must be driven high before
            //______/          \_________     the instruction is executed
            if (aa1024_chip_disable(eepromIndex) == false) return false;
            delay_us(20);

            if (aa1024_chip_enable(eepromIndex) == false) return false;
            spi_send_byte(AA512_PROGRAM);
            #ifdef EEPROM_25AA1024
            spi_send_byte(addr >> 16);
            #endif
            spi_send_byte(addr >> 8);
            spi_send_byte(addr);
            spi_send_byte(crc8bit_buffer((void *)q,size));
            if (aa1024_chip_disable(eepromIndex) == false) return false;
            //delay_ms(T_wc);
			aa1024_check_write_complete(eepromIndex);
        }
    }
    return true;
}
//===========================================================================
//FUNCTION NAME:  aa1024_write_burst_pages_nocrc
//PROTOTYPE:      int aa1024_write_burst_pages_nocrc(uint32_t addr,void *value,
//                                            unsigned long len,uint8_t eepromIndex)
//INPUT:          addr: dia chi can luu
//                *value: con tro toi bo dem can luu
//                len: chieu dai bo dem can luu
//                eepromIndex: chon eeprom
//OUTPUT:
//DESCRIPTION:    ghi chuoi nhieu byte, co the ghi sang trang khac cua eeprom
//NOTE:
//=============================================================================
int aa1024_write_burst_pages_nocrc(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex)
{
    spi_disable();
    int i;
    uint8_t *data;
    unsigned long section_len;
    unsigned long size;
    uint8_t *q;

    q = (uint8_t *) value;
    size = len;
    // If the write spreads across pages in the EEPROM, we need to split the write
    //   into sections.
    while (len > 0)
    {
        if (addr + len > ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)))
        {
            section_len = ((addr + EEPROM_PAGE_SIZE) & ~(EEPROM_PAGE_SIZE - 1)) - addr;
        }
        else
        {
            section_len = len;
        }

        if (aa1024_chip_enable(eepromIndex) == false) return false;
        spi_send_byte(AA512_WREN);
        //        ________
        //       /        \           <== CS# must be driven high before
        //______/          \_________     the instruction is executed
        if (aa1024_chip_disable(eepromIndex) == false) return false;
        delay_us(20);

        if (aa1024_chip_enable(eepromIndex) == false) return false;
        spi_send_byte(AA512_PROGRAM);
#ifdef EEPROM_25AA1024
        spi_send_byte(addr >> 16);
#endif
        spi_send_byte(addr >> 8);
        spi_send_byte(addr);

        data = q;
        for (i = 0; i < section_len; i++)
        {
            spi_send_byte(*data++);
        }
        len -= section_len;
        addr += section_len;
        q += section_len;
        if (aa1024_chip_disable(eepromIndex) == false) return false;
        //delay_ms(T_wc);
		aa1024_check_write_complete(eepromIndex);
//        if (len == 0)
//        {
//            q = (uint8_t *) value;
//            if (aa1024_chip_enable(eepromIndex) == false) return false;
//            spi_send_byte(AA512_WREN);
//            //        ________
//            //       /        \           <== CS# must be driven high before
//            //______/          \_________     the instruction is executed
//            if (aa1024_chip_disable(eepromIndex) == false) return false;
//            delay_us(20);
//
//            if (aa1024_chip_enable(eepromIndex) == false) return false;
//            spi_send_byte(AA512_PROGRAM);
//            #if (EEPROM_25AA1024)
//            spi_send_byte(addr >> 16);
//            #endif
//            spi_send_byte(addr >> 8);
//            spi_send_byte(addr);
//            spi_send_byte(crc8bit_buffer((void *)q,size));
//            if (aa1024_chip_disable(eepromIndex) == false) return false;
//            //delay_ms(T_wc);
//			aa1024_check_write_complete(eepromIndex);
//        }
    }
    return true;
}
//===========================================================================
//FUNCTION NAME:  aa1024_read_burst
//PROTOTYPE:      int aa1024_read_burst(uint32_t addr,void *value,uint32_t len, uint8_t eepromIndex)
//INPUT:          addr: dia chi can doc eeprom
//                *value: con tro toi bo dem can doc
//                len: chieu dai bo dem can doc
//                eepromIndex: chon eeprom
//OUTPUT:
//DESCRIPTION:
//NOTE:
//=============================================================================
int aa1024_read_burst(uint32_t addr, void *value, uint32_t len, uint8_t eepromIndex)
{
    spi_disable();
    uint8_t *data;
    uint8_t temp8;
    int i;
    data = (uint8_t *) value;
    spi_disable();
    NOP();
    if (aa1024_chip_enable(eepromIndex) == false) return false;
    spi_send_byte(AA512_READ);
#ifdef EEPROM_25AA1024
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

    if (aa1024_chip_disable(eepromIndex) == false)
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

void aa1024_sleep_mode(unsigned char eepromIndex)
{
    spi_disable();
    if (aa1024_chip_enable(eepromIndex) == 0) return;
    spi_send_byte(AA512_SLEEP);
    if (aa1024_chip_disable(eepromIndex) == 0) return;
    return;
}

void aa1024_wake_up(uint8_t _eeprom)
{
    spi_disable();
    uint32_t addr = 0x00000001;//dummy addr
    uint8_t data;
    aa1024_chip_enable(_eeprom);
    spi_send_byte(AA512_WAKEUP);

    spi_send_byte(addr >> 16);
    spi_send_byte(addr >> 8);
    spi_send_byte(addr);

    data = spi_get_byte();
    //delay_us(100);
    aa1024_chip_disable(_eeprom);
    delay_ms(100);
    //test UART1_send_byte(data);
    return;
}
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
		-
		-
*/

#endif //EEPROM_SUPPORT