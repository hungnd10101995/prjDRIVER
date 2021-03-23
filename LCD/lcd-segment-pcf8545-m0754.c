#include "../library.h"
#ifdef _ROUTER_
#include "../../ROUTER/FW/main.h"
#include "../../ROUTER/FW/hardware.h"
#endif
#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter-main.h"
#include "..\..\dt03p-rf\fw\emeter-hardware.h"
#endif
#ifdef _DT03M_RF_
#include "..\..\dt03m-rf\fw\emeter-main.h"
#include "..\..\dt03m-rf\fw\emeter-hardware.h"
#endif
#ifdef _LCD_M0754v2_PCF8545_//PCF8545_DISPLAY_SUPPORT
#include "lcd-segment-pcf8545-m0754v2.h"
#include "../framework/macro.h"
#include "../framework/delay.h"

const unsigned char lcd_seg_alphabet[9] = {	CHAR_V, CHAR_E, CHAR_R,CHAR_A,CHAR_B,CHAR_C,
											CHAR_D, CHAR_F, CHAR_DASH};
unsigned char pointer[40];

void pcf8545_spi_disable()
{
    pcf8545_cs_output_high();
    //cac chan CS cua cac khoi khac     
}

void pcf8545_spi_send_byte(unsigned char value)
{
    unsigned char i;
    delay_us(1);//nop();
    for (i = 0; i < 8; i++)
    {
        if(value & 0x80)
        {
            pcf8545_sdi_output_high();
        }
        else
        {
            pcf8545_sdi_output_low();
        }

      pcf8545_sclk_output_high();//SCLK_L;
      delay_us(1);//nop();////delay_us(1);
      pcf8545_sclk_output_low();//SCLK_H;
      delay_us(1);//nop();////delay_us(1);
      value <<= 1;
    }
    pcf8545_sdi_output_low();
    delay_us(1);//nop()
}

void lcd_send_byte(unsigned char bytes)
{
	//unsigned char spi_buf ;
    pcf8545_spi_send_byte(bytes);        
    //while(!spi_data_is_in());
    //spi_buf = spi_get_byte(); // Read SPIBUF 
}

void startup_pcf8545(void)
{
	pcf8545_spi_disable();
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);               
    lcd_send_byte(CTR_LAST_CMD_REG); 
    lcd_send_byte(CMD_INITIALIZE); //reset 
    pcf8545_cs_output_high(); 
    delay_ms(2);
    //-----------------
    pcf8545_cs_output_low();
    lcd_send_byte(WRITE_LCD);               
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_OTP_REFRESH); 
               
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_MODE_SETTING); //BPS = INV = 0
              
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_SET_MUX_MODE + MUX_1_8); // 1:8 multiplex 
               
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_SET_BIAS_MODE + BIAS_1_3); // set bias mode 1/2
              
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_FRAME_FREQUENCY + FREQ_199_7Hz); // cmd_frame_frequency : default frequency
              
    lcd_send_byte(CTR_CONT_CMD_REG); 
    lcd_send_byte(CMD_OSCILLATOR_CONTROL);  // default
               
    lcd_send_byte(CTR_LAST_CMD_REG); 
    lcd_send_byte(CMD_LOAD_DATA_POINTER); // start loading from 0
    delay_us(1);
    pcf8545_cs_output_high(); 

}

void pcf8545_display_init()
{
    pcf8545_cs_dir_output();
    pcf8545_reset_dir_output();
    pcf8545_cs_output_high();
    pcf8545_reset_output_high();
    pcf8545_sdi_dir_output();
    pcf8545_sdi_output_low();
    pcf8545_sclk_dir_output();
    pcf8545_sclk_output_low();
    startup_pcf8545();
    lcd_clear_all();
    lcd_enable();    
}

void init_character()
{
    int i = 0;
	for (i = 0; i < 40; i++)
        pointer[i] = EMPTY;
}

void lcd_disable()
{
	pcf8545_spi_disable();
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);               
    lcd_send_byte(CTR_LAST_CMD_REG); 
    lcd_send_byte(CMD_MODE_SETTING);//(CMD_DISABLE_DISPLAY); 
    delay_us(1);
    pcf8545_cs_output_high();    
}

void lcd_enable()
{
	pcf8545_spi_disable();
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);               
    lcd_send_byte(CTR_LAST_CMD_REG); 
    lcd_send_byte(CMD_MODE_SETTING + DISPLAY_ENABLE);//(CMD_ENABLE_DISPLAY); 
    delay_us(1);
    pcf8545_cs_output_high();    
}



