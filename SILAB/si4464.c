#include <stdint.h>
#include <stdlib.h>
#include "../../prjEMETERS/devices.h"
#ifdef SI4464_MODULE_SUPPORT
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include "string.h"
#include "si4464-registers.h"
#include "si4464.h"
#include "rf.h"
#include "spi.h"
#include "delay.h"
#include "convert.h"
#include "crc.h"
#include "process.h"
//#include "..\uart\uart.h"
#if defined(__MSP430__)
#include <emeter-toolkit.h>
#endif
#ifdef RFMESH_SUPPORT
#include "rfmesh.h"
#include "meter_read.h"
#endif
/*===========================================================================*/
#if defined(_DT03M_RF_) && defined(__MSP430__)
#include "..\..\dt03m-rf\fw\emeter-main.h"
#endif
#if defined(_DT03M_RF_) && defined(__PIC24FJxxxGA410__)
#include "../../prjEMETERS/main.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-main.h"
#endif
#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter-main.h"
#endif

#ifdef _DT01P_RF_
#include "..\..\dt01p80-rf\fw\emeter-main.h"
#endif

#ifdef _DT01PP_RF_
#include "..\..\dt01pp\fw\emeter-main.h"
#endif
#ifdef _DCU_
#include "../../DCU/FW/main.h"
#include "../../DCU/FW/uart.h"							  
#endif
#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#endif
#ifdef _FCI_
#include "../../RemoteAlarm/FW/main.h"
#include "../../RemoteAlarm/FW/hardware.h"
#endif	  
#ifdef _IHD_
#include "..\..\ihd\fw\ihd-main.h"
#endif
#if !defined(_RF_SLOT_)&!defined(_DT01P40_RF_)
#include "process.h"
#endif
#ifdef _RF_SLOT_
#include "../../Elster-RF/FW/FWELSTERRFX/main.h"
#include "../../Elster-RF/FW/FWELSTERRFX/meter/meterpc.h"
#include "../../Elster-RF/FW/FWELSTERRFX/uart.h"
#endif
#ifdef _DT01P40_RF_
#include "../../DT01P-RF/FW/DT01P_RF_PIC16F1947/dt01prf.X/wdt.h"
#include "../../DT01P-RF/FW/DT01P_RF_PIC16F1947/dt01prf.X/uart.h"
#endif
#ifdef _RF_SLOT_
#include "../../Elster-RF/FW/FWELSTERRFX/wdt.h"
#endif
////////////////////////////////////////////////////////////////////////////
/*   debug cac chuc nang ko duoc define    */
////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_MODE
#ifndef TRANSFORMER_SUPPORT
#warning "undef TRANSFORMER_SUPPORT in cc1101.c"
#endif
#ifndef RTC_SUPPORT
#warning "undef RTC_SUPPORT in cc1101.c"
#endif
#ifndef IEC62056_21_SUPPORT
#warning "undef IEC62056_21_SUPPORT in cc1101.c"
#endif
#ifndef TARIFF_SUPPORT
#warning "undef TARIFF_SUPPORT in cc1101.c"
#endif
#ifndef DEFERRED_TARIFF_SUPPORT
#warning "undef DEFERRED_TARIFF_SUPPORT in cc1101.c"
#endif
#ifndef MAX_DEMAND
#warning "undef MAX_DEMAND in cc1101.c"
#endif
#ifndef EVENT
#warning "undef EVENT in cc1101.c"
#endif
#ifndef LOAD_PROFILE
#warning "undef LOAD_PROFILE in cc1101.c"
#endif
#ifndef BILLING
#warning "undef BILLING in cc1101.c"
#endif
#ifndef CUSTOMER_DEFINED_REGS
#warning "undef CUSTOMER_DEFINED_REGS in cc1101.c"
#endif
#ifndef ALERT
#warning "undef ALERT in cc1101.c"
#endif
#ifndef CONFIG_UART
#warning "undef CONFIG_UART in cc1101.c"
#endif
#ifndef RF
#warning "undef RF in cc1101.c"
#endif
#ifndef WOR
#warning "undef WOR in cc1101.c"
#endif
#ifndef LUU_XOAY_VONG_THANH_GHI
#warning "undef LUU_XOAY_VONG_THANH_GHI in cc1101.c"
#endif
#endif
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
unsigned char cfg_buffer[8];
//unsigned char POWER_VALUE = 0x14; //0x14 10dBm

const unsigned char INT_STATUS[4] = {0x20, 0x00, 0x00, 0x00};
/*===========================================================================*/
/* L O C A L   F U N C T I O N   P R O T O T Y P E S                         */

/*===========================================================================*/
unsigned char si4464_read_byte(void)
{
    unsigned char i;
    unsigned char value = 0;
    for (i = 0; i < 8;)
    {
        value <<= 1;
        if (rf_spi_sdo_input()) value |= 0x01;
        rf_spi_sclk_high();
#if (SYS_FREQ > 14745600L)
        delay_us(1);
#endif
        i++; nop();
        rf_spi_sclk_low();
#if (SYS_FREQ > 14745600L)
        delay_us(1);
#endif
    }
    return (value);
}

void si4464_write_byte(unsigned char value)
{
    unsigned char i;
    for (i = 0; i < 8;)
    {
        if (value & 0x80) rf_spi_sdi_high();
        else    rf_spi_sdi_low();
        rf_spi_sclk_high();
        value <<= 1; i++; nop();
//#if (SYS_FREQ > 14745600L)
//        delay_us(1);
//#endif
        rf_spi_sclk_low();
    }
    rf_spi_sdi_low();
}

unsigned char si4464_get_resp(unsigned char resp_length, unsigned char *resp_data)
{
    unsigned char result = FALSE;
    unsigned char CtsValue = 0;
    unsigned int ErrCnt = 0;
    while (CtsValue != 0xFF)
    {   // Wait until radio IC is ready with the data
        si4464_select(); // si4464_cs_output_low(); Vuonglq // select radio IC by pulling its nSEL pin low
        si4464_write_byte(CMD_READ_CMD_BUFF); // Read command cfg_buffer; send command byte
        si4464_send_data_get_resp(1, &CtsValue); // Read command cfg_buffer; get CTS value
        if (CtsValue != 0xFF)
        {
            si4464_de_select(); // si4464_cs_output_high(); vuonglq // de-select radio IC by putting its nSEL pin high
            delay_us(10);
        }
        if (++ErrCnt > MAX_CTS_RETRY)
        {
            si4464_de_select(); // si4464_cs_output_high(); vuonglq si4464_cs_output_high(); // de-select radio IC by putting its nSEL pin high
            result = FALSE;
            return result; // Error handling; if wrong CTS reads exceeds a limit
        }
    }
    si4464_send_data_get_resp(resp_length, resp_data); // CTS value ok, get the response data from the radio IC
    si4464_de_select(); // si4464_cs_output_high(); vuonglq  de-select radio IC by putting its nSEL pin high
    result = TRUE;
    return result; // Error handling; if wrong CTS reads exceeds a limit
}

void si4464_send_data_no_resp(unsigned char data_length, const unsigned char *data)
{
    unsigned char i, data_send;
    for (i = 0; i < data_length; i++)
    {
        data_send = data[i];
        si4464_write_byte(data_send);
    }
}

void si4464_send_data_get_resp(unsigned char data_length, unsigned char *data)
{
    unsigned char i;
    for (i = 0; i < data_length; i++)
    {
        data[i] = si4464_read_byte();
    }
}

