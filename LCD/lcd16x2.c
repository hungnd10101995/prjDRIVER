/*===========================================================================*/
/* I N C L U D E S                                                           */
/*===========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include "..\library.h"

#if defined(_DT03M_RF_) && defined(__MSP430__)
#include "..\..\dt03m-rf\fw\emeter-main.h"
#include "..\..\dt03m-rf\fw\emeter-hardware.h"
#endif
#if defined(_DT03M_RF_) && defined(__PIC24FJxxxGA410__)
#include "..\..\dt03m-rf\fw\main.h"
#include "..\..\dt03m-rf\fw\hardware.h"
#endif

#ifdef _DT03P_RF_
#include "..\..\dt03p-rf\fw\emeter\emeter-main.h"
#include "..\..\dt03p-rf\fw\emeter\emeter-hardware.h"
#endif

#ifdef _DT01M_RF_
#include "..\..\dt01m-rf\fw\emeter\emeter-main.h"
#include "..\..\dt01m-rf\fw\emeter\emeter-hardware.h"
#endif

#ifdef _PREPAID_METER_
#include "..\..\dt01pp\fw\emeter\emeter-main.h"
#include "..\..\dt01pp\fw\emeter\emeter-hardware.h"
#endif
#ifdef _DCU_
#include "../../LIBRARY/framework/delay.h"
#include "../../DCU/FW/hardware.h"
#include "../../DCU/FW/display.h"
#endif
#ifdef _LCD_1602_ //to the end of file
#include "lcd16x2.h"
#include "..\framework\delay.h"
#include "..\framework\process.h"
/*********************** Hardware specific configuration **********************/

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be
   increased by factor 2^N by this constant                                   */
#define DELAY_2N     6

/*------------------------- Text LCD size definitions ------------------------*/

#define LineLen     16                  /* Width (in characters)              */
#define NumLines     2                  /* Hight (in lines)                   */

/*-------------------- LCD interface hardware definitions --------------------*/

//#define PIN_RS                BIT5//(1      << 18)
//#define PIN_RW                BIT6//(1      << 19)
//#define PIN_E                 BIT7//(1      << 20)
//#define PINS_CTRL             (0xE0) //P3.5 -> P3.7//(0x07   << 18)
//#define PINS_DATA             (0xF0) //P4.0 -> P4.3 (0x0FUL << 21)

/* pin E  setting to 0 or 1	*/
//#define LCD_E(x)              ((x) ? (LCD_PINS_CTRL_DIR |= PIN_E)  : (LCD_PINS_CTRL_OUT &= ~PIN_E) ); delay(10);
//thaodtn ((x) ? (LPC_GPIO1->FIOSET |= PIN_E)  : (LPC_GPIO1->FIOCLR |= PIN_E) ); delay(10);

/* pin RW setting to 0 or 1 */
//#define LCD_RW(x)             ((x) ? (LCD_PINS_CTRL_DIR |= PIN_RW) : (LCD_PINS_CTRL_DIR &= ~PIN_RW)); delay(10);
//thaodtn ((x) ? (LPC_GPIO1->FIOSET |= PIN_RW) : (LPC_GPIO1->FIOCLR |= PIN_RW)); delay(10);

/* pin RS setting to 0 or 1 */
//#define LCD_RS(x)             ((x) ? (LCD_PINS_CTRL_DIR |= PIN_RS) : (LCD_PINS_CTRL_DIR &= ~PIN_RS)); delay(10);
//thaodtn ((x) ? (LPC_GPIO1->FIOSET |= PIN_RS) : (LPC_GPIO1->FIOCLR |= PIN_RS)); delay(10);

/* Reading DATA pins */
//#define LCD_DATA_IN           (LCD_PINS_DATA_IN & 0x0F)
       //thaodtn ((LPC_GPIO1->FIOPIN >> 21) & 0xF)

/* Writing value to DATA pins */
//#define LCD_DATA_OUT(x)       {LCD_PINS_DATA_OUT |= (x & 0x0F); LCD_PINS_DATA_OUT &= ((~x) & 0xF);delay(10);}
       //thaodtn LPC_GPIO1->FIOSET  |= ((x & 0xF) << 21); LPC_GPIO1->FIOCLR  |= (((~x) & 0xF) << 21); delay(10);

/* Setting all pins to output mode */
//#define LCD_ALL_DIR_OUT       {LCD_PINS_DATA_DIR |= PINS_DATA; LCD_PINS_CTRL_DIR |= PINS_CTRL; delay(10);}

