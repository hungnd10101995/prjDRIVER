/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

// Header prjEMETERS
#include "library.h"
#include "main.h"
#include "hardware.h"

//Header prjLIB
#include "crc.h"
#include "delay.h"
#include "macro.h"


// Header prjDRVIER
#include "sst25vf016.h"
#include "spi.h"


#ifdef SST25VF016B_SUPPORT //#ifndef SST26VF064B_SUPPORT
#ifndef _RMAL_
unsigned char flag_flash_error = 0;
#endif
/*=======================================================================
FUNCTION NAME: spi_get_byte (eeprom--> pic)
PROTOTYPE: unsigned char spi_get_byte(void)
INPUT: none
OUTPUT: none
DESCRIPTION: 
NOTE: 
=========================================================================*/
unsigned char sst25vf016_spi_get_byte(void)
{
//   unsigned char i;
//   unsigned char value = 0;
//   //SCLK_H;
//   delay_us(1);//nop();//delay_us(1);
//   for (i = 0; i < 8; i++)
//   {
//        // chuyen vi tri doan nay 20120918
//       value <<= 1;
//       if (spi_sdo_input())
//       {
//          value |= 0x01;
//       }
//       spi_sclk_high();
//       delay_us(1);//nop();//delay_us(1);
//       spi_sclk_low();
//       delay_us(1);//nop();//delay_us(1);
//   }
//   return value;
    unsigned char value = 0;
    value = spi_get_byte();
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
void sst25vf016_spi_send_byte(unsigned char value)
{
//   unsigned char i;
//   delay_us(1);//nop();
//   for (i = 0; i < 8; i++)
//   {
//      if(value & 0x80)
//      {
//          spi_sdi_high();
//      }
//      else
//      {
//          spi_sdi_low();
//      }
//
//      spi_sclk_high();//SCLK_L;
//      delay_us(1);//nop();////delay_us(1);
//      spi_sclk_low();//SCLK_H;
//      delay_us(1);//nop();////delay_us(1);
//      value <<= 1;
//   }
//  spi_sdi_low();
//  delay_us(1);//nop();
    spi_send_byte(value);
}
/************************************************************************/
/* PROCEDURE: init							*/
/*									*/
/* This procedure initializes the SCK to low. Must be called prior to 	*/
/* setting up mode 0.							*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Output:								*/
/*		SCK							*/

/************************************************************************/
void sst25_init(void) 
{
  #ifdef FLASH_1_SUPPORT
    flash_cs1_dir_output();  
    flash_cs1_output_high();
  #endif
  #ifdef FLASH_2_SUPPORT
    flash_cs2_dir_output();  
    flash_cs2_output_high();
  #endif    
    spi_sclk_low();//sclk_output_low(); /* set clock to low initial state */
}
#if !defined(_DCU_) && !defined(_RMAL_) && !defined(_RMR_) //dua all sang flash.c
void flash_cs_enable(char flashIndex)
{
    spi_disable();
    switch (flashIndex)
    {
        case FLASH_1:
            flash_cs1_output_low();
            #ifdef FLASH_2_SUPPORT
            flash_cs2_output_high();
            #endif
            break;
        case FLASH_2:
            flash_cs1_output_high();
            #ifdef FLASH_2_SUPPORT
            flash_cs2_output_low();
            #endif
            break;
    }
}
void flash_cs_disable(void)
{
    spi_disable();
    flash_cs1_output_high();
    #ifdef FLASH_2_SUPPORT
    flash_cs2_output_high();
    #endif
}
#endif
/************************************************************************/
/* PROCEDURE: Poll_SO							*/
/*									*/
/* This procedure polls for the SO line during AAI programming  	*/
/* waiting for SO to transition to 1 which will indicate AAI programming*/
/* is completed								*/
/*									*/
/* Input:								*/
/*		SO							*/
/*									*/
/* Output:								*/
/*		None							*/

/************************************************************************/
void sst25_poll_SO(char flashIndex) {
    volatile unsigned char temp = 0;
    volatile unsigned int i = 0;
    flash_cs_enable(flashIndex);
    while ((i < 10) && (temp == 0x00)) {/* waste time until not busy */
        temp = spi_sdo_input();//sdo_input();
        i++;
#ifdef _DCU_
        if (dcu_flags_unlock) {
            delay_ms(5);
            restart_watchdog();
        }
        else
           delay_us(10);
#else
        delay_ms(5);    
#endif
    }
    flash_cs_disable();
}

/************************************************************************/
/* PROCEDURE: Read_Status_Register					*/
/*									*/
/* This procedure read the status register and returns the byte.	*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		byte							*/

/************************************************************************/
unsigned char sst25_read_status_register(char flashIndex) {
    unsigned char byte = 0;
    flash_cs_enable(flashIndex);
    sst25vf016_spi_send_byte(SST25_RDSR); /* send RDSR command */
    byte = sst25vf016_spi_get_byte(); /* receive byte */
    flash_cs_disable(); /* disable device */
    return byte;
}

/************************************************************************/
/* PROCEDURE: EWSR							*/
/*									*/
/* This procedure Enables Write Status Register.  			*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_enable_write_status_register(char flashIndex) {
    flash_cs_enable(flashIndex);
    sst25vf016_spi_send_byte(SST25_EWSR); /* enable writing to the status register */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE: WRSR							*/
/*									*/
/* This procedure writes a byte to the Status Register.			*/
/*									*/
/* Input:								*/
/*		byte							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_write_status_register(uint8_t value, char flashIndex) {
    sst25_enable_write_status_register(flashIndex); //thaodtn
    flash_cs_enable(flashIndex);
    sst25vf016_spi_send_byte(SST25_WRSR); /* select write to status register */
    sst25vf016_spi_send_byte(value); /* data that will change the status of BPx
                                       or BPL (only bits 2,3,4,5,7 can be written) */
    flash_cs_disable(); /* disable the device */
}