unsigned char si4464_wait_cts(void)
{
    unsigned char result = FALSE;    
    unsigned char cts_value = 0;
    unsigned int err_cnt = 0;
    while (cts_value != 0xFF)
    {   // Wait until radio IC is ready with the data
        si4464_select(); // si4464_cs_output_low(); Vuonglq  select radio IC by pulling its nSEL pin low
        si4464_write_byte(CMD_READ_CMD_BUFF); // Read commandcfg_buffer; send command byte
        si4464_send_data_get_resp(1, &cts_value); // Read commandcfg_buffer; get CTS value
        if (++err_cnt > MAX_CTS_RETRY)
        {
            si4464_de_select(); // si4464_cs_output_high(); vuonglq // de-select radio IC by putting its nSEL pin high
            result = FALSE;
            return result;                   // Error handling; if wrong CTS reads exceeds a limit
        }
    }
    si4464_de_select(); // si4464_cs_output_high(); vuonglq  de-select radio IC by putting its nSEL pin high
    result = TRUE;
    return result;
}

void si4464_send_command(unsigned char cmd_length, const unsigned char *cmd_data)
{
    si4464_select(); // si4464_cs_output_low(); Vuonglq select radio IC by pulling its nSEL pin low
    si4464_send_data_no_resp(cmd_length, cmd_data);     //send data array to the radio IC via SPI
    si4464_de_select(); // si4464_cs_output_high(); vuonglq // de-select radio IC by putting its nSEL pin high
}

void si4464_set_config(uint8_t group, uint8_t length, uint8_t number, uint8_t *cmd_data)
{
    si4464_select(); // si4464_cs_output_low(); Vuonglq select radio IC by pulling its nSEL pin low                
    si4464_write_byte(CMD_SET_PROPERTY); //send data array to the radio IC via SPI
    si4464_write_byte(group);
    si4464_write_byte(length);
    si4464_write_byte(number);
    si4464_send_data_no_resp(length, cmd_data);
    si4464_de_select(); // si4464_cs_output_high(); vuonglq // de-select radio IC by putting its nSEL pin high
}
/*===========================================================================*/
/* G L O B A L   F U N C T I O N S   P R O T O T Y P E S                     */

/*===========================================================================*/
void si4464_init(void)
{
    //cau hinh I/O
#ifdef RFPA_SUPPORT
    //cau hinh tx, rx output o rf_init							
#endif
    //chan select
    si4464_cs_dir_output();
    si4464_cs_output_high();
    //chan reset
    si4464_sdn_dir_output();
    si4464_sdn_output_low();
    //chan ngat
    si4464_gdo2_dir_input();
    si4464_gdo2_pullup_enable();
    si4464_gdo2_interrupt_low_to_high();
    si4464_gdo2_interrupt_flag_clear();
    si4464_gdo2_interrupt_enable(); //cc1101_interrupt_enable();    
    //reset chip
    si4464_reset();
    si4464_write_setting_emec();
    //    si4464_write_setting_star();

}