void load_data_pointer(unsigned char start_segment)
{
	pcf8545_spi_disable();
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);
    lcd_send_byte(CTR_LAST_CMD_REG); 
    lcd_send_byte(CMD_LOAD_DATA_POINTER + start_segment); // start loading from 0
    delay_us(1);
    pcf8545_cs_output_high(); 
}

void lcd_clear_all()
{
	unsigned char  i = 0;
    load_data_pointer(0x00); 
    pcf8545_spi_disable();
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);                   
    lcd_send_byte(CTR_LAST_RAM);     
    for( i = 0; i< 40; i++)
    {
        lcd_send_byte(EMPTY);
    }               
    delay_us(1);
    pcf8545_cs_output_high();  
	init_character();
} 


void display_character(unsigned char data_pointer, unsigned char character)
{
	pcf8545_spi_disable();
    load_data_pointer(data_pointer);
    pcf8545_cs_output_low();
    delay_us(1);
    lcd_send_byte(WRITE_LCD);               
    lcd_send_byte(CTR_LAST_RAM);     
    lcd_send_byte(character);
    delay_us(1);
    pcf8545_cs_output_high(); 
}

//-----------------------------------------------------------------------------
void lcd_anten(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_21] |= S6;		
	}else
	{
		pointer[PIN_21] &= ~S6;		
	}	
	display_character(PIN_21,  pointer[PIN_21]);
}

void lcd_lowbat(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_19] |= S7;		
	}else
	{
		pointer[PIN_19] &= ~S7;		
	}	
	display_character(PIN_19, pointer[PIN_19]);
}

void lcd_reverse(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S8;		
	}else
	{
		pointer[PIN_6] &= ~S8;		
	}	
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_Er(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_3] |= S23;		
	}else
	{
		pointer[PIN_3] &= ~S23;		
	}
	display_character(PIN_3, pointer[PIN_3]);
}

void lcd_ACB(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= ACB;		
	}else
	{
		pointer[PIN_6] &= ~ACB;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_MD(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_5] |= S25;		
	}else
	{
		pointer[PIN_5] &= ~S25;
	}	
	display_character(PIN_5, pointer[PIN_5]);

}

void lcd_COL2(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= COL2;
	}else
	{
		pointer[PIN_6] &= ~COL2;
	}
	display_character(PIN_6, pointer[PIN_6]);
} 

void lcd_L1(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S22;		
	}else
	{
		pointer[PIN_11] &= ~S22;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_L2(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S21;		
	}else
	{
		pointer[PIN_11] &= ~S21;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_L3(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S20;
		
	}else
	{
		pointer[PIN_11] &= ~S20;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_GND(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S19;		
	}else
	{
		pointer[PIN_11] &= ~S19;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_magnetic(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_15] |= S18;		
	}else
	{
		pointer[PIN_15] &= ~S18;
	}
	display_character(PIN_15, pointer[PIN_15]);
}

void lcd_unlock(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_5] |= S28;		
	}else
	{
		pointer[PIN_5] &= ~S28;
	}
	display_character(PIN_5, pointer[PIN_5]);
}

