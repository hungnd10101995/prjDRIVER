/*===========================================================================*/
/*
SOURCE_FILE:		cc1101.c
APPLICATION:		RF
MODULE:				RF
ORIGINAL AUTHOR:	thaodtn
VERSION:			2.0
DATE:				21/10/2016
*/
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#if defined(__XC8)||defined(__XC16) 
#include <xc.h>
#endif
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Header prjEMETERS
#include "main.h"
#include "hardware.h"
#include "devices.h"

// Header prjLIB
#include "rf.h"

// Header prjDRVIER
#include "cc1101.h"
#include "spi.h"
#include "delay.h"
#include "convert.h"

#ifdef RF_SUPPORT
#ifdef CC1101_MODULE_SUPPORT //endif to end of file
/*===========================================================================*/

const unsigned char  rfSettings_cpcit[CC1101_CONFIG_SETTING_MAX] = {
    0x07,//07   //*0x0B IOCFG2    GDO2 output pin configuration.
    0x2E,   //*0x2E IOCFG1    GDO1 output pin configuration.
    0x2E,   //*0x0C IOCFG0   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
    0x07,    // FIFOTHR  RX FIFO and TX FIFO thresholds
    0xD3,    // SYNC1  Sync word, high byte
    0x91,    // SYNC0  Sync word, low byte
    0x3D,   // PKTLEN    Packet length. vv
#ifdef RFMESH_BROADCAST
    0x0C,
#else
    0x0E, //0x05 PKTCTRL1  Packet automation control. - CRC-AutoFlush
#endif
    0x05,   //vv *0x12 PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x00,   // CHANNR    Channel number.
    0x06,   //0x0C,   //*0x06 FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
#if defined(FREQ_408MHz)
    0x0F,   // FREQ2     Frequency control word, high byte.
    0xBA,   // FREQ1     Frequency control word, middle byte.
    0x57,   // FREQ0  408.925  //433.050 MHz  Frequency control word, low byte.
#elif defined(FREQ_424MHz)
    0x10,   // FREQ2     Frequency control word, high byte.
    0x4E,   // FREQ1     Frequency control word, middle byte.
    0xC4,   // FREQ0  424MHz  Frequency control word, low byte.
#elif defined(FREQ_433MHz)
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0xE1,   // FREQ0  433.050 MHz  Frequency control word, low byte.
#else
#warning "Check define frequency of cc1101 in cc1101.c"
#endif
    0xF7, //for 4800; F6:2400bps  //VV MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
    0x0B, //0x09    MDMCFG2   Modem configuration.  2_FSK
    0x02,   // MDMCFG1   Modem configuration.  (0x22: 4 byte preamble; 0x02: 2 byte...)  20_3
    0xF8,   // MDMCFG0   Modem configuration.
    0x15,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x07,   //VV *0x07 MCSM2     Main Radio Control State Machine configuration.
    0x3F,   // MCSM1  -> Sau khi TX hoac RC de`u dua ve che do RX
    //  Main Radio Control State Machine configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,//0x36, //add 12-9-08 for offset compensation //0x16 for Non_ofset_compense,    FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x03,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0x87,   // WOREVT1  0x1E
    0x6B,   // WOREVT0  0x1F
    0xF8,   // WORCTRL  0x20
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0xE9,//0xA9,   //*0xE9 FSCAL3    Frequency synthesizer calibration.
    0x2A,//0x2A,   //*0x2A FSCAL2    Frequency synthesizer calibration.
    0x00,//0x0D,   //*0x00 FSCAL1    Frequency synthesizer calibration.
    0x1F,//0x11,   //*0x1F FSCAL0    Frequency synthesizer calibration.
    //them cho psmart
    0x41,  // RCCTRL1             RC Oscillator Configuration
    0x00,  // RCCTRL0             RC Oscillator Configuration
    0x59,  // FSTEST              Frequency Synthesizer Calibration Control
    0x7F,  // PTEST               Production Test
    0x3F,  // AGCTEST             AGC Test
    0x81,  // TEST2               Various Test Settings
    0x35,  // TEST1               Various Test Settings
    0x0B  // TEST0               Various Test Settings
};

