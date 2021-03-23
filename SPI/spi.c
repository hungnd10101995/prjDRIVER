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
#if defined(__XC8)
    #include <xc.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// Header prjEMETERS
#include "devices.h" 
#include "main.h"

// Header prjLIB
#include "spi.h"
#include "delay.h"
#ifdef RTC_SUPPORT
#include "pcf2129.h"
#endif
#ifdef RF_SUPPORT
#include "rf.h"
#endif


// Header prjDRVIER
#ifdef EEPROM_SUPPORT
    #if defined(AA32_SUPPORT) || defined(AA08_SUPPORT)
    #else
#include "aa1024.h"
    #endif
#endif
#ifdef FLASH_SUPPORT
#include "sst25vf016.h"
#endif
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/


/*===========================================================================*/
/*  G L O B A L   D A T A   D E F I N I T I O N S                            */
/*===========================================================================*/


/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/


/*===========================================================================*/
/* L O C A L   D A T A   D E F I N I T I O N S                               */
/*===========================================================================*/


/*===========================================================================*/
/* L O C A L   F U N C T I O N   P R O T O T Y P E S                         */
/*===========================================================================*/
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================

/*===========================================================================*/
/* G L O B A L   F U N C T I O N S   P R O T O T Y P E S                     */
/*===========================================================================*/
void spi_init(void)
{
    //cau hinh I/O
    //sclk output
    spi_sclk_low();    
    spi_sclk_dir_output();
    //sdi output
    spi_sdi_dir_output();
    //sdo input
    spi_sdo_dir_input();
#if defined(_DT01MRF_PCB_TI_S8M_T_V13_) // da co tro ngoai
//    spi_sdo_pullup_enable();
//    spi_sdo_pullup();
#else
    spi_sdo_pullup_enable();
    #if defined(_DT03P_RF_) || defined(_DT01P_RF_) || defined(_DT01M_RF_)
    spi_sdo_pullup(); //ko tat nguon eeprom khi mat dien, len len de phuc vu module ngoai
    #else
    spi_sdo_pulldown(); //tat nguon eeprom khi mat dien nen pull up se ton dong
    #endif
#endif
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
void spi_disable(void)
{
#ifdef EEPROM_1_SUPPORT
    eeprom_cs1_high();   
#endif
#ifdef EEPROM_2_SUPPORT
    eeprom_cs2_high();
#endif
#ifdef EEPROM_3_SUPPORT //DCU
    eeprom_cs3_high();
#endif
#ifdef EEPROM_4_SUPPORT //DCU
    eeprom_cs4_high();
#endif
#ifdef FLASH_1_SUPPORT
    flash_cs1_output_high();
#endif
#ifdef FLASH_2_SUPPORT
    flash_cs2_output_high();
#endif
#ifdef RTC_SUPPORT
#ifdef PCF2129_SUPPORT
    pcf2129_cs_high();
#endif
#endif
#ifdef RF_SUPPORT
    rf_cs_output_high();   
//#else 
//    RF_CS_OUT set RF_CS_BIT; //output high
#endif
    
#ifdef _DCU_ //thaodtn: Tuan xem gop chung voi code o tren duoc ko?
    eeprom_cs1_high();
    eeprom_cs2_high();
#ifdef EEPROM_3_SUPPORT    
    eeprom_cs3_high();
#endif
    eeprom_cs4_high();
//    _pcf2129_cs_output_high();
    flash_cs1_output_high();
    _cc1101_cs_output_high();
#endif
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
unsigned char spi_get_byte(void)
{
   unsigned char i;
   unsigned char value = 0;
   spi_sclk_low();
   for (i = 0; i < 8;)
   {
       value <<= 1;
       if (spi_sdo_input()) {value |= 0x01;}
       spi_sclk_high();
#if (SYS_FREQ > 14745600L)
        delay_us(1);
#endif
       NOP(); NOP();  //Su Dung NOP de can bang 50-50 xung clk
       i++; 
       spi_sclk_low();
#if (SYS_FREQ > 14745600L)
        delay_us(1);
#endif
   }
   return value;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : 
//  Created     :
//  Description :
//  Input	    :
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void spi_send_byte(unsigned char value)
{
    unsigned char i;
    spi_sclk_low(); 
    for (i = 0; i < 8;)
    {
        if(value & 0x80) spi_sdi_high();
        else      spi_sdi_low();
        spi_sclk_high();
        value <<= 1;  i++; //Su Dung NOP de can bang 50-50 xung clk
        nop();
        spi_sclk_low();
    }
    spi_sdi_low();
}

////===========================================================================
////FUNCTION NAME:
////PROTOTYPE:
////INPUT:
////OUTPUT:
////DESCRIPTION:
////NOTE:
////============================================================================
//unsigned char spi_get_byte(void)
//{
//   unsigned char i;
//   unsigned char value = 0;
//
//   spi_sclk_low();
//   delay_us(1);//NOP();//delay_us(1);
//
//   for (i = 0; i < 8; i++)
//   {
//       spi_sclk_high();
//       NOP();//delay_us(1);
//       NOP();
//       NOP();
//        // chuyen vi tri doan nay 20120918
//       value <<= 1;
//       if (spi_sdo_input())
//       {
//          value |= 0x01;
//       }
//
//       //spi_sclk_high();
//       //delay_us(1);//NOP();//delay_us(1);
//       spi_sclk_low();
//       delay_us(1);//NOP();//delay_us(1);
//
//   }
//   return value;
//}
//////////////////////////////////////////////////////////////////////////////////
////  Function    : 
////  Created     :
////  Description :
////  Input	    :
////  Output      :
//////////////////////////////////////////////////////////////////////////////////
//void spi_send_byte(unsigned char value)
//{
//    unsigned char i;
//    spi_sclk_low();
//    NOP();
//    for (i = 0; i < 8; i++)
//    {
//        if(value & 0x80)
//        {
//            spi_sdi_high();
//        }
//        else
//        {
//            spi_sdi_low();
//        }
//
//      spi_sclk_high();//spi_sclk_low();
//      NOP(); //delay_us(1);
//      spi_sclk_low();//spi_sclk_high();
//      NOP();////delay_us(1);
//      value <<= 1;
//    }
//    spi_sdi_low();
//    NOP();
//}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
#ifdef _ADE7953_
void spi_ade_init(void)
{
    //cau hinh I/O
    //sclk output
    ade_cs_dir_output();
    ade_cs_output_high();
    ade_sclk_dir_output();
    ade_sclk_output_low();       
    ade_sdo_dir_output();
    ade_sdi_dir_input();
}
//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:
//OUTPUT:
//DESCRIPTION:
//NOTE:
//============================================================================
unsigned char spi_ade_get_byte(void)
{
   unsigned char i;
   unsigned char value = 0;
   NOP();//delay_us(1);
   for (i = 0; i < 8; i++)
   {
        // chuyen vi tri doan nay 20120918
       value <<= 1;
       if (ade_sdi_input())
       {
          value |= 0x01;
       }
       ade_sclk_output_high();
       NOP();//delay_us(1);
       ade_sclk_output_low();
       NOP();//delay_us(1);
   }
   return value;
}
////////////////////////////////////////////////////////////////////////////////
//  Function    : 
//  Created     :
//  Description :
//  Input	    :
//  Output      :
////////////////////////////////////////////////////////////////////////////////
void spi_ade_send_byte(unsigned char value)
{
    unsigned char i;
    NOP();
    for (i = 0; i < 8; i++)
    {
        if(value & 0x80)
        {
            ade_sdo_output_high();
        }
        else
        {
            ade_sdo_output_low();
        }

      ade_sclk_output_high();//spi_sclk_low();
      NOP();////delay_us(1);
      ade_sclk_output_low();//spi_sclk_high();
      NOP();////delay_us(1);
      value <<= 1;
    }
    ade_sdo_output_low();
    NOP();
}
#endif
/*===========================================================================*/
/* H I S T O R Y                                                             */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/
