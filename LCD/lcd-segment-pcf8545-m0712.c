#include "define.h"
#include "spi_lib.h"
#include "lcd-segment-pcf8545-m0712.h"

void lcd_disable()
{
    LCD_CS = 0;
    send_byte(WRITE_LCD);               
    send_byte(CTR_LAST_CMD_REG); 
    send_byte(CMD_DISABLE_DISPLAY); 
    LCD_CS = 1;    
}

void lcd_enable()
{
    LCD_CS = 0;
    send_byte(WRITE_LCD);               
    send_byte(CTR_LAST_CMD_REG); 
    send_byte(CMD_ENABLE_DISPLAY); 
    LCD_CS = 1;    
}


void load_data_pointer(unsigned char start_segment)
{
    LCD_CS = 0; 
    send_byte(WRITE_LCD);
    send_byte(CTR_LAST_CMD_REG); 
    send_byte(CMD_LOAD_DATA_POINTER + start_segment); // start loading from 0
    LCD_CS = 1;
}

void clear_lcd()
{
	unsigned char  i = 0;
    load_data_pointer(0x00); 
    LCD_CS = 0;
    send_byte(WRITE_LCD);                   
    send_byte(CTR_LAST_RAM);     
    for( i = 1; i<= 22; i++)
    {
        send_byte(0x00);
    }               
    LCD_CS = 1; 
} 


void display_character(unsigned char data_pointer, unsigned char character)
{
    load_data_pointer(data_pointer);
    LCD_CS = 0;
    send_byte(WRITE_LCD);               
    send_byte(CTR_LAST_RAM);     
    send_byte(character);
    LCD_CS = 1;
}

unsigned char swap_byte(unsigned char in_byte)
{
    unsigned char out_byte;
    out_byte = (in_byte << 4) | (in_byte >> 4); 
    return out_byte;
}


unsigned char encode_lcd(unsigned char in_byte)
{
    unsigned char encode_byte = 0;
    
    switch(in_byte)  
    {  
        case 0 : 
                encode_byte = NUMBER_0;
                break;
        case 1 : 
                encode_byte = NUMBER_1;
                break;
        case 2 : 
                encode_byte = NUMBER_2;
                break;
        case 3 : 
                encode_byte = NUMBER_3;
                break;
        case 4 : 
                encode_byte = NUMBER_4;
                break;
        case 5 : 
                encode_byte = NUMBER_5;
                break;   
        case 6 : 
                encode_byte = NUMBER_6;
                break;             
        case 7 : 
                encode_byte = NUMBER_7;
                break;
        case 8 : 
                encode_byte = NUMBER_8;
                break; 
        case 9 : 
                encode_byte = NUMBER_9;
                break;
                                
    }
    return encode_byte;
}
//-----------------------------------------------------------------------------
void lcd_anten(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_2_1, S6);
	}else
	{
		display_character(PIN_2_1, EMPTY);
	}	
}


void lcd_warning(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_4_3, S27);
	}else
	{
		display_character(PIN_4_3, EMPTY);
	}	
}

void lcd_lowbat(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_6_5, S7);
	}else
	{
		display_character(PIN_6_5, EMPTY);
	}	
}

void lcd_reverse(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_8_7, S8);
	}else
	{
		display_character(PIN_8_7, EMPTY);
	}	
}

void lcd_Er(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_10_9, S23);
	}else
	{
		display_character(PIN_10_9, EMPTY);
	}
}

void lcd_ACB(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_10_9, ACB);
	}else
	{
		display_character(PIN_10_9, EMPTY);
	}
}

void lcd_MD(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_10_9, S25);
	}else
	{
		display_character(PIN_10_9, EMPTY);
	}
}

void lcd_COL2(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_10_9, COL2);
	}else
	{
		display_character(PIN_10_9, EMPTY);
	}
} 

void lcd_L1(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_12_11, S22);
	}else
	{
		display_character(PIN_12_11, EMPTY);
	}
}

void lcd_L2(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_10_9, S21);
	}else
	{
		display_character(PIN_10_9, EMPTY);
	}
}

void lcd_L3(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_20_19, S20);
	}else
	{
		display_character(PIN_20_19, EMPTY);
	}
}

void lcd_GND(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_20_19, S19);
	}else
	{
		display_character(PIN_20_19, EMPTY);
	}
}

void lcd_magnetic(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_20_19, S18);
	}else
	{
		display_character(PIN_20_19, EMPTY);
	}
}

void lcd_unlock(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_16_15, S28);
	}else
	{
		display_character(PIN_16_15, EMPTY);
	}
}

void lcd_powerfail(char status)
{
	if(status == SHOW)
	{
		display_character(PIN_28_27, S26);
	}else
	{
		display_character(PIN_28_27, EMPTY);
	}
}

void lcd_display_digit(unsigned char value,unsigned char  pos)
{
	unsigned char  pin = PIN_18_17;
		
	switch(pos)  
    {  
        case 1 : 
                pin = PIN_18_17;
                break;
        case 2 : 
                pin = PIN_16_15;
                break;
        case 3 : 
                pin = PIN_14_13;
                break;
        case 4 : 
                pin = PIN_12_11;
                break;
        case 5 : 
                pin = PIN_8_7;
                break;
        case 6 : 
                pin = PIN_6_5;
                break;   
        case 7 : 
                pin = PIN_4_3;
                break;             
        case 8 : 
                pin = PIN_2_1;
                break;      
    }
    
    
    display_character(pin, encode_lcd(value));
}

void lcd_display_value(unsigned long value)
{
	long value1;
	int idx ;
    unsigned char digit;
    value1 = value;
    
    
    for( idx = 1; idx < 9; idx++)        //la`n lu.ot hie?n thi cac digit tu` 0 -> 7
    {
        digit = value1 % 10;
        lcd_display_digit(digit, idx);
        value1 /= 10;
        if (value1 == 0) //het chu so thi thoat
        {
            idx = 9;
        }
    }
}