const uint8_t rfsetting_PSMART[47] = {
0x06 , 
0x2E ,
0x2E ,
0x07 ,
0x2D ,
0xD4 ,
0x3E,//vuaha sua HU nhan khong > 36 bytes 0x24 , // pkt len
0x04 ,
0x01 ,
0x00 ,
0x00 ,
0x06 ,
0x00 ,
0x0F ,
0xBA ,
0x57 ,
0xF8 ,
0x93 ,
0x12 ,
0x52 ,
0xF8 ,
0x15 ,
0x07 ,
0x30 ,
0x18 ,
0x16 ,
0x6C ,
0x03 ,
0x40 ,
0x91 ,
0x87 ,
0x6B ,
0xFB ,
0x56 ,
0x10 ,
0xE9 ,
0x2A ,
0x00 ,
0x1F ,
0x41 ,
0x00 ,
0x59 ,
0x7F ,
0x3F ,
0x81 ,
0x35 ,
0x0B
};
const unsigned char  rfSettings_LTE66[39] = {
    0x0D,  // IOCFG2   _GDO2 laf chan UART-RX           GDO2 Output Pin Configuration
    0x2E,  // IOCFG1              GDO1 Output Pin Configuration
    0x2E,  // IOCFG0              GDO0 Output Pin Configuration
    0x07,  // FIFOTHR             RX FIFO and TX FIFO Thresholds
    0xD3,  // SYNC1               Sync Word, High Byte
    0x91,  // SYNC0               Sync Word, Low Byte
    0xFF,  // PKTLEN              Packet Length
    0x04,  // PKTCTRL1            Packet Automation Control
    0x62,//0x72, //0x32 // PKTCTRL0            Packet Automation Control

    0x00,  // ADDR                Device Address
    0x00,  // CHANNR              Channel Number
    0x06,  // FSCTRL1             Frequency Synthesizer Control
    0x00,  // FSCTRL0             Frequency Synthesizer Control
    0x0F,   // FREQ2     Frequency control word, high byte.
    0xBA,   // FREQ1     Frequency control word, middle byte.
    0x57,   // FREQ0     Frequency control word, low byte.

    0x28,  //0xF8//BandWidth           Modem Configuration
    0x83,  // MDMCFG3             Modem Configuration
    0x10,   //GFSK //00 = 2-FSK  // MDMCFG2             Modem Configuration
    0x00,  // MDMCFG1             Modem Configuration
    0xF8,  // MDMCFG0             Modem Configuration
    0x15, //0x31,  // DEVIATN             Modem Deviation Setting

    0x07,  // MCSM2               Main Radio Control State Machine Configuration
    0x3F,  // MCSM1               Main Radio Control State Machine Configuration

    0x18,  // MCSM0               Main Radio Control State Machine Configuration
    0x16,  // FOCCFG              Frequency Offset Compensation Configuration
    0x6C,  // BSCFG               Bit Synchronization Configuration
    0x03,  // AGCCTRL2            AGC Control
    0x40,  // AGCCTRL1            AGC Control
    0x91,  // AGCCTRL0            AGC Control
    0x87,  // WOREVT1             High Byte Event0 Timeout
    0x6B,  // WOREVT0             Low Byte Event0 Timeout
    0xFB,  // WORCTRL             Wake On Radio Control
    0x56,  // FREND1              Front End RX Configuration
    0x10,  // FREND0              Front End TX Configuration
    0xE9,  // FSCAL3              Frequency Synthesizer Calibration
    0x2A,  // FSCAL2              Frequency Synthesizer Calibration
    0x00,  // FSCAL1              Frequency Synthesizer Calibration
    0x1F  // FSCAL0              Frequency Synthesizer Calibration
};
const unsigned char  wireless_uart_Settings[39] = {
    0x0D,  // 0x0B IOCFG2    Serial Data Output. Used for asynchronous serial mode.
    0x2E,  // IOCFG1              GDO1 Output Pin Configuration
    0x2E,  // IOCFG0              In TX, the GDO0 pin is used for data input (TX data).
    0x07,  // FIFOTHR             RX FIFO and TX FIFO Thresholds
    0xD3,  // SYNC1               Sync Word, High Byte
    0x91,  // SYNC0               Sync Word, Low Byte
    0xFF,  // PKTLEN              Packet Length
    0x04,  // PKTCTRL1            Packet Automation Control
    0x32,  // PKTCTRL0            Packet Automation Control
    0x00,  // ADDR                Device Address
    0x00,  // CHANNR              Channel Number
    0x06,  // FSCTRL1             Frequency Synthesizer Control
    0x00,  // FSCTRL0             Frequency Synthesizer Control
    0x0F,  // FREQ2               Frequency Control Word, High Byte
    0xBA,  // FREQ1               Frequency Control Word, Middle Byte
    0x56, //0x63, //dieu chinh lai tan so 408.925 (theo may phan tich pho)//0x56,  // FREQ0               Frequency Control Word, Low Byte

    0x1A,//0x1A,//9600bps  // 9600 MDMCFG4 , F5=1200bps            Modem Configuration
    0x83,  // MDMCFG3             Modem Configuration
    0x00,  // MDMCFG2             Modem Configuration
    0x00,  // MDMCFG1             Modem Configuration
    0xF8,  // MDMCFG0             Modem Configuration
    0x31,  // DEVIATN             Modem Deviation Setting

    0x07,  // MCSM2               Main Radio Control State Machine Configuration
    0x3F,  // MCSM1               Main Radio Control State Machine Configuration

    0x18,  // MCSM0               Main Radio Control State Machine Configuration
    0x16,  // FOCCFG              Frequency Offset Compensation Configuration
    0x6C,  // BSCFG               Bit Synchronization Configuration
    0x03,  // AGCCTRL2            AGC Control
    0x40,  // AGCCTRL1            AGC Control
    0x91,  // AGCCTRL0            AGC Control
    0x87,  // WOREVT1             High Byte Event0 Timeout
    0x6B,  // WOREVT0             Low Byte Event0 Timeout
    0xFB,  // WORCTRL             Wake On Radio Control
    0x56,  // FREND1              Front End RX Configuration
    0x10,  // FREND0              Front End TX Configuration
    0xE9,  // FSCAL3              Frequency Synthesizer Calibration
    0x2A,  // FSCAL2              Frequency Synthesizer Calibration
    0x00,  // FSCAL1              Frequency Synthesizer Calibration
    0x1F  // FSCAL0              Frequency Synthesizer Calibration
};
const unsigned char rfSettings_DTS27[39] = {
    0x2E, //*0x0B IOCFG2    GDO2 output pin configuration.
    0x2E, //*0x2E IOCFG1    GDO1 output pin configuration.
    0x07, //0x06,   //*0x0C IOCFG0   GDO0 output pin configuration.
    0x07, // FIFOTHR  RX FIFO and TX FIFO thresholds
    0x9B, // SYNC1  Sync word, high byte
    0xAD, // SYNC0  Sync word, low byte
    0x0D, // PKTLEN    Packet length. vv
    0x04, //0x05 PKTCTRL1  Packet automation control. - CRC-AutoFlush
    0x44, //vv *0x12 PKTCTRL0  Packet automation control.
    0x00, // ADDR      Device address.
    0x00, // CHANNR    Channel number.
    0x08, //0x0C,   //*0x06 FSCTRL1   Frequency synthesizer control.
    0x00, // FSCTRL0   Frequency synthesizer control.

    0x0F, // FREQ2     Frequency control word, high byte.
    0xBA, // FREQ1     Frequency control word, middle byte.
    0x56, // FREQ0  433.050 MHz  Frequency control word, low byte.

    0x5B, //F6:2400bps  //VV MDMCFG4   Modem configuration.
    0xF8, // MDMCFG3   Modem configuration.
    0x03, //0x09    MDMCFG2   Modem configuration.  2_FSK
    0x22, // MDMCFG1   Modem configuration.  (0x22: 4 byte preamble; 0x02: 2 byte...)  20_3
    0xF8, // MDMCFG0   Modem configuration.
    0x47, // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x07, //VV *0x07 MCSM2     Main Radio Control State Machine configuration.
    0x3F, //30  // MCSM1  -> Sau khi TX hoac RC de`u dua ve che do RX
    //  Main Radio Control State Machine configuration.
    0x18, // MCSM0     Main Radio Control State Machine configuration.
    0x1D, //add 12-9-08 for offset compensation //0x16 for Non_ofset_compense,
    0x1C, // BSCFG     Bit synchronization Configuration.
    0xC7, // AGCCTRL2  AGC control.
    0x00, // AGCCTRL1  AGC control.
    0XB2, // AGCCTRL0  AGC control.
    0x87, // WOREVT1  0x1E
    0x6B, // WOREVT0  0x1F
    0xF8, // WORCTRL  0x20
    0xB6, // FREND1    Front end RX configuration.
    0x10, //0x17,//vudh //0x10,   // FREND0    Front end RX configuration.
    0xEF, //0xA9,   //*0xE9 FSCAL3    Frequency synthesizer calibration.
    0x0B, //0x2A,   //*0x2A FSCAL2    Frequency synthesizer calibration.
    0x2A, //0x0D,   //*0x00 FSCAL1    Frequency synthesizer calibration.
    0x1F//0x11,   //*0x1F FSCAL0    Frequency synthesizer calibration.
};
const unsigned char rfSettings_GELEX[39] =  
{
    0x07,
    0x2E,
    0x06,
    0x47,
    0xD3,
    0x91,
    0xFF,
    0x64,
    0x45,
    0x00,
    0x00,
    0x06,
    0x00,
    0x0F,
    0xBA,
    0x56,
    0xF6,
    0x83,
    0x13,
    0x22,
    0xF8,
    0x15,
    0x07,
    0x30,
    0x18,
    0x16,
    0x6C,
    0x03,
    0x40,
    0x91,
    0x87,
    0x6B,
    0xFB,
    0x56,
    0x10,
    0xEF,
    0x0C,
    0x28,
    0x1F
};
const unsigned char rfSettings_DDS26D[] =
{
    0x07,   //*0x0B IOCFG2    GDO2 output pin configuration.
    0x2E,   //*0x2E IOCFG1    GDO1 output pin configuration.
    0x2E, //0x06,   //*0x0C IOCFG0   GDO0 output pin configuration.
    0x47,    // FIFOTHR  RX FIFO and TX FIFO thresholds
    0x9B,    // SYNC1  Sync word, high byte
    0xAD,    // SYNC0  Sync word, low byte
    0x3E,   // PKTLEN    Packet length. vv
    0x04, //0x05 PKTCTRL1  Packet automation control. - CRC-AutoFlush
    0x44,   //vv *0x12 PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x00,   // CHANNR    Channel number.
    0x06,   //0x0C,   //*0x06 FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.

    0x0F,   // FREQ2     Frequency control word, high byte.
    0xBA,   // FREQ1     Frequency control word, middle byte.
    0x56,   // FREQ0  433.050 MHz  Frequency control word, low byte.

    0xC9, //F6:2400bps  //VV MDMCFG4   Modem configuration.
    0x93,   // MDMCFG3   Modem configuration.
    0x03, //0x09    MDMCFG2   Modem configuration.  2_FSK
    0xA2,   // MDMCFG1   Modem configuration.(0x22: 4 byte preamble; 0x02: 2 byte...)
    0xF8,   // MDMCFG0   Modem configuration.
    0x35,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x00,   //VV *0x07 MCSM2     Main Radio Control State Machine configuration.
    0x30, //30  // MCSM1  -> Sau khi TX hoac RC de`u dua ve che do RX
    //  Main Radio Control State Machine configuration.            
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16, //add 12-9-08 for offset compensation //0x16 for Non_ofset_compense,
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0X91,   // AGCCTRL0  AGC control.
    0x87,   // WOREVT1  0x1E
    0x6B,   // WOREVT0  0x1F
    0xFB,   // WORCTRL  0x20
    0x56,   // FREND1    Front end RX configuration.
    0x17,//0x17,//vudh //0x10,   // FREND0    Front end RX configuration.
    0xE9,//0xA9,   //*0xE9 FSCAL3    Frequency synthesizer calibration.
    0x2A,//0x2A,   //*0x2A FSCAL2    Frequency synthesizer calibration.
    0x00,//0x0D,   //*0x00 FSCAL1    Frequency synthesizer calibration.
    0x1F//0x11,   //*0x1F FSCAL0    Frequency synthesizer calibration.
};
//======================================================================//
//  Function  : cc1101_cmd_strobe
//  Comments  : gui lenh cho cc1101
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_cmd_strobe(uint8_t strobe)
{
    spi_disable();
    spi_sclk_low();
    cc1101_cs_output_low();
    cc1101_write_byte(strobe);
    cc1101_cs_output_high();
}
//======================================================================//
//  Function  : cc1101_read_byte
//  Comments  : doc 1 byte tu cc1101
//  Input     : None
//  Output    : None
//======================================================================//
unsigned char cc1101_read_byte(void)
{
    unsigned char  i;
    unsigned char  value = 0;
    for (i = 0; i < 8; i++) {
        spi_sclk_high();
        value <<= 1;
        if(spi_sdo_input())  {
            value |= 0x01;
        }
        spi_sclk_low();
    }
    return(value);
}
//======================================================================//
//  Function  : cc1101_write_byte
//  Comments  : ghi 1 byte xuong cc1101
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_write_byte(unsigned char value)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        if(value & 0x80)
        {
            spi_sdi_high();
        }
        else {
            spi_sdi_low();
        }
        spi_sclk_high();
        value <<= 1;
        spi_sclk_low();
    }
    spi_sdi_low();
}
//======================================================================//
//  Function  : cc1101_init
//  Comments  : khoi tao ban dau cho cc1101
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_init(void)
{
#ifdef RFPA_SUPPORT
    cc1101_tx_pa_dir_output();
    cc1101_rx_pa_dir_output();
    cc1101_power_pa_dir_output();
#endif
    //chan chon chip
    cc1101_cs_dir_output();
    cc1101_cs_output_high();
    //ngat RF
    cc1101_gdo2_dir_input();
    cc1101_gdo2_pullup_enable();
#if defined(_DT03M_RF_)
    //thaodtn test cc1101_gdo2_pulldown();//gdo2_pullup();   //up -> down giam duoc 0.1mA
#else
    cc1101_gdo2_pullup();
#endif
    cc1101_gdo2_interrupt_low_to_high();
    cc1101_gdo2_interrupt_flag_clear();
    cc1101_gdo2_interrupt_enable();//cc1101_interrupt_enable();  
#ifdef _PIC32_
    rf_gdo2_set_interrupt_priority();
#endif
    cc1101_reset();
    delay_ms(50);
    cc1101_config_power(0xC0);
}
//======================================================================//
//  Function  : cc1101_reset
//  Comments  : reset cc1101
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_reset(void)
{
    int i = 0;
    spi_disable();
    spi_sclk_high();
    spi_sdi_low();
    cc1101_cs_output_low();
    cc1101_cs_output_high();
    delay_us(50);
    spi_sclk_low();
    cc1101_cs_output_low();
    while ((i<100)&&spi_sdo_input())//tuantm, rf ko reset dc van cho pass
    {
        i++;
        delay_ms(1);
    }
    i = 0;
    cc1101_write_byte(CC1101_SRES);
    while ((i<100)&&spi_sdo_input())//tuantm, rf ko reset dc van cho pass
    {
        i++;
        delay_ms(1);
    }
    cc1101_cs_output_high();
}
void cc1101_shutdown(void)
{
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(1);
    cc1101_cmd_strobe(CC1101_SPWD);
}
//======================================================================//
//  Function  : cc1101_refresh
//  Comments  : refresh cc1101
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_refresh(void)
{
    uint8_t rf_tmp = 0;
    #ifdef RFPA_SUPPORT
    cc1101_tx_pa_dir_output();
    cc1101_rx_pa_dir_output();
    cc1101_power_pa_dir_output();
    #endif
    //chan chon chip
    cc1101_cs_dir_output();
    cc1101_cs_output_high();
    //ngat RF
    cc1101_gdo2_dir_input();
    cc1101_gdo2_pullup_enable();
#if defined(_DT03M_RF_)
    //thaodtn test cc1101_gdo2_pulldown();//gdo2_pullup();   //up -> down giam duoc 0.1mA
#else
    cc1101_gdo2_pullup();
#endif
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(2);
    cc1101_reset();
    delay_ms(50);
    cc1101_config_power(0xC0);
    delay_ms(2);
    rf_tmp = cc1101_read_reg(CC1101_RXFIFO);    
//    cc1101_gdo2_interrupt_low_to_high(); // bo 3 lenh nay vi da co o ham rf refresh
//    cc1101_gdo2_interrupt_flag_clear();
//    cc1101_gdo2_interrupt_enable();
}
//======================================================================//
//  Function  : cc1101_read_reg
//  Comments  : doc thanh ghi cua cc1101
//  Input     : addr: dia chi thanh ghi
//  Output    : register value
//======================================================================//
uint8_t cc1101_read_reg(uint8_t addr)
{
    uint8_t value;
    spi_disable();
	spi_sclk_low();
    cc1101_cs_output_low();
    addr |= 0x80;
    cc1101_write_byte(addr);
    value = cc1101_read_byte();
    cc1101_cs_output_high();
    return value;
}
//======================================================================//
//  Function  : cc1101_read_status_addr
//  Comments  : doc thanh ghi trang thai cua cc1101
//  Input     : addr: dia chi thanh ghi
//  Output    : status register value
//======================================================================//
uint8_t cc1101_read_status_addr(uint8_t addr)
{
    uint8_t value;
    spi_sclk_low();
    cc1101_cs_output_low();
    addr |= 0xC0;
    cc1101_write_byte(addr);
    value = cc1101_read_byte();
    cc1101_cs_output_high();
    return(value);
}
//======================================================================//
//  Function  : cc1101_write_reg
//  Comments  : ghi gia tri vao thanh ghi cua cc1101
//  Input     : addr: dia chi thanh ghi; value: gia tri can ghi
//  Output    : none
//======================================================================//
void cc1101_write_reg(uint8_t addr, uint8_t value)
{
    spi_disable();
    spi_sclk_low();
    cc1101_cs_output_low();
    cc1101_write_byte(addr);
    cc1101_write_byte(value);
    cc1101_cs_output_high();
}
//======================================================================//
//  Function  : cc1101_read_status
//  Comments  : doc trang thai cua cc1101
//  Input     : addr: dia chi thanh ghi
//  Output    : status register value
//======================================================================//
uint8_t cc1101_read_status(void)
{
    //== doc thanh ghi trang thai cua cc1101================================
    uint8_t status_cc1101;
	spi_disable();
    spi_sclk_low();
    cc1101_cs_output_low();
    cc1101_write_byte(CC1101_SNOP);
    // Lay byte Status
    status_cc1101 = cc1101_read_byte();
    status_cc1101 = status_cc1101 & 0x70; // 0b01110000;
    cc1101_cs_output_high();
    //doc thanh ghi GDO2
    status_cc1101 = cc1101_read_reg(CC1101_IOCFG2);
    cc1101_cs_output_high();
    /*
    //doc bo dem
    RX_len = CC1101_BUFFER_LENGTH;
    cc1101_read_burst_reg(CC1101_RXFIFO,CC1101_RX_BUFFER,RX_len);
    uint8_t data8_rf = cc1101_read_reg (CC1101_RXFIFO);
    data8_rf = cc1101_read_reg (CC1101_RXFIFO);
    */
    return status_cc1101;
}
//======================================================================//
//  Function  : cc1101_read_burst_reg
//  Comments  : doc nhieu thanh ghi cua cc1101
//  Input     : addr: dia chi thanh ghi bat dau
//				cnt: so thanh ghi can doc
//  Output    : buf: bo dem gan gia tri vao
//======================================================================//
void cc1101_read_burst_reg (uint8_t addr, uint8_t *buf, uint8_t cnt)
{
    uint8_t i;
    spi_disable();
	spi_sclk_low();
    cc1101_cs_output_low();
    addr |= 0xC0;
    cc1101_write_byte(addr);

    for (i = 0; i < cnt; i++)
    {
        *(buf + i) = cc1101_read_byte();
    }
    cc1101_cs_output_high();
}
//======================================================================//
//  Function  : cc1101_write_burst_reg
//  Comments  : ghi nhieu thanh ghi cua cc1101
//  Input     : addr: dia chi thanh ghi bat dau
//				buf: bo dem gan gia tri vao
//				cnt: so thanh ghi can ghi
//  Output    : 
//======================================================================//
void cc1101_write_burst_reg(uint8_t addr, uint8_t *buf, uint8_t cnt)
{
    uint8_t i;
    uint8_t value;
    spi_disable();
    NOP();
    spi_sclk_low();
    cc1101_cs_output_low();
    addr |= 0x40;
    cc1101_write_byte(addr);
    for (i = 0; i < cnt; i++)
    {
        value = *(buf + i);
        cc1101_write_byte(value);
    }
    cc1101_cs_output_high();
}

