#include "../../prjEMETERS/library.h"



#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#endif

#if defined(ADE7858_SUPPORT) || defined(ADE7758_SUPPORT)
#include <stdio.h>
#include "../../prjLIB/prjDELAY/delay.h"
#include "ade7758.h"
#include "ade7758.h"
#include "../../prjLIB/prjCONVERT/macro.h"
//#include "calibration-data.h"
#include "../UART/uart.h"


unsigned char dread1, dread2, dread3;

/******************************************************************************/
unsigned char ade7758_spi_get_byte(void)
{
   unsigned char i;
   unsigned char value = 0;
   //SCLK_H;
   delay_us(10);//nop();//delay_us(10);
   for (i = 0; i < 8; i++)
   {
        // chuyen vi tri doan nay 20120918
       value <<= 1;
       if (ade7758_sdo_input())
       {
          value |= 0x01;
       }
       ade7758_sclk_output_low();
       delay_us(10);//nop();//delay_us(10);
       ade7758_sclk_output_high();
       delay_us(10);//nop();//delay_us(10);
   }
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
void ade7758_spi_send_byte(unsigned char value)
{
   unsigned char i;
   delay_us(10);//nop();
   for (i = 0; i < 8; i++)
   {
      if(value & 0x80)
      {
          ade7758_sdi_output_high();
      }
      else
      {
          ade7758_sdi_output_low();
      }
      ade7758_sclk_output_low();//SCLK_L;
      delay_us(10);//nop();////delay_us(10);
      ade7758_sclk_output_high();//SCLK_H;
      delay_us(10);//nop();////delay_us(10);
      value <<= 1;
   }
  ade7758_sclk_output_low();
  ade7758_sdi_output_low();
  //delay_us(10);//nop();
}
void ade7758_spi_setup(void)
{
    ade7758_sdi_dir_output();
    ade7758_sclk_dir_output();
    ade7758_sdo_dir_input();
}
void write_7758_1byte(unsigned char address, unsigned char dwrite) {
    //disable_spi();
    ade7758_cs_low(); //CS=0;   
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(address | 0x80);
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(dwrite);
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_sdi_output_low(); //DIN=0;
    ade7758_cs_high(); //CS = 1; 
}
/******************************************************************************/

// Write 7758 thanh ghi 2 bytes dwrite1: MSB, dwrite2: LSB

void write_7758_2byte(unsigned char address, unsigned char dwrite1, unsigned char dwrite2) {
    //disable_spi();
    ade7758_cs_low(); //CS=0;
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(address | 0x80);
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(dwrite1);
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(dwrite2);
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_sdi_output_low(); //DIN=0;
    ade7758_cs_high(); //CS=1;
}
/******************************************************************************/
// Write 7758 thanh ghi 3 bytes dwrite1: MSB, dwrite2: LSB, dwrite2: LSB

void write_7758_3byte(unsigned char address, unsigned char dwrite1, unsigned char dwrite2, unsigned char dwrite3) {
    //disable_spi();
    ade7758_cs_low(); //CS=0;
    delay_us(10);
    ade7758_spi_send_byte(address | 0x80);
    delay_us(10);
    ade7758_spi_send_byte(dwrite1);
    delay_us(10);
    ade7758_spi_send_byte(dwrite2);
    delay_us(10);
    ade7758_spi_send_byte(dwrite3);
    delay_us(10);
    ade7758_sdi_output_low(); //DIN=0;
    ade7758_cs_high(); //CS=1;
}

/******************************************************************************/


void read_7758(unsigned int address, unsigned char bytecount) {
    //disable_spi();
    ade7758_cs_low(); //CS = 0;
    delay_us(10); //#asm("nop");//delay_us(10);
    ade7758_spi_send_byte(0x01);//read
    ade7758_spi_send_byte(address>>8);
    ade7758_spi_send_byte(address);
    delay_us(10);
    spi_change_falling_edge_mode(FALSE); //doc du lieu suon len
    switch (bytecount) {
        case 1: // read thanh ghi co 1 byte :  dread3
            dread1 = 0;
            dread2 = 0;
            dread3 = ade7758_spi_get_byte();
            delay_us(10); //#asm("nop");//delay_us(10);
            break;
        case 2: // read thanh ghi co 2 byte :  dread2:dread3
            dread1 = 0;
            dread2 = ade7758_spi_get_byte();
            delay_us(10);
            dread3 = ade7758_spi_get_byte();
            delay_us(10); //#asm("nop");//delay_us(10);
            break;
        case 3: // read thanh ghi co 3 byte :  dread1:dread2:dread3
            dread1 = ade7758_spi_get_byte();
            delay_us(10); //#asm("nop");//delay_us(10);
            dread2 = ade7758_spi_get_byte();
            delay_us(10); //#asm("nop");//delay_us(10);
            dread3 = ade7758_spi_get_byte();
            delay_us(10); //#asm("nop");//delay_us(10);
            break;
    }
    ade7758_cs_high(); //CS=1;
    ade7758_sdi_output_low(); //DIN = 0;
    ade7758_sclk_output_high();
    spi_change_falling_edge_mode(TRUE);
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : ade7758_config
//  Created     : 26/02/2015, by thaodtn
//  Description : cau hinh cac thanh ghi hoat dong va do dem
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void ade7758_config(void) {
    unsigned char value = 0;
    write_7758_1byte(OPMODE, 0x00); //enable pulse
    delay_us(2);
    write_7758_1byte(GAIN, 0x00);
    delay_us(2);
    write_7758_1byte(LCYCMODE, 0xBF); //;0x78);//0xBF);
    delay_us(2);
    write_7758_2byte(LINECYC, 0x03, 0x00);
    delay_us(2);
    //thong so cua xung WATT
    write_7758_2byte(APCFNUM, 0x00, 0x00);
    delay_us(2);
    write_7758_2byte(APCFDEN, 0x02, 0x37);
    delay_us(2);
    //thong so cua xung VAR
    write_7758_2byte(VARCFNUM, 0x00, 0x00);
    delay_us(2);
    write_7758_2byte(VARCFDEN, 0x02, 0x37);
    delay_us(2);

#ifdef CALIBRATION_PHASE_A       
    write_7758_1byte(LCYCMODE, 0xB8); //0x3F);//0x0F);
    delay_us(2);
    value = TERMSEL_A;
    write_7758_1byte(COMPMODE, value);
    delay_us(2);
    value = FREQSEL_A;
    write_7758_1byte(MMODE, value);
    delay_us(2);
    //write_7758_2byte(WATT_GAIN_A_ADDR,0x00,0);    
    write_7758_2byte(WATT_GAIN_A_ADDR, DEFAULT_WATT_GAIN_A_BYTE_2, DEFAULT_WATT_GAIN_A_BYTE_1);
    delay_us(2);
    //write_7758_2byte(VAR_GAIN_A_ADDR,0x00,0);      
    write_7758_2byte(VAR_GAIN_A_ADDR, DEFAULT_VAR_GAIN_A_BYTE_2, DEFAULT_VAR_GAIN_A_BYTE_1);
    delay_us(2);
    //write_7758_1byte(PHASE_CAL_A_ADDR,0);     
    write_7758_1byte(PHASE_CAL_A_ADDR, DEFAULT_PHASE_CAL_A_BYTE_1);
    delay_us(2);
    //write_7758_2byte(WATT_OFFSET_A_ADDR,0x00,0);      
    write_7758_2byte(WATT_OFFSET_A_ADDR, DEFAULT_WATT_OFFSET_A_BYTE_2, DEFAULT_WATT_OFFSET_A_BYTE_1);
    delay_us(2);
    //write_7758_2byte(VAR_OFFSET_A_ADDR,0x0,0);    
    write_7758_2byte(VAR_OFFSET_A_ADDR, DEFAULT_VAR_OFFSET_A_BYTE_2, DEFAULT_VAR_OFFSET_A_BYTE_1);
    delay_us(2);
#else
    write_7758_2byte(WATT_GAIN_A_ADDR, DEFAULT_WATT_GAIN_A_BYTE_2, DEFAULT_WATT_GAIN_A_BYTE_1);
    delay_us(2);
    write_7758_2byte(VAR_GAIN_A_ADDR, DEFAULT_VAR_GAIN_A_BYTE_2, DEFAULT_VAR_GAIN_A_BYTE_1);
    delay_us(2);
    write_7758_1byte(PHASE_CAL_A_ADDR, DEFAULT_PHASE_CAL_A_BYTE_1);
    delay_us(2);
    write_7758_2byte(WATT_OFFSET_A_ADDR, DEFAULT_WATT_OFFSET_A_BYTE_2, DEFAULT_WATT_OFFSET_A_BYTE_1);
    delay_us(2);
    write_7758_2byte(VAR_OFFSET_A_ADDR, DEFAULT_VAR_OFFSET_A_BYTE_2, DEFAULT_VAR_OFFSET_A_BYTE_1);
    delay_us(2);
#endif    

#ifdef CALIBRATION_PHASE_B       
    write_7758_1byte(LCYCMODE, 0xB8); //0x3F);//0x0F);
    delay_us(2);
    value = TERMSEL_B;
    write_7758_1byte(COMPMODE, value);
    delay_us(2);
    value = FREQSEL_B;
    write_7758_1byte(MMODE, value);
    delay_us(2);
    write_7758_2byte(WATT_GAIN_B_ADDR, 0, 0); //0x061C     
    //write_7758_2byte(WATT_GAIN_B_ADDR,DEFAULT_WATT_GAIN_B_BYTE_2,DEFAULT_WATT_GAIN_B_BYTE_1); //0x061C    
    delay_us(2);
    write_7758_2byte(VAR_GAIN_B_ADDR, 0x00, 0);
    //write_7758_2byte(VAR_GAIN_B_ADDR,DEFAULT_VAR_GAIN_B_BYTE_2,DEFAULT_VAR_GAIN_B_BYTE_1);
    delay_us(2);
    write_7758_1byte(PHASE_CAL_B_ADDR, 0);
    //write_7758_1byte(PHASE_CAL_B_ADDR,DEFAULT_PHASE_CAL_B_BYTE_1); 
    delay_us(2);
    write_7758_2byte(WATT_OFFSET_B_ADDR, 0, 0);
    //write_7758_2byte(WATT_OFFSET_B_ADDR,DEFAULT_WATT_OFFSET_B_BYTE_2,DEFAULT_WATT_OFFSET_B_BYTE_1);   
    delay_us(2);
    write_7758_2byte(VAR_OFFSET_B_ADDR, 0, 0);
    //write_7758_2byte(VAR_OFFSET_B_ADDR,DEFAULT_VAR_OFFSET_B_BYTE_2,DEFAULT_VAR_OFFSET_B_BYTE_1);
    delay_us(2);
#else
    write_7758_2byte(WATT_GAIN_B_ADDR, DEFAULT_WATT_GAIN_B_BYTE_2, DEFAULT_WATT_GAIN_B_BYTE_1); //0x061C     
    delay_us(2);
    write_7758_2byte(VAR_GAIN_B_ADDR, DEFAULT_VAR_GAIN_B_BYTE_2, DEFAULT_VAR_GAIN_B_BYTE_1);
    delay_us(2);
    write_7758_1byte(PHASE_CAL_B_ADDR, DEFAULT_PHASE_CAL_B_BYTE_1);
    delay_us(2);
    write_7758_2byte(WATT_OFFSET_B_ADDR, DEFAULT_WATT_OFFSET_B_BYTE_2, DEFAULT_WATT_OFFSET_B_BYTE_1);
    delay_us(2);
    write_7758_2byte(VAR_OFFSET_B_ADDR, DEFAULT_VAR_OFFSET_B_BYTE_2, DEFAULT_VAR_OFFSET_B_BYTE_1);
    delay_us(2);
#endif 

#ifdef CALIBRATION_PHASE_C       
    write_7758_1byte(LCYCMODE, 0xB8); //0x3F);//0x0F);
    delay_us(2);
    value = TERMSEL_C;
    write_7758_1byte(COMPMODE, value);
    delay_us(2);
    value = FREQSEL_C;
    write_7758_1byte(MMODE, value);
    delay_us(2);
    //write_7758_2byte(WATT_GAIN_C_ADDR,0x00,0); //72     
    write_7758_2byte(WATT_GAIN_C_ADDR, DEFAULT_WATT_GAIN_C_BYTE_2, DEFAULT_WATT_GAIN_C_BYTE_1); //72 
    delay_us(2);
    //write_7758_2byte(VAR_GAIN_C_ADDR,0x00,0);      
    write_7758_2byte(VAR_GAIN_C_ADDR, DEFAULT_VAR_GAIN_C_BYTE_2, DEFAULT_VAR_GAIN_C_BYTE_1);
    delay_us(2);
    //write_7758_1byte(PHASE_CAL_C_ADDR,0); //0xFD    
    write_7758_1byte(PHASE_CAL_C_ADDR, DEFAULT_PHASE_CAL_C_BYTE_1); //20  
    delay_us(2);
    //write_7758_2byte(WATT_OFFSET_C_ADDR,0,0);      
    write_7758_2byte(WATT_OFFSET_C_ADDR, DEFAULT_WATT_OFFSET_C_BYTE_2, DEFAULT_WATT_OFFSET_C_BYTE_1);
    delay_us(2);
    //write_7758_2byte(VAR_OFFSET_C_ADDR,0,0);      
    write_7758_2byte(VAR_OFFSET_C_ADDR, DEFAULT_VAR_OFFSET_C_BYTE_2, DEFAULT_VAR_OFFSET_C_BYTE_1);
    delay_us(2);
#else
    write_7758_2byte(WATT_GAIN_C_ADDR, DEFAULT_WATT_GAIN_C_BYTE_2, DEFAULT_WATT_GAIN_C_BYTE_1); //72     
    delay_us(2);
    write_7758_2byte(VAR_GAIN_C_ADDR, DEFAULT_VAR_GAIN_C_BYTE_2, DEFAULT_VAR_GAIN_C_BYTE_1);
    delay_us(2);
    write_7758_1byte(PHASE_CAL_C_ADDR, DEFAULT_PHASE_CAL_C_BYTE_1); //20    
    delay_us(2);
    write_7758_2byte(WATT_OFFSET_C_ADDR, DEFAULT_WATT_OFFSET_C_BYTE_2, DEFAULT_WATT_OFFSET_C_BYTE_1);
    delay_us(2);
    write_7758_2byte(VAR_OFFSET_C_ADDR, DEFAULT_VAR_OFFSET_C_BYTE_2, DEFAULT_VAR_OFFSET_C_BYTE_1);
    delay_us(2);
#endif 


#ifdef CABLIBRATION
    write_7758_2byte(AVRMSOS, 0, 0);
    delay_us(2);
    write_7758_2byte(BVRMSOS, 0, 0);
    delay_us(2);
    write_7758_2byte(CVRMSOS, 0, 0);
    delay_us(2);
    write_7758_2byte(AIRMSOS, 0, 0);
    delay_us(2);
    write_7758_2byte(BIRMSOS, 0, 0);
    delay_us(2);
    write_7758_2byte(CIRMSOS, 0, 0);
    delay_us(2);
#else    
    write_7758_2byte(AVRMSOS, DEFAULT_AVRMSOS_BYTE_2, DEFAULT_AVRMSOS_BYTE_1);
    delay_us(2);
    write_7758_2byte(BVRMSOS, DEFAULT_BVRMSOS_BYTE_2, DEFAULT_BVRMSOS_BYTE_1);
    delay_us(2);
    write_7758_2byte(CVRMSOS, DEFAULT_CVRMSOS_BYTE_2, DEFAULT_CVRMSOS_BYTE_1);
    delay_us(2);
    write_7758_2byte(AIRMSOS, DEFAULT_AIRMSOS_BYTE_2, DEFAULT_AIRMSOS_BYTE_1);
    delay_us(2);
    write_7758_2byte(BIRMSOS, DEFAULT_BIRMSOS_BYTE_2, DEFAULT_BIRMSOS_BYTE_1);
    delay_us(2);
    write_7758_2byte(CIRMSOS, DEFAULT_CIRMSOS_BYTE_2, DEFAULT_CIRMSOS_BYTE_1);
    delay_us(2);
#endif     

    //0x08
    //write_7758_3byte(MASK,0x0F,0xFF,0xFF);     
    write_7758_3byte(MASK, 0x00, 0x10, 0x00);
    delay_us(2);
    read_7758(RSTATUS, 3);
}


////////////////////////////////////////////////////////////////////////////////
//  Function    : void ade7758_calibration(char ph) 
//  Created     : 26/02/2015, by thaodtn
//  Description : can chinh tung pha
//  Input		: ch: phase duoc can chinh
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void ade7758_calibration(unsigned char ph) {
    //step 1: enable pulse output
    write_7758_1byte(OPMODE, 0x00);
    delay_us(2);
    //step 3: select phase to calibrate, disable other phases
    switch (ph) {
        case 1: //pha A
            write_7758_1byte(COMPMODE, 0x04);
            delay_us(2);
            write_7758_1byte(MMODE, 0x24);
            delay_us(2);
            break;
        case 2: //pha B
            write_7758_1byte(COMPMODE, 0x08);
            delay_us(2);
            write_7758_1byte(MMODE, 0x49);
            delay_us(2);
            break;
        case 3: //pha C
            write_7758_1byte(COMPMODE, 0x10);
            delay_us(2);
            write_7758_1byte(MMODE, 0x92);
            delay_us(2);
            break;
    }
}

/******************************************************************************/
void ade7758_init(void) {
    unsigned char i1;
    ade7758_spi_setup();
    ade7758_cs_dir_output();
    ade7758_cs_high();
    ade7758_sdi_output_low(); //DIN = 0;
    ade7758_sclk_output_low(); //SCLK=0;
    ade7758_cs_high(); //CS=1;
    i1 = ade7758_sdo_input();
    ade7758_config();
}

////////////////////////////////////////////////////////////////////////////////
//  Function    : 
//  Created     : //, by thaodtn
//  Description : 
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////

void measure_task(void) {
    if (emeter.flags.active_pulse == TRUE) {
        emeter.flags.active_pulse = FALSE;
        check_reverse_active_power();
        //if (emeter.active.power >= 0) //Watt thuan
        if (emeter.flags.reverse_active_power == FALSE) {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('+');
            uart_transmit_byte('A');
#endif
            emeter.active.forward.pulse_count++;
            if (emeter.active.forward.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.active.forward.pulse_count = 0;
                emeter.active.forward.value++;
                if (emeter.active.forward.value > KWH_WRAP) {
                    emeter.active.forward.value = 0;
                }
                //luu eeprom
                if (emeter.active.forward.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        } else // Watt nguoc
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('-');
            uart_transmit_byte('A');
#endif
            emeter.active.reverse.pulse_count++;
            if (emeter.active.reverse.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.active.reverse.pulse_count = 0;
                emeter.active.reverse.value++;
                if (emeter.active.reverse.value > KWH_WRAP) {
                    emeter.active.reverse.value = 0;
                }
                //luu eeprom
                if (emeter.active.reverse.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        }

    }
    if (emeter.flags.reactive_pulse == TRUE) {
        emeter.flags.reactive_pulse = FALSE;
        check_reverse_reactive_power();
        //if (emeter.reactive.power >= 0) // var thuan
        if (emeter.flags.reverse_reactive_power == FALSE) // Q > 0
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('+');
            uart_transmit_byte('R');
#endif
            emeter.reactive.forward.pulse_count++;
            if (emeter.reactive.forward.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.reactive.forward.pulse_count = 0;
                emeter.reactive.forward.value++;
                if (emeter.reactive.forward.value > KWH_WRAP) {
                    emeter.reactive.forward.value = 0;
                }
                if (emeter.flags.reverse_active_power == FALSE) // P > 0
                {
                    emeter.reactive.q1++;
                    if (emeter.reactive.q1 > KWH_WRAP) {
                        emeter.reactive.q1 = 0;
                    }
                } else // P < 0
                {
                    emeter.reactive.q2++;
                    if (emeter.reactive.q2 > KWH_WRAP) {
                        emeter.reactive.q2 = 0;
                    }
                }
                //luu eeprom
                if (emeter.reactive.forward.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }

        } else //var nguoc
        {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
            uart_transmit_byte('-');
            uart_transmit_byte('R');
#endif
            emeter.reactive.reverse.pulse_count++;
            if (emeter.reactive.reverse.pulse_count >= TOTAL_ENERGY_PULSES_PER_KW_HOUR) {
                emeter.reactive.reverse.pulse_count = 0;
                emeter.reactive.reverse.value++;
                if (emeter.reactive.reverse.value > KWH_WRAP) {
                    emeter.reactive.reverse.value = 0;
                }
                if (emeter.flags.reverse_active_power == FALSE) // P > 0
                {
                    emeter.reactive.q4++;
                    if (emeter.reactive.q4 > KWH_WRAP) {
                        emeter.reactive.q4 = 0;
                    }
                } else // P < 0
                {
                    emeter.reactive.q3++;
                    if (emeter.reactive.q3 > KWH_WRAP) {
                        emeter.reactive.q3 = 0;
                    }
                }
                //luu eeprom
                if (emeter.reactive.reverse.value % SAVE_DATA_THRESHOLD == 0) // enough 0.5 kwh
                {
                    emeter.flags.save_data = TRUE;
                    //Meter_state = Flag_save_data;
                }
            }
        }
    }
    //if (flags.vrms == 1) //doc dien ap
    //{
    //flags.vrms = 0;
    //measure_vrms();   
    //flags.display = 1; 
    //}
    //if (flags.irms == 1) //doc dien ap
    //{
    //flags.irms = 0;
    //measure_irms();   
    //flags.display = 1; 
    //} 
    //if (flags.freq == 1) //doc dien ap
    //{
    //flags.freq = 0;
    //measure_freq();   
    //flags.display = 1; 
    //} 
    //if (flags.temp == 1) //doc nhiet do
    //{
    //flags.temp = 0;
    //measure_temperature();
    //flags.display = 1;
    //}
    //if (flags.active_power == 1)
    //{
    //flags.active_power = 0;
    //measure_activepower();
    //flags.display = 1;
    //}

}

//****************************************************************

void measure_vrms(void) {
    read_7758(CVRMS, 3);
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Voltage A: \t"); //thaodtn
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    //measure.vrmsA = (long)((float)((unsigned long)dread1 * 65536 + (unsigned long)dread2*256 + dread3)*V_SCALE/1000);///4.065);
    //ok ade7758.vrmsA = ((unsigned long)dread1 * 0x10000) + ((unsigned long)dread2 * 0x100) + ((unsigned long)dread3);    
    //ok
    /*ade7758.vrmsA = (unsigned long)dread1 * 0x10000;
    ade7758.vrmsA += (unsigned long)dread2 * 0x100;
    ade7758.vrmsA +=  (unsigned long)dread3;  */
    //ade7758.vrmsA = (unsigned long)dread1 << 16 + (unsigned long)dread2 <<8 + (unsigned long)dread3;
    /*putc(*((unsigned char *)&measure.vrmsA + 3));
    putc(*((unsigned char *)&measure.vrmsA + 2));
    putc(*((unsigned char *)&measure.vrmsA + 1));
    putc(*((unsigned char *)&measure.vrmsA + 0));
    putc('\n');*/
    //cach nay dung    
    *((unsigned char *) &ade7758.vrmsA + 3) = 0;
    *((unsigned char *) &ade7758.vrmsA + 2) = dread1;
    *((unsigned char *) &ade7758.vrmsA + 1) = dread2;
    *((unsigned char *) &ade7758.vrmsA + 0) = dread3;

    read_7758(BVRMS, 3);
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Voltage B: \t"); //thaodtn
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    //measure.vrmsB = (long)((float)((unsigned long)dread1 * 65536 + (unsigned long)dread2*256 + dread3)*V_SCALE/1000);///4.065);
    //measure.vrmsB = (unsigned long)dread1 * 0x10000 + (unsigned long)dread2 * 0x100 + (unsigned long)dread3;
    *((unsigned char *) &ade7758.vrmsB + 3) = 0;
    *((unsigned char *) &ade7758.vrmsB + 2) = dread1;
    *((unsigned char *) &ade7758.vrmsB + 1) = dread2;
    *((unsigned char *) &ade7758.vrmsB + 0) = dread3;

    read_7758(AVRMS, 3);
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Voltage C: \t"); //thaodtn
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    //measure.vrmsC = (long)((float)((unsigned long)dread1 * 65536 + (unsigned long)dread2*256 + dread3)*V_SCALE/1000);///4.065);
    //measure.vrmsC = (unsigned long)dread1 * 0x10000 + (unsigned long)dread2 * 0x100 + (unsigned long)dread3; 
    *((unsigned char *) &ade7758.vrmsC + 3) = 0;
    *((unsigned char *) &ade7758.vrmsC + 2) = dread1;
    *((unsigned char *) &ade7758.vrmsC + 1) = dread2;
    *((unsigned char *) &ade7758.vrmsC + 0) = dread3;
    /*
        read_7758(AVRMS,3);
        measure.vrmsA = (long)((float)(dread2*256+dread3)/1.205);
        read_7758(BVRMS,3);
        measure.vrmsB = (long)((float)(dread2*256+dread3)/1.205);
        read_7758(CVRMS,3);
        measure.vrmsC = (long)((float)(dread2*256+dread3)/1.205); 
     */
}

void measure_irms(void) {
    read_7758(CIRMS, 3);
    //measure.irmsA = (long)((float)((unsigned long)dread1*65536+(unsigned long)dread2*256+dread3)*I_SCALE/1000);    
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Current A: \t");
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    *((unsigned char *) &ade7758.irmsA + 3) = 0;
    *((unsigned char *) &ade7758.irmsA + 2) = dread1;
    *((unsigned char *) &ade7758.irmsA + 1) = dread2;
    *((unsigned char *) &ade7758.irmsA + 0) = dread3;

    read_7758(BIRMS, 3);
    //ade7758.irmsB = (long)((float)((unsigned long)dread1*65536+(unsigned long)dread2*256+dread3)*I_SCALE/1000);
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Current B: \t");
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    *((unsigned char *) &ade7758.irmsB + 3) = 0;
    *((unsigned char *) &ade7758.irmsB + 2) = dread1;
    *((unsigned char *) &ade7758.irmsB + 1) = dread2;
    *((unsigned char *) &ade7758.irmsB + 0) = dread3;

    read_7758(AIRMS, 3);
    //measure.irmsC = (long)((float)((unsigned long)dread1*65536+(unsigned long)dread2*256+dread3)*I_SCALE/1000);    
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    fprintf(STREAM_UART_2, "Current C: \t");
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    *((unsigned char *) &ade7758.irmsC + 3) = 0;
    *((unsigned char *) &ade7758.irmsC + 2) = dread1;
    *((unsigned char *) &ade7758.irmsC + 1) = dread2;
    *((unsigned char *) &ade7758.irmsC + 0) = dread3;
    /*
        read_7758(AIRMS,3);
        ade7758.irmsA = (long)((float)(dread2*256+dread3)/9.06);
        read_7758(BIRMS,3);
        ade7758.irmsB = (long)((float)(dread2*256+dread3)/9.06);
        read_7758(CIRMS,3);
        ade7758.irmsC = (long)((float)(dread2*256+dread3)/9.06);  
     */
}

void measure_activepower(void) {
    read_7758(CWATTHR, 2);
    //ade7758.active_powerA = (dread2 * 256 + dread3); 
    *((unsigned char *) &ade7758.active_powerA + 3) = 0;
    *((unsigned char *) &ade7758.active_powerA + 2) = 0;
    *((unsigned char *) &ade7758.active_powerA + 1) = dread2;
    *((unsigned char *) &ade7758.active_powerA + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Active Power A: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(BWATTHR, 2);
    //ade7758.active_powerB = (dread2 * 256 + dread3);
    *((unsigned char *) &ade7758.active_powerB + 3) = 0;
    *((unsigned char *) &ade7758.active_powerB + 2) = 0;
    *((unsigned char *) &ade7758.active_powerB + 1) = dread2;
    *((unsigned char *) &ade7758.active_powerB + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power B: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(AWATTHR, 2);
    //ade7758.active_powerC = (dread2 * 256 + dread3);   
    *((unsigned char *) &ade7758.active_powerC + 3) = 0;
    *((unsigned char *) &ade7758.active_powerC + 2) = 0;
    *((unsigned char *) &ade7758.active_powerC + 1) = dread2;
    *((unsigned char *) &ade7758.active_powerC + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power C: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif   
}

void measure_reactivepower(void) {
    read_7758(CVARHR, 2);
    //ade7758.active_powerA = (dread2 * 256 + dread3); 
    *((unsigned char *) &ade7758.reactive_powerA + 3) = 0;
    *((unsigned char *) &ade7758.reactive_powerA + 2) = 0;
    *((unsigned char *) &ade7758.reactive_powerA + 1) = dread2;
    *((unsigned char *) &ade7758.reactive_powerA + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power A: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(BVARHR, 2);
    //ade7758.active_powerB = (dread2 * 256 + dread3);
    *((unsigned char *) &ade7758.reactive_powerB + 3) = 0;
    *((unsigned char *) &ade7758.reactive_powerB + 2) = 0;
    *((unsigned char *) &ade7758.reactive_powerB + 1) = dread2;
    *((unsigned char *) &ade7758.reactive_powerB + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power B: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(AVARHR, 2);
    //ade7758.active_powerC = (dread2 * 256 + dread3);   
    *((unsigned char *) &ade7758.reactive_powerC + 3) = 0;
    *((unsigned char *) &ade7758.reactive_powerC + 2) = 0;
    *((unsigned char *) &ade7758.reactive_powerC + 1) = dread2;
    *((unsigned char *) &ade7758.reactive_powerC + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power C: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif   
}

void measure_apparentpower(void) {
    read_7758(CVAHR, 2);
    //ade7758.active_powerA = (dread2 * 256 + dread3); 
    *((unsigned char *) &ade7758.apparent_powerA + 3) = 0;
    *((unsigned char *) &ade7758.apparent_powerA + 2) = 0;
    *((unsigned char *) &ade7758.apparent_powerA + 1) = dread2;
    *((unsigned char *) &ade7758.apparent_powerA + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Apparent Power A: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(BVAHR, 2);
    //ade7758.active_powerB = (dread2 * 256 + dread3);
    *((unsigned char *) &ade7758.apparent_powerB + 3) = 0;
    *((unsigned char *) &ade7758.apparent_powerB + 2) = 0;
    *((unsigned char *) &ade7758.apparent_powerB + 1) = dread2;
    *((unsigned char *) &ade7758.apparent_powerB + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power B: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif
    read_7758(AVAHR, 2);
    //ade7758.active_powerC = (dread2 * 256 + dread3);    
    *((unsigned char *) &ade7758.apparent_powerC + 3) = 0;
    *((unsigned char *) &ade7758.apparent_powerC + 2) = 0;
    *((unsigned char *) &ade7758.apparent_powerC + 1) = dread2;
    *((unsigned char *) &ade7758.apparent_powerC + 0) = dread3;
#if defined(DEBUG_MAIN) && defined(DEBUG_MEASURE) && defined(DEBUG_DISPLAY)
    //fprintf(STREAM_UART_2,"Power C: \t");
    //fprintf(STREAM_UART_2,"%02X",dread1);
    //fprintf(STREAM_UART_2,"%02X",dread2);
    //fprintf(STREAM_UART_2,"%02X",dread3);    
    //putc('\n');
#endif   
}

void measure_freq(void) {
    read_7758(FREQ, 2);
    ade7758.freq = (dread2 * 256 + dread3) / 16;
}

void measure_line_cycles(void) {
    read_7758(FREQ, 2);
    *((unsigned char *) &ade7758.line_cycles + 3) = 0;
    *((unsigned char *) &ade7758.line_cycles + 2) = 0;
    *((unsigned char *) &ade7758.line_cycles + 1) = dread2;
    *((unsigned char *) &ade7758.line_cycles + 0) = dread3;
}

void measure_temperature(void) {
    read_7758(TEMP, 1);
    ade7758.temperature = (dread3 - 218) * 3 + 25; //datasheet p.28     
}

void check_reverse_active_power(void) {
#if defined(USE_APCFNUM_DETECT_REVERSE_POWER)
    long power_total = 0;
    long power_A = 0;
    long power_B = 0;
    long power_C = 0;
    //doc thanh ghi APCFNUM
    read_7758(APCFNUM, 2);
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
    fprintf(STREAM_UART_2, "APCFNUM: \t"); //thaodtn
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    //kiem tra bit reverse cua moi pha (thu tu A-B-C tren PCB nguoc voi A-B-C cua ADE)
    //nen bit 15, 14, 13 phai dao nguoc lai
    //bit 13 - phase A
    if (dread2 & 0x20) {
        emeter.flags.reverse_active_phaseA = TRUE;
        power_A = -emeter.measure.active_powerA; //chuyen sang so am
    } else {
        emeter.flags.reverse_active_phaseA = FALSE;
        power_A = emeter.measure.active_powerA;
    }
    //bit 14 - phase B
    if (dread2 & 0x40) {
        emeter.flags.reverse_active_phaseB = TRUE;
        power_B = -emeter.measure.active_powerB; //chuyen sang so am
    } else {
        emeter.flags.reverse_active_phaseB = FALSE;
        power_B = emeter.measure.active_powerB;
    }
    //bit 15 - phase C
    if (dread2 & 0x80) {
        emeter.flags.reverse_active_phaseC = TRUE;
        power_C = -emeter.measure.active_powerC; //chuyen sang so am
    } else {
        emeter.flags.reverse_active_phaseC = FALSE;
        power_C = emeter.measure.active_powerC;
    }
    //quyet dinh nguoc pha hay ko?
    power_total = power_A + power_B + power_C;
    if (power_total < 0) {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
        uart_transmit_byte('-');
        uart_transmit_byte('P');
#endif		
        emeter.flags.reverse_active_power = TRUE;
    } else {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
        uart_transmit_byte('+');
        uart_transmit_byte('P');
#endif
        emeter.flags.reverse_active_power = FALSE;
    }
    //if ((emeter.flags.reverse_phaseA == TRUE)||(emeter.flags.reverse_phaseB == TRUE)||(emeter.flags.reverse_phaseC == TRUE))
    //{
    //#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
    //putc('-');
    //putc('P');
    //#endif		
    //emeter.flags.reverse_active_power = TRUE;
    //}
    //else
    //{
    //#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
    //putc('+');
    //putc('P');
    //#endif
    //emeter.flags.reverse_active_power = FALSE;
    //}
#else
    //cach nay doc thanh ghi nang luong
    long total_power = 0;
    measure_activepower();
    measure_apparentpower();
    total_power = (long) ade7758.active_powerA + (long) ade7758.active_powerB + (long) ade7758.active_powerC;
    if (total_power < 0) emeter.flags.reverse_active_power = TRUE;
    else emeter.flags.reverse_active_power = FALSE;
    if (ade7758.active_powerA >= 0) emeter.flags.reverse_phaseA = FALSE;
    else emeter.flags.reverse_phaseA = TRUE;
    if (ade7758.active_powerB >= 0) emeter.flags.reverse_phaseB = FALSE;
    else emeter.flags.reverse_phaseB = TRUE;
    if (ade7758.active_powerC >= 0) emeter.flags.reverse_phaseC = FALSE;
    else emeter.flags.reverse_phaseC = TRUE;
#endif
}

void check_reverse_reactive_power(void) {
#if defined(USE_APCFNUM_DETECT_REVERSE_POWER)
    long power_total = 0;
    long power_A = 0;
    long power_B = 0;
    long power_C = 0;
    //doc thanh ghi VARCFNUM
    read_7758(VARCFNUM, 2);
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
    fprintf(STREAM_UART_2, "VARCFNUM: \t"); //thaodtn
    fprintf(STREAM_UART_2, "%02X", dread1);
    fprintf(STREAM_UART_2, "%02X", dread2);
    fprintf(STREAM_UART_2, "%02X", dread3);
    uart_transmit_byte('\n');
#endif
    //kiem tra bit reverse cua moi pha (thu tu A-B-C tren PCB nguoc voi A-B-C cua ADE)
    //nen bit 15, 14, 13 phai dao nguoc lai
    //bit 13 - phase A
    if (dread2 & 0x20) {
        emeter.flags.reverse_reactive_phaseA = TRUE;
        power_A = -emeter.measure.reactive_powerA; //chuyen sang so am
    } else {
        emeter.flags.reverse_reactive_phaseA = FALSE;
        power_A = emeter.measure.reactive_powerA;
    }
    //bit 14 - phase B
    if (dread2 & 0x40) {
        emeter.flags.reverse_reactive_phaseB = TRUE;
        power_B = -emeter.measure.reactive_powerB; //chuyen sang so am
    } else {
        emeter.flags.reverse_reactive_phaseB = FALSE;
        power_B = emeter.measure.reactive_powerB;
    }
    //bit 15 - phase C
    if (dread2 & 0x80) {
        emeter.flags.reverse_reactive_phaseC = TRUE;
        power_C = -emeter.measure.reactive_powerC; //chuyen sang so am
    } else {
        emeter.flags.reverse_reactive_phaseC = FALSE;
        power_C = emeter.measure.reactive_powerC;
    }
    //quyet dinh nguoc pha hay ko?
    power_total = power_A + power_B + power_C;
    if (power_total < 0) {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
        uart_transmit_byte('-');
        uart_transmit_byte('Q');
#endif		
        emeter.flags.reverse_reactive_power = TRUE;
    } else {
#if defined(DEBUG_MAIN) && defined(DEBUG_PULSE)
        uart_transmit_byte('+');
        uart_transmit_byte('Q');
#endif
        emeter.flags.reverse_reactive_power = FALSE;
    }
#else	
    long total_power = 0;
    measure_reactivepower();
    total_power = (long) ade7758.reactive_powerA + (long) ade7758.reactive_powerB + (long) ade7758.reactive_powerC;
    if (total_power >= 0)
        emeter.flags.reverse_reactive_power = FALSE;
    else
        emeter.flags.reverse_reactive_power = TRUE;
#endif		
}
#endif