/************************************************************************/
/* PROCEDURE: WREN							*/
/*									*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.					*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_write_enable(char flashIndex) {
    flash_cs_enable(flashIndex);/* enable device */
    sst25vf016_spi_send_byte(SST25_WREN); /* send WREN command */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE: WRDI							*/
/*									*/
/* This procedure disables the Write Enable Latch.			*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_write_disable(char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_WRDI); /* send WRDI command */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE: EBSY							*/
/*									*/
/* This procedure enable SO to output RY/BY# status during AAI 		*/
/* programming.								*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_EBSY(char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_EBSY); /* send EBSY command */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE: DBSY							*/
/*									*/
/* This procedure disable SO as output RY/BY# status signal during AAI	*/
/* programming.								*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_DBSY(char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_DBSY); /* send DBSY command */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE: Read_ID							*/
/*									*/
/* This procedure Reads the manufacturer's ID and device ID.  It will 	*/
/* use 90h or ABh as the command to read the ID (90h in this sample).   */
/* It is up to the user to give the last byte ID_addr to determine      */
/* whether the device outputs manufacturer's ID first, or device ID 	*/
/* first.  Please see the product datasheet for details.  Returns ID in */
/* variable byte.							*/
/*									*/
/* Input:								*/
/*		ID_addr							*/
/*									*/
/* Returns:								*/
/*		byte:	ID1(Manufacture's ID = BFh or Device ID = 80h)	*/
/*									*/

/************************************************************************/
unsigned char sst25_read_id(uint8_t ID_addr, char flashIndex) {
    uint8_t value;
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_RDID); /* send read ID command (90h or ABh) */
    sst25vf016_spi_send_byte(0x00); /* send address */
    sst25vf016_spi_send_byte(0x00); /* send address */
    sst25vf016_spi_send_byte(ID_addr); /* send address - either 00H or 01H */
    value = sst25vf016_spi_get_byte(); /* receive byte */
    flash_cs_disable(); /* disable device */
    return value;
}

/************************************************************************/
/* PROCEDURE: Jedec_ID_Read						*/
/*									*/
/* This procedure Reads the manufacturer's ID (BFh), memory type (25h)  */
/* and device ID (41h).  It will use 9Fh as the JEDEC ID command.    	*/
/* Please see the product datasheet for details.  			*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		IDs_Read:ID1(Manufacture's ID = BFh, Memory Type (25h), */
/*		 and Device ID (80h)					*/
/*									*/

/************************************************************************/
unsigned long sst25_jedec_id_read(char flashIndex) {
    uint32_t temp;
    temp = 0;
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(0x9F); /* send JEDEC ID command (9Fh) */
    temp = (temp | sst25vf016_spi_get_byte()) << 8; /* receive byte */
    temp = (temp | sst25vf016_spi_get_byte()) << 8;
    temp = (temp | sst25vf016_spi_get_byte()); /* temp value = 0xBF2541 */
    flash_cs_disable(); /* disable device */
    return temp;
}