void lcd_powerfail(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S26;
		
	}else
	{
		pointer[PIN_6] &= ~S26;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_display_digit(unsigned char value,unsigned char  pos)
{
	unsigned char  pin1;
    unsigned char  pin2;
    unsigned char SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G;
	switch(pos)  
    {  
        case 1 : 
            pin1 = PIN_14;
            pin2 = PIN_15;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 2 : 
            pin1 = PIN_12;
            pin2 = PIN_13;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 3 : 
            pin1 = PIN_9;
            pin2 = PIN_10;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 4 : 
            pin1 = PIN_7;
            pin2 = PIN_8;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 5 : 
            pin1 = PIN_4;
            pin2 = PIN_5;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 6 : 
            pin1 = PIN_2;
            pin2 = PIN_3;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;  
        case 7 : 
            pin1 = PIN_16;
            pin2 = PIN_1;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;            
        case 8 : 
            pin1 = PIN_18;
            pin2 = PIN_17;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;  
        case 17:
            pin1 = PIN_20;
            pin2 = PIN_19;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 18:
            pin1 = PIN_22;
            pin2 = PIN_21;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 9:
            pin1 = PIN_7;
            pin2 = PIN_8;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 10:
            pin1 = PIN_4;
            pin2 = PIN_5;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 11:
            pin1 = PIN_2;
            pin2 = PIN_3;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 12:
            pin1 = PIN_16;
            pin2 = PIN_1;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 13:
            pin1 = PIN_18;
            pin2 = PIN_17;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 14:
            pin1 = PIN_20;
            pin2 = PIN_19;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 15:
            pin1 = PIN_22;
            pin2 = PIN_21;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 16:
            pin1 = PIN_9;
            pin2 = PIN_10;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        default:
            return;
    }
    pointer[pin1] &= ~(SEG_D + SEG_E + SEG_F + SEG_A);
    pointer[pin2] &= ~(SEG_C + SEG_B + SEG_G);
    switch (value)
    {
        case 0:
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B;
            break;
        case 1:            
            pointer[pin2] |= SEG_C + SEG_B;
            break;
        case 2:
            pointer[pin1] |= SEG_D + SEG_E + SEG_A;
            pointer[pin2] |= SEG_B + SEG_G;
            break;
        case 3:
            pointer[pin1] |= SEG_D + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case 4:
            pointer[pin1] |= SEG_F;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case 5:
            pointer[pin1] |= SEG_D + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_G;
            break;
        case 6:
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_G;
            break;
        case 7:
            pointer[pin1] |= SEG_A;
            pointer[pin2] |= SEG_C + SEG_B;
            break;
        case 8:
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case 9:
            pointer[pin1] |= SEG_D + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case DISPLAY_OBIS_CHARACTER_C:
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            break;
        case 'V':
            pointer[pin1] |= SEG_D + SEG_E + SEG_F;
            pointer[pin2] |= SEG_C + SEG_B;
            break;
        case 'E':
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_G;
            break;
        case 'R':
            pointer[pin1] |= SEG_E + SEG_F + SEG_A;
            break;
        case 'A':
            pointer[pin1] |= SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case 'B':
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;
            pointer[pin2] |= SEG_C + SEG_B + SEG_G;
            break;
        case 'C':
            pointer[pin1] |= SEG_D + SEG_E + SEG_F + SEG_A;            
            break;
        case '-':
            pointer[pin2] |= SEG_G;            
            break;
        default:
            break;
    }
    display_character(pin1, pointer[pin1]);
    display_character(pin2, pointer[pin2]);
}

void lcd_hide_digit(unsigned char  pos)
{
    unsigned char  pin1;
    unsigned char  pin2;
    unsigned char SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G;
	switch(pos)  
    {  
        case 1 : 
            pin1 = PIN_14;
            pin2 = PIN_15;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 2 : 
            pin1 = PIN_12;
            pin2 = PIN_13;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 3 : 
            pin1 = PIN_9;
            pin2 = PIN_10;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 4 : 
            pin1 = PIN_7;
            pin2 = PIN_8;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 5 : 
            pin1 = PIN_4;
            pin2 = PIN_5;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 6 : 
            pin1 = PIN_2;
            pin2 = PIN_3;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;  
        case 7 : 
            pin1 = PIN_16;
            pin2 = PIN_1;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;            
        case 8 : 
            pin1 = PIN_18;
            pin2 = PIN_17;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;  
        case 17:
            pin1 = PIN_20;
            pin2 = PIN_19;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 18:
            pin1 = PIN_22;
            pin2 = PIN_21;
            SEG_A = COM3; SEG_B = COM3; SEG_C = COM1; SEG_D = COM0; SEG_E = COM1; SEG_F = COM2; SEG_G = COM2;
            break;
        case 9:
            pin1 = PIN_7;
            pin2 = PIN_8;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 10:
            pin1 = PIN_4;
            pin2 = PIN_5;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 11:
            pin1 = PIN_2;
            pin2 = PIN_3;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        case 12:
            pin1 = PIN_16;
            pin2 = PIN_1;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 13:
            pin1 = PIN_18;
            pin2 = PIN_19;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 14:
            pin1 = PIN_20;
            pin2 = PIN_19;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 15:
            pin1 = PIN_22;
            pin2 = PIN_21;
            SEG_A = COM7; SEG_B = COM7; SEG_C = COM5; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM6;
            break;
        case 16:
            pin1 = PIN_9;
            pin2 = PIN_10;
            SEG_A = COM7; SEG_B = COM6; SEG_C = COM4; SEG_D = COM4; SEG_E = COM5; SEG_F = COM6; SEG_G = COM5;
            break;
        default:
            return;
    }
    pointer[pin1] &= ~(SEG_D + SEG_E + SEG_F + SEG_A);
    pointer[pin2] &= ~(SEG_C + SEG_B + SEG_G);    
    display_character(pin1, pointer[pin1]);
    display_character(pin2, pointer[pin2]);
}

void lcd_phA(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('A',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}

void lcd_phB(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('B',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}

void lcd_phC(char status)
{
    if(status == SHOW)
	{
		lcd_display_digit('C',18);		
	}else
	{
		lcd_hide_digit(18);
	}
}

//void lcd_display_value(unsigned long value)
void lcd_display_value(uint64_t value)
{
	uint64_t temp;
	int idx, idx_tmp;
    unsigned char digit;
    temp = value;
    for( idx = 1; idx <= MAX_DIGITS; idx++)        
    {
        idx_tmp = idx;
        digit = temp % 10;
        if(idx == 9) idx_tmp = 17;
        if(idx == 10) idx_tmp = 18;
        lcd_display_digit(digit, idx_tmp);
        temp /= 10;
        if (temp == 0) //thoat
        {
            idx = MAX_DIGITS + 1;
        }
    }
}

void lcd_T(char status)
{
    if(status == SHOW)
	{
		pointer[PIN_8] |= T;		
	}else
	{
		pointer[PIN_8] &= ~T;
	}
	display_character(PIN_8, pointer[PIN_8]);
}

void lcd_display_tariff(unsigned char value, char status)
{
    lcd_T(status);
	if (status == SHOW)
    {  
        lcd_display_digit(value%10, 16);
        lcd_display_digit(value/10, 9);        
    }
    else
    {
        lcd_hide_digit(16);
        lcd_hide_digit(9);
    }
}

void lcd_Q_reverse(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S1;		
	}else
	{
		pointer[PIN_6] &= ~S1;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_P_reverse(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S2;		
	}else
	{
		pointer[PIN_6] &= ~S2;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_P_active(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S4;		
	}else
	{
		pointer[PIN_6] &= ~S4;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_Q_active(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_6] |= S3;		
	}else
	{
		pointer[PIN_6] &= ~S3;
	}
	display_character(PIN_6, pointer[PIN_6]);
}

void lcd_point1(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_13] |= P1;		
	}else
	{
		pointer[PIN_13] &= ~P1;
	}
	display_character(PIN_13, pointer[PIN_13]);
}

void lcd_point2(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_10] |= P2;		
	}else
	{
		pointer[PIN_10] &= ~P2;
	}
	display_character(PIN_10, pointer[PIN_10]);
}

