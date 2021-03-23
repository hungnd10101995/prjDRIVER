#ifndef SPI_H
#define SPI_H
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
#include "library.h"
#include "hardware.h"

// Header prjLIB
#include "macro.h"

#define spi_sdi_dir_output()        SPI_SDI_DIR setOut  SPI_SDI_BIT //_spi_sdi_dir_output()
#define spi_sdi_pullup_enable()     nop() //SPI_SDI_REN set     SPI_SDI_BIT //_spi_sdi_pullup_enable()
#define spi_sdi_pullup()            nop() //SPI_SDI_OUT set     SPI_SDI_BIT //_spi_sdi_pullup()
#define spi_sdi_low()               SPI_SDI_OUT clr     SPI_SDI_BIT //_spi_sdi_low()
#define spi_sdi_high()              SPI_SDI_OUT set     SPI_SDI_BIT //_spi_sdi_high()

#define spi_sdo_dir_input()         SPI_SDO_DIR setIn  SPI_SDO_BIT //_spi_sdo_dir_input()
#define spi_sdo_pullup_enable()     nop()//SPI_SDO_REN set    SPI_SDO_BIT//_spi_sdo_pullup_enable()
#define spi_sdo_pullup_disable()    nop()//SPI_SDO_REN clr    SPI_SDO_BIT//_spi_sdo_pullup_disable()
#define spi_sdo_pullup()            SPI_SDO_OUT set    SPI_SDO_BIT //_spi_sdo_pullup()
#define spi_sdo_pulldown()          SPI_SDO_OUT clr    SPI_SDO_BIT //_spi_sdo_pulldown()
#define spi_sdo_input()             (SPI_SDO_IN & SPI_SDO_BIT)//_spi_sdo_input()

#define spi_sclk_dir_output()       SPI_SCLK_DIR setOut     SPI_SCLK_BIT//_spi_sclk_dir_output()
#define spi_sclk_pullup_enable()    nop()//SPI_SCLK_REN set        SPI_SCLK_BIT//_spi_sclk_pullup_enable()
#define spi_sclk_pullup_disable()   nop()//SPI_SCLK_REN clr        SPI_SCLK_BIT//_spi_sclk_pullup_disable()
#define spi_sclk_pullup()           SPI_SCLK_OUT set        SPI_SCLK_BIT//_spi_sclk_pullup()
#define spi_sclk_low()              SPI_SCLK_OUT clr        SPI_SCLK_BIT//_spi_sclk_low()
#define spi_sclk_high()             SPI_SCLK_OUT set        SPI_SCLK_BIT//_spi_sclk_high()

#ifdef SST26VF064B_SUPPORT
#define spi_sdo_dir_output()        _spi_sdo_dir_output()
#define spi_sdo_low()               _spi_sdo_low()
#define spi_sdo_high()              _spi_sdo_high()

#define spi_sdi_dir_input()        _spi_sdi_dir_input()
#define spi_sdi_input()            _spi_sdi_input()
#endif

////////////////////////////////////////////////////////////
//status register of AA512vf040
#define SPI_BUSY     0x01   // 1 = Internal Write operation is in progress
#define WEL      0x02   // 1 = Device is memory Write enabled
#define BP0      0x04
#define BP1      0x08
#define AAI      0x40   // 1 = AAI programming mode
#define BPL      0x80   // 1 => BP1, BP0 are read-only bits, 0 => BP1, BP0 are read/writable

#define t_BP   20   //t_BP = 20us [datasheet p.17]
#define t_SCE   100000 // t_SCE = 100ms
#define t_BE   25000 // t_BE = t_SE = 25ms


//--------------------------------------------------------
extern void spi_init(void);
extern void spi_disable(void);
extern unsigned char spi_get_byte(void);
extern void spi_send_byte(unsigned char value);
#ifdef _ADE7953_
extern void spi_ade_init(void);
extern unsigned char spi_ade_get_byte(void);
extern void spi_ade_send_byte(unsigned char value);
#endif
#endif