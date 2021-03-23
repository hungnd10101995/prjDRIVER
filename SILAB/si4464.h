#ifndef _SI4464_H_
#define _SI4464_H_

#include "library.h"
#include "functions-dt03mrf.h"
////////////////////////////////////////////////////////////////////////////////
#ifdef SI4464_MODULE_SUPPORT
/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include "spi.h"
#include "hardware.h"
#include "main.h"
#include "process.h"
#endif
/*===========================================================================*/
/*  G L O B A L   S Y M B O L   D E C L A R A T I O N S                      */
/*===========================================================================*/
#define CRC16_POLY 0x8005 //check CRC
//#define VCXO_FREQ 30000000
#define VCXO_FREQ 27000000       // Frequency of the crystal (oscillator) attached to the Si446x in Hz 
#define DEFAULT_FREQ 433550000   // Frequency for the first initialisation of the transmitter in Hz
#define DEFAULT_SHIFT 400        // Shift (=channel spacing) for the first initialisation of the transmitter in Hz
#define DEFAULT_POWER_LEVEL 20   // Power level of the PA
// RF frequency is 408.925 MHz
// Mod type is 2FSK,	DataRate: 10 ksps;   FreqDev: 20 kHz;   RX BW: 150.0 kHz
// Frequency 408; deviation: 5.15;

////////////////////////////////////////////////////////////////////////////////
// Max number of times we will try to read CTS from the radio

#if (SYS_FREQ > 14745600L)
#define MAX_CTS_RETRY           2500
#else
#define MAX_CTS_RETRY           500
#endif


#ifdef DEBUG
#define SI4464_BUFFER_MAX       50
#else
#define SI4464_BUFFER_MAX       125//120//vuonglq-giam-dung-luong-code
#endif

//-----------------------------------------------------------------------------
//	Commands
//-----------------------------------------------------------------------------
#define MANG                    1, 2, 3
// Boot Commands
#define CMD_POWER_UP			0x02
#define CMD_PATCH_IMAGE			0x04

// Common Commands
#define CMD_NOP			     	0x00
#define CMD_PART_INFO           0x01
#define CMD_FUNC_INFO			0x10
#define CMD_SET_PROPERTY		0x11
#define CMD_GET_PROPERTY		0x12
#define CMD_GPIO_PIN_CFG		0x13
#define CMD_FIFO_INFO			0x15
#define CMD_GET_INT_STATUS		0x20
#define CMD_REQUEST_DEVICE_STATE	0x33
#define CMD_CHANGE_STATE		0x34
#define CMD_READ_CMD_BUFF		0x44
#define CMD_FRR_A_READ			0x50
#define CMD_FRR_B_READ			0x51
#define CMD_FRR_C_READ			0x53
#define CMD_FRR_D_READ			0x57
#define CMD_WRITE_TX_FIFO		0x66
#define CMD_READ_RX_FIFO		0x77
#define CMD_GET_ADC_READING		0x14
#define CMD_PACKET_INFO			0x16
#define CMD_IRCAL				0x17
#define CMD_PROTOCOL_CFG		0x18
#define CMD_GET_PH_STATUS		0x21
#define CMD_GET_MODEM_STATUS	0x22
#define CMD_GET_CHIP_STATUS		0x23
#define CMD_RX_HOP				0x36

// Tx Commands
#define CMD_START_TX			0x31
// Rx Commands
#define CMD_START_RX			0x32

// cac state cua CMD_CHANGE_STATE
#define SI446x_STATE_NOCHANGE       0x00
#define SI446x_STATE_SLEEP          0x01
#define SI446x_STATE_SPI_ACTIVE     0x02
#define SI446x_STATE_READY          0x03
#define SI446x_STATE_TX_TUNE        0x05
#define SI446x_STATE_RX_TUNE        0x06
#define SI446x_STATE_TX             0x07
#define SI446x_STATE_RX             0x08

// sub cmd cua CMD_FIFO_INFO
#define SUBCMD_FIFO_INFO_TX         0x01
#define SUBCMD_FIFO_INFO_RX         0x02
#define SUBCMD_FIFO_INFO_TX_RX      0x03 //ca 2 bo dem TX va RX

//-----------------------------------------------------------------------------
//   Properties
//-----------------------------------------------------------------------------
#define PROP_GLOBAL_GROUP                       0x00
#define	PROP_GLOBAL_XO_TUNE                     0x00
#define	PROP_GLOBAL_CLK_CFG                     0x01
#define	PROP_GLOBAL_LOW_BATT_THRESH             0x02
#define	PROP_GLOBAL_CONFIG                      0x03
#define	PROP_GLOBAL_WUT_CONFIG                  0x04
#define	PROP_GLOBAL_WUT_M                       0x05
#define	PROP_GLOBAL_WUT_R                       0x07
#define	PROP_GLOBAL_WUT_LDC                     0x08
#define	PROP_GLOBAL_WUT_CAL                     0x09