/************************************************************************/
/* PROCEDURE:	Read							*/
/*									*/
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.						*/
/*									*/
/*									*/
/*									*/
/* Input:								*/
/*		Dst:	Destination Address 000000H - 1FFFFFH		*/
/*      								*/
/*									*/
/* Returns:								*/
/*		byte							*/
/*									*/

/************************************************************************/
unsigned char sst25_read_byte(uint32_t address, char flashIndex) {
    unsigned char byte = 0;
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_READ); /* read command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    byte = sst25vf016_spi_get_byte();
    flash_cs_disable(); /* disable device */
    return byte; /* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	Read_Cont						*/
/*									*/
/* This procedure reads multiple addresses of the device and stores	*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*      	no_bytes	Number of bytes to read	(max = 128)	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_read_burst(uint32_t address, uint8_t *buffer, uint32_t no_bytes, uint8_t checkCRC, char flashIndex) 
{
    unsigned long i = 0;
    //uint8_t tmp_r[30];
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_READ); /* read command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    for (i = 0; i < no_bytes; i++) /* read until no_bytes is reached */ {
        buffer[i] = sst25vf016_spi_get_byte(); /* receive byte and store at address 80H - FFH */
        //tmp_r[i] = buffer[i];
    }
    //uart_transmit_buffer(buffer,i,UART_PC);
    if (checkCRC == TRUE) {
        if (buffer[i] != crc8bit_buffer(buffer, i))
            flag_flash_error = 1;
    }
    //tmp_r[i] = 0;
    flash_cs_disable(); /* disable device */
}
/************************************************************************/
/* PROCEDURE:	sst25_write_burst				        */
/* chi ghi duoc so chan byte									*/

/************************************************************************/
void sst25_write_burst(uint32_t address, uint8_t *buffer, uint32_t length, uint8_t writeCRC, char flashIndex) {
    uint8_t crc, *q; //, tmp_w[30];
    sst25_write_enable(flashIndex);
    sst25_EBSY(flashIndex); /* enable RY/BY# status for SO in AAI */
    if (writeCRC == TRUE) //write CRC
    {
        q = (unsigned char *) buffer;
        crc = crc8bit_buffer((void *) q, length - 1);
        buffer[length - 1] = crc;
    }
    //memcpy(tmp_w,buffer,length);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(buffer[0]); /* send 1st byte to be programmed */
    sst25vf016_spi_send_byte(buffer[1]); /* send 2nd byte to be programmed */
    flash_cs_disable(); /* disable device */
    sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
    //volatile uint32_t i = 0;
    int i = 0;    
    for (i = 2; i < length; i += 2) 
    {
        flash_cs_enable(flashIndex); /* enable device */
        sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
        sst25vf016_spi_send_byte(buffer[i]); /* send 1st byte to be programmed */
        sst25vf016_spi_send_byte(buffer[i + 1]); /* send 2nd byte to be programmed */
        flash_cs_disable(); /* disable device */
        sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
    }
    sst25_write_disable(flashIndex); /* Exit AAI before executing DBSY */
    sst25_DBSY(flashIndex); /* disable SO as RY/BY# output if in AAI */
    sst25_wait_busy_AAI(flashIndex); //waits until device is no longer busy for AAI mode
}

/*ghi du lieu vao 2 trang lien tiep
 chi ghi duoc so chan byte */