/* Setting DATA pins to input mode */
//#define LCD_DATA_DIR_IN       {LCD_PINS_DATA_DIR &= ~PINS_DATA; delay(10);}

/* Setting DATA pins to output mode */
//#define LCD_DATA_DIR_OUT      {LCD_PINS_DATA_DIR |= PINS_DATA; delay(10);}

/******************************************************************************/

/* 8 user defined characters to be loaded into CGRAM (used for bargraph)      */
const unsigned char UserFont[8][8] = {
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10 },
  { 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18 },
  { 0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C },
  { 0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E },
  { 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },
  { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }
};


/************************ Local auxiliary functions ***************************/

/*******************************************************************************
* Delay in while loop cycles                                                   *
*   Parameter:    cnt:    number of while cycles to delay                      *
*   Return:                                                                    *
*******************************************************************************/
/*
static void delay (unsigned long cnt) {

  cnt <<= DELAY_2N;
  while (cnt--);
}
*/
void delay(const int cnt) {

	//unsigned long i;
	//for(i=0;i<25*cnt;i++);
    delay_us(cnt);
}

/*******************************************************************************
* Read status of LCD controller                                                *
*   Parameter:    none                                                         *
*   Return:       Status byte contains busy flag and address pointer           *
*******************************************************************************/

static unsigned char lcd_read_status (void) {
  unsigned char status;
#ifdef LCD8BIT
    lcd_enable_output_low(); //E = 0
    lcd_rs_output_low(); //instruction = 0
    lcd_wr_output_high(); //WR = 1

    lcd_datas_dir_output();
    lcd_datas_output(0xFF); //gui lenh doc BF
    lcd_enable_output_high();

    lcd_datas_dir_input();
    status = lcd_datas_input();

    lcd_enable_output_low();
    lcd_wr_output_low();
    lcd_datas_dir_output();
#else
    lcd_datas_dir_input();
    lcd_rs_output_low();
    lcd_wr_output_high();
    delay(10);
    lcd_enable_output_high();
    delay(10);
    status = lcd_datas_input() << 4;
    lcd_enable_output_low();
    delay(10);
    lcd_enable_output_high();
    delay(10);
    status |= lcd_datas_input();
    lcd_enable_output_low();
    lcd_datas_dir_output();
#endif
    return status;
}


/*******************************************************************************
* Wait until LCD controller busy flag is 0                                     *
*   Parameter:                                                                 *
*   Return:       Status byte of LCD controller (busy + address)               *
*******************************************************************************/

unsigned char wait_while_busy (void)
{
    //tuantm
    delay_ms(100);
    return TRUE;
    //
    unsigned char status = 0;
    int i = 0;
    do
    {
        i++;
        delay_ms(1);
        status = lcd_read_status();
    }while ((i<100)&&(status & 0x80));             /* Wait for busy flag                 */
    //return (status);
    if ((status & 0x80) == 0) return TRUE;
    else return FALSE;
}


/*******************************************************************************
* Write 4-bits to LCD controller                                               *
*   Parameter:    c:      command to be written                                *
*   Return:                                                                    *
*******************************************************************************/

unsigned char lcd_read_4bit(void)
{
    unsigned char hi_nibble, lo_nibble;
    lcd_datas_output(0xF0);

    lcd_enable_output_high();
    lcd_datas_dir_input();
    hi_nibble = lcd_datas_input() & 0xF0;
    lcd_enable_output_low();

    lcd_enable_output_high();
    lo_nibble = lcd_datas_input() & 0xF0;
    lcd_enable_output_low();

    lo_nibble >>= 4;
    return (hi_nibble | lo_nibble);
}

#if defined(PCB_DEMO) || defined(PCB_DCU1P_V10) && !defined(LCD8BIT)
void lcd_datas_output(unsigned char Data)
{
    if(Data & 0x01) LCD_PINS_DATA_OUT set LCD_D4;
    else LCD_PINS_DATA_OUT clr LCD_D4;

    if((Data>>1)&1) LCD_PINS_DATA_OUT set LCD_D5;
    else LCD_PINS_DATA_OUT clr LCD_D5;

    if((Data>>2)&1) LCD_PINS_DATA_OUT set LCD_D6;
    else LCD_PINS_DATA_OUT clr LCD_D6;

    if((Data>>3)&1) LCD_PINS_DATA_OUT set LCD_D7;
    else LCD_PINS_DATA_OUT clr LCD_D7;
}
#endif