void si4464_write_setting_emec(void)
{
#ifdef SI4463_C2A
     //==============Start Radio=======================//
    /*Command to power-up the device and select the operational mode and functionality*/
    si4464_send_command(sizeof (RF_POWER_UP), RF_POWER_UP); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    si4464_send_command(sizeof (INT_STATUS), INT_STATUS);
    si4464_get_resp(0x08, cfg_buffer); // Make sure that CTS is ready, then get the response
    si4464_send_command(sizeof (RF_GPIO_PIN_CFG), RF_GPIO_PIN_CFG); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Clock configuration options*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_XO_TUNE_2), PROP_GLOBAL_XO_TUNE, (uint8_t *) RF_GLOBAL_XO_TUNE_2); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Global configuration settings*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_CONFIG_1), PROP_GLOBAL_CONFIG, (uint8_t *) RF_GLOBAL_CONFIG_1); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Settings interrupt on the NIRQ output pin*/
    si4464_set_config(PROP_INT_GROUP, sizeof (RF_INT_CTL_ENABLE_2), PROP_INT_CTL_ENABLE, (uint8_t *) RF_INT_CTL_ENABLE_2); // Send command to the radio IC
    si4464_wait_cts();
    //    si4464_read_command(PROP_INT_GROUP,3,PROP_INT_CTL_ENABLE, cfg_buffer);  // Wait for CTS
    /*Fast Response Register A,B,C,D Configuration*/
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PREAMBLE_GROUP, sizeof (RF_PREAMBLE_TX_LENGTH_9), PROP_PREAMBLE_TX_LENGTH, (uint8_t *) RF_PREAMBLE_TX_LENGTH_9); //si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Sync word*/
    si4464_set_config(PROP_SYNC_GROUP, sizeof (RF_SYNC_CONFIG_5), PROP_SYNC_CONFIG, (uint8_t *) RF_SYNC_CONFIG_5); //si4464_set_config(0x09, RF_SYNC_CONFIG_5);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Packet CRC*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_CRC_CONFIG_7), PROP_PKT_CRC_CONFIG, (uint8_t *) RF_PKT_CRC_CONFIG_7); //si4464_set_config(0x0B, RF_PKT_CRC_CONFIG_7);
    si4464_wait_cts(); // Wait for CTS
    /*Packet Length*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_LEN_12), PROP_PKT_LEN, (uint8_t *) RF_PKT_LEN_12); //si4464_set_config(0x10, RF_PKT_LEN_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_FIELD_2_CRC_CONFIG_12), PROP_PKT_FIELD_2_CRC_CONFIG, (uint8_t *) RF_PKT_FIELD_2_CRC_CONFIG_12); //si4464_set_config(0x10, RF_PKT_FIELD_2_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_FIELD_5_CRC_CONFIG_12), PROP_PKT_FIELD_5_CRC_CONFIG, (uint8_t *) RF_PKT_FIELD_5_CRC_CONFIG_12); //si4464_set_config(0x10, RF_PKT_FIELD_5_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS
    // Mo ra de cau hinh co dinh theo CC1101 - end

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_RX_FIELD_3_CRC_CONFIG_9), PROP_PKT_RX_FIELD_3_CRC_CONFIG, (uint8_t *) RF_PKT_RX_FIELD_3_CRC_CONFIG_9); //si4464_set_config(0x10, RF_PKT_RX_FIELD_3_CRC_CONFIG_9);
    si4464_wait_cts(); // Wait for CTS

    //    si4464_goto_433_1200();
#if defined(FREQ_408MHz)
    si4464_goto_408_4800();
#elif defined(FREQ_433MHz)
     si4464_goto_433_4800();
#elif defined(FREQ_421MHz)
    si4464_goto_421_4800();//
#elif defined(FREQ_433MHz_19200)
    si4464_goto_433_19200();
#endif
    si4464_set_config(PROP_PA_GROUP, sizeof (rf_pa_mode_4), PROP_PA_MODE, (uint8_t *) rf_pa_mode_4);
    //si4464_set_config(PROP_PA_GROUP, sizeof (RF_PA_MODE_4), PROP_PA_MODE, (uint8_t *) RF_PA_MODE_4); //si4464_set_config(8,cfg_buffer);         // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_SYNTH_GROUP, sizeof (RF_SYNTH_PFDCP_CPFF_7), PROP_SYNTH_PFDCP_CPFF, (uint8_t *) RF_SYNTH_PFDCP_CPFF_7); //si4464_set_config(0x0B, RF_SYNTH_PFDCP_CPFF_7);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MATCH_GROUP, sizeof (RF_MATCH_VALUE_1_12), PROP_MATCH_VALUE_1, (uint8_t *) RF_MATCH_VALUE_1_12); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS
#else // notSI4463_C2A
     //==============Start Radio=======================//
    /*Command to power-up the device and select the operational mode and functionality*/
    si4464_send_command(sizeof (RF_POWER_UP), RF_POWER_UP); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /* Read ITs, clear pending ones */
    si4464_send_command(sizeof (INT_STATUS), INT_STATUS);
    si4464_get_resp(0x08, cfg_buffer); // Make sure that CTS is ready, then get the response
    /*Configures the GPIO pins for Antena deversity*/
    si4464_send_command(sizeof (RF_GPIO_PIN_CFG), RF_GPIO_PIN_CFG); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Clock configuration options*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_XO_TUNE_2), PROP_GLOBAL_XO_TUNE, (uint8_t *) RF_GLOBAL_XO_TUNE_2); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Global configuration settings*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_CONFIG_1), PROP_GLOBAL_CONFIG, (uint8_t *) RF_GLOBAL_CONFIG_1); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Settings interrupt on the NIRQ output pin*/
    si4464_set_config(PROP_INT_GROUP, sizeof (RF_INT_CTL_ENABLE_2), PROP_INT_CTL_ENABLE, (uint8_t *) RF_INT_CTL_ENABLE_2); // Send command to the radio IC
    si4464_wait_cts();
    //    si4464_read_command(PROP_INT_GROUP,3,PROP_INT_CTL_ENABLE, cfg_buffer);  // Wait for CTS
    /*Fast Response Register A,B,C,D Configuration*/
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    // Mo ra de cau hinh co dinh theo CC1101
    /*Settings preamble TX Lengh*/
    si4464_set_config(PROP_PREAMBLE_GROUP, sizeof (RF_PREAMBLE_TX_LENGTH_9), PROP_PREAMBLE_TX_LENGTH, (uint8_t *) RF_PREAMBLE_TX_LENGTH_9); //si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Sync word*/
    si4464_set_config(PROP_SYNC_GROUP, sizeof (RF_SYNC_CONFIG_5), PROP_SYNC_CONFIG, (uint8_t *) RF_SYNC_CONFIG_5); //si4464_set_config(0x09, RF_SYNC_CONFIG_5);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Packet CRC*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_CRC_CONFIG_7), PROP_PKT_CRC_CONFIG, (uint8_t *) RF_PKT_CRC_CONFIG_7); //si4464_set_config(0x0B, RF_PKT_CRC_CONFIG_7);
    si4464_wait_cts(); // Wait for CTS
    /*Packet Length*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_LEN_12), PROP_PKT_LEN, (uint8_t *) RF_PKT_LEN_12); //si4464_set_config(0x10, RF_PKT_LEN_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_FIELD_2_CRC_CONFIG_12), PROP_PKT_FIELD_2_CRC_CONFIG, (uint8_t *) RF_PKT_FIELD_2_CRC_CONFIG_12); //si4464_set_config(0x10, RF_PKT_FIELD_2_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_FIELD_5_CRC_CONFIG_12), PROP_PKT_FIELD_5_CRC_CONFIG, (uint8_t *) RF_PKT_FIELD_5_CRC_CONFIG_12); //si4464_set_config(0x10, RF_PKT_FIELD_5_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS
    // Mo ra de cau hinh co dinh theo CC1101 - end

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_PKT_RX_FIELD_3_CRC_CONFIG_9), PROP_PKT_RX_FIELD_3_CRC_CONFIG, (uint8_t *) RF_PKT_RX_FIELD_3_CRC_CONFIG_9); //si4464_set_config(0x10, RF_PKT_RX_FIELD_3_CRC_CONFIG_9);
    si4464_wait_cts(); // Wait for CTS

    //    si4464_goto_433_1200();
#if defined(FREQ_408MHz)
    si4464_goto_408_4800();
#elif defined(FREQ_433MHz)
    si4464_goto_433_4800();
#elif defined(FREQ_421MHz)
    si4464_goto_421_4800();
#endif
    /*Selects the type of modulation. In TX mode, additionally selects the source of the modulation*/

    /*Setting Devitation frequency*/
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_FREQ_DEV_0_1), PROP_MODEM_FREQ_DEV_0, (uint8_t *) RF_MODEM_FREQ_DEV_0_1); //si4464_set_config(0x05, RF_MODEM_FREQ_DEV_0_1);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AGC_CONTROL_1), PROP_MODEM_AGC_CONTROL, (uint8_t *) RF_MODEM_AGC_CONTROL_1); //si4464_set_config(0x05, RF_MODEM_AGC_CONTROL_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_RSSI_CONTROL_1), PROP_MODEM_RSSI_CONTROL, (uint8_t *) RF_MODEM_RSSI_CONTROL_1); //si4464_set_config(0x05, RF_MODEM_RSSI_CONTROL_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_RSSI_COMP_1), PROP_MODEM_RSSI_COMP, (uint8_t *) RF_MODEM_RSSI_COMP_1); //si4464_set_config(0x05, RF_MODEM_RSSI_COMP_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_CLKGEN_BAND_1), PROP_MODEM_CLKGEN_BAND, (uint8_t *) RF_MODEM_CLKGEN_BAND_1); //si4464_set_config(0x05, RF_MODEM_CLKGEN_BAND_1);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PA_GROUP, sizeof (RF_POWER_VALUE), PROP_PA_MODE, (uint8_t *) RF_POWER_VALUE); //si4464_set_config(8,cfg_buffer);         // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_SYNTH_GROUP, sizeof (RF_SYNTH_PFDCP_CPFF_7), PROP_SYNTH_PFDCP_CPFF, (uint8_t *) RF_SYNTH_PFDCP_CPFF_7); //si4464_set_config(0x0B, RF_SYNTH_PFDCP_CPFF_7);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MATCH_GROUP, sizeof (RF_MATCH_VALUE_1_12), PROP_MATCH_VALUE_1, (uint8_t *) RF_MATCH_VALUE_1_12); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_RX_HOP_GROUP, sizeof (RF_RX_HOP_CONTROL), PROP_RX_HOP_CONTROL, (uint8_t *) RF_RX_HOP_CONTROL); //si4464_set_config(0x05, RF_MODEM_FREQ_DEV_0_1);
    si4464_wait_cts(); // Wait for CTS
    //03.01 si4464_set_stage(SI4464_RX);