void lcd_point3(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_8] |= P3;		
	}else
	{
		pointer[PIN_8] &= ~P3;
	}
	display_character(PIN_8, pointer[PIN_8]);
}

void lcd_point6(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_1] |= P6;		
	}else
	{
		pointer[PIN_1] &= ~P6;
	}
	display_character(PIN_1, pointer[PIN_1]);
}

void lcd_point7(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_17] |= P7;		
	}else
	{
		pointer[PIN_17] &= ~P7;
	}
	display_character(PIN_17, pointer[PIN_17]);
}

void lcd_point8(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_19] |= P8;		
	}else
	{
		pointer[PIN_19] &= ~P8;
	}
	display_character(PIN_19, pointer[PIN_19]);
}

void lcd_point9(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_21] |= P9;		
	}else
	{
		pointer[PIN_21] &= ~P9;
	}
	display_character(PIN_21, pointer[PIN_21]);
}

void lcd_p4(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_15] |= P4;		
	}else
	{
		pointer[PIN_15] &= ~P4;
	}
	display_character(PIN_15, pointer[PIN_15]);
}

void lcd_p5(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_15] |= P5;		
	}else
	{
		pointer[PIN_15] &= ~P5;
	}
	display_character(PIN_15, pointer[PIN_15]);
}

void lcd_s9(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_15] |= S9;		
	}else
	{
		pointer[PIN_15] &= ~S9;
	}
	display_character(PIN_15, pointer[PIN_15]);
}

void lcd_s10(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S10;		
	}else
	{
		pointer[PIN_11] &= ~S10;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_s11(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S11;		
	}else
	{
		pointer[PIN_11] &= ~S11;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_s12(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S12;		
	}else
	{
		pointer[PIN_11] &= ~S12;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_s13(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_11] |= S13;		
	}else
	{
		pointer[PIN_11] &= ~S13;
	}
	display_character(PIN_11, pointer[PIN_11]);
}

void lcd_s14(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_12] |= S14;		
	}else
	{
		pointer[PIN_12] &= ~S14;
	}
	display_character(PIN_12, pointer[PIN_12]);
}