void lcd_write_4bit(unsigned char c)
{
    unsigned char hi_nibble, lo_nibble, temp;
    //doc 4 bit thap cua data de mask
    temp = lcd_datas_input() & 0x0F;

    hi_nibble = c & 0xF0;
    lo_nibble = (c << 4) & 0xF0;

    lcd_datas_output(hi_nibble|temp);
    lcd_enable_output_high();
    lcd_enable_output_low();

    lcd_datas_output(lo_nibble|temp);
    lcd_enable_output_high();
    lcd_enable_output_low();
}

void lcd_write_8bit(unsigned char c)
{
    lcd_enable_output_high();
    delay(10);
    lcd_datas_output(c);
    lcd_enable_output_low();
    delay(10);
}


/*******************************************************************************
* Write command to LCD controller                                              *
*   Parameter:    c:      command to be written                                *
*   Return:                                                                    *
*******************************************************************************/

void lcd_write_cmd (unsigned char c)
{    
    if (wait_while_busy() == FALSE) return;
    lcd_rs_output_low();
#ifdef LCD8BIT
    lcd_write_8bit(c);
#else
    lcd_write_4bit(c>>4);
    lcd_write_4bit(c);
#endif
}


/*******************************************************************************
* Write data to LCD controller                                                 *
*   Parameter:    c:      data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

static void lcd_write_data (unsigned char c)
{
    if (wait_while_busy() == FALSE) return;//wait_while_busy();
    lcd_rs_output_high();
#ifdef LCD8BIT
    lcd_write_8bit(c);
#else
    lcd_write_4bit(c>>4);
    lcd_write_4bit(c);
#endif
}


/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the LCD controller                                                *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void LCD_init (void)
{
    int i;
    unsigned char const *p;
    /* Set all pins for LCD as outputs    */
    lcd_controls_dir_output();
    lcd_datas_dir_output();
    delay_ms(25);//thaodtn //delay (25000);
    /* function set */
    lcd_rs_output_low();
    lcd_wr_output_low();
    lcd_enable_output_low();
#ifdef LCD8BIT
    lcd_write_8bit(0x38);
    if (wait_while_busy() == FALSE) return;//wait_while_busy();
#else
    lcd_enable_output_high();
    lcd_datas_output(0x20);
    lcd_enable_output_low();
    if (wait_while_busy() == FALSE) return;//wait_while_busy();
#endif
    /* Display control */
    lcd_rs_output_low();
#ifdef LCD8BIT
    lcd_write_8bit(0x0E);
#else
    lcd_write_4bit(0x0E);
#endif
    if (wait_while_busy() == FALSE) return;//wait_while_busy();
    /* Entry mode set */
    lcd_rs_output_low();
#ifdef LCD8BIT
    lcd_write_8bit(0x06);
#else
    lcd_write_4bit(0x06);
#endif
    if (wait_while_busy() == FALSE) return;//wait_while_busy();
    LCD_cls();
    LCD_cur_off();
}


/*******************************************************************************
* Set cursor position on LCD display                                           *
*   Parameter:    column: column position                                      *
*                 line:   line position                                        *
*   Return:                                                                    *
*******************************************************************************/

void LCD_gotoxy (unsigned char x, unsigned char y) {
  unsigned char address;

  address = (y * 0x40) + x;
  address = 0x80 + (address & 0x7F);
  lcd_write_cmd(address);               /* Set DDRAM address counter to 0     */
}


/*******************************************************************************
* Clear the LCD display                                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void LCD_cls (void) {

  lcd_write_cmd(0x01);                  /* Display clear                      */
  LCD_gotoxy(0, 0);
}

/*******************************************************************************
* Turn cursor off                                                              *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void LCD_cur_off (void) {

  lcd_write_cmd(0x0C);                  /* Switch off LCD cursor              */
}

void LCD_cur_blink (void) {

  lcd_write_cmd(0x0F);                  /* Switch off LCD cursor, blink       */
}


/*******************************************************************************
* Print Character to current cursor position                                   *
*   Parameter:    c:      character to be printed                              *
*   Return:                                                                    *
*******************************************************************************/

void LCD_putc (unsigned char c) {

  lcd_write_data(c);
}


/*******************************************************************************
* Print sting to LCD display                                                   *
*   Parameter:    string: pointer to output string                             *
*   Return:                                                                    *
*******************************************************************************/

void LCD_print (unsigned char x, unsigned char y, unsigned char *string) {

  LCD_gotoxy(x, y);
  while (*string)
    LCD_putc (*string++);
}