//======================================================================//
//  Function  : cc1101_write_rf_setting
//  Comments  : ghi cau hinh rf cho cc1101
//  Input     : setting_buffer: bo dem chua gia tri thanh ghi
//				length_buffer: chieu dai bo dem setting_buffer
//  Output    : none
//======================================================================//
void cc1101_write_rf_setting(uint8_t *setting_buffer, uint8_t length_buffer)
{
    // quanhdt 20/10/2019 uint8_t buff[CC1101_CONFIG_SETTING_MAX];
    unsigned char i;
    spi_sclk_low();
    cc1101_cs_output_low();
    cc1101_write_byte(WRITE_BURST+CC1101_IOCFG2);    
    for (i = 0; i < length_buffer; i++)
    {
        cc1101_write_byte(setting_buffer[i]);
    }    
    cc1101_cs_output_high();
#if defined(ROUTER) || defined(_DCU_) || defined(_FCI_)
    //doc lai kiem tra luc khoi tao
    if(rf.flags.check_config == FALSE) return;
    rf.flags.check_config = FALSE;
    cc1101_read_burst_reg(CC1101_IOCFG2, (uint8_t *) buff, length_buffer);
    i = 0;
    rf.flags.hardware_error = FALSE;    
    //so sanh voi gia tri ghi vao
    if (memcmp(setting_buffer, buff, length_buffer) != 0) 
    {
#ifdef CC1101_CONFIG_RETRY
        rf.flags.hardware_error = TRUE;
#else //neu ko retry
        //basic_timer_close();
        CloseTimer1();
        while (1); //reset lai Router neu ko cau hinh dc
#endif
    } else {
        rf.flags.hardware_error = FALSE;
		led_rf_tx_off();
    }
#endif
}
void cc1101_write_rf_setting_PSMART() 
{
    spi_sclk_low();
    cc1101_cs_output_low();
    cc1101_write_reg(CC1101_IOCFG2, 0x06); //GDO0 Output Pin Configuration
    cc1101_write_reg(CC1101_FIFOTHR, 0x47); //RX FIFO and TX FIFO Thresholds
    cc1101_write_reg(CC1101_SYNC1, 0x2D); //Sync Word, High Byte
    cc1101_write_reg(CC1101_SYNC0, 0xD4); //Sync Word, Low Byte
    cc1101_write_reg(CC1101_PKTLEN, 0x24); //Packet Length
    cc1101_write_reg(CC1101_CHANNR, 0x00);
    cc1101_write_reg(CC1101_PKTCTRL0, 0x01); //Packet Automation Control
    cc1101_write_reg(CC1101_FSCTRL1, 0x06); //Frequency Synthesizer Control
    cc1101_write_reg(CC1101_FREQ2, 0x0F); //Frequency Control Word, High Byte
    cc1101_write_reg(CC1101_FREQ1, 0xBA); //Frequency Control Word, Middle Byte
    cc1101_write_reg(CC1101_FREQ0, 0x57); //Frequency Control Word, Low Byte
    cc1101_write_reg(CC1101_MDMCFG4, 0xF8); //Modem Configuration
    cc1101_write_reg(CC1101_MDMCFG3, 0x93); //Modem Configuration
    cc1101_write_reg(CC1101_MDMCFG2, 0x12); //Modem Configuration
    cc1101_write_reg(CC1101_MDMCFG1, 0x52); //Modem Configuration
    cc1101_write_reg(CC1101_DEVIATN, 0x15); //Modem Deviation Setting
    cc1101_write_reg(CC1101_MCSM0, 0x18); //Main Radio Control State Machine Configuration
    cc1101_write_reg(CC1101_FOCCFG, 0x16); //Frequency Offset Compensation Configuration
    cc1101_write_reg(CC1101_WORCTRL, 0xFB); //Wake On Radio Control
    cc1101_write_reg(CC1101_FSCAL3, 0xE9); //Frequency Synthesizer Calibration
    cc1101_write_reg(CC1101_FSCAL2, 0x2A); //Frequency Synthesizer Calibration
    cc1101_write_reg(CC1101_FSCAL1, 0x00); //Frequency Synthesizer Calibration
    cc1101_write_reg(CC1101_FSCAL0, 0x1F); //Frequency Synthesizer Calibration
    cc1101_write_reg(CC1101_TEST2, 0x81); //Various Test Settings
    cc1101_write_reg(CC1101_TEST1, 0x35); //Various Test Settings
    cc1101_write_reg(CC1101_RSSI, 0x80); //Received Signal Strength Indication
    cc1101_write_reg(CC1101_MARCSTATE, 0x01); //Main Radio Control State Machine State
    cc1101_write_reg(CC1101_VCO_VC_DAC, 0x94); //Current Setting from PLL Calibration Module
    cc1101_cs_output_high();
}
//======================================================================//
//  Function  : cc1101_set_stage
//  Comments  : cau hinh trang thai hoat dong cho cc1101
//  Input     : stage: RX / TX / IDLE
//  Output    : none
//======================================================================//
//void cc1101_set_stage_rx(void)
//{
//	cc1101_gdo2_interrupt_low_to_high();
//	cc1101_write_reg(CC1101_IOCFG2,0x07);	  // ext_edge_low_to_high_status
//	cc1101_write_reg(CC1101_FIFOTHR,0x0F);	  // nhan toi da 64 byte
//	cc1101_cmd_strobe(CC1101_SIDLE);
//	delay_ms(10);
//	cc1101_cmd_strobe(CC1101_SFRX);
//	cc1101_cmd_strobe(CC1101_SFRX);
//	cc1101_cmd_strobe(CC1101_SRX);
//}
//void cc1101_set_stage_tx(void)
//{
//	// ghi lai bo dem phat
//	//cc1101_write_reg(CC1101_FIFOTHR,0);	  // phat toi da 60 byte
//	cc1101_cmd_strobe(CC1101_STX);
//}
//void cc1101_set_stage_idle(void)
//{
//	cc1101_cmd_strobe(CC1101_SIDLE);
//	delay_ms(10);
//}
void cc1101_set_stage(unsigned char stage)
{
	//cc1101.stage = stage;
	switch (stage)
	{
	case CC1101_TX_STATE:
		// ghi lai bo dem phat
		//cc1101_write_reg(CC1101_FIFOTHR,0);	  // phat toi da 60 byte
            //rf.status = RF_TX;
            cc1101_cmd_strobe(CC1101_STX);
            cc1101_gdo2_interrupt_high_to_low(); 
            cc1101_gdo2_interrupt_flag_clear();
            cc1101_gdo2_interrupt_enable();
            break;
		//case CC1101_RX:
		//	break;
	default:
            //delay_ms(100);
            cc1101_cmd_strobe(CC1101_SIDLE);
            delay_ms(1); //delay_ms(10);
            if(rf.rf_type != RF_TYPE_PSMART) 
             cc1101_write_reg(CC1101_IOCFG2,0x07);	  // ext_edge_low_to_high_status
            cc1101_write_reg(CC1101_FIFOTHR,0x0F);	  // nhan toi da 64 byte
            //cc1101_write_reg(CC1101_FIFOTHR,0x0F);	  // nhan toi da 64 byte
            cc1101_cmd_strobe(CC1101_SFRX);
            cc1101_cmd_strobe(CC1101_SFRX);
            cc1101_cmd_strobe(CC1101_SRX);
            //kiem tra nen chan gdo2 con o muc 1 ti read buffer 1 lan nua de keo xuong
            if(cc1101_gdo2_input())
                cc1101_read_reg(CC1101_RXFIFO);
            rf.status = RF_RX;
            if(rf.rf_type != RF_TYPE_PSMART)
            {
                cc1101_gdo2_interrupt_low_to_high();
            }
            else
            {
                cc1101_gdo2_interrupt_high_to_low();
            }
            //cc1101_gdo2_interrupt_flag_clear();
            cc1101_gdo2_interrupt_enable();
	    break;
	}
}
//======================================================================//
//  Function  : cc1101_flush_tx
//  Comments  : xoa bo dem phat TX
//  Input     : none
//  Output    : none
//======================================================================//
void cc1101_flush_tx(void)
{
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_us(1000);
    cc1101_cmd_strobe(CC1101_SFTX);
    delay_us(1000);//100
}
//======================================================================//
//  Function  : cc1101_flush_rx
//  Comments  : xoa bo dem thu RX
//  Input     : none
//  Output    : none
//======================================================================//
void cc1101_flush_rx(void)
{
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_us(1000);
    cc1101_cmd_strobe(CC1101_SFRX);
    delay_us(1000);//100
}
//======================================================================//
//  Function  : cc1101_config_broadcast
//  Comments  : cau hinh broadcast
//  Input     : none
//  Output    : none
//======================================================================//
void cc1101_config_broadcast(void)
{
    cc1101_write_reg(CC1101_PKTCTRL1,0x0C);
    cc1101_write_reg(CC1101_ADDR,0x00); // ghi dia chi 0 khi phat ban tin HHU
}
//======================================================================//
//  Function  : cc1101_config_address
//  Comments  : cau hinh dia chi cho cc1101
//  Input     : none
//  Output    : none
//======================================================================//
void cc1101_config_address(uint8_t addr, uint8_t addr_0_enable) //khuet data dong
{
    //if (addr_0_enable == TRUE)
    //{
        cc1101_write_reg(CC1101_PKTCTRL1,0x0E);//khuet 2 mode : 0 vs ADDR
    //}
    cc1101_write_reg(CC1101_ADDR,addr); // ghi dia chi 0 khi phat ban tin HHU
}
//======================================================================//
//  Function  : cc1101_goto_408_4800
//  Comments  : cau hinh cc1101 cua hu sang tan so 408.925mhz - 4800bps
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_goto_408_4800(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x0F);
	cc1101_write_reg(CC1101_FREQ1, 0xBA);
	cc1101_write_reg(CC1101_FREQ0, 0x57); //tuantm sua lai thanh 0x57
	cc1101_write_reg(CC1101_MDMCFG4, 0xF7);    
}
void cc1101_goto_433_4800(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x10);
    cc1101_write_reg(CC1101_FREQ1, 0xA7);
    cc1101_write_reg(CC1101_FREQ0, 0xE1);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF7);    
}
void cc1101_goto_433_2400(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x10);
    cc1101_write_reg(CC1101_FREQ1, 0xA7);
    cc1101_write_reg(CC1101_FREQ0, 0xE1);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF6);    
}
void cc1101_goto_408_2400(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x0F);
	cc1101_write_reg(CC1101_FREQ1, 0xBA);
	cc1101_write_reg(CC1101_FREQ0, 0x57);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF6);    
}
void cc1101_goto_408_1200(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x0F);
	cc1101_write_reg(CC1101_FREQ1, 0xBA);
	cc1101_write_reg(CC1101_FREQ0, 0x57);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF5);    
}
#ifdef _DCU_
void cc1101_goto_frequecy(void)
{
    switch(dcu.board.rf_frequecy)
    {
        case RF_4334800:
            cc1101_goto_433_4800();
            break;
        case RF_4332400:
            cc1101_goto_433_2400();
            break;
        case RF_4331200:
            cc1101_goto_433_1200();
            break;
        case RF_4082400:
            cc1101_goto_408_2400();
            break;
        case RF_4081200:
            cc1101_goto_408_1200();
            break;
        case RF_4084800:
        default:
            cc1101_goto_408_4800();
            break;
    }
}
#endif
//======================================================================//
//  Function  : cc1101_goto_433_1200
//  Comments  : cau hinh cc1101 cua hu sang tan so 433.050mhz -1200bps
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_goto_433_1200(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x10);
    cc1101_write_reg(CC1101_FREQ1, 0xA7);
    cc1101_write_reg(CC1101_FREQ0, 0xE1);
    cc1101_write_reg(CC1101_MDMCFG4, 0xF5); //1200bps    
}
//======================================================================//
//  Function  : si4464_goto_433_4800
//  Comments  : cau hinh cc1101 cua hu sang tan so 433.050mhz -4800bps
//  Input     : None
//  Output    : None
//======================================================================//
void si4464_goto_433_4800(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x10);
	cc1101_write_reg(CC1101_FREQ1, 0xA7);
	cc1101_write_reg(CC1101_FREQ0, 0xE1);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF7);    
}
//======================================================================//
//  Function  : cc1101_goto_408_4800
//  Comments  : cau hinh cc1101 cua hu sang tan so 421mhz - 4800bps
//  Input     : None
//  Output    : None
//======================================================================//
void cc1101_goto_421_4800(void) 
{
    cc1101_write_reg(CC1101_FREQ2, 0x10);
	cc1101_write_reg(CC1101_FREQ1, 0x31);
	cc1101_write_reg(CC1101_FREQ0, 0xB8);
	cc1101_write_reg(CC1101_MDMCFG4, 0xF7);
}
//======================================================================//
//  Function  : cc1101_config_power
//  Comments  : cau hinh cong suat cho cc1101
//  Input     : paTable: gia tri cac thanh ghi PA table
//  Output    : None
//======================================================================//
void cc1101_config_power(uint8_t paTable)
{
	uint8_t pa_table_array[8];
    int n;
	for( n = 0; n < 8; n++) pa_table_array[n] = paTable;
	cc1101_write_burst_reg(CC1101_PATABLE, pa_table_array, sizeof(pa_table_array));
}
//======================================================================//
//  Function  : cc1101_send_buffer
//  Comments  : phat buffer
//  Input     : txBuffer: bo dem can phat
//              size: kich thuoc bo dem can phat
//              write_length: TRUE - ghi size vao CC1101_TXFIFO; FALSE: ko ghi
//  Output    : 
//======================================================================//
void cc1101_send_buffer(uint8_t *txBuffer, uint8_t size, uint8_t write_length)
{
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(1);
    cc1101_write_reg(CC1101_IOCFG2, 0x06);
    #ifdef _STAR_SUPPORT 
    if(rf.rf_type != RF_STAR_NEW) 
    #endif
        cc1101_write_reg(CC1101_FIFOTHR,0);// phat toi da 60 byte
    cc1101_cmd_strobe(CC1101_SFRX); //xoa bo dem thu
    cc1101_cmd_strobe(CC1101_SFTX); //xoa bo dem phat
    delay_ms(1);
//    cc1101_read_burst_reg(0,rf.rx.buffer,39);
    if (write_length == TRUE) cc1101_write_reg(CC1101_TXFIFO, size);       
    //xem lai thu tu nay: de lenh set TX sau khi ghi thi HU moi nhan
    cc1101_write_burst_reg(CC1101_TXFIFO, txBuffer, size);
    //cc1101_set_stage(CC1101_TX_STATE);//cc1101_set_stage_tx(); // ra ngoai set stage RF
//    cc1101_gdo2_interrupt_high_to_low(); // da dua vao ham cc1101_set_stage
//    cc1101_gdo2_interrupt_enable();
}