void sst25_write_burst_pages(uint32_t addr, uint8_t *buffer, uint32_t len, uint8_t writeCRC, char flashIndex)
{
    spi_disable();
    int i;
    uint8_t crc = 0;
    uint8_t *data;
    unsigned long section_len, size;
    uint8_t *q;
    if (writeCRC == TRUE) //write CRC
    {
        q = (unsigned char *) buffer;
        crc = crc8bit_buffer((void *) q, len - 1);
        buffer[len - 1] = crc;
    }
    q = (uint8_t *) buffer;
    size = len;
    // If the write spreads across pages in the EEPROM, we need to split the write
    //   into sections.
    while (len > 0)
    {
        if (addr + len > ((addr + SECTOR_LENGTH) & ~(SECTOR_LENGTH - 1)))
        {
            section_len = ((addr + SECTOR_LENGTH) & ~(SECTOR_LENGTH - 1)) - addr;
        }
        else
        {
            section_len = len;
        }
        data = q;
        sst25_write_enable(flashIndex);
        sst25_EBSY(flashIndex); /* enable RY/BY# status for SO in AAI */
        flash_cs_enable(flashIndex); /* enable device */
        sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
        sst25vf016_spi_send_byte(((addr & 0xFFFFFF) >> 16)); /* send 3 address bytes */
        sst25vf016_spi_send_byte(((addr & 0xFFFF) >> 8));
        sst25vf016_spi_send_byte(addr & 0xFF);
        sst25vf016_spi_send_byte(data[0]); /* send 1st byte to be programmed */
        sst25vf016_spi_send_byte(data[1]); /* send 2nd byte to be programmed */
        flash_cs_disable(); /* disable device */
        sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
        volatile uint32_t i = 0;
        for (i = 2; i < section_len; i += 2) 
        {
            flash_cs_enable(flashIndex); /* enable device */
            sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
            sst25vf016_spi_send_byte(data[i]); /* send 1st byte to be programmed */
            sst25vf016_spi_send_byte(data[i + 1]); /* send 2nd byte to be programmed */
            flash_cs_disable(); /* disable device */
            sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
        }
        sst25_write_disable(flashIndex); /* Exit AAI before executing DBSY */
        sst25_DBSY(flashIndex); /* disable SO as RY/BY# output if in AAI */
        sst25_wait_busy_AAI(flashIndex); //waits until device is no longer busy for AAI mode
        
        len -= section_len;
        addr += section_len;
        q += section_len;
    }
}
/************************************************************************/
/* PROCEDURE:	HighSpeed_Read						*/
/*									*/
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.						*/
/*									*/
/*									*/
/*									*/
/* Input:								*/
/*		Dst:	Destination Address 000000H - 1FFFFFH		*/
/*      								*/
/*									*/
/* Returns:								*/
/*		byte							*/
/*									*/

/************************************************************************/
unsigned char sst25_highspeed_read_byte(uint32_t address, char flashIndex) {
    unsigned char byte = 0;
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_HSREAD); /* read command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(0xFF); /*dummy byte*/
    byte = sst25vf016_spi_get_byte();
    flash_cs_disable(); /* disable device */
    return byte; /* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read_Cont					*/
/*									*/
/* This procedure reads multiple addresses of the device and stores	*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*      	no_bytes	Number of bytes to read	(max = 128)	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_highspeed_read_burst(uint32_t address, uint8_t *buffer, uint32_t no_bytes, char flashIndex) {
    unsigned long i = 0;
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_HSREAD); /* read command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(0xFF); /*dummy byte*/
    for (i = 0; i < no_bytes; i++) /* read until no_bytes is reached */ {
        buffer[i] = sst25vf016_spi_get_byte(); /* receive byte and store at address 80H - FFH */
    }
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Byte_Program						*/
/*									*/
/* This procedure programs one address of the device.			*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/
/*									*/
/*									*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*		byte:		byte to be programmed			*/
/*      								*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_write_byte(uint32_t address, uint8_t value, char flashIndex) {
    sst25_write_enable(flashIndex);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_WRITE); /* send Byte Program command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(value); /* send byte to be programmed */
    flash_cs_disable(); /* disable device */
    sst25_wait_busy(flashIndex); //thaodtn
}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncA						*/
/*									*/
/* This procedure programs consecutive addresses of 2 bytes of data into*/
/* the device:  1st data byte will be programmed into the initial 	*/
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be be 	*/
/* programmed into initial address [A23-A1] and with A0  = 1.  This  	*/
/* is used to to start the AAI process.  It should be followed by 	*/
/* Auto_Add_IncB.							*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/
/*									*/
/*									*/
/* Note: Only RDSR command can be executed once in AAI mode with SO  	*/
/* 	 disable to output RY/BY# status.  Use WRDI to exit AAI mode 	*/
/*	 unless AAI is programming the last address or last address of  */
/* 	 unprotected block, which automatically exits AAI mode.		*/
/*									*/
/* Input:								*/

/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*		byte1:		1st byte to be programmed		*/
/*      	byte1:		2nd byte to be programmed		*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_auto_addr_incA(uint32_t address, uint8_t byte1, uint8_t byte2, char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(byte1); /* send 1st byte to be programmed */
    sst25vf016_spi_send_byte(byte2); /* send 2nd byte to be programmed */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncB						*/