#endif // End SI4463-C2A  
}
#ifdef _STAR_SUPPORT
void si4464_write_setting_star(void)
{
#ifdef SI4463_C2A
    // Vudh Chua co code SI4463
#else // notSI4463_C2A
     //==============Start Radio=======================//
    /*Command to power-up the device and select the operational mode and functionality*/
    si4464_send_command(sizeof (RF_POWER_UP), RF_POWER_UP); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /* Read ITs, clear pending ones */
    si4464_send_command(sizeof (INT_STATUS), INT_STATUS);
    si4464_get_resp(0x08, cfg_buffer); // Make sure that CTS is ready, then get the response
    /*Configures the GPIO pins for Antena deversity*/
    si4464_send_command(sizeof (RF_GPIO_PIN_CFG), RF_GPIO_PIN_CFG); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Clock configuration options*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_XO_TUNE_2), PROP_GLOBAL_XO_TUNE, (uint8_t *) RF_GLOBAL_XO_TUNE_2); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Global configuration settings*/
    si4464_set_config(PROP_GLOBAL_GROUP, sizeof (RF_GLOBAL_CONFIG_1), PROP_GLOBAL_CONFIG, (uint8_t *) RF_GLOBAL_CONFIG_1); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    /*Settings interrupt on the NIRQ output pin*/
    si4464_set_config(PROP_INT_GROUP, sizeof (RF_STAR_INT_CTL_ENABLE_2), PROP_INT_CTL_ENABLE, (uint8_t *) RF_STAR_INT_CTL_ENABLE_2); // Send command to the radio IC
    si4464_wait_cts();
    //    si4464_read_command(PROP_INT_GROUP,3,PROP_INT_CTL_ENABLE, cfg_buffer);  // Wait for CTS
    /*Fast Response Register A,B,C,D Configuration*/
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    // Mo ra de cau hinh co dinh theo CC1101
    /*Settings preamble TX Lengh*/
    si4464_set_config(PROP_PREAMBLE_GROUP, sizeof (RF_STAR_PREAMBLE_TX_LENGTH_9), PROP_PREAMBLE_TX_LENGTH, (uint8_t *) RF_STAR_PREAMBLE_TX_LENGTH_9); //si4464_set_config(0x0D, RF_STAR_PREAMBLE_TX_LENGTH_9);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Sync word*/
    si4464_set_config(PROP_SYNC_GROUP, sizeof (RF_STAR_SYNC_CONFIG_5), PROP_SYNC_CONFIG, (uint8_t *) RF_STAR_SYNC_CONFIG_5); //si4464_set_config(0x09, RF_STAR_SYNC_CONFIG_5);
    si4464_wait_cts(); // Wait for CTS
    /*Configures Packet CRC*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_STAR_PKT_CRC_CONFIG_7), PROP_PKT_CRC_CONFIG, (uint8_t *) RF_STAR_PKT_CRC_CONFIG_7); //si4464_set_config(0x0B, RF_STAR_PKT_CRC_CONFIG_7);
    si4464_wait_cts(); // Wait for CTS
    /*Packet Length*/
    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_STAR_PKT_LEN_12), PROP_PKT_LEN, (uint8_t *) RF_STAR_PKT_LEN_12); //si4464_set_config(0x10, RF_STAR_PKT_LEN_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_STAR_PKT_FIELD_2_CRC_CONFIG_12), PROP_PKT_FIELD_2_CRC_CONFIG, (uint8_t *) RF_STAR_PKT_FIELD_2_CRC_CONFIG_12); //si4464_set_config(0x10, RF_STAR_PKT_FIELD_2_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_STAR_PKT_FIELD_5_CRC_CONFIG_12), PROP_PKT_FIELD_5_CRC_CONFIG, (uint8_t *) RF_STAR_PKT_FIELD_5_CRC_CONFIG_12); //si4464_set_config(0x10, RF_STAR_PKT_FIELD_5_CRC_CONFIG_12);
    si4464_wait_cts(); // Wait for CTS
    // Mo ra de cau hinh co dinh theo CC1101 - end

    si4464_set_config(PROP_PKT_GROUP, sizeof (RF_STAR_PKT_RX_FIELD_3_CRC_CONFIG_9), PROP_PKT_RX_FIELD_3_CRC_CONFIG, (uint8_t *) RF_STAR_PKT_RX_FIELD_3_CRC_CONFIG_9); //si4464_set_config(0x10, RF_STAR_PKT_RX_FIELD_3_CRC_CONFIG_9);
    si4464_wait_cts(); // Wait for CTS

    //    si4464_set_config(PROP_GLOBAL_GROUP,sizeof(RF_GLOBAL_XO_TUNE_2),PROP_GLOBAL_XO_TUNE, (uint8_t *)RF_GLOBAL_XO_TUNE_2);  // Send command to the radio IC
    //    si4464_wait_cts();  // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_STAR_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_STAR_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_STAR_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_STAR_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_STAR_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_STAR_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_STAR_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_STAR_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_STAR_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_STAR_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_STAR_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_STAR_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_STAR_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_STAR_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_STAR_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_STAR_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_STAR_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_STAR_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    /*Selects the type of modulation. In TX mode, additionally selects the source of the modulation*/

    /*Setting Devitation frequency*/
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_FREQ_DEV_0_1), PROP_MODEM_FREQ_DEV_0, (uint8_t *) RF_STAR_MODEM_FREQ_DEV_0_1); //si4464_set_config(0x05, RF_STAR_MODEM_FREQ_DEV_0_1);
    si4464_wait_cts(); // Wait for CTS
    /*Settings TX ramp delay*/

    //    /*Setting for RX*/


    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_AGC_CONTROL_1), PROP_MODEM_AGC_CONTROL, (uint8_t *) RF_STAR_MODEM_AGC_CONTROL_1); //si4464_set_config(0x05, RF_STAR_MODEM_AGC_CONTROL_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_RSSI_CONTROL_1), PROP_MODEM_RSSI_CONTROL, (uint8_t *) RF_STAR_MODEM_RSSI_CONTROL_1); //si4464_set_config(0x05, RF_STAR_MODEM_RSSI_CONTROL_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_RSSI_COMP_1), PROP_MODEM_RSSI_COMP, (uint8_t *) RF_STAR_MODEM_RSSI_COMP_1); //si4464_set_config(0x05, RF_STAR_MODEM_RSSI_COMP_1);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_STAR_MODEM_CLKGEN_BAND_1), PROP_MODEM_CLKGEN_BAND, (uint8_t *) RF_STAR_MODEM_CLKGEN_BAND_1); //si4464_set_config(0x05, RF_STAR_MODEM_CLKGEN_BAND_1);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_PA_GROUP, sizeof (RF_POWER_VALUE), PROP_PA_MODE, (uint8_t *) RF_POWER_VALUE); //si4464_set_config(8,cfg_buffer);         // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_SYNTH_GROUP, sizeof (RF_STAR_SYNTH_PFDCP_CPFF_7), PROP_SYNTH_PFDCP_CPFF, (uint8_t *) RF_STAR_SYNTH_PFDCP_CPFF_7); //si4464_set_config(0x0B, RF_STAR_SYNTH_PFDCP_CPFF_7);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MATCH_GROUP, sizeof (RF_STAR_MATCH_VALUE_1_12), PROP_MATCH_VALUE_1, (uint8_t *) RF_STAR_MATCH_VALUE_1_12); //si4464_set_config(0x10, RF_STAR_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_RX_HOP_GROUP, sizeof (RF_RX_HOP_CONTROL), PROP_RX_HOP_CONTROL, (uint8_t *) RF_RX_HOP_CONTROL); //si4464_set_config(0x05, RF_STAR_MODEM_FREQ_DEV_0_1);
    si4464_wait_cts(); // Wait for CTS
    //03.01 si4464_set_stage(SI4464_RX);
    #endif // End SI4463-C2A
}
   
#endif
void si4464_reset(void)
{
    //Reset the radio
    si4464_sdn_output_high();
    // Wait ~300us (SDN pulse width)
    delay_ms(5);
    // Wake up the chip from SDN
    si4464_sdn_output_low();
    // Wait for POR (power on reset); ~5ms
    delay_ms(10);
}

void si4464_shutdown(void)
{
    //Reset the radio
    si4464_sdn_output_high();
    // Wait ~300us (SDN pulse width)
    delay_ms(5);
    // Wake up the chip from SDN
//    si4464_sdn_output_low();
//    // Wait for POR (power on reset); ~5ms
//    delay_ms(10);
}

void si4464_reset_fifo(uint8_t stage)
{
//    //=====================Put modem to Ready Mode==================//
//    cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
//    cfg_buffer[1] = SI446x_STATE_READY; // Go to Ready mode
//    si4464_send_command(2, cfg_buffer); // Send command to the radio IC
//    si4464_wait_cts(); // Wait for CTS
    //===============Reset TX FIFO========================//
    cfg_buffer[0] = CMD_FIFO_INFO;
    cfg_buffer[1] = SUBCMD_FIFO_INFO_TX_RX;
    si4464_send_command(2, cfg_buffer); // Send command to the radio IC
    si4464_get_resp(2, cfg_buffer);
}

