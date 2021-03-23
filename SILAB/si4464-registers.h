#ifndef _SI4464_REGISTERS_H_
#define _SI4464_REGISTERS_H_

#include "..\library.h"
////////////////////////////////////////////////////////////////////////////////
#ifdef SI4464_MODULE_SUPPORT

#ifdef SI4463_C2A
//Silab P-Smart
#ifdef SI4464_PSMART_CONFIG
extern const unsigned char RF_POWER_UP[7];
extern const unsigned char RF_GPIO_PIN_CFG[8];
extern const unsigned char RF_GLOBAL_XO_TUNE_2[2];
extern const unsigned char RF_GLOBAL_CONFIG_1[1];
extern const unsigned char RF_INT_CTL_ENABLE_2[4];
extern const unsigned char RF_FRR_CTL_A_MODE_4[4];
extern const unsigned char RF_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_SYNC_CONFIG_5[5];
extern const unsigned char RF_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_PKT_LEN_12[12];
extern const unsigned char RF_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_POWER_VALUE[4];
extern const unsigned char RF_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_MATCH_VALUE_1_12[12];
extern const unsigned char RF_FREQ_CONTROL_INTE_8[8];
#endif
//Silab P-Smart
#ifdef SI4464_EMEC_CONFIG
extern const unsigned char RF_POWER_UP[7];
extern const unsigned char RF_GPIO_PIN_CFG[8];
extern const unsigned char RF_GLOBAL_XO_TUNE_2[2];
extern const unsigned char RF_GLOBAL_CONFIG_1[1];
extern const unsigned char RF_INT_CTL_ENABLE_2[2];
extern const unsigned char RF_FRR_CTL_A_MODE_4[4];
extern const unsigned char RF_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_SYNC_CONFIG_5[6];
extern const unsigned char RF_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_PKT_LEN_12[12];
extern const unsigned char RF_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
//// =========== Vudh - Thay doi thanh ghi modem cho SI4463 =============
extern const unsigned char RF_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_MODEM_TX_RAMP_DELAY_12[12];
extern const unsigned char RF_MODEM_BCR_NCO_OFFSET_2_12[12];
extern const unsigned char RF_MODEM_AFC_LIMITER_1_3[3];
extern const unsigned char RF_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_MODEM_AGC_WINDOW_SIZE_12[12];
extern const unsigned char RF_MODEM_RAW_CONTROL_10[10];
extern const unsigned char RF_MODEM_RAW_SEARCH2_2[2];
extern const unsigned char RF_MODEM_SPIKE_DET_2[2];
extern const unsigned char RF_MODEM_RSSI_MUTE_1[1];
extern const unsigned char RF_MODEM_DSA_CTRL1_5[5];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_PA_MODE_4[4];
extern       unsigned char rf_pa_mode_4[4];
extern const unsigned char RF_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_MATCH_VALUE_1_12[12];
extern const unsigned char RF_FREQ_CONTROL_INTE_8[8];
//// =========== Vudh - Thay doi thanh ghi modem cho SI4463 =============

//---------------------------------------
//Freq 433
//1200bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_1200_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_1200_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_1200_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_1200_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_1200_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_1200_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_1200_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
//4800bps
#ifdef FREQ_433MHz
extern const unsigned char RF_433_4800_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_4800_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_4800_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_4800_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_4800_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_4800_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_4800_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
#endif
//19200bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_19200_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_19200_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_19200_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_19200_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_19200_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_19200_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_19200_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00\