void lcd_s5(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_12] |= S5;		
	}else
	{
		pointer[PIN_12] &= ~S5;
	}
	display_character(PIN_12, pointer[PIN_12]);
}

void lcd_s15(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_13] |= S15;		
	}else
	{
		pointer[PIN_13] &= ~S15;
	}
	display_character(PIN_13, pointer[PIN_13]);
}

void lcd_s17(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_13] |= S17;		
	}else
	{
		pointer[PIN_13] &= ~S17;
	}
	display_character(PIN_13, pointer[PIN_13]);
}

void lcd_s16(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_14] |= S16;		
	}else
	{
		pointer[PIN_14] &= ~S16;
	}
	display_character(PIN_14, pointer[PIN_14]);
}

void lcd_s24(char status)
{
	if(status == SHOW)
	{
		pointer[PIN_14] |= S24;		
	}else
	{
		pointer[PIN_14] &= ~S24;
	}
	display_character(PIN_14, pointer[PIN_14]);
}

void lcd_V_high(char status)
{
    if(status == SHOW)
	{
		pointer[PIN_17] |= S30;		
	}else
	{
		pointer[PIN_17] &= ~S30;
	}
	display_character(PIN_17, pointer[PIN_17]);
}

void lcd_V_low(char status)
{
    if(status == SHOW)
	{
		pointer[PIN_1] |= S29;		
	}else
	{
		pointer[PIN_1] &= ~S29;
	}
	display_character(PIN_1, pointer[PIN_1]);
}

void lcd_A_high(char status)
{
    if(status == SHOW)
	{
		pointer[PIN_3] |= S27;		
	}else
	{
		pointer[PIN_3] &= ~S27;
	}
	display_character(PIN_3, pointer[PIN_3]);
}

void lcd_display_obis(unsigned long value)
{
    unsigned long value1;
    unsigned char temp;
    unsigned char i;

    value1 = value;
    lcd_point6(1);
    lcd_point7(1);
    if (value1 == 91)
    {
        lcd_display_digit(1,11);
        lcd_display_digit(9,12);
        lcd_display_digit(0,13);
    }
    else
    {
        temp = value1 % 100;
        if (temp == 0)
        {
            value1 /= 10;
            for(i = 11; i < 15; i++)
            {
                temp = value1%10;
                if (!((i == 14)&&(temp == 0)))
                {
                    lcd_display_digit(temp,i);
                }
                value1 /= 10;
            }
        }
        else
        {
            for (i = 10; i < 15; i++)
            {
                temp = value1 % 10;
                if (!((i == 14)&&(temp == 0)))
                {
                    lcd_display_digit(temp,i);
                }
                value1 /= 10;
            }
        }
    }
}

void lcd_M(char status)
{
	lcd_s9(status);
    lcd_s11(status);
}

void lcd_k(char status)
{
	lcd_s9(status);
    lcd_s10(status);
}

void lcd_V(char status)
{
	lcd_s12(status);
}

void lcd_A(char status)
{
	lcd_s14(status);
    lcd_s5(status);
}

void lcd_r(char status)
{
	lcd_s15(status);
}

void lcd_W(char status)
{
	lcd_s12(status);
    lcd_s13(status);
    lcd_s14(status);
}

void lcd_h(char status)
{
	lcd_s24(status);
}

void lcd_h_left(char status)
{
	lcd_s17(status);
    lcd_s15(status);
    lcd_s16(status);
}

void lcd_kWh(char status)
{
	lcd_k(status);
    lcd_W(status);
    lcd_h_left(status);
}

void lcd_Wh(char status)
{
	lcd_W(status);
    lcd_h_left(status);
}

void lcd_kVArh(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
}

void lcd_VArh(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
    lcd_h(status);
}

void lcd_kVAh(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
}

void lcd_VAh(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_h_left(status);
}

void lcd_kV(char status)
{
	lcd_k(status);
    lcd_V(status);
}

void lcd_kA(char status)
{
	lcd_k(status);
    lcd_A(status);
}

void lcd_kW(char status)
{
	lcd_k(status);
    lcd_W(status);
}

void lcd_MW(char status)
{
    lcd_M(status);
    lcd_W(status);
}

void lcd_VA(char status)
{
	lcd_V(status);
    lcd_A(status);
}