void si4464_set_stage(uint8_t stage)
{
#ifdef SI4463_C2A
    switch (stage)
    {
        case SI4464_TX:
            cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
            cfg_buffer[1] = SI446x_STATE_READY; // Go to TX Tune state
            si4464_send_command(2, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS    
            
            //===============Read ITs, clear pending ones===============//
            si4464_send_command(sizeof (INT_STATUS), INT_STATUS); // Send command to the radio IC
            si4464_get_resp(8, cfg_buffer); // Make sure that CTS is ready, then get the
            
            //===============Reset TX FIFO========================//
            cfg_buffer[0] = CMD_FIFO_INFO;
            cfg_buffer[1] = SUBCMD_FIFO_INFO_TX_RX;
            si4464_send_command(2, cfg_buffer);
            si4464_get_resp(2, cfg_buffer);
 
        break;
        
        case SI4464_IDLE:
            //=====================Put modem to Ready Mode==================//
            cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
            cfg_buffer[1] = SI446x_STATE_READY; // Go to Ready mode
            si4464_send_command(2, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS
        break;
        
        case SI4464_RX:
        default:
            cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
            cfg_buffer[1] = SI446x_STATE_READY; // Go to TX Tune state
            si4464_send_command(2, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS    

            si4464_send_command(4, INT_STATUS); // Send command to the radio IC
            si4464_get_resp(8, cfg_buffer); // Make sure that CTS is ready, then get the respons

            cfg_buffer[0] = CMD_FIFO_INFO;
            cfg_buffer[1] = SUBCMD_FIFO_INFO_TX_RX;
            si4464_send_command(2, cfg_buffer);
            si4464_get_resp(2, cfg_buffer);

            //=====================Put modem to RX Mode==================//
            cfg_buffer[0] = CMD_START_RX; //Use Start RX command
#ifdef RF_409_125MHZ
            cfg_buffer[1] = 0x01; //Set chanel number //
#elif RF_409_325MHZ
            cfg_buffer[1] = 0x02; //Set chanel number //
#else
            cfg_buffer[1] = 0x00; //Set chanel number //
#endif
            cfg_buffer[2] = 0x00; //Start RX immediately
            cfg_buffer[3] = 0x00; //packet fields used, do not enter packet length here
            cfg_buffer[4] = 0x00;
            cfg_buffer[5] = 0x03; //RX state if RX timeout
            cfg_buffer[6] = 0x03; //No change state after RX
            cfg_buffer[7] = 0x03; //Ready state if RX invalid
            si4464_send_command(8, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS
            si4464_gdo2_interrupt_high_to_low(); //thaodtn ext_interrupt_enable();
            si4464_gdo2_interrupt_flag_clear();
            si4464_gdo2_interrupt_enable();
        break;        
    }
#else // notSI4463_C2A
    switch (stage)
    {
        case SI4464_TX:
        //===============Read ITs, clear pending ones===============//
        si4464_send_command(sizeof (INT_STATUS), INT_STATUS); // Send command to the radio IC
        si4464_get_resp(8, cfg_buffer); // Make sure that CTS is ready, then get the
        
        cfg_buffer[0] = CMD_REQUEST_DEVICE_STATE;
        cfg_buffer[1] = 0x00;
        si4464_send_command(2, cfg_buffer); // Send command to the radio IC
        si4464_get_resp(2, cfg_buffer);
        
        if (cfg_buffer[0] != SI446x_STATE_TX)
        {
            cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
            cfg_buffer[1] = SI446x_STATE_READY; // Go to TX Tune state
            si4464_send_command(2, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS           
            
            //===============Reset TX FIFO========================//
            cfg_buffer[0] = CMD_FIFO_INFO;
            cfg_buffer[1] = SUBCMD_FIFO_INFO_TX_RX;
            si4464_send_command(2, cfg_buffer);
            si4464_get_resp(2, cfg_buffer);
            
            cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
            cfg_buffer[1] = SI446x_STATE_TX_TUNE; // Go to TX Tune state
            si4464_send_command(2, cfg_buffer); // Send command to the radio IC
            si4464_wait_cts(); // Wait for CTS    
            
        }
        break;
        
        case SI4464_IDLE:
        //=====================Put modem to Ready Mode==================//
        cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
        cfg_buffer[1] = SI446x_STATE_SLEEP; // Go to Ready mode
        si4464_send_command(2, cfg_buffer); // Send command to the radio IC
        si4464_wait_cts(); // Wait for CTS
        break;
        
        case SI4464_RX:
        default:
        si4464_send_command(4, INT_STATUS); // Send command to the radio IC
        si4464_get_resp(8, cfg_buffer); // Make sure that CTS is ready, then get the respons
        cfg_buffer[0] = CMD_CHANGE_STATE; // Change state command
        cfg_buffer[1] = SI446x_STATE_RX_TUNE; // Go to Ready mode
        si4464_send_command(2, cfg_buffer); // Send command to the radio IC
        si4464_wait_cts(); // Wait for CTS
        
        cfg_buffer[0] = CMD_FIFO_INFO;
        cfg_buffer[1] = SUBCMD_FIFO_INFO_TX_RX;
        si4464_send_command(2, cfg_buffer);
        si4464_get_resp(2, cfg_buffer);
        //=====================Put modem to RX Mode==================//
        cfg_buffer[0] = CMD_START_RX; //Use Start RX command
#ifdef RF_409_125MHZ
            cfg_buffer[1] = 0x01; //Set chanel number //
#elif RF_409_325MHZ
            cfg_buffer[1] = 0x02; //Set chanel number //
#else
            cfg_buffer[1] = 0x00; //Set chanel number //
#endif
        cfg_buffer[2] = 0x00; //Start RX immediately
        cfg_buffer[3] = 0x00; //packet fields used, do not enter packet length here
        cfg_buffer[4] = 0x00;
        cfg_buffer[5] = 0x03; //RX state if RX timeout
        cfg_buffer[6] = 0x03; //No change state after RX
        cfg_buffer[7] = 0x03; //Ready state if RX invalid
        si4464_send_command(8, cfg_buffer); // Send command to the radio IC
        si4464_wait_cts(); // Wait for CTS
        si4464_gdo2_interrupt_high_to_low(); //thaodtn ext_interrupt_enable();
        si4464_gdo2_interrupt_flag_clear();
        si4464_gdo2_interrupt_enable();
        break;        
    }
#endif // End SI4463-C2A    
    
}

void si4464_interrupt_enable(void)
{
    si4464_gdo2_dir_input();
    si4464_gdo2_pullup_disable();
    si4464_gdo2_pullup_enable();
    si4464_gdo2_pullup();
    si4464_gdo2_interrupt_high_to_low();
    si4464_gdo2_interrupt_enable();
    si4464_gdo2_interrupt_flag_clear();
}

void si4464_interrupt_disable(void)
{
    si4464_gdo2_interrupt_flag_clear();
    si4464_gdo2_interrupt_disable();
}

void si4464_goto_408_4800(void)
{
#ifdef SI4463_C2A
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_FREQ_DEV_0_1), PROP_MODEM_FREQ_DEV_0, (uint8_t *) RF_MODEM_FREQ_DEV_0_1); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_TX_RAMP_DELAY_12), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_MODEM_TX_RAMP_DELAY_12); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_BCR_NCO_OFFSET_2_12), PROP_MODEM_BCR_NCO_OFFSET, (uint8_t *) RF_MODEM_BCR_NCO_OFFSET_2_12); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AFC_LIMITER_1_3), PROP_MODEM_AFC_LIMITER_1, (uint8_t *) RF_MODEM_AFC_LIMITER_1_3); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AGC_CONTROL_1), PROP_MODEM_AGC_CONTROL, (uint8_t *) RF_MODEM_AGC_CONTROL_1); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AGC_WINDOW_SIZE_12), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_MODEM_AGC_WINDOW_SIZE_12); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_RAW_CONTROL_10), PROP_MODEM_RAW_CONTROL, (uint8_t *) RF_MODEM_RAW_CONTROL_10); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_RAW_SEARCH2_2), PROP_MODEM_RAW_SEARCH2, (uint8_t *) RF_MODEM_RAW_SEARCH2_2); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_SPIKE_DET_2), PROP_MODEM_SPIKE_DET, (uint8_t *) RF_MODEM_SPIKE_DET_2); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
            
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_RSSI_MUTE_1), PROP_MODEM_RSSI_MUTE, (uint8_t *) RF_MODEM_RSSI_MUTE_1); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_DSA_CTRL1_5), PROP_MODEM_DSA_CTRL1, (uint8_t *) RF_MODEM_DSA_CTRL1_5); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
   
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS
#else 
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS
#endif
}