extern const unsigned char RF_MODEM_433_19200_MOD_TYPE_12 [12];// =                    { 0x11, 0x20, 0x0C, 0x00, 0x02, 0x00, 0x07, 0x02, 0xEE, 0x00, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01};
extern const unsigned char RF_MODEM_433_19200_FREQ_DEV_0_1 [1];// =                    { 0x11, 0x20, 0x01, 0x0C, 0x68};
extern const unsigned char RF_MODEM_433_19200_TX_RAMP_DELAY_12 [12];// =               { 0x11, 0x20, 0x0C, 0x18, 0x01, 0x00, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20, 0x00, 0xE8, 0x00, 0x62};
extern const unsigned char RF_MODEM_433_19200_BCR_NCO_OFFSET_2_12 [12];// =            { 0x11, 0x20, 0x0C, 0x24, 0x05, 0x3E, 0x2D, 0x07, 0xFF, 0x02, 0x00, 0x00, 0x00, 0x12, 0xC0, 0xA8};
extern const unsigned char RF_MODEM_433_19200_AFC_LIMITER_1_3 [3];// =                 { 0x11, 0x20, 0x03, 0x30, 0x02, 0x1C, 0xE0};
extern const unsigned char RF_MODEM_433_19200_AGC_CONTROL_1 [1];// =                   { 0x11, 0x20, 0x01, 0x35, 0xE0};
extern const unsigned char RF_MODEM_433_19200_AGC_WINDOW_SIZE_12 [12];// =             { 0x11, 0x20, 0x0C, 0x38, 0x11, 0x15, 0x15, 0x80, 0x02, 0x22, 0x55, 0x00, 0x28, 0x0C, 0xA4, 0x23};
extern const unsigned char RF_MODEM_433_19200_RAW_CONTROL_10 [10];// =                 { 0x11, 0x20, 0x0A, 0x45, 0x03, 0x00, 0xB0, 0x02, 0x00, 0xFF, 0x0F, 0x00, 0x18, 0x40};
extern const unsigned char RF_MODEM_433_19200_RAW_SEARCH2_2 [2];// =                   { 0x11, 0x20, 0x02, 0x50, 0x84, 0x0A};
extern const unsigned char RF_MODEM_433_19200_SPIKE_DET_2 [2];// =                     { 0x11, 0x20, 0x02, 0x54, 0x03, 0x07};
extern const unsigned char RF_MODEM_433_19200_RSSI_MUTE_1 [1];// =                     { 0x11, 0x20, 0x01, 0x57, 0x00};
extern const unsigned char RF_MODEM_433_19200_DSA_CTRL1_5 [5];// =                     { 0x11, 0x20, 0x05, 0x5B, 0x40, 0x04, 0x06, 0x78, 0x20};
extern const unsigned char RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE13_7_0_12 [12];// =   { 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11};
extern const unsigned char RF_MODEM_433_19200_CHFLT_RX1_CHFLT_COE1_7_0_12 [12];// =    { 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0x39, 0x2B, 0x00, 0xC3, 0x7F, 0x3F};
extern const unsigned char RF_MODEM_433_19200_CHFLT_RX2_CHFLT_COE7_7_0_12 [12];// =    { 0x11, 0x21, 0x0C, 0x18, 0x0C, 0xEC, 0xDC, 0xDC, 0xE3, 0xED, 0xF6, 0xFD, 0x15, 0xC0, 0xFF, 0x0F};
extern const unsigned char RF_FREQ_433_19200_CONTROL_INTE_8 [8];// =                  { 0x11, 0x40, 0x08, 0x00, 0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE};