/*									*/
/* This procedure programs consecutive addresses of 2 bytes of data into*/
/* the device:  1st data byte will be programmed into the initial 	*/
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be be 	*/
/* programmed into initial address [A23-A1] and with A0  = 1.    This  	*/
/* is used after Auto_Address_IncA.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/

/*									*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy 	*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI*/
/* 	 to exit AAI mode unless AAI is programming the last address or */
/*	 last address of unprotected block, which automatically exits 	*/
/*	 AAI mode.							*/
/*									*/
/* Input:								*/
/*									*/
/*		byte1:		1st byte to be programmed		*/
/*		byte2:		2nd byte to be programmed		*/
/*      								*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_auto_addr_incB(uint8_t byte1, uint8_t byte2, char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
    sst25vf016_spi_send_byte(byte1); /* send 1st byte to be programmed */
    sst25vf016_spi_send_byte(byte2); /* send 2nd byte to be programmed */
    flash_cs_disable(); /* disable device */
}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncA_EBSY					*/
/*									*/
/* This procedure is the same as procedure Auto_Add_IncA except that it */
/* uses EBSY and Poll_SO functions to check for RY/BY. It programs 	*/
/* consecutive addresses of the device.  The 1st data byte will be 	*/
/* programmed into the initial address [A23-A1] and with A0 = 0.  The 	*/
/* 2nd data byte will be programmed into initial address [A23-A1] and 	*/
/* with A0  = 1.  This is used to to start the AAI process.  It should  */
/* be followed by Auto_Add_IncB_EBSY.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/
/*									*/
/*									*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy 	*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI*/
/* 	 to exit AAI mode unless AAI is programming the last address or */
/*	 last address of unprotected block, which automatically exits 	*/
/*	 AAI mode.							*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*		byte1:		1st byte to be programmed		*/
/*      	byte1:		2nd byte to be programmed		*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_auto_addr_incA_EBSY(uint32_t address, uint8_t byte1, uint8_t byte2, char flashIndex) {
    sst25_EBSY(flashIndex); /* enable RY/BY# status for SO in AAI */
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    sst25vf016_spi_send_byte(byte1); /* send 1st byte to be programmed */
    sst25vf016_spi_send_byte(byte2); /* send 2nd byte to be programmed */
    flash_cs_disable(); /* disable device */
    sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
}

/************************************************************************/
/* PROCEDURE:	Auto_Add_IncB_EBSY					*/
/*									*/
/* This procedure is the same as Auto_Add_IncB excpet that it uses 	*/
/* Poll_SO to poll for RY/BY#.  It demonstrate on how to use DBSY after	*/
/* AAI programmming is completed.  It programs consecutive addresses of */
/* the device.  The 1st data byte will be programmed into the initial   */
/* address [A23-A1] and with A0 = 0.  The 2nd data byte will be 	*/
/* programmed into initial address [A23-A1] and with A0  = 1.  This is 	*/
/* used after Auto_Address_IncA.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.					*/
/*									*/
/* Note: Only WRDI and AAI command can be executed once in AAI mode 	*/
/*	 with SO enabled as RY/BY# status.  When the device is busy 	*/
/*	 asserting CE# will output the status of RY/BY# on SO.  Use WRDI*/
/* 	 to exit AAI mode unless AAI is programming the last address or */
/*	 last address of unprotected block, which automatically exits 	*/
/*	 AAI mode.							*/
/*									*/
/* Input:								*/
/*									*/
/*		byte1:		1st byte to be programmed		*/
/*		byte2:		2nd byte to be programmed		*/
/*      								*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/*									*/

/************************************************************************/
void sst25_auto_addr_incB_EBSY(uint8_t byte1, uint8_t byte2, char flashIndex) {
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_AAIP); /* send AAI command */
    sst25vf016_spi_send_byte(byte1); /* send 1st byte to be programmed */
    sst25vf016_spi_send_byte(byte2); /* send 2nd byte to be programmed */
    flash_cs_disable(); /* disable device */
    sst25_poll_SO(flashIndex); /* polls RY/BY# using SO line */
    sst25_write_disable(flashIndex); /* Exit AAI before executing DBSY */
    sst25_DBSY(flashIndex); /* disable SO as RY/BY# output if in AAI */
}