float Si4464_read_temp_vol(void)
{
    volatile float temp;
    cfg_buffer[0] = CMD_GET_ADC_READING;
    cfg_buffer[1] = 0x18; //doc temp va vol
    cfg_buffer[2] = 0x99; // Voltage 0V-3.6V
    si4464_send_command(3, cfg_buffer);
    si4464_get_resp(8, cfg_buffer);
    temp = (float) ((cfg_buffer[4] & 0x07)*256.0) + cfg_buffer[5];
    temp = (899.0 / 4096) * temp - 293;
    return (100 * temp);
}
//

void si4464_goto_433_1200(void)
{
#ifdef SI4463_C2A

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_1200_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_1200_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_1200_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_1200_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_1200_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_1200_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_1200_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_1200_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#else // notSI4463_C2A
        si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    //    si4464_set_config(PROP_GLOBAL_GROUP,sizeof(RF_433_1200_GLOBAL_XO_TUNE_2),PROP_GLOBAL_XO_TUNE, (uint8_t *)RF_GLOBAL_XO_TUNE_2);  // Send command to the radio IC
    //    si4464_wait_cts();  // Wait for CTS
    //    si4464_set_config(PROP_PREAMBLE_GROUP,sizeof(RF_433_1200_PREAMBLE_TX_LENGTH_9),PROP_PREAMBLE_TX_LENGTH, (uint8_t *)RF_433_1200_PREAMBLE_TX_LENGTH_9);//si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    //    si4464_wait_cts();  // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_1200_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_1200_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_1200_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_1200_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_1200_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_1200_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_1200_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_1200_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_1200_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif // End SI4463-C2A
}

void si4464_goto_433_4800(void)
{
#ifdef SI4463_C2A
    #ifdef FREQ_433MHz
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_MODEM_433_4800_MOD_TYPE_12); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_FREQ_DEV_0_1), PROP_MODEM_FREQ_DEV_0, (uint8_t *) RF_MODEM_433_4800_FREQ_DEV_0_1); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_TX_RAMP_DELAY_12), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_MODEM_433_4800_TX_RAMP_DELAY_12); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_BCR_NCO_OFFSET_2_12), PROP_MODEM_BCR_NCO_OFFSET, (uint8_t *) RF_MODEM_433_4800_BCR_NCO_OFFSET_2_12); 
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_AFC_LIMITER_1_3), PROP_MODEM_AFC_LIMITER_1, (uint8_t *) RF_MODEM_433_4800_AFC_LIMITER_1_3); 
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_AGC_CONTROL_1), PROP_MODEM_AGC_CONTROL, (uint8_t *) RF_MODEM_433_4800_AGC_CONTROL_1); 
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_AGC_WINDOW_SIZE_12), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_MODEM_433_4800_AGC_WINDOW_SIZE_12); 
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_RAW_CONTROL_10), PROP_MODEM_RAW_CONTROL, (uint8_t *) RF_MODEM_433_4800_RAW_CONTROL_10); 
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_RAW_SEARCH2_2), PROP_MODEM_RAW_SEARCH2, (uint8_t *) RF_MODEM_433_4800_RAW_SEARCH2_2); 
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_SPIKE_DET_2), PROP_MODEM_SPIKE_DET, (uint8_t *) RF_MODEM_433_4800_SPIKE_DET_2); 
    si4464_wait_cts(); // Wait for CTS
            
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_RSSI_MUTE_1), PROP_MODEM_RSSI_MUTE, (uint8_t *) RF_MODEM_433_4800_RSSI_MUTE_1); 
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_4800_DSA_CTRL1_5), PROP_MODEM_DSA_CTRL1, (uint8_t *) RF_MODEM_433_4800_DSA_CTRL1_5); 
    si4464_wait_cts(); // Wait for CTS
   
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE13_7_0_12); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE1_7_0_12); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_4800_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_MODEM_433_4800_CHFLT_RX2_CHFLT_COE7_7_0_12); 
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_FREQ_433_4800_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_FREQ_433_4800_CONTROL_INTE_8); 
    si4464_wait_cts(); // Wait for CTS
#endif // End FREQ_433MHz	
#else // notSI4463_C2A
#ifdef FREQ_433MHz	
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_4800_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_4800_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_4800_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_4800_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_4800_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_4800_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_4800_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_4800_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_4800_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_4800_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_4800_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif // End FREQ_433MHz
#endif // End SI4463-C2A
}

void si4464_goto_433_19200(void)
{
    
#ifdef SI4463_C2A
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_MODEM_433_19200_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_433_19200_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_FREQ_DEV_0_1), PROP_MODEM_FREQ_DEV_0, (uint8_t *) RF_MODEM_433_19200_FREQ_DEV_0_1); //si4464_set_config(0x10, RF_MODEM_433_19200_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_TX_RAMP_DELAY_12), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_MODEM_433_19200_TX_RAMP_DELAY_12); //si4464_set_config(0x0C, RF_MODEM_433_19200_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_BCR_NCO_OFFSET_2_12), PROP_MODEM_BCR_NCO_OFFSET, (uint8_t *) RF_MODEM_433_19200_BCR_NCO_OFFSET_2_12); //si4464_set_config(0x0D, RF_MODEM_433_19200_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_AFC_LIMITER_1_3), PROP_MODEM_AFC_LIMITER_1, (uint8_t *) RF_MODEM_433_19200_AFC_LIMITER_1_3); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_AGC_CONTROL_1), PROP_MODEM_AGC_CONTROL, (uint8_t *) RF_MODEM_433_19200_AGC_CONTROL_1); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_AGC_WINDOW_SIZE_12), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_MODEM_433_19200_AGC_WINDOW_SIZE_12); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_RAW_CONTROL_10), PROP_MODEM_RAW_CONTROL, (uint8_t *) RF_MODEM_433_19200_RAW_CONTROL_10); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_RAW_SEARCH2_2), PROP_MODEM_RAW_SEARCH2, (uint8_t *) RF_MODEM_433_19200_RAW_SEARCH2_2); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_SPIKE_DET_2), PROP_MODEM_SPIKE_DET, (uint8_t *) RF_MODEM_433_19200_SPIKE_DET_2); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
            
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_RSSI_MUTE_1), PROP_MODEM_RSSI_MUTE, (uint8_t *) RF_MODEM_433_19200_RSSI_MUTE_1); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
    
    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_MODEM_433_19200_DSA_CTRL1_5), PROP_MODEM_DSA_CTRL1, (uint8_t *) RF_MODEM_433_19200_DSA_CTRL1_5); //si4464_set_config(0x0B, RF_MODEM_433_19200_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS
   
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_MODEM_433_19200_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_MODEM_433_19200_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_433_19200_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_FREQ_433_19200_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_FREQ_433_19200_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS    
#else // notSI4463_C2A
     si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    //    si4464_set_config(PROP_GLOBAL_GROUP,sizeof(RF_433_19200_GLOBAL_XO_TUNE_2),PROP_GLOBAL_XO_TUNE, (uint8_t *)RF_GLOBAL_XO_TUNE_2);  // Send command to the radio IC
    //    si4464_wait_cts();  // Wait for CTS
    //    si4464_set_config(PROP_PREAMBLE_GROUP,sizeof(RF_433_19200_PREAMBLE_TX_LENGTH_9),PROP_PREAMBLE_TX_LENGTH, (uint8_t *)RF_433_19200_PREAMBLE_TX_LENGTH_9);//si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    //    si4464_wait_cts();  // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_19200_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_19200_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_19200_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_19200_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_19200_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_19200_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_19200_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_19200_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_19200_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_19200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_19200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif // End SI4463-C2A
   
}