#define PROP_INT_GROUP                          0x01
#define	PROP_INT_CTL_ENABLE                     0x00
#define	PROP_INT_CTL_PH_ENABLE                  0x01
#define	PROP_INT_CTL_MODEM_ENABLE               0x02
#define	PROP_INT_CTL_CHIP_ENABLE                0x03

#define PROP_FRR_GROUP                          0x02
#define	PROP_FRR_CTL_A_MODE			0x00
#define	PROP_FRR_CTL_B_MODE			0x01
#define	PROP_FRR_CTL_C_MODE			0x02
#define	PROP_FRR_CTL_D_MODE			0x03


#define PROP_PREAMBLE_GROUP                         0x10
#define PROP_PREAMBLE_TX_LENGTH			            0x00
#define PROP_PREAMBLE_CONFIG_STD_1	    		    0x01
#define PROP_PREAMBLE_CONFIG_NSTD		      	    0x02
#define PROP_PREAMBLE_CONFIG_STD_2	    		    0x03
#define PROP_PREAMBLE_CONFIG			            0x04
#define PROP_PREAMBLE_PATTERN_31_24	    		    0x05
#define PROP_PREAMBLE_PATTERN_23_16	    		    0x06
#define PROP_PREAMBLE_PATTERN_15_8	      		    0x07
#define PROP_PREAMBLE_PATTERN_7_0		      	    0x08
#define PROP_PREAMBLE_POSTAMBLE_CONFIG	  		    0x09
#define PROP_PREAMBLE_POSTAMBLE_PATTERN_31_24       0x0A
#define PROP_PREAMBLE_POSTAMBLE_PATTERN_23_16       0x0B
#define PROP_PREAMBLE_POSTAMBLE_PATTERN_15_8        0x0C
#define PROP_PREAMBLE_POSTAMBLE_PATTERN_7_0         0x0D

#define PROP_SYNC_GROUP					            0x11
#define PROP_SYNC_CONFIG				            0x00
#define PROP_SYNC_BITS_31_24		    		    0x01
#define PROP_SYNC_BITS_23_16			    	    0x02
#define PROP_SYNC_BITS_15_8				            0x03
#define PROP_SYNC_BITS_7_0				            0x04

#define PROP_PKT_GROUP						        0x12
#define PROP_PKT_CRC_CONFIG					        0x00
#define PROP_PKT_WHT_POLY_15_8			    	  	0x01
#define PROP_PKT_WHT_POLY_7_0				    	0x02
#define PROP_PKT_WHT_SEED_15_8                      0x03
#define PROP_PKT_WHT_SEED_7_0			     		0x04
#define PROP_PKT_WHT_BIT_NUM			    		0x05
#define PROP_PKT_CONFIG1					        0x06
#define PROP_PKT_RESERVED_18_7                      0x07
#define PROP_PKT_LEN                                0x08
#define PROP_PKT_LEN_FIELD_SOURCE			        0x09
#define PROP_PKT_LEN_ADJUST                         0x0A
#define PROP_PKT_TX_THRESHOLD			    		0x0B
#define PROP_PKT_RX_THRESHOLD		    			0x0C
#define PROP_PKT_FIELD_1_LENGTH_12_8                0x0D
#define PROP_PKT_FIELD_1_LENGTH_7_0		    		0x0E
#define PROP_PKT_FIELD_1_CONFIG				    	0x0F
#define PROP_PKT_FIELD_1_CRC_CONFIG	    			0x10
#define PROP_PKT_FIELD_2_LENGTH_12_8    			0x11
#define PROP_PKT_FIELD_2_LENGTH_7_0			    	0x12
#define PROP_PKT_FIELD_2_CONFIG			    		0x13
#define PROP_PKT_FIELD_2_CRC_CONFIG		    		0x14
#define PROP_PKT_FIELD_3_LENGTH_12_8    			0x15
#define PROP_PKT_FIELD_3_LENGTH_7_0	    			0x16
#define PROP_PKT_FIELD_3_CONFIG				      	0x17
#define PROP_PKT_FIELD_3_CRC_CONFIG	    			0x18
#define PROP_PKT_FIELD_4_LENGTH_12_8    			0x19
#define PROP_PKT_FIELD_4_LENGTH_7_0			    	0x1A
#define PROP_PKT_FIELD_4_CONFIG			    		0x1B
#define PROP_PKT_FIELD_4_CRC_CONFIG			     	0x1C
#define PROP_PKT_FIELD_5_LENGTH_12_8                0x1D
#define PROP_PKT_FIELD_5_LENGTH_7_0			    	0x1E
#define PROP_PKT_FIELD_5_CONFIG					    0x1F
#define PROP_PKT_FIELD_5_CRC_CONFIG			    	0x20
#define PROP_PKT_RX_FIELD_1_LENGTH_12_8             0x21
#define PROP_PKT_RX_FIELD_1_LENGTH_7_0	  		    0x22
#define PROP_PKT_RX_FIELD_1_CONFIG			    	0x23
#define PROP_PKT_RX_FIELD_1_CRC_CONFIG	  		    0x24
#define PROP_PKT_RX_FIELD_2_LENGTH_12_8	  		    0x25
#define PROP_PKT_RX_FIELD_2_LENGTH_7_0	  		    0x26
#define PROP_PKT_RX_FIELD_2_CONFIG			     	0x27
#define PROP_PKT_RX_FIELD_2_CRC_CONFIG	  		    0x28
#define PROP_PKT_RX_FIELD_3_LENGTH_12_8	  		    0x29
#define PROP_PKT_RX_FIELD_3_LENGTH_7_0	  		    0x2A
#define PROP_PKT_RX_FIELD_3_CONFIG			     	0x2B
#define PROP_PKT_RX_FIELD_3_CRC_CONFIG	  		    0x2C
#define PROP_PKT_RX_FIELD_4_LENGTH_12_8	  		    0x2D
#define PROP_PKT_RX_FIELD_4_LENGTH_7_0	  		    0x2E
#define PROP_PKT_RX_FIELD_4_CONFIG		    		0x2F
#define PROP_PKT_RX_FIELD_4_CRC_CONFIG  			0x30
#define PROP_PKT_RX_FIELD_5_LENGTH_12_8	  		    0x31
#define PROP_PKT_RX_FIELD_5_LENGTH_7_0	   		    0x32
#define PROP_PKT_RX_FIELD_5_CONFIG		    		0x33
#define PROP_PKT_RX_FIELD_5_CRC_CONFIG  			0x34