extern const unsigned char RF_MODEM_433_4800_MOD_TYPE_12 [12];// =                    { 0x11, 0x20, 0x0C, 0x00, 0x02, 0x00, 0x07, 0x02, 0xEE, 0x00, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01};
extern const unsigned char RF_MODEM_433_4800_FREQ_DEV_0_1 [1];// =                    { 0x11, 0x20, 0x01, 0x0C, 0x68};
extern const unsigned char RF_MODEM_433_4800_TX_RAMP_DELAY_12 [12];// =               { 0x11, 0x20, 0x0C, 0x18, 0x01, 0x00, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20, 0x00, 0xE8, 0x00, 0x62};
extern const unsigned char RF_MODEM_433_4800_BCR_NCO_OFFSET_2_12 [12];// =            { 0x11, 0x20, 0x0C, 0x24, 0x05, 0x3E, 0x2D, 0x07, 0xFF, 0x02, 0x00, 0x00, 0x00, 0x12, 0xC0, 0xA8};
extern const unsigned char RF_MODEM_433_4800_AFC_LIMITER_1_3 [3];// =                 { 0x11, 0x20, 0x03, 0x30, 0x02, 0x1C, 0xE0};
extern const unsigned char RF_MODEM_433_4800_AGC_CONTROL_1 [1];// =                   { 0x11, 0x20, 0x01, 0x35, 0xE0};
extern const unsigned char RF_MODEM_433_4800_AGC_WINDOW_SIZE_12 [12];// =             { 0x11, 0x20, 0x0C, 0x38, 0x11, 0x15, 0x15, 0x80, 0x02, 0x22, 0x55, 0x00, 0x28, 0x0C, 0xA4, 0x23};
extern const unsigned char RF_MODEM_433_4800_RAW_CONTROL_10 [10];// =                 { 0x11, 0x20, 0x0A, 0x45, 0x03, 0x00, 0xB0, 0x02, 0x00, 0xFF, 0x0F, 0x00, 0x18, 0x40};
extern const unsigned char RF_MODEM_433_4800_RAW_SEARCH2_2 [2];// =                   { 0x11, 0x20, 0x02, 0x50, 0x84, 0x0A};
extern const unsigned char RF_MODEM_433_4800_SPIKE_DET_2 [2];// =                     { 0x11, 0x20, 0x02, 0x54, 0x03, 0x07};
extern const unsigned char RF_MODEM_433_4800_RSSI_MUTE_1 [1];// =                     { 0x11, 0x20, 0x01, 0x57, 0x00};
extern const unsigned char RF_MODEM_433_4800_DSA_CTRL1_5 [5];// =                     { 0x11, 0x20, 0x05, 0x5B, 0x40, 0x04, 0x06, 0x78, 0x20};
extern const unsigned char RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE13_7_0_12 [12];// =   { 0x11, 0x21, 0x0C, 0x00, 0xCC, 0xA1, 0x30, 0xA0, 0x21, 0xD1, 0xB9, 0xC9, 0xEA, 0x05, 0x12, 0x11};
extern const unsigned char RF_MODEM_433_4800_CHFLT_RX1_CHFLT_COE1_7_0_12 [12];// =    { 0x11, 0x21, 0x0C, 0x0C, 0x0A, 0x04, 0x15, 0xFC, 0x03, 0x00, 0x39, 0x2B, 0x00, 0xC3, 0x7F, 0x3F};
extern const unsigned char RF_MODEM_433_4800_CHFLT_RX2_CHFLT_COE7_7_0_12 [12];// =    { 0x11, 0x21, 0x0C, 0x18, 0x0C, 0xEC, 0xDC, 0xDC, 0xE3, 0xED, 0xF6, 0xFD, 0x15, 0xC0, 0xFF, 0x0F};
extern const unsigned char RF_FREQ_433_4800_CONTROL_INTE_8 [8];// =                  { 0x11, 0x40, 0x08, 0x00, 0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE};

//38400bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_38400_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_38400_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_38400_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_38400_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_38400_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_38400_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_38400_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00	
#ifdef FREQ_421MHz
extern const unsigned char RF_421_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x37, 0x09, 0x11, 0x11, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_421_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_421_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_421_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_421_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_421_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09, 0x38, 0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_421_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
#endif
#endif // end SI4464_EMEC_CONFIG

