#include "../library.h"
#ifdef _RMR_
#include "../../RMR/FW/hardware.h"
#endif
#include    <conio.h>
#include    <stdio.h>
#include "../framework/delay.h"
#include "../../RMR/FW/uart.h"
#ifdef UARTSOFT_SUPPORT
/*************************************
 *    Tunable parameters
 */

/* Transmit and Receive port bits */  
#define TxData uart_soft_tx_out() 
#define RxData uart_soft_rx_in()

#define    XTAL            _XTAL_FREQ    /* Crystal frequency (Hz). */
#define    BRATE            9600    /* Baud rate. */
#define RX_OVERSAMPLE    8        /* Amount of oversampling the receiver does. 
                                   Must be a power of two */

/*
 *    Don't change anything else
 ************************************/

#define TIMER_VALUE    XTAL / (4 * BRATE * RX_OVERSAMPLE)

// 1 start bit + 8 data bits + 1 stop bits + safe.
#define TRANSMIT_NUM_BITS    12    

#if    (RX_OVERSAMPLE-1)&RX_OVERSAMPLE
#error    RX_OVERSAMPLE_value must be a power of 2
#endif

// Receiver states.
enum receiver_state {
    RS_HAVE_NOTHING,
    RS_WAIT_HALF_A_BIT,
    RS_HAVE_STARTBIT,
    RS_WAIT_FOR_STOP = RS_HAVE_STARTBIT+8
};

static unsigned char    sendbuffer;            // Where the output char is stored.
static unsigned char    receivebuffer;        // Where the input char is stored as 
                                              // it is received.
volatile unsigned char receivebufferfull;    // 1 = receivebuffer is full.


volatile unsigned char    send_bitno;
static unsigned char    receivestate;        // Initial state of the receiver (0)
static unsigned char    skipoversamples;    // Used to skip receive samples.
static unsigned char    rxshift;
volatile unsigned char tx_next_bit;

volatile unsigned char uart_soft_timer = 0, uart_soft_enable = 0;

char data;
/**
 * init_uart
 * 
 * Initialises the serial port:
 * 
 *     Sets up the I/O directions for the appropriate PortA pins;
 *     Sets up Timer0.
 * 
 * */
void init_uartsoft(void)
{
    receivestate = RS_HAVE_NOTHING;
    skipoversamples = 1;            // check each interrupt for start bit
    receivebufferfull = 0;

   
    uart_soft_tx_dir_out();  //TX output
    uart_soft_tx_high();//TX output high
    uart_soft_rx_dir_input(); //RX input
    //TRISIO = 0x39;                    // Set up I/O direction.
    
    /* Set up the timer. */
    //T0CS = 0;                        // Set timer mode for Timer0.
    //TMR0 = (2-TIMER_VALUE);            // +2 as timer stops for 2 cycles
                                    // when writing to TMR0
    //T0IE = 1;                        // Enable the Timer0 interrupt.
    //GIE = 1;
    
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_2, TIMER_VALUE); //104us - tuantm
    TMR2 = (2-TIMER_VALUE); 
    INTEnable(INT_T2, INT_ENABLED);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_TIMER);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_1);
}

void putch(char c)
{
    while(send_bitno)
        continue;
    tx_next_bit = 0;
    sendbuffer = c;
    send_bitno = TRANSMIT_NUM_BITS*RX_OVERSAMPLE;
}
char getch(void)
{
    while(!receivebufferfull)
        continue;
    receivebufferfull = 0;
    return receivebuffer;
}

/**
 * serial_isr
 * 
 * Transmits and receives characters which have been
 * "putch"ed and "getch"ed.
 * 
 * This ISR runs BRATE * RX_OVERSAMPLE times per second.
 * 
 * */
void uartsoft_interrupt_procees()
{
    timer2_reg += -TIMER_VALUE + 4; // +2 as timer stops for 2 cycles when writing 
    // to TMR0 +2 for tweak
    /*** RECEIVE ***/
    if (--skipoversamples == 0)
    {
        skipoversamples++; // check next time
        switch (receivestate)
        {

        case RS_HAVE_NOTHING:
            /* Check for start bit of a received char. */
            if (!RxData)
            {
                skipoversamples = RX_OVERSAMPLE / 2;
                receivestate++;
            }
            break;

        case RS_WAIT_HALF_A_BIT:
            if (!RxData)
            { // valid start bit
                skipoversamples = RX_OVERSAMPLE;
                receivestate++;
            }
            else
                receivestate = RS_HAVE_NOTHING;
            break;

            // case RS_HAVE_STARTBIT: and subsequent values
        default:
            rxshift = (rxshift >> 1) | (RxData << 7);
            skipoversamples = RX_OVERSAMPLE;
            receivestate++;
            break;

        case RS_WAIT_FOR_STOP:
            receivebuffer = rxshift;
            receivebufferfull = 1;
            uart_pc_receice(receivebuffer);
            //uart_soft_timer = 0;
            //uart_soft_enable = 1;
            //uart.module = UART_PC;
            //uart_receice(receivebuffer, uart.module); 
            receivestate = RS_HAVE_NOTHING;
            break;
        }
    }
    /*** TRANSMIT ***/
    /* This will be called every RX_OVERSAMPLEth time
     * (because the RECEIVE needs to over-sample the incoming
     * data). */
    if (send_bitno)
    {
        if ((send_bitno & (RX_OVERSAMPLE - 1)) == 0)
        {
            TxData = tx_next_bit; // Send next bit.
            tx_next_bit = sendbuffer & 1;
            sendbuffer = (sendbuffer >> 1) | 0x80;
        }
        send_bitno--;
    }
}
#endif