#define PROP_MODEM_GROUP			          		0x20
#define PROP_MODEM_MOD_TYPE		        			0x00
#define PROP_MODEM_MAP_CONTROL	      				0x01
#define PROP_MODEM_DATA_RATE_2	      				0x03
#define PROP_MODEM_DATA_RATE_1	      				0x04
#define PROP_MODEM_DATA_RATE_0	      				0x05
#define PROP_MODEM_FREQ_DEV_2		        		0x0A
#define PROP_MODEM_FREQ_DEV_1		        		0x0B
#define PROP_MODEM_FREQ_DEV_0		        		0x0C
#define PROP_MODEM_FREQ_OFFSET_1      				0x0D
#define PROP_MODEM_FREQ_OFFSET_0      				0x0E
#define PROP_MODEM_TX_RAMP_DELAY      				0x18
#define PROP_MODEM_BCR_OSR                          0x22
#define PROP_MODEM_BCR_NCO_OFFSET                   0x24
#define PROP_MODEM_AFC_GEAR			         		0x2C
#define PROP_MODEM_AFC_WAIT			        		0x2D
#define PROP_MODEM_AFC_GAIN_1		        		0x2E
#define PROP_MODEM_AFC_GAIN_0		        		0x2F
#define PROP_MODEM_AFC_LIMITER_1      				0x30
#define PROP_MODEM_AFC_LIMITER_0      				0x30
#define PROP_MODEM_AFC_MISC                         0x32
#define PROP_MODEM_AFC_ZIFOFF		        		0x33
#define PROP_MODEM_ADC_CTRL                         0x34
#define PROP_MODEM_AGC_CONTROL                      0x35
#define PROP_MODEM_AGC_WINDOW_SIZE                  0x38
#define PROP_MODEM_FSK4_MAP 			        	0x3F
#define PROP_MODEM_OOK_CNT1                         0x42
#define PROP_MODEM_RAW_CONTROL                      0x45

#define PROP_MODEM_ANT_DIV_CONTROL    				0x49
#define PROP_MODEM_RSSI_THRESH		      			0x4A
#define PROP_MODEM_RSSI_JUMP_THRESH			    	0x4B
#define PROP_MODEM_RSSI_CONTROL					    0x4C
#define PROP_MODEM_RSSI_CONTROL2                    0x4D
#define PROP_MODEM_RSSI_COMP					    0x4E
#define PROP_MODEM_RAW_SEARCH2  				    0x50
#define PROP_MODEM_CLKGEN_BAND                      0x51
#define PROP_MODEM_SPIKE_DET                        0x54
#define PROP_MODEM_RSSI_MUTE                        0x57
#define PROP_MODEM_DSA_CTRL1                        0x5B

#define PROP_MODEM_CHFLT_GROUP		          		0x21
#define PROP_MODEM_CHFLT_RX1_CHFLT_COE13_7          0x00
#define PROP_MODEM_CHFLT_RX1_CHFLT_COE1_7           0x0C
#define MODEM_CHFLT_RX2_CHFLT_COE7_7                0x18