#ifdef _STAR_SUPPORT
//const unsigned char RF_STAR_GLOBAL_XO_TUNE_2[2] = { 0x52, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_STAR_GPIO_PIN_CFG[8];
extern const unsigned char RF_STAR_INT_CTL_ENABLE_2[2];
extern const unsigned char RF_STAR_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_STAR_SYNC_CONFIG_5[5];
extern const unsigned char RF_STAR_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_STAR_PKT_LEN_12[12];
extern const unsigned char RF_STAR_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_STAR_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_STAR_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_STAR_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_STAR_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_STAR_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_STAR_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_STAR_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_STAR_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_STAR_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_STAR_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_STAR_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_STAR_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_STAR_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_STAR_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_STAR_MATCH_VALUE_1_12[12];
extern const unsigned char RF_STAR_FREQ_CONTROL_INTE_8[8];
#endif
//Silab P-Smart
#ifdef SI4464_PSMART_CONFIG
extern const unsigned char RF_POWER_UP[7];
extern const unsigned char RF_GPIO_PIN_CFG[8];
extern const unsigned char RF_GLOBAL_XO_TUNE_2[2];
extern const unsigned char RF_GLOBAL_CONFIG_1[1];
extern const unsigned char RF_INT_CTL_ENABLE_2[4];
extern const unsigned char RF_FRR_CTL_A_MODE_4[4];
extern const unsigned char RF_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_SYNC_CONFIG_5[5];
extern const unsigned char RF_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_PKT_LEN_12[12];
extern const unsigned char RF_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_POWER_VALUE[4];
extern const unsigned char RF_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_MATCH_VALUE_1_12[12];
extern const unsigned char RF_FREQ_CONTROL_INTE_8[8];
#endif
#endif //SI4464_MODULE_SUPPORT

#else //Not SI4463_C2A
#ifdef SI4464_MODULE_SUPPORT

