/*===========================================================================*/
/*
SOURCE_FILE:		cc1101.h
APPLICATION:		DT03M-RF
MODULE:				rf
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
#ifndef _CC1101_H_
#define _CC1101_H_
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>

// Header prjEMETERS
#include "library.h"

#ifdef RF_SUPPORT
#ifdef CC1101_MODULE_SUPPORT

/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/

#define CC1101_BUFFER_MAX               64
#define CC1101_CONFIG_SETTING_MAX       47

#define WRITE_BURST   0x40
#define READ_SINGLE   0x80
#define READ_BURST    0xC0

#define CC1101_RX_STATE  0x00
#define CC1101_TX_STATE  0xFF



typedef struct S_RF_SETTINGS{
    uint8_t IOCFG2;    // GDO2 output pin configuration
    uint8_t IOCFG1;    // GDO1 output pin configuration
    uint8_t IOCFG0;    // GDO0 output pin configuration
    uint8_t FIFOTHR;	 // RX FIFO and TX FIFO thresholds
    uint8_t SYNC1;	 // Sync word, high byte
    uint8_t SYNC0;	 // Sync word, low byte
    uint8_t PKTLEN;    // Packet length.
    uint8_t PKTCTRL1;  // Packet automation control.
    uint8_t PKTCTRL0;  // Packet automation control.
    uint8_t ADDR;      // Device address.
    uint8_t CHANNR;    // Channel number.
    uint8_t FSCTRL1;   // Frequency synthesizer control.
    uint8_t FSCTRL0;   // Frequency synthesizer control.
    uint8_t FREQ2;     // Frequency control word, high uint8_t.
    uint8_t FREQ1;     // Frequency control word, middle uint8_t.
    uint8_t FREQ0;     // Frequency control word, low uint8_t.
    uint8_t MDMCFG4;   // Modem configuration.
    uint8_t MDMCFG3;   // Modem configuration.
    uint8_t MDMCFG2;   // Modem configuration.
    uint8_t MDMCFG1;   // Modem configuration.
    uint8_t MDMCFG0;   // Modem configuration.
    uint8_t DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    uint8_t MCSM2;     // Main Radio Control State Machine configuration.
    uint8_t MCSM1;     // Main Radio Control State Machine configuration.
    uint8_t MCSM0;     // Main Radio Control State Machine configuration.
    uint8_t FOCCFG;    // Frequency Offset Compensation Configuration.
    uint8_t BSCFG;     // Bit synchronization Configuration.
    uint8_t AGCCTRL2;  // AGC control.
    uint8_t AGCCTRL1;  // AGC control.
    uint8_t AGCCTRL0;  // AGC control.
    uint8_t WOREVT1;   // High byte event0 timeout
    uint8_t WOREVT0;	 // Low byte event0 timeout
    uint8_t WORCTRL;	 // Wake On Radio control
    uint8_t FREND1;    // Front end RX configuration.
    uint8_t FREND0;    // Front end RX configuration.
    uint8_t FSCAL3;    // Frequency synthesizer calibration.
    uint8_t FSCAL2;    // Frequency synthesizer calibration.
    uint8_t FSCAL1;    // Frequency synthesizer calibration.
    uint8_t FSCAL0;    // Frequency synthesizer calibration.

} RF_SETTINGS;

extern const unsigned char  rfSettings_cpcit[CC1101_CONFIG_SETTING_MAX];
extern const unsigned char  rfSettings_LTE66[39];
extern const unsigned char  wireless_uart_Settings[39];
extern const unsigned char rfSettings_DTS27[39];
extern const unsigned char rfSettings_GELEX[39];
extern const unsigned char rfSettings_DDS26D[39];
extern const uint8_t rfsetting_PSMART[47];
/*===========================================================================*/
/*  L O C A L   S Y M B O L   D E C L A R A T I O N S                        */
/*===========================================================================*/
//status byte
#define CC1101_IDLE       0x00 //IDLE state
#define CC1101_RX         0x10 //  RX  Receive mode
#define CC1101_TX         0x20 //  TX  Transmit mode
#define CC1101_FSTXON     0x30 //  Fast TX ready
#define CC1101_CALIBRATE  0x40 //  Frequency synthesizer calibration is running
#define CC1101_SETTLING   0x50 //  PLL is settling
#define CC1101_RXFIFO_OVERFLOW  0x60 //RX FIFO has overflowed
#define CC1101_TXFIFO_UNDERFLOW 0x70 //TX FIFO has underflowed

#define CC1101_IOCFG2       0x00
#define CC1101_IOCFG1       0x01
#define CC1101_IOCFG0       0x02
#define CC1101_FIFOTHR      0x03
#define CC1101_SYNC1        0x04
#define CC1101_SYNC0        0x05
#define CC1101_PKTLEN       0x06
#define CC1101_PKTCTRL1     0x07
#define CC1101_PKTCTRL0     0x08
#define CC1101_ADDR         0x09
#define CC1101_CHANNR       0x0A
#define CC1101_FSCTRL1      0x0B
#define CC1101_FSCTRL0      0x0C
#define CC1101_FREQ2        0x0D
#define CC1101_FREQ1        0x0E
#define CC1101_FREQ0        0x0F
#define CC1101_MDMCFG4      0x10
#define CC1101_MDMCFG3      0x11
#define CC1101_MDMCFG2      0x12
#define CC1101_MDMCFG1      0x13
#define CC1101_MDMCFG0      0x14
#define CC1101_DEVIATN      0x15
#define CC1101_MCSM2        0x16
#define CC1101_MCSM1        0x17
#define CC1101_MCSM0        0x18
#define CC1101_FOCCFG       0x19
#define CC1101_BSCFG        0x1A
#define CC1101_AGCCTRL2     0x1B
#define CC1101_AGCCTRL1     0x1C
#define CC1101_AGCCTRL0     0x1D
#define CC1101_WOREVT1      0x1E
#define CC1101_WOREVT0      0x1F
#define CC1101_WORCTRL      0x20
#define CC1101_FREND1       0x21
#define CC1101_FREND0       0x22
#define CC1101_FSCAL3       0x23
#define CC1101_FSCAL2       0x24
#define CC1101_FSCAL1       0x25
#define CC1101_FSCAL0       0x26
#define CC1101_RCCTRL1      0x27
#define CC1101_RCCTRL0      0x28
#define CC1101_FSTEST       0x29
#define CC1101_PTEST        0x2A
#define CC1101_AGCTEST      0x2B
#define CC1101_TEST2        0x2C
#define CC1101_TEST1        0x2D
#define CC1101_TEST0        0x2E
#define CC1101_RCCTRL1_STATUS     0x3C //thao
#define CC1101_RCCTRL0_STATUS     0x3D //thao