/************************************************************************/
/* PROCEDURE: Chip_Erase						*/
/*									*/
/* This procedure erases the entire Chip.				*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_chip_erase(char flashIndex) {
    sst25_write_enable(flashIndex);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_CHIP_ERASE); /* send Chip Erase command (60h or C7h) */
    flash_cs_disable(); /* disable device */
    sst25_wait_busy(flashIndex);
}

/************************************************************************/
/* PROCEDURE: Sector_Erase						*/
/*									*/
/* This procedure Sector Erases the Chip.				*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_sector_erase(uint32_t address, char flashIndex) {
    sst25_write_enable(flashIndex);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_SECTOR_ERASE); /* send Sector Erase command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    flash_cs_disable(); /* disable device */
    sst25_wait_busy(flashIndex);
}

/************************************************************************/
/* PROCEDURE: Block_Erase_32K						*/
/*									*/
/* This procedure Block Erases 32 KByte of the Chip.			*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_block_erase_32K(uint32_t address, char flashIndex) {
    sst25_write_enable(flashIndex);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_BLOCK32_ERASE); /* send 32 KByte Block Erase command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    flash_cs_disable(); /* disable device */
    sst25_wait_busy(flashIndex);
}

/************************************************************************/
/* PROCEDURE: Block_Erase_64K						*/
/*									*/
/* This procedure Block Erases 64 KByte of the Chip.			*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_block_erase_64K(uint32_t address, char flashIndex) {
    sst25_write_enable(flashIndex);
    flash_cs_enable(flashIndex); /* enable device */
    sst25vf016_spi_send_byte(SST25_BLOCK64_ERASE); /* send 64KByte Block Erase command */
    sst25vf016_spi_send_byte(((address & 0xFFFFFF) >> 16)); /* send 3 address bytes */
    sst25vf016_spi_send_byte(((address & 0xFFFF) >> 8));
    sst25vf016_spi_send_byte(address & 0xFF);
    flash_cs_disable(); /* disable device */
    sst25_wait_busy(flashIndex);
}

/************************************************************************/
/* PROCEDURE: Wait_Busy							*/
/*									*/
/* This procedure waits until device is no longer busy (can be used by	*/
/* Byte-Program, Sector-Erase, Block-Erase, Chip-Erase).		*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
//void sst25_wait_busy(char flashIndex) {
//    volatile unsigned int i = 0;
//    while ((i < 10) && ((sst25_read_status_register(flashIndex) & 0x03) == 0x03)) 
//    {/* waste time until not busy */
//        sst25_read_status_register(flashIndex);
//        i++;
//        //if (dcu_flags_unlock) {
//            delay_ms(5);
//            restart_watchdog();//wdt_restart();
//        //}
//    }
//    //if (i >= 10)
//    //    dcu.board.spi_error |= (1 << 4);
//}
void sst25_wait_busy(char flashIndex)
{
    int i = 0;
    while ((i < 100) &&(sst25_read_status_register(flashIndex) & 0x03) == 0x03)	/* waste time until not busy */
    {
        sst25_read_status_register(flashIndex);
        delay_ms(1);
        i++;
    }
	
}

/************************************************************************/
/* PROCEDURE: Wait_Busy_AAI						*/
/*									*/
/* This procedure waits until device is no longer busy for AAI mode.	*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_wait_busy_AAI(char flashIndex) 
{
    volatile unsigned int i = 0;
    while ((i < 10) && ((sst25_read_status_register(flashIndex) & 0x43) == 0x43))/* waste time until not busy */ 
    {
        sst25_read_status_register(flashIndex);
        i++;
#ifdef _DCU_
        if (dcu_flags_unlock)
            delay_ms(5);
        else
            delay_us(10);
#else
        //if (dcu_flags_unlock) {
            delay_ms(5);
            restart_watchdog();//wdt_restart();
        //}
#endif
        restart_watchdog();
    }
    //if (i >= 10)
    //    dcu.board.spi_error |= (1 << 4);
}

/************************************************************************/
/* PROCEDURE: WREN_Check						*/
/*									*/
/* This procedure checks to see if WEL bit set before program/erase.	*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_write_enable_check(char flashIndex) {
    uint8_t byte;
    byte = sst25_read_status_register(flashIndex); /* read the status register */
    if (byte != 0x02) /* verify that WEL bit is set */ {
        while (1) {

        }
        /* add source code or statements for this file */
        /* to compile   			       */
        /* i.e. option: insert a display to view error on LED? */
        /* option: insert a display to view error on LED? */
    }
}