//===========================================================================
//FUNCTION NAME:
//PROTOTYPE:
//INPUT:	rxBuffer: bo dem received
//			length: chieu dai bo dem received
//			read_length: TRUE - truyen thong tin chieu dai can read/ FALSE: doc tu buffer
//OUTPUT:	TRUE: CRC OK / FALSE: CRC wrong
//          rssi
//DESCRIPTION:
//NOTE:
//============================================================================
uint8_t cc1101_read_buffer(uint8_t *rxBuffer, uint8_t *length, uint8_t read_length, uint8_t *rssi)
{
	uint8_t data8_rf = 0;
	if (read_length == TRUE) *length = cc1101_read_reg(CC1101_RXFIFO);
	if (*length > CC1101_BUFFER_MAX) *length = CC1101_BUFFER_MAX;
	cc1101_read_burst_reg(CC1101_RXFIFO, (unsigned char *)rxBuffer, *length);
	*rssi = cc1101_read_reg(CC1101_RXFIFO);
    data8_rf = cc1101_read_reg(CC1101_RXFIFO);
	if (data8_rf & CRC_OK) return TRUE;
	else return FALSE;
}

#ifdef WOR_SUPPORT
//======================================================================//
//  Function  : cc1101_enter_wor_mode
//  Comments  : vao mode wake on radio
//  Input     : none
//  Output    : none
//======================================================================//
void cc1101_enter_wor_mode(void)
{
    uint8_t calib1, calib0;
    cc1101_gdo2_interrupt_disable();
    //display_anten(HIDE);
    cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(1);
    //tan so hoat dong khi mat dien la 400.000 MHz
    cc1101_write_reg(CC1101_FREQ2,0x0F);
    cc1101_write_reg(CC1101_FREQ1,0x62);
    cc1101_write_reg(CC1101_FREQ0,0x77);
    //baudrate
    cc1101_write_reg(CC1101_MDMCFG4,0xF9);//0xF8);
    cc1101_write_reg(CC1101_MDMCFG3,0x2E);//0x93);
    cc1101_write_reg(CC1101_MDMCFG2,0x01);

    //sync words
    cc1101_write_reg(CC1101_SYNC1,0xAA);
    cc1101_write_reg(CC1101_SYNC0,0xAA);
    // t event0 = 3s => EVENT0 = 0x0CB2
    cc1101_write_reg(CC1101_MCSM2 ,0x06);//0x00);

    cc1101_write_reg(CC1101_WOREVT1,0x87);//0x0C); t event0 = 1s
    cc1101_write_reg(CC1101_WOREVT0,0x6B);//0xB2);
    //WOR_RES = 1, EVENT1 = 3
    cc1101_write_reg(CC1101_WORCTRL,0x38);//0x39);

    delay_ms(1);
    //cc1101_write_reg(CC1101_WORCTRL,0x30);
    calib1 = cc1101_read_status_addr(CC1101_RCCTRL1_STATUS);
    calib0 = cc1101_read_status_addr(CC1101_RCCTRL0_STATUS);

    cc1101_write_reg(CC1101_RCCTRL1,calib1);
    cc1101_write_reg(CC1101_RCCTRL0,calib0);
#ifndef _IHD_ //doan nay can dieu chinh lai, khong su dung bie'n toan cuc cua san pham # vao day
    cc1101_write_reg(CC1101_ADDR, (unsigned char)read_meter.id); //tuantm
#else
    cc1101_write_reg(CC1101_ADDR, (unsigned char)ihd.meter_id); //tuantm
#endif
    cc1101_write_reg(CC1101_IOCFG2, 0x06); //0x06 tuantm
    //delay_ms(1);
    // vao mode WOR
    //cc1101_cmd_strobe(CC1101_SIDLE);
    delay_ms(1);
    cc1101_cmd_strobe(CC1101_SWORRST);
    cc1101_cmd_strobe(CC1101_SWOR);

#ifdef RFPA_SUPPORT
    //rf_pa_rx();
    cc1101_tx_pa_disable();
    cc1101_rx_pa_disable();
#endif

    rf.status = RF_RX;//cc1101.stage = CC1101_RX;//rf.status = CC1101_RX_STATE; //
    rf.flags.wor_mode = 1;//cc1101.wor.flags.mode = 1;
    rf.flags.wake_up = 0;//cc1101.wor.flags.wakeup = 0;//rf.wake_up_flag = 0;
    rf.flags.received = 0;
    rf.flags.transmiting = 0;
    rf.flags.processing = 0;
    rf.flags.error_tx = 0;

    //cc1101.flags.bits.tx = 0;//rf.tx.flags.transmiting = 0;
    cc1101.wor.flags.exit = 0;
    cc1101.wor.flags.disable = FALSE;

#warning "kiem tra lai lenh duoi"
    //rf.tx.flags.finish = 0; //loi WOR chi doc dc 1 lan, tuantm 13/05/2014
    cc1101.flags.bits.finish = FALSE;//rf.rx.flags.finish = 0;
    spi_sdi_low();//SDI_L;
    spi_sclk_low();//SCLK_L;
    cc1101_gdo2_interrupt_low_to_high(); //tuantm
    /*bo sung cho uart ko day*/
    //cc1101_select_io(); //chuyen chan gdo2 ve I/O binh thuong
    cc1101_gdo2_interrupt_enable();//enter wor mode
    //tuantm - ghi lai dia chi Address = Meter ID
}
//======================================================================//
//  Function  : cc1101_exit_wor_mode
//  Comments  : thoat wor, vao che do RX binh thuong
//  Input     : setting_buffer: cau hinh cc1101
//  Output    : none
//======================================================================//
void cc1101_exit_wor_mode(unit8_t *setting_buffer, uint8_t length_buffer)
{
    cc1101_cmd_strobe(CC1101_SIDLE);
    // cau hinh lai che do binh thuong
    rf.status = RF_RX ;//cc1101.stage = CC1101_RX;//rf.status = CC1101_RX_STATE;
    cc1101_reset();
    delay_us(500);//delay_us(100);
    cc1101_write_rf_setting(setting_buffer, length_buffer);
    cc1101_write_burst_reg(CC1101_PATABLE, paTable, sizeof(paTable));
    cc1101_cmd_strobe(CC1101_SFRX);
    #ifdef _DT01P_RF_
    rf.flags.freq_421 = 0;
    rf.flags.freq_408 = 1;
    #endif
    delay_us(100);    
#ifndef _IHD_ //doan nay can dieu chinh lai, khong su dung bie'n toan cuc cua san pham # vao day
    cc1101_write_reg(CC1101_ADDR, (unsigned char)read_meter.id); //tuantm
#else
    cc1101_write_reg(CC1101_ADDR, (unsigned char)ihd.meter_id); //tuantm
#endif
    cc1101_cmd_strobe(CC1101_SRX);
    cc1101.wor.flags.exit = 1;
    /*bo sung cho uart ko day*/
    //cc1101_select_uart();
    cc1101_gdo2_interrupt_enable();//exit wor mode
    //RF_Sts_ToRX();
}
#endif //WOR_SUPPORT
#endif //CC1101_MODULE_SUPPORT
#endif