/*******************************************************************************
* Display bargraph on LCD display                                              *
*   Parameter:    string: pointer to output string                             *
*   Return:                                                                    *
*******************************************************************************/

void LCD_bargraph (unsigned char x, unsigned char y, unsigned char size, unsigned char val) {
  char i;

  val = val * size / 20;                /* Scale value for 5x8 characters     */
  LCD_gotoxy(x, y);
  for (i = 0; i < size; i++)  {
    if (val > 5) {
      LCD_putc(0x05);
      val -= 5;
    } else {
      LCD_putc(val);
      val  = 0;
    }
  }
}
/*******************************************************************************
* Display digit on LCD display at column pos, row 1                            *
*   Parameter:    'value':gia tri ta muon hien thi(0...9)                      *
*   Return:                                                                    *
*******************************************************************************/
void lcd_display_digit(unsigned char value,unsigned char  pos)
{
    if (pos > 15) return;
    if (value > 9) return;
    LCD_gotoxy(9 - pos, 1);
    LCD_putc(value + 0x30);
}
/*******************************************************************************
* Display: Rate xx Reg xx                                                      *
*   Parameter:    long: value to display; status: active/deactive              *
*   Return:                                                                    *
*******************************************************************************/
void lcd_display_tariff(unsigned char source, unsigned char index)
{
    uint8_t dig1, dig2;
    dig1 = index%10;
    dig2 = (index/10)%10;
    switch (source)
    {
        case SOURCE_IMPORT_ACTIVE_ENERGY:
        LCD_print (0, 0, "Rate ImpW T     ");
        LCD_gotoxy(11,0);
        break;
        case SOURCE_EXPORT_ACTIVE_ENERGY:
        LCD_print (0, 0, "Rate ExpW T     ");
        LCD_gotoxy(11,0);
        break;
        case SOURCE_IMPORT_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate ImpVar T   ");
        LCD_gotoxy(13,0);
        break;
        case SOURCE_EXPORT_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate ExpVar T   ");
        LCD_gotoxy(13,0);
        break;
        case SOURCE_APPARENT_ENERGY:
        LCD_print (0, 0, "Rate ImpVA T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q1_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate Q1Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q2_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate Q2Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q3_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate Q3Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q4_REACTIVE_ENERGY:
        LCD_print (0, 0, "Rate Q4Var T    ");
        LCD_gotoxy(12,0);
        break;
    }
    LCD_putc(dig2 + 0x30);
    LCD_putc(dig1 + 0x30);

}

void lcd_display_maxdemand(unsigned char source, unsigned char index)
{
    uint8_t dig1, dig2;
    dig1 = index%10;
    dig2 = (index/10)%10;
    switch (source)
    {
        case SOURCE_IMPORT_ACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD ImpW T     ");
        LCD_gotoxy(11,0);
        break;
        case SOURCE_EXPORT_ACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD ExpW T     ");
        LCD_gotoxy(11,0);
        break;
        case SOURCE_IMPORT_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD ImpVar T   ");
        LCD_gotoxy(13,0);
        break;
        case SOURCE_EXPORT_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD ExpVar T   ");
        LCD_gotoxy(13,0);
        break;
        case SOURCE_APPARENT_ENERGY:
        LCD_print (0, 0, "MaxD ImpVA T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q1_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD Q1Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q2_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD Q2Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q3_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD Q3Var T    ");
        LCD_gotoxy(12,0);
        break;
        case SOURCE_Q4_REACTIVE_ENERGY:
        LCD_print (0, 0, "MaxD Q4Var T    ");
        LCD_gotoxy(12,0);
        break;
    }
    LCD_putc(dig2 + 0x30);
    LCD_putc(dig1 + 0x30);
}

/*******************************************************************************
* Display value on LCD display at column 0, row 1                              *
*   Parameter:    long: value to display; point: vi tri dau phay thap phan     *
*   Return:       123456789.0                                                  *
*******************************************************************************/
void lcd_display_value(long long value, int point)
{
    long long value1;
    unsigned char var[10];
    int i;
    int lsb = 0; //vi tri lsb
    if (value >= 0) value1 = value;
    else value1 = -value;
    //lay gia tri
    for(i = 0; i < 10; i++)        //10 chu so
    {
        var[i] = value1 % 10;
        value1 /= 10;
        if (var[i] > 0) lsb = i + 1;
    }
    //hien thi
    switch (point)
    {
        case 1: //987654321.0
        LCD_gotoxy(8, 1);
        LCD_putc('0');
        LCD_putc(',');
        LCD_putc(var[0] + 0x30);
        for (i = 1; i < lsb; i++)
        {
            LCD_gotoxy(9 - i, 1);
            LCD_putc(var[i] + 0x30);
        }
        break;
        case 2: //98765432.10
        LCD_gotoxy(7, 1);
        LCD_putc('0');
        LCD_putc(',');
        LCD_putc(var[1] + 0x30);
        LCD_putc(var[0] + 0x30);
        for (i = 2; i < lsb; i++)
        {
            LCD_gotoxy(9 - i, 1);
            LCD_putc(var[i] + 0x30);
        }
        break;
        case 3: //9876543.210
        LCD_gotoxy(6, 1);
        LCD_putc('0');
        LCD_putc(',');
        LCD_putc(var[2] + 0x30);
        LCD_putc(var[1] + 0x30);
        LCD_putc(var[0] + 0x30);
        for (i = 3; i < lsb; i++)
        {
            LCD_gotoxy(9 - i, 1);
            LCD_putc(var[i] + 0x30);
        }
        break;
        default: //9876543210
        LCD_gotoxy(9, 1);
        LCD_putc('0');
        for (i = 0; i < lsb; i++)
        {
            LCD_gotoxy(9 - i, 1);
            LCD_putc(var[i] + 0x30);
        }
        break;
    }
}

/******************************************************************************/
void lcd_anten(char status){};
void lcd_reverse(char status){};
void lcd_magnetic(char status){};
void lcd_lowbat(char status){};
void lcd_powerfail(char status)
{
    LCD_print (0, 0, "POWER DOWN      ");
    LCD_print (0, 1, "SAVED DATA      ");
}
void lcd_L1(char status){};
void lcd_L2(char status){};
void lcd_L3(char status){};
void lcd_COL2(unsigned char status){};
void lcd_Er(char status){};
void lcd_unlock(char status){};
void lcd_phA(char status){};
void lcd_phB(char status){};
void lcd_phC(char status){};
void lcd_phase(int ph, int x, int y)
{
    LCD_gotoxy(x,y);
    switch (ph)
    {
        case PHASE_A: LCD_putc('A'); break;
        case PHASE_B: LCD_putc('B'); break;
        case PHASE_C: LCD_putc('C'); break;
        case NEUTRAL: LCD_putc('N'); break;
        case LIVE: LCD_putc('P'); break;
        case PHASE_TOTAL: LCD_putc('T'); break;
    }
}
void lcd_V(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "V   ");
};
void lcd_A(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "A   ");
};
void lcd_point1(char status)
{
    LCD_gotoxy(9,1);
    LCD_putc(',');
};
void lcd_point2(char status)
{
    LCD_gotoxy(8,1);
    LCD_putc(',');
};
void lcd_point3(char status)
{
    LCD_gotoxy(7,1);
    LCD_putc(',');
};
void lcd_point4(char status){};
void lcd_point6(char status){};
void lcd_point7(char status){};
void lcd_point8(char status){};
void lcd_point9(char status){};
void lcd_warning(char status){};
void lcd_W(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "W    ");
};
void lcd_kW(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "kW   ");
};
void lcd_VAr(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "VAr  ");
};
void lcd_kVAr(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "kVAr ");
};
void lcd_VA(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "VA   ");
};
void lcd_kVA(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "kVA  ");
};
void lcd_Hz(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "Hz   ");
};
void lcd_kWh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "kWh  ");
};
void lcd_Wh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "Wh   ");
};
void lcd_kVArh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION - 1, 1, "kVArh");
};
void lcd_VArh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "VArh ");
};
void lcd_kVAh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "kVAh ");
};
void lcd_VAh(char status)
{
    LCD_print(DISPLAY_UNIT_POSITION, 1, "VAh  ");
};
void lcd_P_active(char status){};
void lcd_P_reverse(char status){};
void lcd_Q_active(char status){};
void lcd_Q_reverse(char status){};
void lcd_GND(char status){};
void lcd_MD(char status){};
void lcd_ACB(char status){};
void lcd_clear_all(void)
{
    LCD_cls();
};
void lcd_hide_digit(unsigned char  pos){};
void lcd_display_alphabet(unsigned char value,unsigned char  pos){};
void lcd_over(char status){};
void lcd_lo(char status){};
#endif //_LCD_1602_