/************************************************************************/
/* PROCEDURE: WREN_AAI_Check						*/
/*									*/
/* This procedure checks for AAI and WEL bit once in AAI mode.		*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_write_enable_AAI_check(char flashIndex) {
    unsigned char byte;
    byte = sst25_read_status_register(flashIndex); /* read the status register */
    if (byte != 0x42) /* verify that AAI and WEL bit is set */ {
        while (1) {

        }
        /* add source code or statements for this file */
        /* to compile   			       */
        /* i.e. option: insert a display to view error on LED? */
        /* option: insert a display to view error on LED? */

    }
}

/************************************************************************/
/* PROCEDURE: Verify							*/
/*									*/
/* This procedure checks to see if the correct byte has be read.	*/
/*									*/
/* Input:								*/
/*		byte:		byte read				*/
/*		cor_byte:	correct_byte that should be read	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/

/************************************************************************/
void sst25_verify(uint8_t byte, uint8_t cor_byte) {
    if (byte != cor_byte) {
        while (1) {

        }
        /* add source code or statement for this file */
        /* to compile   			       */
        /* i.e. option: insert a display to view error on LED? */

        /* option: insert a display to view error on LED? */
    }
}

void sst25_unblock_write_protection(char flashIndex) {
    sst25_write_status_register(0x00, flashIndex);
    uint8_t status = 0;
    status = sst25_read_status_register(flashIndex);
}

void sst25_block_write_protection(char flashIndex) {
    sst25_write_status_register(0x1C, flashIndex);
    uint8_t status = 0;
    status = sst25_read_status_register(flashIndex);
}

/*chi xoa tung phan vung luu thong tin*/
void sst25_chip_erase_partition(uint8_t partition, char flashIndex) {
    uint8_t i;
    sst25_unblock_write_protection(flashIndex);
    switch (partition) {
        case UPDATE_FW_PARTITION:
            for (i = 0; i < 11; i++)
                sst25_block_erase_64K(UPDATE_FW_START_ADDRESS + i * 0x10000, flashIndex);
            break;
        case SPIDER_PATH_PARTITION:
            for (i = 0; i < 5; i++)
                sst25_block_erase_64K(SPIDER_PATH_START_ADDRESS + i * 0x10000, flashIndex);
            break;
        case METER_INFO_PARTITION:
            //chi xoa block ngay hien tai - co can doc lai rtc khong?
            //rtc_read_time();
#ifdef RTC_SUPPORT
#ifdef DCU
            sst25_block_erase_32K(METER_INFO_START_ADDRESS + (datetime.day - 1)*0x8000, flashIndex);
#endif
#endif
            //sst25_chip_erase();
            break;
        case ERASE_ALL_PARTITION:
            sst25_chip_erase(flashIndex);
            break;
        default: //nothing
            break;
    }
    sst25_block_write_protection(flashIndex);
}
/*----------------------------------------------------------------------------*/
void sst25_write_4byte(uint32_t address, uint32_t value, char index)
{
    uint8_t crc8;
	crc8 = crc8bit_buffer((uint8_t*)&value,4);
    sst25_write_byte(address,(uint8_t)value,index);
    sst25_write_byte(address+1,(uint8_t)(value>>8),index);
    sst25_write_byte(address+2,(uint8_t)(value>>16),index);
    sst25_write_byte(address+3,(uint8_t)(value>>24),index);
    sst25_write_byte(address+4,crc8,index);
}
/*----------------------------------------------------------------------------*/
uint32_t sst25_read_4byte(uint32_t address, char index)
{
    uint8_t byte, crc8;
    uint32_t value;
    byte = sst25_read_byte(address,index);
	value = byte;
	byte = sst25_read_byte(address+1,index);
	value = value + (unsigned int)byte*0x100;
	byte = sst25_read_byte(address+2,index);
	value = value + (unsigned long)byte * 0x10000;
	byte = sst25_read_byte(address+3,index);
	value = value + (unsigned long)byte * 0x1000000;
	crc8 = sst25_read_byte(address+4,index);
	if (crc8 != crc8bit_buffer((unsigned char*)&value,4)) flag_flash_error = 1;
	return value;
}
#endif