# define SI446x_CHANEL 0x00
//Silab P-Smart
#ifdef SI4464_PSMART_CONFIG
extern const unsigned char RF_POWER_UP[7];
extern const unsigned char RF_GPIO_PIN_CFG[8];
extern const unsigned char RF_GLOBAL_XO_TUNE_2[2];
extern const unsigned char RF_GLOBAL_CONFIG_1[1];
extern const unsigned char RF_INT_CTL_ENABLE_2[4];
extern const unsigned char RF_FRR_CTL_A_MODE_4[4];
extern const unsigned char RF_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_SYNC_CONFIG_5[5];
extern const unsigned char RF_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_PKT_LEN_12[12];
extern const unsigned char RF_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_POWER_VALUE[4];
extern const unsigned char RF_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_MATCH_VALUE_1_12[12];
extern const unsigned char RF_FREQ_CONTROL_INTE_8[8];
#endif
//Silab P-Smart
#ifdef SI4464_EMEC_CONFIG
extern const unsigned char RF_POWER_UP[7];
extern const unsigned char RF_GPIO_PIN_CFG[8];
extern const unsigned char RF_GLOBAL_XO_TUNE_2[2];
extern const unsigned char RF_GLOBAL_CONFIG_1[1];
extern const unsigned char RF_INT_CTL_ENABLE_2[2];
extern const unsigned char RF_FRR_CTL_A_MODE_4[4];
extern const unsigned char RF_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_SYNC_CONFIG_5[5];
extern const unsigned char RF_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_PKT_LEN_12[12];
extern const unsigned char RF_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_POWER_VALUE[4];
extern const unsigned char RF_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_MATCH_VALUE_1_12[12];
extern const unsigned char RF_FREQ_CONTROL_INTE_8[8];
extern const unsigned char RF_RX_HOP_CONTROL[1];
//---------------------------------------
//Freq 433
//1200bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_1200_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_1200_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_1200_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_1200_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_1200_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_1200_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_1200_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_1200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
//4800bps
#ifdef FREQ_433MHz
extern const unsigned char RF_433_4800_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_4800_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_4800_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_4800_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_4800_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_4800_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_4800_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_4800_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
#endif
//19200bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_19200_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_19200_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_19200_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_19200_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_19200_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_19200_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_19200_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_19200_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
//38400bps
//const unsigned char RF_433_1200_GLOBAL_XO_TUNE_2[2] = { 0x40, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_433_38400_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08,0x00,0x38, 0x0D, 0xEB, 0x85, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_433_38400_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00,0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_433_38400_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08,0x18,0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_433_38400_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09,0x22,0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_433_38400_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07,0x2C,0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_433_38400_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09,0x38,0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_433_38400_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09,0x42,0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00,0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C,0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_433_38400_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18,0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00		  
#ifdef FREQ_421MHz
extern const unsigned char RF_421_FREQ_CONTROL_INTE_8[8];
//#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x37, 0x09, 0x11, 0x11, 0x44, 0x44, 0x20, 0xFE
extern const unsigned char RF_421_MODEM_MOD_TYPE_12[12];
//#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x02, 0x00, 0x07, 0x00, 0xBB, 0x80, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x01
extern const unsigned char RF_421_MODEM_TX_RAMP_DELAY_8[8];
//#define RF_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x80, 0x00, 0x30, 0x20
extern const unsigned char RF_421_MODEM_BCR_OSR_1_9[9];
//#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x01, 0x87, 0x01, 0x4F, 0x8B, 0x00, 0xA8, 0x02, 0xC2
extern const unsigned char RF_421_MODEM_AFC_GEAR_7[7];
//#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0x80, 0x0E, 0x14, 0xC9, 0x80
extern const unsigned char RF_421_MODEM_AGC_WINDOW_SIZE_9[9];
//#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09, 0x38, 0x11, 0x56, 0x56, 0x00, 0x02, 0x89, 0x55, 0x00, 0x2A
extern const unsigned char RF_421_MODEM_OOK_CNT1_9[9];
//#define RF_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0x95, 0x01, 0x80, 0xFF
extern const unsigned char RF_421_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C
extern const unsigned char RF_421_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
//#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5
extern const unsigned char RF_421_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
//#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00
#endif
#ifdef _STAR_SUPPORT
//const unsigned char RF_STAR_GLOBAL_XO_TUNE_2[2] = { 0x52, 0x00}; //0x29; 0x52 // bo xanh duong
extern const unsigned char RF_STAR_GPIO_PIN_CFG[8];
extern const unsigned char RF_STAR_INT_CTL_ENABLE_2[2];
extern const unsigned char RF_STAR_PREAMBLE_TX_LENGTH_9[9];
extern const unsigned char RF_STAR_SYNC_CONFIG_5[5];
extern const unsigned char RF_STAR_PKT_CRC_CONFIG_7[7];
extern const unsigned char RF_STAR_PKT_LEN_12[12];
extern const unsigned char RF_STAR_PKT_FIELD_2_CRC_CONFIG_12[12];
extern const unsigned char RF_STAR_PKT_FIELD_5_CRC_CONFIG_12[12];
extern const unsigned char RF_STAR_PKT_RX_FIELD_3_CRC_CONFIG_9[9];
extern const unsigned char RF_STAR_MODEM_MOD_TYPE_12[12];
extern const unsigned char RF_STAR_MODEM_FREQ_DEV_0_1[1];
extern const unsigned char RF_STAR_MODEM_TX_RAMP_DELAY_8[8];
extern const unsigned char RF_STAR_MODEM_BCR_OSR_1_9[9];
extern const unsigned char RF_STAR_MODEM_AFC_GEAR_7[7];
extern const unsigned char RF_STAR_MODEM_AGC_CONTROL_1[1];
extern const unsigned char RF_STAR_MODEM_AGC_WINDOW_SIZE_9[9];
extern const unsigned char RF_STAR_MODEM_OOK_CNT1_9[9];
extern const unsigned char RF_STAR_MODEM_RSSI_CONTROL_1[1];
extern const unsigned char RF_STAR_MODEM_RSSI_COMP_1[1];
extern const unsigned char RF_STAR_MODEM_CLKGEN_BAND_1[1];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12[12];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12[12];
extern const unsigned char RF_STAR_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12[12];
extern const unsigned char RF_STAR_SYNTH_PFDCP_CPFF_7[7];
extern const unsigned char RF_STAR_MATCH_VALUE_1_12[12];
extern const unsigned char RF_STAR_FREQ_CONTROL_INTE_8[8];
#endif																					
#endif

#endif //SI4464_MODULE_SUPPORT
#endif //SI4463_C2A


#endif //_SI4464_REGISTERS_H_