void si4464_goto_433_38400(void)
{
#ifdef SI4463_C2A
       si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    //    si4464_set_config(PROP_GLOBAL_GROUP,sizeof(RF_433_38400_GLOBAL_XO_TUNE_2),PROP_GLOBAL_XO_TUNE, (uint8_t *)RF_GLOBAL_XO_TUNE_2);  // Send command to the radio IC
    //    si4464_wait_cts();  // Wait for CTS
    //    si4464_set_config(PROP_PREAMBLE_GROUP,sizeof(RF_433_38400_PREAMBLE_TX_LENGTH_9),PROP_PREAMBLE_TX_LENGTH, (uint8_t *)RF_433_38400_PREAMBLE_TX_LENGTH_9);//si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    //    si4464_wait_cts();  // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_38400_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_38400_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_38400_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_38400_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_38400_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_38400_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_38400_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_38400_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#else // notSI4463_C2A
       si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    //    si4464_set_config(PROP_GLOBAL_GROUP,sizeof(RF_433_38400_GLOBAL_XO_TUNE_2),PROP_GLOBAL_XO_TUNE, (uint8_t *)RF_GLOBAL_XO_TUNE_2);  // Send command to the radio IC
    //    si4464_wait_cts();  // Wait for CTS
    //    si4464_set_config(PROP_PREAMBLE_GROUP,sizeof(RF_433_38400_PREAMBLE_TX_LENGTH_9),PROP_PREAMBLE_TX_LENGTH, (uint8_t *)RF_433_38400_PREAMBLE_TX_LENGTH_9);//si4464_set_config(0x0D, RF_PREAMBLE_TX_LENGTH_9);
    //    si4464_wait_cts();  // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_433_38400_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_433_38400_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_433_38400_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_433_38400_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_433_38400_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_433_38400_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_433_38400_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_433_38400_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_433_38400_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif // End SI4463-C2A
}

void si4464_goto_421_4800(void)
{
#ifdef  SI4463_C2A   
#ifdef FREQ_421MHz	
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_421_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_421_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_421_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_421_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_421_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_421_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_421_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_421_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif
#else // not  SI4463_C2A
        
#ifdef FREQ_421MHz	
    si4464_set_config(PROP_FRR_GROUP, sizeof (RF_FRR_CTL_A_MODE_4), PROP_FRR_CTL_A_MODE, (uint8_t *) RF_FRR_CTL_A_MODE_4); //si4464_set_config(0x08, RF_FRR_CTL_A_MODE_2);  // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_FREQ_CONTROL_GROUP, sizeof (RF_421_FREQ_CONTROL_INTE_8), PROP_FREQ_CONTROL_INTE, (uint8_t *) RF_421_FREQ_CONTROL_INTE_8); //si4464_set_config(0x10, RF_MATCH_VALUE_1_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_MOD_TYPE_12), PROP_MODEM_MOD_TYPE, (uint8_t *) RF_421_MODEM_MOD_TYPE_12); //si4464_set_config(0x10, RF_MODEM_MOD_TYPE_12);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_TX_RAMP_DELAY_8), PROP_MODEM_TX_RAMP_DELAY, (uint8_t *) RF_421_MODEM_TX_RAMP_DELAY_8); //si4464_set_config(0x0C, RF_MODEM_TX_RAMP_DELAY_8);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_BCR_OSR_1_9), PROP_MODEM_BCR_OSR, (uint8_t *) RF_421_MODEM_BCR_OSR_1_9); //si4464_set_config(0x0D, RF_MODEM_BCR_OSR_1_9);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_AFC_GEAR_7), PROP_MODEM_AFC_GEAR, (uint8_t *) RF_421_MODEM_AFC_GEAR_7); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_AGC_WINDOW_SIZE_9), PROP_MODEM_AGC_WINDOW_SIZE, (uint8_t *) RF_421_MODEM_AGC_WINDOW_SIZE_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_GROUP, sizeof (RF_421_MODEM_OOK_CNT1_9), PROP_MODEM_OOK_CNT1, (uint8_t *) RF_421_MODEM_OOK_CNT1_9); //si4464_set_config(0x0B, RF_MODEM_AFC_GEAR_7);
    si4464_wait_cts(); // Wait for CTS

    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7, (uint8_t *) RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12), PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7, (uint8_t *) RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12);
    si4464_wait_cts(); // Wait for CTS
    si4464_set_config(PROP_MODEM_CHFLT_GROUP, sizeof (RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12), MODEM_CHFLT_RX2_CHFLT_COE7_7, (uint8_t *) RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12); //si4464_set_config(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12);
    si4464_wait_cts(); // Wait for CTS
#endif
#endif // end if  SI4463_C2A
}

void si4464_config_address(uint8_t addr)
{
    //    uint8_t match[10] = {0x11, 0x30, 0x03, 0x00, 0x00, 0xFF, 0x41};//, 0x00, 0xFF,0x41};
    //    match[4] = addr;
    //    si4464_send_command(0x07, match);
    //    si4464_wait_cts();  // Wait for CTSsi4464_send_command(0x10, RF_MATCH_VALUE_1_12);
}

void si4464_config_broadcast(void)
{
    //    uint8_t match[16] = {0x11, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //    si4464_send_command(0x10, match);
    //    si4464_wait_cts();  // Wait for CTSsi4464_send_command(0x10, RF_MATCH_VALUE_1_12);
}

unsigned char si4464_get_fast_resp_regs(unsigned char StartReg, unsigned char NmbrOfRegs, unsigned char * RegValues)
{
    if ((NmbrOfRegs == 0) || (NmbrOfRegs > 4)) return 1;
    si4464_select(); // si4464_cs_output_low(); Vuonglq select radio IC by pulling its nSEL pin low
    si4464_write_byte(StartReg);
    si4464_send_data_get_resp(NmbrOfRegs, RegValues);
    si4464_de_select(); // si4464_cs_output_high(); vuonglq  de-select radio IC by putting its nSEL pin high
    return 0;
}

uint8_t si4464_calc_rssi(uint8_t rssi)
{
    int16_t tempRSSI;
    uint8_t result = 0;
    tempRSSI = rssi / 2 - 134;
    if (tempRSSI <= -75)
    {
        tempRSSI = 150 + tempRSSI * 2;
        result = (uint8_t) tempRSSI;
    }
    else if (tempRSSI > -75)
    {
        tempRSSI = 406 + tempRSSI * 2;
        result = (uint8_t) tempRSSI;
    }
    return result;
}

uint8_t si4464_read_rx_buffer(uint8_t RxFifoLength, uint8_t *RxFifoData, uint8_t *rssi)
{
    unsigned char temp;
    si4464_get_fast_resp_regs(CMD_FRR_A_READ, 1, &temp); // --- Read RSSI Fast Response Register A ---
    *rssi = si4464_calc_rssi(temp);                     // --- Calculate RSSI compatible with RSSI of CC1101 ---
    si4464_select(); // si4464_cs_output_low(); Vuonglq - Select radio IC by pulling its nSEL pin low ---
    si4464_write_byte(CMD_READ_RX_FIFO); // --- Send Rx read command (READ RX FIFO) ----
#ifdef _STAR_SUPPORT
    if (rf.rf_type != RF_TYPE_DTS27)
#endif
    {
        rf.count = si4464_read_byte();  // ---- Read 1 byte from RX FIFO --- Field 1 contained length byte (mode LEAVE IN) --- 
#ifdef DEBUG
        uart_transmit_flash((uint8_t *)"SI4464 GET LENGTH!\n ");
        hex2ascii(swap4bit(rf.count),uart_buffer);
        uart_transmit_buffer(uart_buffer,2);
        uart_transmit_byte('\n');
        
        hex2ascii(swap4bit(RxFifoLength),uart_buffer);
        uart_transmit_buffer(uart_buffer,2);
        uart_transmit_byte('\n');
#endif           
        if (rf.count != RxFifoLength)  // --- If the first byte of FIFO is not same RX length from SI446x - Reset flag 
        {
            rf.flags.reset = TRUE;
            si4464_de_select(); // si4464_cs_output_high(); vuonglq // -- DeSelect radio IC by pulling its nSEL pin high ---
            return FALSE;
        }
    }
    if (RxFifoLength > RF_BUFFER_MAX - 2) RxFifoLength = RF_BUFFER_MAX - 2;
    si4464_send_data_get_resp(RxFifoLength, RxFifoData); // Read RX FIFO
    si4464_de_select(); // si4464_cs_output_high(); vuonglq de-select radio IC by putting its nSEL pin high
    return TRUE;
}

