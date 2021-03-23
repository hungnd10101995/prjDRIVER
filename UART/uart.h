#ifndef _UART_H_
#define _UART_H_

//#define UART_TEST
//#define UART_TEST_RF
//#define UART_RF_HOST
//#define UART_RF_METER

#define  IDLE_STATE   0
typedef enum {UART_OPTICAL = 0x01, UART_PC = 0x02, UART_RFSLOT = 0x03, UART_GSMSLOT = 0x04} UART_PORT; //UART_DEVICE = 0x03

#define PREAMBLE1     0x55//0x66//0xAA
#define PREAMBLE2     0xFF
#define PREAMBLE3     0x68//0x66
#define PREAMBLE4     0x16

//baud
#define BAUD_115200 0x38
#define BAUD_14400 0x37
#define BAUD_19200 0x36
#define BAUD_9600  0x35
#define BAUD_4800  0x34
#define BAUD_2400  0x33
#define BAUD_1200  0x32
#define BAUD_600   0x31
#define BAUD_300   0x30//0x00//vuonglq-28/1/2019

//====================================================================================
extern void uart_init(void);
extern void uart_enable(void);
extern void uart_disable(void);

#ifdef COMMUNICATION_OPTICAL_SUPPORT
extern void uart_optical_init(void);
#endif
#ifdef COMMUNICATION_PC_SUPPORT
extern void uart_pc_init(void);
#endif
#ifdef COMMUNICATION_RFSLOT_SUPPORT
extern void uart_rfslot_init(void);
#endif
#ifdef COMMUNICATION_GSMSLOT_SUPPORT
extern void uart_gsmslot_init(void);
#endif

extern void send_message(int port, int len);
extern void reset_rf_uart(void);
//#if defined(__MSP430__) || defined(_18F86J94)
extern int config_baudrate(int uart, int value);
extern void uart_pc_process(void);
//#endif
extern void uart2_write(char c);
extern uint8_t uart_rx_frame_process(uint8_t *data);
extern void uart_send_byte(char uart_ch, uint8_t port);
extern void uart_transmit_buffer(uint8_t *buffer, uint8_t length, int port);
//void uart_pc_send_byte(char c);
//void uart_optical_send_byte(char c);
extern void UART0_init(void);
extern void UART1_init(void);
extern void UART1_send_byte(char c);
//extern void uart_send_string_numbers(uint64_t value, uint8_t port); 
extern void uart_task(void);
extern void uart_pc_rx_interrupt(void); //namhv 05/11/2019
#endif // _UART_H_