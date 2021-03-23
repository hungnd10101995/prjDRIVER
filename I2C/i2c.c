/*===========================================================================*/
/* I N C L U D E S */
/*===========================================================================*/
#if defined(__XC8)
    #include <xc.h>
#endif
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef __MSP430__
#include <io.h>
#ifndef _IHD_
#include <emeter-toolkit.h>
#endif
#else
#endif

#include "../devices.h"


#ifdef DT03M_RF
    #include "..\..\dt03m-rf\fw\emeter\emeter-main.h"
    #include "..\..\dt03m-rf\fw\emeter\emeter-structs.h"
#endif
#ifdef DT01M_RF
    #include "..\..\dt01m-rf\fw\emeter\emeter-main.h"
    //#include "..\..\dt01m-rf\fw\emeter\emeter-structs.h"
#endif

#if defined(_DT01P_RF_) && defined(__MSP430__)
    #include "..\..\dt01p80-rf\fw\emeter-main.h"
    #include "..\..\dt01p80-rf\fw\emeter-hardware.h"
#endif
#if defined(_DT01P_RF_) && defined(_18F86J94) 
    #include "..\..\dt01p80-rf\fw\main.h"
    #include "..\..\dt01p80-rf\fw\hardware.h"
#endif

#if defined(_DT01P_RF_) && defined(_PIC16XX1919Y_) //namhv 01/11/2019
    #include "../../dt01p-rf/fw/emeter-main.h"
    #include "../../dt01p-rf/fw/emeter-hardware.h"
#endif

#ifdef _DT01PP_RF_
    #include "..\..\prepaid-meter\fw\emeter\emeter-main.h"
    #include "..\..\prepaid-meter\fw\emeter\emeter-hardware.h"
#endif
#ifdef _IHD_
    #include "..\..\ihd\fw\ihd-main.h"
    #include "..\..\ihd\fw\ihd-hardware.h"
#endif
#include "i2c.h"
#include "..\framework\delay.h"
//------------------------------------------------------------------------------
// 	Routine:	i2c_init
//	Inputs:		none
//	Outputs:	none
//	Purpose:	Initialize I2C for the ADu812C
//------------------------------------------------------------------------------
void i2c_init(void)
{											// To initialize we need the output
    i2c_sda_dir_output();										// data high, the clock high, and											// the I2C system in Master mode
    i2c_scl_dir_output();
	i2c_sda_output_high();								// Set the data bit to HIGH
    i2c_scl_output_high();
}

//------------------------------------------------------------------------------
// I2C Functions - Bit Banged
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 	Routine:	i2c_start
//	Inputs:		none
//	Outputs:	none
//	Purpose:	Sends I2C Start Trasfer - State "B"
//------------------------------------------------------------------------------
void i2c_start(void)
{
    i2c_scl_dir_output();
    i2c_sda_dir_output();
//    delay_us(I2C_TBUF); 
//    i2c_sda_output_high();
//    delay_us(I2C_TSU_DAT);
//    i2c_scl_output_high(); 
//    delay_us(I2C_TSU_STA);
//    i2c_sda_output_low();
//    delay_us(I2C_THD_STA);
//    i2c_scl_output_low(); 
    
    i2c_sda_output_high();
    i2c_scl_output_high();
    delay_us(2);
    i2c_sda_output_low();
    i2c_scl_output_low();
}
//------------------------------------------------------------------------------
// 	Routine:	i2c_stop
//	Inputs:		none
//	Outputs:	none
//	Purpose:	Sends I2C Stop Trasfer - State "C"
//------------------------------------------------------------------------------
void i2c_stop(void)
{
    i2c_scl_dir_output();
    i2c_sda_dir_output();
    
    i2c_scl_output_low();
    i2c_sda_output_low();
    i2c_scl_output_high();
    delay_us(2);
    i2c_sda_output_high();
}

//------------------------------------------------------------------------------
// 	Routine:	i2c_reset
//	Inputs:		none
//	Outputs:	none
//	Purpose:
//------------------------------------------------------------------------------
void i2c_reset(void) {
    char i;
    i2c_init();
    i2c_start();
    for (i=0;i<9;i++)
    {
        i2c_sda_output_high();     // SDA=1
        delay_us(5);
        i2c_scl_output_high();
        delay_us(5);
        i2c_scl_output_low();
        delay_us(5);
    }
    i2c_start();
    i2c_stop();
}

//------------------------------------------------------------------------------
// 	Routine:	i2c_write
//	Inputs:		output byte
//	Outputs:	none
//	Purpose:	Writes data over the I2C bus
//------------------------------------------------------------------------------
void i2c_write (unsigned char output_data)
{
    int index; //unsigned char
    i2c_sda_dir_output();
    i2c_scl_dir_output();
    delay_us(2);

    for(index = 0; index < 8; index++)  	// Send 8 bits to the I2C Bus
    {
        // Output the data bit to the I2C Bus
        if (output_data & 0x80)   i2c_sda_output_high();
        else i2c_sda_output_low();

        output_data  <<= 1;            		// Shift the byte by one bit
        i2c_scl_output_high();
        delay_us(2);
        i2c_scl_output_low();
        delay_us(2);
    }
    i2c_sda_dir_input();
    i2c_scl_output_high();   		        			// Clock the ACK from the I2C Bus
    delay_us(2);
    i2c_scl_output_low();
    delay_us(2);
}


//------------------------------------------------------------------------------
// 	Routine:	i2c_read
//	Inputs:		send_ack (if TRUE send the ACK signal)
//	Outputs:	input byte
//	Purpose:	Reads data from the I2C bus
//------------------------------------------------------------------------------
unsigned char i2c_read (char ack)
{
    unsigned char index, input_data;

    input_data = 0x00;
    i2c_scl_dir_output();
    i2c_sda_dir_input();		// Put data pin into read mode
#ifdef __MSP430__
    i2c_sda_pullup_enable();
    i2c_sda_pullup();
#endif
    delay_us(2);

    for(index = 0; index < 8; index++)  	// Send 8 bits to the I2C Bus
    {
        input_data <<= 1;					// Shift the byte by one bit
        i2c_scl_output_high();
        delay_us(2);// Clock the data into the I2C Bus
        if (i2c_sda_check_input())
            input_data |= 1;
        else  input_data |= 0;
        i2c_scl_output_low();
        delay_us(2); 
    }

    i2c_sda_dir_output();		// Put data pin into Write mode

    if (ack) i2c_sda_output_low();				// Set data pin LOW to Ack the read
    else i2c_sda_output_high();					// Set data pin HIGH to Not Ack the read
    delay_us(2);
    i2c_scl_output_high();						// Set the clock pin HIGH
    delay_us(2);
    i2c_scl_output_low();						// Set the clock pin LOW
	delay_us(2);
    i2c_sda_output_low();
    return (input_data);
}