uint8_t si4464_read_buffer(uint8_t *buffer, uint8_t *length, uint8_t read_length, uint8_t *rssi)
{
    uint8_t result = FALSE;
    if (read_length == TRUE) *length = si4464_get_rx_length();
    //if (*length > RF_BUFFER_MAX - 2) *length = RF_BUFFER_MAX - 2;
	if ((*length > (RF_BUFFER_MAX-2)) || (*length == 0)) return FALSE; //tuantm 290620
    result = si4464_read_rx_buffer(*length, buffer, rssi);
    si4464_reset_fifo(SI4464_RX); // clear FIFO
//    delay_ms(10); //khuet them de thu phat cham lai 
    return result;
}

void si4464_send_buffer(unsigned char *DataPacket, unsigned char LengthPaket, uint8_t write_length)
{
    uint8_t i;
    //thaodtn: them
    rf_gdo2_interrupt_flag_clear();
    //    rf_gdo2_interrupt_high_to_low();
    //    rf_gdo2_interrupt_enable();
    si4464_reset_fifo(SI4464_TX);
    rf_set_stage(RF_TX);
    rf.flags.error_tx = 0; //vinh, dat truoc rf.status = TX
    //    rf.count_rf_tx = 0; //vinh, dat truoc rf.status = TX
    rf.flags.transmiting = 0; //vinh
    rf.flags.processing = 0; //vinh
    rf.status = RF_TX;
    ////////////////////////////////////////////////////////////////////////////
    uint16_t crcTemp = 0xFFFF;
//    if (write_length == TRUE)
//    {
//        crcTemp = si4464_calCRC(LengthPaket, crcTemp);
//    }
    //them 2 byte crc
#ifdef SI4464_EMEC_CONFIG
//    for (i = 0; i < LengthPaket; i++)
//    {
//        crcTemp = si4464_calCRC(DataPacket[i], crcTemp);
//    }
#endif
    
    //================= Write data to TXcfg_buffer =============//
    si4464_select(); // si4464_cs_output_low(); Vuonglq select radio IC by pulling its nSEL pin low
    si4464_write_byte(CMD_WRITE_TX_FIFO); // Send Tx write command
    if (write_length == TRUE)
    {
        si4464_write_byte(LengthPaket); // Send Tx write command
    }
    si4464_send_data_no_resp(LengthPaket, DataPacket); // Write Tx FIFO

//#ifdef SI4464_EMEC_CONFIG
//    si4464_write_byte((uint8_t) (crcTemp >> 8));
//    si4464_write_byte((uint8_t) (crcTemp));
//#endif
    si4464_de_select(); // si4464_cs_output_high(); vuonglq  de-select radio IC by putting its nSEL pin high
    si4464_wait_cts(); // Wait for CTS
#ifdef RFPA_SUPPORT
    rf_pa_tx();
#endif
    //=======================Send TX Paket length===================//
    if (write_length == TRUE) LengthPaket++;
//#ifdef SI4464_EMEC_CONFIG
//    LengthPaket += 2; //2 byteCRC
//#endif      
    cfg_buffer[0] = CMD_SET_PROPERTY; // Use property command
    cfg_buffer[1] = PROP_PKT_GROUP; // Select property group
    cfg_buffer[2] = 0x02; // Number of properties to be written
    cfg_buffer[3] = PROP_PKT_FIELD_1_LENGTH_12_8; // Specify first property
    cfg_buffer[4] = 0; //Field length( variable paket length info + 2 bytes CRC)
    cfg_buffer[5] = LengthPaket; //Field length( variable paket length info + 2 bytes CRC)
    si4464_send_command(6, cfg_buffer); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    //=============Start TX===========//
    cfg_buffer[0] = CMD_START_TX; //Use Start TX command
#ifdef RF_409_125MHZ
    cfg_buffer[1] = 0x01; //Set chanel number //
#elif RF_409_325MHZ
    cfg_buffer[1] = 0x02; //Set chanel number //
#else
    cfg_buffer[1] = 0x00; //Set chanel number //
#endif
    cfg_buffer[2] = 0x30; //TX to ready //0x80;//TX to RX //vudh//0x30;          //Ready state after TX, Start RX immediately
    cfg_buffer[3] = 0x00; //packet fields used, do not enter packet length here
    cfg_buffer[4] = 0x00;
    si4464_send_command(5, cfg_buffer); // Send command to the radio IC
    si4464_wait_cts(); // Wait for CTS
    rf_gdo2_interrupt_high_to_low();
    rf_gdo2_interrupt_enable();
}

unsigned char si4464_get_rx_length(void)
{
    //use Packet info command to get the length
    cfg_buffer[0] = CMD_PACKET_INFO;
    cfg_buffer[1] = 0x00; //vudh
    cfg_buffer[2] = 0x00;
    cfg_buffer[3] = 0x00;
    cfg_buffer[4] = 0x00;
    cfg_buffer[5] = 0x00;
    si4464_send_command(6, cfg_buffer); // Send command to the radio IC
    si4464_get_resp(2, cfg_buffer); //Get response
    return cfg_buffer[1];
}

uint8_t si4464_get_int_stt(uint8_t value_check)
{
    cfg_buffer[0] = CMD_GET_INT_STATUS;
    cfg_buffer[1] = 0x00;
    cfg_buffer[2] = 0x00;
    cfg_buffer[3] = 0x00;
    si4464_send_command(4, cfg_buffer); // Send command to the radio IC
    si4464_get_resp(8, cfg_buffer); //Get response
    if ((cfg_buffer[3] & value_check) == value_check) return TRUE;
    else return FALSE;
}

uint16_t si4464_calCRC(uint8_t crcData, uint16_t crcReg)
{
    uint8_t j;
    for (j = 0; j < 8; j++)
    {
        if (((uint8_t) ((crcReg & 0x8000) >> 8) ^ (crcData & 0x80)) != 0)
        {
            crcReg = (uint16_t) ((crcReg << 1) ^ CRC16BIT_POLYNOME);
        }
        else
        {
            crcReg = (uint16_t) (crcReg << 1);
        }
        crcData = (uint8_t) (crcData << 1);
    }
    return crcReg;
}
//
//void si4464_read_temp_vol()
//{
//    cfg_buffer[0] = 0x14;
//    cfg_buffer[1] = 0x18;
//    cfg_buffer[2] = 0x99; // Voltage 0V-3.6V
//    si4464_send_command(3,cfg_buffer);
//    si4464_get_resp(8,cfg_buffer);
//    //uart_transmit_buffer(cfg_buffer, 8);
//}
//
//void si4464_read_chip_info()
//{
//    cfg_buffer[0] = 0x01;
//    cfg_buffer[1] = 0x00;
//    si4464_send_command(2,cfg_buffer);
//    si4464_get_resp(8,cfg_buffer);
//    //uart_transmit_buffer(cfg_buffer, 8);
//}

////////////////////////////////////////////////////////////////////////////
/*   end of file    */
////////////////////////////////////////////////////////////////////////////
#endif //SI4464_MODULE_SUPPORT