#define CC1101_SRES         0x30
#define CC1101_SFSTXON      0x31
#define CC1101_SXOFF        0x32
#define CC1101_SCAL         0x33
#define CC1101_SRX          0x34
#define CC1101_STX          0x35
#define CC1101_SIDLE        0x36
#define CC1101_SAFC         0x37
#define CC1101_SWOR         0x38
#define CC1101_SPWD         0x39
#define CC1101_SFRX         0x3A
#define CC1101_SFTX         0x3B
#define CC1101_SWORRST      0x3C
#define CC1101_SNOP         0x3D

#define CC1101_PARTNUM      0x30
#define CC1101_VERSION      0x31
#define CC1101_FREQEST      0x32
#define CC1101_LQI          0x33
#define CC1101_RSSI         0x34
#define CC1101_MARCSTATE    0x35
#define CC1101_WORTIME1     0x36
#define CC1101_WORTIME0     0x37
#define CC1101_PKTSTATUS    0x38
#define CC1101_VCO_VC_DAC   0x39
#define CC1101_TXBYTES      0x3A
#define CC1101_RXBYTES      0x3B
#define CC1101_PATABLE      0x3E
#define CC1101_TXFIFO       0x3F
#define CC1101_RXFIFO       0x3F

#define CRC_OK	           0x80
#define MARC_SLEEP        0
#define MARC_IDLE         1
#define MARC_XOFF         2
#define MARC_VCOON_MC     3
#define MARC_REGON_MC     4
#define MARC_MANCAL       5
#define MARC_VCOON        6
#define MARC_REGON        7
#define MARC_STARTCAL     8
#define MARC_BWBOOST      9
#define MARC_FS_LOCK      10
#define MARC_IFADCON      11
#define MARC_ENDCAL       12
#define MARC_RX           13
#define MARC_RX_END       14
#define MARC_RX_RST       15
#define MARC_TXRX_SWITCH  16
#define MARC_RX_OVERFLOW  17
#define MARC_FSTXON       18
#define MARC_TX           19
#define MARC_TX_END       20
#define MARC_RXTX_SWITCH  21
#define MARC_TX_UNDERFLOW 22

/*===========================================================================*/
/*  G L O B A L  F U N C T I O N S  P R O T O T Y P E S                      */
/*===========================================================================*/
extern void cc1101_init(void);
extern void cc1101_reset(void);
extern void cc1101_shutdown(void);
extern void cc1101_refresh(void);
extern void cc1101_write_rf_setting(uint8_t *setting_buffer, uint8_t length_buffer);
//extern void cc1101_set_stage_rx(void);
//extern void cc1101_set_stage_tx(void);
//extern void cc1101_set_stage_idle(void);
extern void cc1101_set_stage(unsigned char stage);
extern void cc1101_config_broadcast(void);
extern void cc1101_config_address(uint8_t addr, uint8_t addr_0_enable);
extern void cc1101_goto_408_4800(void);
extern void cc1101_goto_433_1200(void);
extern void si4464_goto_433_4800(void);
extern void cc1101_goto_421_4800(void);
extern void cc1101_goto_433_4800(void);
extern void cc1101_goto_frequecy(void);
extern void cc1101_config_power(uint8_t paTable);
extern void cc1101_flush_tx(void);
extern void cc1101_flush_rx(void);
extern void cc1101_send_buffer(uint8_t *txBuffer, uint8_t size, uint8_t write_length);
extern uint8_t cc1101_read_buffer(uint8_t *rxBuffer, uint8_t *length, uint8_t read_length, uint8_t *rssi);
/*===========================================================================*/
/*  L O C A L  F U N C T I O N S   P R O T O T Y P E S                       */
/*===========================================================================*/
void cc1101_cmd_strobe(uint8_t strobe);
unsigned char cc1101_read_byte(void);
void cc1101_write_byte(unsigned char value);
uint8_t cc1101_read_reg(uint8_t addr);
void cc1101_write_reg(uint8_t addr, uint8_t value);
void cc1101_read_burst_reg (uint8_t addr, uint8_t *buf, uint8_t cnt);
void cc1101_write_burst_reg(uint8_t addr, uint8_t *buf, uint8_t cnt);
extern void cc1101_write_rf_setting_PSMART(void);


/*===========================================================================*/
/*  H I S T O R Y                                                            */
/*===========================================================================*/
/*
	Rev 1.0: thaodtn - 24/10/2013
		-
		-
*/
#endif
#endif
#endif /* _CC1101_H_ */