void lcd_kVA(char status)
{
	lcd_k(status);
    lcd_V(status);
    lcd_A(status);
}

void lcd_MVA(char status)
{
	lcd_M(status);
    lcd_V(status);
    lcd_A(status);
}

void lcd_VAr(char status)
{
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
}

void lcd_kVAr(char status)
{
	lcd_k(status);
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
}

void lcd_MVAr(char status)
{
	lcd_M(status);
	lcd_V(status);
    lcd_A(status);
    lcd_r(status);
}

void lcd_Hz(char status)
{
	lcd_s24(status);
    lcd_p5(status);
    lcd_p4(status);
}

void lcd_warning(char status)
{
    
}

void lcd_display_obis_code(unsigned char c, unsigned char d, unsigned char e, unsigned char ph)
{

	unsigned char e1, e0;
    switch(ph)
	{
        case PHASE_A:
            c = c + OBIS_PHASE_A;
            break;
        case PHASE_B:
            c = c + OBIS_PHASE_B;
            break;
        case PHASE_C:
            c = c + OBIS_PHASE_C;
            break;
        default:
            break;
	}
	lcd_point6(SHOW);
	lcd_point7(SHOW);
	//hien thi C
	if (c == DISPLAY_OBIS_CHARACTER_C)
	{
		lcd_display_digit(DISPLAY_OBIS_CHARACTER_C,13);
	}
	else
	{
		unsigned char c1, c0;
		c0 = c % 10;
		c1 = (c/10) % 10;
		lcd_display_digit(c0,13);
		if (c1 != 0)
		{
			lcd_display_digit(c1,14);
		}
	}
	//hien thi D
	lcd_display_digit(d,12);
	//hien thi E
	
	e0 = e % 10;
	e1 = (e/10) % 10;
	if (e1 != 0)
	{
		lcd_display_digit(e1,11);
		lcd_display_digit(e0,10);
	}
	else
	{
		lcd_display_digit(e0,11);
	}

}

void lcd_version(unsigned int version)
{
    unsigned int i = 0;
    unsigned int v = 0;
    unsigned int v1 = version;
    for (i = 1; i <= 3; i++)
    {
        v = v1 % 10;
        lcd_display_digit(v, i);
        v1 = v1 / 10;
    }
    lcd_display_digit('V', 18);
    lcd_display_digit('E', 17);
    lcd_display_digit('R', 8);
    lcd_point1(SHOW);
    lcd_point2(SHOW);    
}

void lcd_display_all(void)
{
	//lcd_clear_all();
//    lcd_anten(SHOW);
//    lcd_lowbat(SHOW);
//    lcd_reverse(SHOW);
//	lcd_point1(SHOW);
//	lcd_point2(SHOW);
//	lcd_point3(SHOW);
//	lcd_point6(SHOW);
//	lcd_point7(SHOW);
//	lcd_point8(SHOW);
//	lcd_point9(SHOW);
//    int i = 0;
//    for (i = 1; i <= 18; i++)
//    {
//        lcd_display_digit(0, i);
//    }
    lcd_version(456);
	//lcd_display_value(88888888); //Init system
//	display_obis_code(88,8,88,88);//lcd_display_obis(888888);
//	lcd_display_digit(8, 15);
//	lcd_display_tariff(88,1);
//	lcd_kVArh(SHOW);
//    lcd_M(SHOW);
    //lcd_kWh(SHOW);
//	lcd_kW(SHOW);
//	lcd_Hz(SHOW);
//	lcd_anten(SHOW);
//	lcd_lowbat(SHOW);
//	lcd_reverse(SHOW);
//	lcd_L1(SHOW);
//	lcd_L2(SHOW);
//	lcd_L3(SHOW);
//	lcd_phA(SHOW);
//	lcd_phB(SHOW);
//	lcd_phC(SHOW);
//	lcd_COL2(SHOW);
//	lcd_GND(SHOW);
//	lcd_P_active(SHOW);
//	lcd_Q_active(SHOW);
//	lcd_P_reverse(SHOW);
//	lcd_Q_reverse(SHOW);
//	lcd_magnetic(SHOW);
//	lcd_unlock(SHOW);
//	lcd_MD(SHOW);
//	lcd_ACB(SHOW);
//	lcd_powerfail(SHOW);
//	lcd_Er(SHOW);
//    lcd_V_high(SHOW);
//    lcd_V_low(SHOW);
//    lcd_A_high(SHOW);
}
#endif