#define PROP_PA_GROUP		      	                0x22
#define PROP_PA_MODE			                    0x00
#define PROP_PA_PWR_LVL			                    0x01
#define PROP_PA_BIAS_CLKDUTY		                0x02
#define PROP_PA_TC			                        0x03
#define PROP_PA_RAMP_EX			                    0x04
#define PROP_PA_RAMP_DOWN_DELAY		                0x05


#define			PROP_SYNTH_GROUP                0x23
#define			PROP_SYNTH_PFDCP_CPFF			0x00
#define			PROP_SYNTH_PFDCP_CPINT			0x01
#define			PROP_SYNTH_VCO_KV               0x02
#define			PROP_SYNTH_LPFILT3              0x03
#define			PROP_SYNTH_LPFILT2              0x04
#define			PROP_SYNTH_LPFILT1              0x05
#define			PROP_SYNTH_LPFILT0              0x06
#define			PROP_SYNTH_VCO_KVCAL			0x07


#define PROP_MATCH_GROUP                            0x30
#define PROP_MATCH_VALUE_1                          0x00

#define PROP_FREQ_CONTROL_GROUP                     0x40
#define PROP_FREQ_CONTROL_INTE                      0x00

#define PROP_RX_HOP_GROUP                           0x50
#define PROP_RX_HOP_CONTROL                         0x00

///////////////////////////////////////
////DEFINE SPI-NEW/////////////////////
//SET TRANG THAI CS; CLK///////////////
//truoc khi select cho dung mode///////
//////////////////////////////////////
//hungnd 21/01/2021 chinh sua spi rieng cho rf
#define rf_spi_sclk_dir_output()       spi_sclk_dir_output()
#define rf_spi_sdi_dir_output()        spi_sdi_dir_output()
#define rf_spi_sdo_dir_input()         spi_sdo_dir_input()
#define rf_spi_sdo_pullup_enable()     spi_sdo_pullup_enable()
#define rf_spi_sdo_pulldown()          spi_sdo_pulldown()

#define rf_spi_sdo_input()            spi_sdo_input()
#define rf_spi_sclk_high()            spi_sclk_high()
#define rf_spi_sclk_low()             spi_sclk_low()
#define rf_spi_sdi_high()             spi_sdi_high()
#define rf_spi_sdi_low()              spi_sdi_low()

#define si4464_select()    { spi_disable(); rf_spi_sclk_low(); si4464_cs_output_low();}
#define si4464_de_select()    { rf_spi_sclk_low(); si4464_cs_output_high();}

typedef enum {SI4464_IDLE=0x40,SI4464_READY = 0x30, SI4464_TX = 0x20,SI4464_RX = 0x10} SI4464_STAGE;

extern unsigned char cfg_buffer[8];
//extern unsigned char POWER_VALUE;


void si4464_reset(void);
void si4464_interrupt_enable(void);
extern void si4464_write_byte(unsigned char value);
extern void si4464_send_data_get_resp(unsigned char data_length, unsigned char *data);
extern void si4464_send_data_no_resp(unsigned char data_length, const unsigned char *data);
extern unsigned char si4464_get_resp(unsigned char resp_length, unsigned char *resp_data);
extern void si4464_send_command(unsigned char cmd_length, const unsigned char *cmd_data);
extern void si4464_set_config(uint8_t group,uint8_t length,uint8_t number, uint8_t *cmd_data);
extern void si4464_init(void);
extern void si4464_shutdown(void);
extern void si4464_goto_408_4800(void);
extern void si4464_goto_433_4800(void);
extern void si4464_goto_433_1200(void);
extern void si4464_goto_421_4800(void);
extern void si4464_set_stage(uint8_t stage);
extern void si4464_process_data_receive(void);
extern void si4464_send_buffer(unsigned char *DataPacket, unsigned char LengthPaket,uint8_t write_length);
extern uint8_t si4464_read_buffer(uint8_t *buffer,uint8_t *length,uint8_t read_length, uint8_t *rssi);
extern unsigned char si4464_get_fast_resp_regs(unsigned char StartReg, unsigned char NmbrOfRegs, unsigned char * RegValues);
uint16_t si4464_calCRC(uint8_t crcData, uint16_t crcReg);
unsigned char si4464_get_rx_length(void);
extern uint8_t si4464_get_int_stt(uint8_t value_check) ;
extern unsigned char si4464_get_state(uint8_t stage);
extern void si4464_config_address(uint8_t addr);
extern void si4464_config_broadcast(void);
extern float Si4464_read_temp_vol(void);		
void si4464_write_setting_emec(void);
void si4464_write_setting_star(void);
#endif