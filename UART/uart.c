#if defined(__XC8)
    #include <xc.h>
#endif
#include <stdint.h>
#if defined(__GNUC__)
#include <signal.h>
#endif

// Header prjEMETERS
#include "devices.h"
#include "main.h"
#include "hardware.h"

// Header prjLIB
#include "uart.h"
#include "convert.h"
#include "delay.h"
#include "eeprom.h"
#include "communication.h"
#include "emeter-display.h"
#include "timers.h"
#ifdef RF_SUPPORT
#include "cc1101.h"
#include "rf.h"
#endif


#ifdef DTP_SUPPORT
#include "dtp.h"
#endif
#ifdef IEC62056_21_EVN_SUPPORT
#include "iec62056-21.h"
#endif
#ifdef IEC62056_21_EMEC_SUPPORT
#include "iec62056-21-emec.h"
#endif
#ifdef __MSP430__
#include "..\communication\calibration.h"
#endif

////////////////////////////////////////////////////////////////////////////
/*   debug cac chuc nang ko duoc define    */
////////////////////////////////////////////////////////////////////////////
#ifndef TRANSFORMER_SUPPORT
#warning "undef TRANSFORMER_SUPPORT in uart.c"
#endif
#ifndef RTC_SUPPORT
#warning "undef RTC_SUPPORT in uart.c"
#endif
#ifndef IEC62056_21_EVN_SUPPORT
#warning "undef IEC62056_21_SUPPORT in uart.c"
#endif
#ifndef IEC62056_21_EMEC_SUPPORT
#warning "undef IEC62056_21_SUPPORT in uart.c"
#endif
#ifndef TARIFF_SUPPORT
#warning "undef TARIFF_SUPPORT in uart.c"
#endif
#ifndef DEFERRED_TARIFF_SUPPORT
#warning "undef DEFERRED_TARIFF_SUPPORT in uart.c"
#endif
#ifndef MAX_DEMAND_SUPPORT
#warning "undef MAX_DEMAND in uart.c"
#endif
#ifndef EVENT_SUPPORT
#warning "undef EVENT in uart.c"
#endif
#ifndef LOAD_PROFILE_SUPPORT
#warning "undef LOAD_PROFILE in uart.c"
#endif
#ifndef BILLING_SUPPORT
#warning "undef BILLING in uart.c"
#endif
#ifndef CUSTOMER_DEFINED_REGS_SUPPORT
#warning "undef CUSTOMER_DEFINED_REGS in uart.c"
#endif
#ifndef ALERT_SUPPORT
#warning "undef ALERT in uart.c"
#endif
#ifndef CONFIG_UART
#warning "undef CONFIG_UART in uart.c"
#endif
#ifndef RF_SUPPORT
#warning "undef RF in uart.c"
#endif
#ifndef WOR_SUPPORT
#warning "undef WOR in uart.c"
#endif
#ifndef LUU_XOAY_VONG_THANH_GHI_SUPPORT
#warning "undef LUU_XOAY_VONG_THANH_GHI in uart.c"
#endif

uint8_t uart1_preamble_count = 0;
uint8_t uart_preamble1 = FALSE;
uint8_t uart_preamble2 = FALSE;
uint8_t uart_preamble3 = FALSE;
////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 11/17/2014, by thaodtn
//  Description : ham ngat uart0 cho cong hong ngoai
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if (!defined(_DT01P_RF_) && !defined(_IHD_)) && defined(__MSP430__)
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
ISR(USCIAB0RX, serial_rx_interrupt0)
#elif defined(_DT01M_RF_)  || defined(_PREPAID_METER_)
ISR(USCI_A0, serial_interrupt0)
#else
#warning "uart.c : Check meter type define"
#endif
{
    char ch;
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
    ch = UCA0RXBUF;

#ifdef TEST_PCB
    UART0_send_byte(ch + 1);
    return;
#endif
#elif defined(_DT01M_RF_)  || defined(_PREPAID_METER_)
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE:
        break;
        ////////////////////////////////////////////////////////////////////////////
        /*   ngat RX    */
        ////////////////////////////////////////////////////////////////////////////
    case USCI_UART_UCRXIFG:             // RXIFG ngat RX
        ch = UCA0RXBUF; // doc thanh ghi
        #ifdef UART_TEST
        UART1_send_byte(ch);
        //UART0_send_byte(ch + 1);
        //return;
        #endif
#else
#warning "uart.c : Check meter type define"
#endif
        if (uart_rx_frame_process(&ch)== FALSE) return;
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef DTP_SUPPORT
        dtp_receice(ch, UART_OPTICAL);
        #endif 
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc sts    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
        uart_sts_receive(ch);
        #endif
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc iec62056-21    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef IEC62056_21_SUPPORT
        iec62056_optical_receive(ch);
        #endif
        
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
    }
#elif defined(_DT01M_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
    break;
    ////////////////////////////////////////////////////////////////////////////
    /*   ngat TX    */
    ////////////////////////////////////////////////////////////////////////////
case USCI_UART_UCTXIFG: break; // no interrupt

default:
    break;
}
}
#else
#warning "uart.c : Check meter type define"
#endif
#endif
#if defined(COMMUNICATION_OPTICAL_SUPPORT) && defined(__PIC24FJxxxGA410__)
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt()
{
    volatile unsigned char ch;
    display_anten(SHOW);
    ch = uart_optical_rx_data_register(); 
    
    if (uart_optical_error_interrupt_flag_status() || uart_optical_get_error()) 
    {
        uart_optical_rx_interrupt_flag_clear(); //tuantm
        uart_optical_error_interrupt_flag_clear();
        uart_optical_clear_all_error();
        uart_optical_rx_interupt_enable();
        return;
    }
/*#elif defined(_PIC16XX1919Y_)
void uart_optical_rx_interrupt(void)
{
    uint8_t ch;
#endif */
    if (uart_optical_rx_interrupt_flag_status() && uart_optical_rx_interrupt_status()) 
    {
        uart_optical_rx_interrupt_flag_clear();
//        if (uart_rx_frame_process(&ch)== FALSE) return;    
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef DTP_SUPPORT
        dtp_receice(ch, UART_OPTICAL);
        #endif 
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc sts    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
        uart_sts_receive(ch);
        #endif
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc iec62056-21    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef IEC62056_21_SUPPORT
        //iec62056_optical_receive(ch);
        iec62056_receive(ch, UART_OPTICAL);
        #endif
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 11/10/2014, by thaodtn
//  Description : ngat RX cua UART1 theo IEC62056-21, ngat UART giao tiep voi PC
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(__MSP430__)
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
ISR(USCIAB1RX, serial_rx_interrupt1)
#elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
ISR(USCI_A1, serial_interrupt1)
#else
#warning "uart.c : Check meter type define"
#endif
{
  char ch;
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
  ch = UCA1RXBUF;
#ifdef TEST_PCB
  UART1_send_byte(ch + 1);
  return;
#endif
  
  
#elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
  case USCI_NONE:
    UART1_send_byte('x');
    break;              // No interrupt
    ////////////////////////////////////////////////////////////////////////////
    /*   ngat RX    */
    ////////////////////////////////////////////////////////////////////////////
  case USCI_UART_UCRXIFG:             // RXIFG ngat RX
    ch = UCA1RXBUF;
    ////////////////////////////////////////////////////////////////////////////
    /*   debug    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef UART_TEST
    UART1_send_byte(ch + 1);
    return;
#endif
#else
#warning "uart.c : Check meter type define"
#endif
    //            UART1_send_byte(ch);
    //            if (uart_rx_frame_process(&ch)== FALSE) return;
    ////////////////////////////////////////////////////////////////////////////
    /*   giao thuc dtp    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef DTP_SUPPORT
    dtp_receice(ch, UART_PC);        
#endif        
    ////////////////////////////////////////////////////////////////////////////
    /*   giao thuc sts    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef STS_SUPPORT
    uart_sts_receive(ch);
#endif
    ////////////////////////////////////////////////////////////////////////////
    /*   giao thuc can chinh dlt645    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef CALIBRATION_SUPPORT
    if (emeter.flags.calib_enable == 1)
    {
#ifdef TWO_BUFFER_CALIB
      calib_buffer[ptr_write] = ch;
      if (++ptr_write >= CALIB_BUFFER_SIZE) ptr_write = 0;
#else
      dlt645_rx_byte(0, ch);
#endif           
    }
#endif
    ////////////////////////////////////////////////////////////////////////////
    /*   giao thuc IEC62056-21    */
    ////////////////////////////////////////////////////////////////////////////
#ifdef IEC62056_21_SUPPORT
    iec62056_receive(ch, UART_PC);
#endif //IEC62056_21_SUPPORT
#if defined(_DT03M_RF_) || defined(_DT03P_RF_)
  }  //het ham ngat RX
#elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
  break; //
  
  ////////////////////////////////////////////////////////////////////////////
  /*   ngat TX    */
  ////////////////////////////////////////////////////////////////////////////
case USCI_UART_UCTXIFG: // TXIFG ngat TX
  UCA1TXBUF = tx_msg[0].buf.uint8[tx_msg[0].ptr++]; //define lai thanh ghi chung nay UCA1TXBUF
  if (tx_msg[0].ptr >= tx_msg[0].len)
  {
    UCA1IE &= ~UCTXIE;
    tx_msg[0].ptr = 0;
    tx_msg[0].len = 0;
  }
  break;
  
default:
  break;
}
}
#else
#warning "uart.c : Check meter type define"
#endif
#endif //__MSP430__

#if defined(COMMUNICATION_PC_SUPPORT)&& !defined(__MSP430__)
#if defined(__PIC24FJxxxGA410__)
void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void) //_U1RXInterrupt
{
    volatile unsigned char ch;
    display_anten(SHOW);
    ch = uart_pc_rx_data_register();      
    if (uart_pc_error_interrupt_flag_status() || uart_pc_get_error()) 
    {
        uart_pc_rx_interrupt_flag_clear();
        uart_pc_error_interrupt_flag_clear();
        uart_pc_clear_all_error();
        uart_pc_rx_interupt_enable();
        return;
    }
#elif defined(_PIC16XX1919Y_)
void uart_pc_rx_interrupt(void)
{
    uint8_t ch;
#endif

    if (uart_pc_rx_interrupt_flag_status() && uart_pc_rx_interrupt_status()) 
    {
        uart_pc_rx_interrupt_flag_clear();
#if defined(_PIC16XX1919Y_)//namhv 03/01/2019
        ch = uart_pc_rx_data_register();
#endif
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        
        #ifdef TEST_HARDWARE
            ch = uart_pc_rx_data_register();
            flag_uart = TRUE;
            char_test = ch;
            LATDbits.LATD7 ^= 1;
//            uart_pc_rx_interupt_enable();
//            uart_pc_tx_interupt_enable();
        #endif
        #ifdef DTP_SUPPORT
        dtp_receice(ch, UART_PC);        
        #endif 
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc sts    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
        uart_sts_receive(ch);
        #endif            
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc IEC62056-21    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef IEC62056_21_SUPPORT
        iec62056_receive(ch, UART_PC);
        #endif //IEC62056_21_SUPPORT
        
    }
}

#endif

#if defined (COMMUNICATION_RFSLOT_SUPPORT) 
#if defined(__PIC24FJxxxGA410__)
void __attribute__((interrupt, auto_psv)) _U4RXInterrupt(void) 
{
    uint8_t ch;
    display_anten(SHOW);  
    ch = uart_rfslot_rx_data_register();
    if (uart_rfslot_error_interrupt_flag_status() || uart_rfslot_get_error()) 
    {
        uart_rfslot_rx_interrupt_flag_clear();
        uart_rfslot_error_interrupt_flag_clear();
        uart_rfslot_clear_all_error();
        uart_rfslot_rx_interupt_enable();
        return;
    }
    if (uart_rfslot_rx_interrupt_flag_status() && uart_rfslot_rx_interrupt_status()) 
    {
        uart_rfslot_rx_interrupt_flag_clear();
//        if (uart_rx_frame_process(&ch)== FALSE) return;  
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef DTP_SUPPORT
        dtp_receice(ch, UART_RFSLOT);        
        #endif        
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc sts    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
        uart_sts_receive(ch);
        #endif            
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc IEC62056-21    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef IEC62056_21_SUPPORT
        iec62056_receive(ch, UART_RFSLOT);
        #endif //IEC62056_21_SUPPORT
    }
}
#endif
#endif

#if defined (COMMUNICATION_GSMSLOT_SUPPORT) 
#if defined(__PIC24FJxxxGA410__)
void __attribute__((interrupt, auto_psv)) _U3RXInterrupt(void) 
{
    uint8_t ch;
    display_anten(SHOW);
    ch = uart_gsmslot_rx_data_register();
    if (uart_gsmslot_error_interrupt_flag_status() || uart_gsmslot_get_error()) 
    {
        uart_gsmslot_rx_interrupt_flag_clear();
        uart_gsmslot_error_interrupt_flag_clear();
        uart_gsmslot_clear_all_error();
        uart_gsmslot_rx_interupt_enable();
        return;
    }
    if (uart_gsmslot_rx_interrupt_flag_status() && uart_gsmslot_rx_interrupt_status()) 
    {
        uart_gsmslot_rx_interrupt_flag_clear();
//        if (uart_rx_frame_process(&ch)== FALSE) return;    
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef DTP_SUPPORT
        dtp_receice(ch, UART_GSMSLOT);        
        #endif        
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc sts    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
        uart_sts_receive(ch);
        #endif            
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc IEC62056-21    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef IEC62056_21_SUPPORT
        iec62056_receive(ch, UART_GSMSLOT);
        #endif //IEC62056_21_SUPPORT
    }
}
#endif
#endif

#if defined(_18F86J94)
//void __attribute__((interrupt, auto_psv)) _U3RXInterrupt(void) 
//{
//    uint8_t ch;
//    if (uart_pc_get_error()) 
//    {
//        uart_pc_rx_interrupt_flag_clear();
//        uart_pc_clear_all_error(); 
//        ch = uart_pc_rx_data_register();  // doc thanh ghi RCREG3 de xoa co FERR
//        uart_pc_rx_interupt_enable();
//        return;
//    }
//    ch = uart_pc_rx_data_register();    
//    if (uart_pc_rx_interrupt_flag_status() && uart_pc_rx_interrupt_status()) 
//    {
//        uart_pc_rx_interrupt_flag_clear();
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc dtp    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef DTP_SUPPORT
//        dtp_receice(ch, UART_PC);        
//        #endif        
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc sts    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef STS_SUPPORT
//        uart_sts_receive(ch);
//        #endif            
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc IEC62056-21    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef IEC62056_21_SUPPORT
//        iec62056_pc_receive(ch);
//        #endif //IEC62056_21_SUPPORT
//    }
//}
#endif
////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 11/10/2014, by thaodtn
//  Description : ngat RX cua UART2 cho rf uart
//  Input	:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
//#if defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
#ifdef DTP_DEVICE_COMMUNICATION
#ifdef __MSP430__
ISR(USCI_A2, serial_interrupt2)
{
    char ch;
    switch(__even_in_range(UCA2IV,USCI_UART_UCTXCPTIFG))
    {
        case USCI_UART_UCRXIFG:
            ch = UCA2RXBUF;
            ////////////////////////////////////////////////////////////////////////////
            /*   debug    */
            ////////////////////////////////////////////////////////////////////////////
        #ifdef UART_TEST
            UART2_send_byte(ch + 1);
            return;
        #endif
            ////////////////////////////////////////////////////////////////////////////
            /*   giao thuc dtp    */
            ////////////////////////////////////////////////////////////////////////////
        #ifdef DTP_SUPPORT
            dtp_receice(ch, UART_DEVICE);        
        #endif        
            ////////////////////////////////////////////////////////////////////////////
            /*   giao thuc sts    */
            ////////////////////////////////////////////////////////////////////////////
        #ifdef STS_SUPPORT
            uart_sts_receive(ch);
        #endif
        ////////////////////////////////////////////////////////////////////////////
        /*   giao thuc dtp    */
        ////////////////////////////////////////////////////////////////////////////
        #ifdef RELAY_SUPPORT //DTP_SUPPORT
             dtp_device_receice(UCA2RXBUF);
             //just for test dtp_transmit_byte(UCA2RXBUF, DTP_DEVICE);
        #endif
            ////////////////////////////////////////////////////////////////////////////
            /*   giao thuc IEC62056-21    */
            ////////////////////////////////////////////////////////////////////////////             
        #ifdef IEC62056_21_SUPPORT
            iec62056_receive(ch,UART_DEVICE);
        #endif //IEC62056_21_SUPPORT
            break;
        default:
            break;
    }
}
#else

#endif

#endif // DTP_DEVICE_COMMUNICATION
////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 11/28/2014, by thaodtn
//  Description : ham ngat TX uart1
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__MSP430__)
ISR(USCIAB1TX, serial_tx_interrupt1)
{
    UCA1TXBUF = tx_msg[0].buf.uint8[tx_msg[0].ptr++];
    if (tx_msg[0].ptr >= tx_msg[0].len)
    {
        /* Stop transmission */
        UC1IE &= ~UCA1TXIE;
        tx_msg[0].ptr = 0;
        tx_msg[0].len = 0;
    }
}
#elif defined(_DT01M_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
//ngat TX cua 1 pha nam chung trong ham ngat o tren
#else
#warning "uart.c : Check meter type define"
#endif

void uart_init(void)
{
#ifdef COMMUNICATION_OPTICAL_SUPPORT
    uart_optical_init();
    config_baudrate(UART_OPTICAL, BAUD_300); 
#endif
#ifdef COMMUNICATION_PC_SUPPORT
    uart_pc_init();
    #if defined(UART_PC_BAUDRATE_300)
    config_baudrate(UART_PC, BAUD_300);
    #else
    config_baudrate(UART_PC, BAUD_9600);
    #endif
#endif
#ifdef COMMUNICATION_RFSLOT_SUPPORT
    uart_rfslot_init();
    #if defined(UART_RFSLOT_BAUDRATE_300)
    config_baudrate(UART_RFSLOT, BAUD_300);
    #else  
    config_baudrate(UART_RFSLOT, BAUD_9600);
    #endif
#endif
#ifdef COMMUNICATION_GSMSLOT_SUPPORT
    uart_gsmslot_init();
    #if defined(UART_GSMSLOT_BAUDRATE_300)
    config_baudrate(UART_GSMSLOT, BAUD_300);
    #else    
    config_baudrate(UART_GSMSLOT, BAUD_9600);    
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 
//  Description : Uart 0 - uart giao tiep cong quang
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#ifdef COMMUNICATION_OPTICAL_SUPPORT
void uart_optical_init(void)
{
#if defined(__MSP430__)
    UART0_INTR_REG set UART0_INTR_RX; //quy hoach lai
    UART0_init();
#elif defined(__PIC24FJxxxGA410__)
    uart_optical_rx_dir_input();  //dinh nghia trong hardware.h
    uart_optical_rx_select_pin();
    
    uart_optical_tx_dir_output();
    uart_optical_tx_select_pin();
    
    //cau hinh toc do baud    
    #if OPTICAL_DESIRED_BAUDRATE == 300
        UART_OPTICAL_BAUDRATE_REG = 1535;    
    #endif
    #if OPTICAL_DESIRED_BAUDRATE == 9600
        UART_OPTICAL_BAUDRATE_REG = 47; //9600
    #endif
    #if OPTICAL_DESIRED_BAUDRATE == 19200
        UART_OPTICAL_BAUDRATE_REG = 23; //19200    
    #endif
    #if OPTICAL_DESIRED_BAUDRATE == 57600
        UART_OPTICAL_BAUDRATE_REG = 7;    
    #endif   
    #if OPTICAL_DESIRED_BAUDRATE == 38400
        UART_OPTICAL_BAUDRATE_REG = 11;    
    #endif        
    uart_optical_level_interrupt(PRIORITY_INT_LEVEL_UART);   
    uart_optical_enable();
    uart_optical_transmit_enable();
    //enable ngat
    unsigned char tmp = uart_optical_rx_data_register(); //khac phuc tinh trang treo uart forever
    uart_optical_rx_interrupt_flag_clear();
    uart_optical_rx_interupt_enable();  
#elif defined(__PIC18__) //...
#elif defined(__PIC32__) //...
#else
#error "No MCU support"
#endif
}
#endif

#ifdef COMMUNICATION_PC_SUPPORT
void uart_pc_init(void)
{
#if defined(__MSP430__)
#if defined(_DT01M_RF_) || defined(_DT01P_RF_)
  UART1_init();
#else
#endif
  
    UART1_INTR_REG set UART1_INTR_RX; //quy hoach lai
    UART1_init();
#elif defined(__PIC24FJxxxGA410__)
    uart_pc_rx_dir_input();  //dinh nghia trong hardware.h
    uart_pc_rx_select_pin();
    
    uart_pc_tx_dir_output();
    uart_pc_tx_select_pin();
    
    //cau hinh toc do baud    
    #if PC_DESIRED_BAUDRATE == 300
        UART_PC_BAUDRATE_REG = 1535;    
    #endif
    #if PC_DESIRED_BAUDRATE == 9600
        UART_PC_BAUDRATE_REG = 47; //9600
    #endif
    #if PC_DESIRED_BAUDRATE == 19200
        UART_PC_BAUDRATE_REG = 23; //19200    
    #endif
    #if PC_DESIRED_BAUDRATE == 57600
        UART_PC_BAUDRATE_REG = 7;    
    #endif   
    #if PC_DESIRED_BAUDRATE == 38400
        UART_PC_BAUDRATE_REG = 11;    
    #endif        
    uart_pc_level_interrupt(PRIORITY_INT_LEVEL_UART);
    uart_pc_enable();
    uart_pc_transmit_enable();
    //enable ngat
    unsigned char tmp = uart_pc_rx_data_register(); //khac phuc tinh trang treo uart forever
    uart_pc_rx_interrupt_flag_clear();
    uart_pc_rx_interupt_enable();
#elif defined(_PIC16XX1919Y_)//namhv 30/10/2019 
    uart_1_init();
    uart_pc_enable();
    uart_pc_transmit_enable();
    //enable ngat
    unsigned char tmp = uart_pc_rx_data_register(); //khac phuc tinh trang treo uart forever
    uart_pc_rx_interrupt_flag_clear();
    uart_pc_rx_interupt_enable();
#elif defined(__PIC18__) //...
#elif defined(__PIC32__) //...
#else
#error "No MCU support"
#endif
}
#endif

#ifdef COMMUNICATION_RFSLOT_SUPPORT
void uart_rfslot_init(void)
{
#if defined(__MSP430__)
    UART1_INTR_REG set UART1_INTR_RX; //quy hoach lai
#elif defined(__PIC24FJxxxGA410__)
    uart_rfslot_rx_dir_input();  //dinh nghia trong hardware.h
    uart_rfslot_rx_select_pin();
    
    uart_rfslot_tx_dir_output();
    uart_rfslot_tx_select_pin();
        
    //cau hinh toc do baud    
    #if RFSLOT_DESIRED_BAUDRATE == 300
        UART_RFSLOT_BAUDRATE_REG = 1535;    
    #endif
    #if RFSLOT_DESIRED_BAUDRATE == 9600
        UART_RFSLOT_BAUDRATE_REG = 47; //9600
    #endif
    #if RFSLOT_DESIRED_BAUDRATE == 19200
        UART_RFSLOT_BAUDRATE_REG = 23; //19200    
    #endif
    #if RFSLOT_DESIRED_BAUDRATE == 57600
        UART_RFSLOT_BAUDRATE_REG = 7;    
    #endif   
    #if RFSLOT_DESIRED_BAUDRATE == 38400
        UART_RFSLOT_BAUDRATE_REG = 11;    
    #endif        
    uart_rfslot_level_interrupt(PRIORITY_INT_LEVEL_UART);
    uart_rfslot_enable();
    uart_rfslot_transmit_enable();
    //enable ngat
    unsigned char tmp = uart_rfslot_rx_data_register(); //khac phuc tinh trang treo uart forever
    uart_rfslot_rx_interrupt_flag_clear();
    uart_rfslot_rx_interupt_enable();  
#elif defined(__PIC18__) //...
#elif defined(__PIC32__) //...
#else
#error "No MCU support"
#endif
}
#endif

#ifdef COMMUNICATION_GSMSLOT_SUPPORT
void uart_gsmslot_init(void)
{
#if defined(__MSP430__)
    UART1_INTR_REG set UART1_INTR_RX; //quy hoach lai
#elif defined(__PIC24FJxxxGA410__)
    uart_gsmslot_rx_dir_input();  //dinh nghia trong hardware.h
    uart_gsmslot_rx_select_pin();
  
    uart_gsmslot_tx_dir_output();
    uart_gsmslot_tx_select_pin();
    
    //cau hinh toc do baud    
    #if GSMSLOT_DESIRED_BAUDRATE == 300
        UART_GSMSLOT_BAUDRATE_REG = 1535;    
    #endif
    #if GSMSLOT_DESIRED_BAUDRATE == 9600
        UART_GSMSLOT_BAUDRATE_REG = 47; //9600
    #endif
    #if GSMSLOT_DESIRED_BAUDRATE == 19200
        UART_GSMSLOT_BAUDRATE_REG = 23; //19200    
    #endif
    #if GSMSLOT_DESIRED_BAUDRATE == 57600
        UART_GSMSLOT_BAUDRATE_REG = 7;    
    #endif   
    #if GSMSLOT_DESIRED_BAUDRATE == 38400
        UART_GSMSLOT_BAUDRATE_REG = 11;    
    #endif        
    uart_gsmslot_level_interrupt(PRIORITY_INT_LEVEL_UART);    
    uart_gsmslot_enable();
    uart_gsmslot_transmit_enable();
    //U4STAbits.URXEN = 1;
    //enable ngat
    unsigned char tmp = uart_gsmslot_rx_data_register(); //khac phuc tinh trang treo uart forever
    uart_gsmslot_rx_interrupt_flag_clear();
    uart_gsmslot_rx_interupt_enable();  
#elif defined(__PIC18__) //...
#elif defined(__PIC32__) //...
#else
#error "No MCU support"
#endif
}
#endif

void uart_enable(void)
{
#ifdef COMMUNICATION_OPTICAL_SUPPORT
    uart_optical_enable();
    uart_optical_transmit_enable();
    uart_optical_rx_interrupt_flag_clear();
    uart_optical_rx_interupt_enable();  
#endif
#ifdef COMMUNICATION_PC_SUPPORT
    uart_pc_enable();
    uart_pc_transmit_enable();
    uart_pc_rx_interrupt_flag_clear();
    uart_pc_rx_interupt_enable();  
#endif
#ifdef COMMUNICATION_RFSLOT_SUPPORT
    uart_rfslot_enable();
    uart_rfslot_transmit_enable();
    uart_rfslot_rx_interrupt_flag_clear();
    uart_rfslot_rx_interupt_enable();  
#endif
#ifdef COMMUNICATION_GSMSLOT_SUPPORT
    uart_gsmslot_enable();
    uart_gsmslot_transmit_enable();
    uart_gsmslot_rx_interrupt_flag_clear();
    uart_gsmslot_rx_interupt_enable();  
#endif
}

void uart_disable(void)
{
#ifdef COMMUNICATION_OPTICAL_SUPPORT
    uart_optical_disable();
    uart_optical_rx_interupt_disable();
#endif
#ifdef COMMUNICATION_PC_SUPPORT
    uart_pc_disable();
    uart_pc_rx_interupt_disable();
#endif
#ifdef COMMUNICATION_RFSLOT_SUPPORT
    uart_rfslot_disable();
    uart_rfslot_rx_interupt_disable();
#endif
#ifdef COMMUNICATION_GSMSLOT_SUPPORT
    uart_gsmslot_disable();
    uart_gsmslot_rx_interupt_disable();
#endif
}
//////////////////////////////////////////////////////////////////////////
//	Function :	uart_receice
//	Comments :  writes a character to the serial port
//	Input	 :	none
//	Output   : 	none
//////////////////////////////////////////////////////////////////////////
uint8_t uart_rx_frame_process(uint8_t *data)
{
#ifdef UART_CONFIG_7_EVEN_1
  uint8_t parity = 0;
  uint8_t i;
if(iec62056.flags.check_parity ==TRUE)
{
    for( i = 1;i<8;i++)
    {
        parity ^= (((*data) << i)&0x80);
    }
    if(((*data)&0x80)!= (parity&0x80)) return FALSE;
}
     (*data)  = (*data) clr BIT7;
    return TRUE;
#else
    return TRUE;
#endif
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	Function :	uart_tx_frame_process
//	Comments :  writes a character to the serial port
//	Input	 :	none
//	Output   : 	none
//////////////////////////////////////////////////////////////////////////
uint8_t uart_tx_frame_process(uint8_t data)
{
#ifdef UART_CONFIG_7_EVEN_1
    uint8_t parity = 0;
    for(uint8_t i = 1;i<8;i++)
    {
        parity ^= ((data << i)&0x80);
    }
    return(data| (parity&0x80));
//     return (((((((data)^(((data)<<4)|((data)>>4))) + 0x41) | 0x7C ) +2 ) & 0x80)>>7);
#else
    return data;
#endif
}
//////////////////////////////////////////////////////////////////////////

#ifdef RF_UART
void reset_rf_uart(void)
{
    rx_msg[0].ptr = 0;
    uart_preamble1 = FALSE;
    uart_preamble2 = FALSE;
    uart_preamble3 = FALSE;
}
#endif

void UART0_send_byte(char c)
{
#if defined(__MSP430__)    
    while (!(UART0_FLAG_REG & UART0_FLAG_TX));
    UART0_BUFFER_TX  = c;
    while (!(UART0_FLAG_REG & UART0_FLAG_TX));
#elif defined(__PIC24FJxxxGA410__)
    uart_optical_send_byte(c);
#endif
}

void UART1_send_byte(char c)
{
#if defined(__MSP430__)      
    while (!(UART1_FLAG_REG & UART1_FLAG_TX));
    UART1_BUFFER_TX  = c;
    while (!(UART1_FLAG_REG & UART1_FLAG_TX));
#elif defined(__PIC24FJxxxGA410__) || defined(_18F86J94)
    uart_pc_send_byte(c);
#endif
}

#if defined(DTP_DEVICE_COMMUNICATION) || defined(_DT01PP_RF_)
void UART2_send_byte(char c)
{
    #if defined(__MSP430__)  
    while (!(UART2_FLAG_REG & UART2_FLAG_TX));
    UART2_BUFFER_TX  = c;
    while (!(UART2_FLAG_REG & UART2_FLAG_TX));
    #else
    uart_device_send_byte(c);
    #endif
}
#endif

void uart_send_byte(char uart_ch, uint8_t port)
{
#ifdef DISPLAY_SUPPORT
    display_anten(SHOW);
#endif
    uart_ch = uart_tx_frame_process(uart_ch);
    switch(port)
    {
        #if defined(COMMUNICATION_PC_SUPPORT)
        case UART_PC:
            uart_pc_send_byte(uart_ch);
            break;
        #endif
        #if defined(COMMUNICATION_OPTICAL_SUPPORT)
        case UART_OPTICAL:
            uart_optical_send_byte(uart_ch);
            break;
        #endif
        #if defined(COMMUNICATION_RFSLOT_SUPPORT)
        case UART_RFSLOT:
            uart_rfslot_send_byte(uart_ch);
            break;
        #endif
        #if defined(COMMUNICATION_GSMSLOT_SUPPORT)
        case UART_GSMSLOT:
            uart_gsmslot_send_byte(uart_ch);
            break;
        #endif
        default: 
            
            break;
    }
#ifdef DISPLAY_SUPPORT
    display_anten(HIDE);
#endif    
}

//void uart_send_string_numbers(uint64_t value, uint8_t port) 
//{
//    int len = 0;
//    uint64_t value1 = value;
//    char chr = 0;
//    char temp = 0;
//    char array[14];
//    int i = 0;
//    for(i = 1; i < 14; i++)
//    {
//        temp = value1 % 10;
//        chr = temp + 0x30;
//        uart_send_byte(chr, port);
//        value1 /= 10;
//        if (value1 == 0) //het chu so thi thoat
//        {
//            i = 14;
//        }
//    }
//}

void uart2_write(char c)
{
#if defined(_DT01P_RF_) || defined(_DT01M_RF_)
    #if defined(__MSP430__)
    while (!(UCA2IFG&UCTXIFG));
    UCA2TXBUF  = c;
    #endif
#endif
}
/*==============================================================================
FUNCTION NAME: config_baudrate
PROTOTYPE: void config_baudrate(int uart, int value)
INPUT: uart: chon uart nao?
value: toc do baud (theo IEC62056-21)
OUTPUT: true = 1 <=> cau hinh duoc
false = 0 <=> cau hinh khong dc, sai value
DESCRIPTION: cau hinh baudrate
NOTE: su dung trong ham check_indentification()
COMMENT: http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection
===============================================================================*/
#if defined(__MSP430__)
int config_baudrate(int uart, int value)
{
    switch (value)
    {
        case BAUD_300:
            switch(uart)
            {
                case UART_OPTICAL:
//                    UCA0CTL1 &= ~UCSSEL_0;
//                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
//        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
//                    UCA0BR1 = 208;
//                    UCA0BR0 = 85;
//                    UCA0MCTL = UCBRS_2 + UCBRF_0;//2
//        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
//                    UCA0BR1 = 216;
//                    UCA0BR0 = 191;
//                    UCA0MCTLW = UCBRS1 + UCBRF_0;
//                    UCA0CTL1 &= ~UCSWRST;
//                    UCA0IE |= UCRXIE;  
//		#else
//        #warning "check meter type define in uart.c"
//        #endif
                  //============
                    /* BRCLK source clock = SMCLK = 16MHz */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0CTL1 &= ~UCSSEL0;
                    UCA0CTL1 |= UCSSEL1;

                    UCA0CTL1 |= UCSWRST;
                    UCA0BR1 = 208;
                    UCA0BR0 = 85;
                    //kiem tra lai cau hinh UCA0MCTL
                    UCA0MCTL = UCBRS_2 + UCBRF_0;//2
                    ///////////////////////////
                    UCA0MCTL = UCBRS_6 + UCBRF_0;
                    UCA0CTL1 &= ~UCSWRST;
                    UC0IE |= UCA0RXIE;
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    //thaodtn : chua sua xong
                    UCA0CTL1 |= UCSWRST;
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
                    // UCA0CTL1 |= UCSSEL__SMCLK;                     // UCLK = ACLK
                    UCA0BR1 = 216;
                    UCA0BR0 = 191;
                    UCA0MCTLW = UCBRS1 + UCBRF_0;
                    //                UCA0MCTLW = UCBRS5;//0x5300;
                    //                UCA0MCTLW = UCBRF_6 | UCOS16;
                    UCA0CTL1 &= ~UCSWRST;
                    UCA0IE |= UCRXIE;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    //============
                    
                break;   
                //=======================================================
                case UART_PC:
//                    UCA1CTL1 &= ~UCSSEL_0;
//                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
//        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
//                    UCA1BR1 = 208;
//                    UCA1BR0 = 85;
//                    UCA1MCTL = UCBRS_2 + UCBRF_0;//2
//        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
//                    UCA1BR1 = 216;
//                    UCA1BR0 = 191;
//                    UCA1MCTLW = UCBRS1 + UCBRF_0;
//                    UCA1CTL1 &= ~UCSWRST;
//                    UCA1IE |= UCRXIE;
//        #else
//        #warning "check meter type define in uart.c"
//        #endif      
                  //============
                    /* BRCLK source clock = SMCLK = 16MHz */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1CTL1 &= ~UCSSEL0;
                    UCA1CTL1 |= UCSSEL1;

                    UCA1CTL1 |= UCSWRST;
                    UCA1BR1 = 208;
                    UCA1BR0 = 85;
                    UCA1MCTL = UCBRS_2 + UCBRF_0;//2
                    UCA1MCTL = UCBRS_6 + UCBRF_0;
                    UCA1CTL1 &= ~UCSWRST;
                    UC1IE |= UCA1RXIE;
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)                 
                    UCA1CTL1 |= UCSWRST;
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL__SMCLK;                     // UCLK = ACLK
                    UCA1BR1 = 216;//0;
                    UCA1BR0 = 191;//108;
                    UCA1MCTLW = UCBRS1 + UCBRF_0;//UCBRS0 + UCBRF_6 + UCOS16;
                    //                UCA0MCTLW = UCBRS5;//0x5300;
                    //                UCA0MCTLW = UCBRF_6 | UCOS16;
                    UCA1CTL1 &= ~UCSWRST;
                    UCA1IE |= UCRXIE;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    //============
                    break;
                //=======================================================  
    #if defined(DTP_DEVICE_COMMUNICATION)
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 208;
                    UCA2BR0 = 85;
                    UCA2MCTL = UCBRS_2 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 216;
                    UCA2BR0 = 191;
                    UCA2MCTLW = UCBRS1 + UCBRF_0;
                    UCA1CTL1 &= ~UCSWRST;
                    UCA1IE |= UCRXIE;
        #else
        #warning "check meter type define in uart.c"
        #endif                       
                    break;
    #endif
                default:
                    return FALSE;
            }
            break;
        case BAUD_600:
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 104;
                    UCA0BR0 = 42;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 108;
                    UCA0BR0 = 95;
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    //===============================================
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 104;
                    UCA1BR0 = 42;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 108;
                    UCA1BR0 = 95;
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    //=============================================
        #if defined(DTP_DEVICE_COMMUNICATION)            
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
            #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 104;
                    UCA2BR0 = 42;
                    UCA2MCTL = UCBRS_6 + UCBRF_0;//2
            #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 108;
                    UCA2BR0 = 95;
            #else
            #warning "check meter type define in uart.c"
            #endif                
                    break;
        #endif
                    //===========================================
                default:
                    return FALSE;
            }
            break;
        case BAUD_1200:
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 52;
                    UCA0BR0 = 21;
                    UCA0MCTL = UCBRS_3 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 54;
                    UCA0BR0 = 47;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    //=====================================
                    break;   
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 52;
                    UCA1BR0 = 21;
                    UCA0MCTL = UCBRS_3 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 54;
                    UCA1BR0 = 47;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                    //===============================
        #if defined(DTP_DEVICE_COMMUNICATION)            
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
            #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 52;
                    UCA2BR0 = 21;
                    UCA2MCTL = UCBRS_3 + UCBRF_0;//2
            #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 54;
                    UCA2BR0 = 47;
            #else
            #warning "check meter type define in uart.c"
            #endif
                    break;
        #endif
                    //===============================
                default:
                    return FALSE;
            }
            break;
        case BAUD_2400: //2400
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 26;
                    UCA0BR0 = 10;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 27;
                    UCA0BR0 = 23;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                    //==============================
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 26;
                    UCA1BR0 = 10;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 27;
                    UCA1BR0 = 23;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                    //===============================
        #if defined(DTP_DEVICE_COMMUNICATION)            
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
            #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 26;
                    UCA2BR0 = 10;
                    UCA2MCTL = UCBRS_6 + UCBRF_0;//2
            #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 27;
                    UCA2BR0 = 23;
            #else
            #warning "check meter type define in uart.c"
            #endif
                    break;
        #endif
                default:
                    return FALSE;
            }
            break;
        case BAUD_4800://4800
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 13;
                    UCA0BR0 = 5;
                    UCA0MCTL = UCBRS_3 + UCBRF_0;// + UCOS16;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 0;
                    UCA0BR0 = 216;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 13;
                    UCA1BR0 = 5;
                    UCA0MCTL = UCBRS_3 + UCBRF_0;// + UCOS16;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 0;
                    UCA1BR0 = 216;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
            #if defined(DTP_DEVICE_COMMUNICATION)     
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
                    #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 13;
                    UCA2BR0 = 5;
                    UCA2MCTL = UCBRS_3 + UCBRF_0;// + UCOS16;//2
                    #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 0;
                    UCA2BR0 = 216;
                    #else
                    #warning "check meter type define in uart.c"
                    #endif                
                    break;
            #endif
                default:
                    return FALSE;
            }
            break;
        case BAUD_9600: //ok
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
            #ifdef CLOCK_12MHz
                    UCA0BR1 = 0x04;
                    UCA0BR0 = 0xE8;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;// + UCOS16;//2
            #else
                    UCA0BR1 = 6;
                    UCA0BR0 = 130;
                    UCA0MCTL = UCBRS_6 + UCBRF_0;// + UCOS16;//2
            #endif
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 0x00;
                    UCA0BR0 = 108;
                    UCA0MCTLW = UCBRS0 + UCBRF_6 + UCOS16;
                    UCA0CTL1 &= ~UCSWRST;
                    UCA0IE |= UCRXIE;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                    //========================
                case UART_PC:
                    /* BRCLK source clock = SMCLK = 16MHz */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1CTL1 &= ~UCSSEL0;
                    UCA1CTL1 |= UCSSEL1;

                    UCA1CTL1 |= UCSWRST;
            #ifdef CLOCK_12MHz
                    UCA1BR1 = 0x04;
                    UCA1BR0 = 0xE8;
            #else
                    UCA1BR1 = 6;
                    UCA1BR0 = 130;
            #endif
                    UCA1MCTL = UCBRS_6 + UCBRF_0;
                    UCA1CTL1 &= ~UCSWRST;
                    UC1IE |= UCA1RXIE;
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    //thaodtn : chua sua xong
                    UCA1CTL1 |= UCSWRST;
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL__SMCLK;                     // UCLK = ACLK
                    UCA1BR1 = 0;
                    UCA1BR0 = 108;
                    UCA1MCTLW = UCBRS0 + UCBRF_6 + UCOS16;
                    //                UCA0MCTLW = UCBRS5;//0x5300;
                    //                UCA0MCTLW = UCBRF_6 | UCOS16;
                    UCA1CTL1 &= ~UCSWRST;
                    UCA1IE |= UCRXIE;
        #else
        #warning "check meter type define in uart.c"
        #endif
                    break;
                    //============================
        #ifdef DTP_DEVICE_COMMUNICATION            
                case UART_DEVICE:
                    #if defined(_DT01M_RF_)|| defined (_DT01P_RF_) || defined(_IHD_)
                        UCA2CTL1 |= UCSWRST;
                        //9600bps
                        UCA2CTL1 |= UCSSEL__SMCLK;
                        UCA2BR1 = 0x00;
                        UCA2BR0 = 108;
                        UCA2MCTLW = UCBRS0 + UCBRF_6 + UCOS16;
                        UCA2CTL1 &= ~UCSWRST;
                        UCA2IE |= UCRXIE;
                    #elif defined(_PREPAID_METER_)

                        UCA2CTL1 |= UCSWRST;
                        UCA2CTL1 |= UCSSEL__ACLK;                     /* UCLK = ACLK */

                        UCA2BR1 = 0x0;
                        UCA2BR0 = 0x3;
                        UCA2MCTLW_H = 0x92;
                        UCA2CTL1 &= ~UCSWRST;
                        UCA2IE |= UCRXIE;

                    #elif defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    #else
                    #warning "uart.c : Check meter type define"
                    #endif
                    break;
          #endif
                    //========================================
                default:
                    return FALSE;
            }
            break;
        case BAUD_19200: //19200
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 3;
                    UCA0BR0 = 65;
                    UCA0MCTL = UCBRS_2 + UCBRF_0;// + UCOS16;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA0BR1 = 0;
                    UCA0BR0 = 54;
                    //thaodtn : chua sua xong
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 3;
                    UCA1BR0 = 65;
                    UCA1MCTL = UCBRS_2 + UCBRF_0;
                    //UCA1MCTL = UCBRS_0 + UCBRF_3 + UCOS16;
                    UC1IE |= UCA1RXIE;
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 0;
                    UCA1BR0 = 54;
                    //thaodtn : chua sua xong
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    
            #ifdef DTP_DEVICE_COMMUNICATION           
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
                #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 3;
                    UCA2BR0 = 65;
                    UCA2MCTL = UCBRS_2 + UCBRF_0;
                    //UCA1MCTL = UCBRS_0 + UCBRF_3 + UCOS16;
                    UC2IE |= UCA1RXIE;
                #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 0;
                    UCA2BR0 = 54;
                    //thaodtn : chua sua xong
                #else
                #warning "check meter type define in uart.c"
                #endif                
                    break;
            #endif
                    
                default:
                    return FALSE;
            }
            break;
        case BAUD_14400:
            switch(uart)
            {
                case UART_OPTICAL:
                    UCA0CTL1 &= ~UCSSEL_0;
                    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA0BR1 = 4;
                    UCA0BR0 = 87;
                    UCA0MCTL = UCBRS_1 + UCBRF_0;//+ UCOS16;//2
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
                    UCA0BR1 = 0;
                    UCA0BR0 = 72;
                    //thaodtn : chua sua xong
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    
                case UART_PC:
                    UCA1CTL1 &= ~UCSSEL_0;
                    UCA1CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1BR1 = 4;
                    UCA1BR0 = 87;
                    UCA1MCTL = UCBRS_1 + UCBRF_0;
                    //UCA1MCTL = UCBRS_0 + UCBRF_4 + UCOS16;
                    UC1IE |= UCA1RXIE;
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA1BR1 = 0;
                    UCA1BR0 = 72;
                    //thaodtn : chua sua xong
        #else
        #warning "check meter type define in uart.c"
        #endif                
                    break;
                    
           #ifdef DTP_DEVICE_COMMUNICATION             
                case UART_DEVICE:
                    UCA2CTL1 &= ~UCSSEL_0;
                    UCA2CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
                    #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA2BR1 = 4;
                    UCA2BR0 = 87;
                    UCA2MCTL = UCBRS_1 + UCBRF_0;
                    //UCA1MCTL = UCBRS_0 + UCBRF_4 + UCOS16;
                    UC2IE |= UCA1RXIE;
                    #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_)
                    UCA2BR1 = 0;
                    UCA2BR0 = 72;
                    //thaodtn : chua sua xong
                    #else
                    #warning "check meter type define in uart.c"
                    #endif                          
                    break;
            #endif
                default:
                    return FALSE;
            }
            break;
            case BAUD_115200:
                switch(uart)
                {
                    case UART_OPTICAL:
                        break;
                        
                    case UART_PC:
        #if defined(_DT03M_RF_) || defined(_DT03P_RF_)
                    UCA1CTL1 &= ~UCSSEL0;
                    UCA1CTL1 |= UCSSEL1;
                    UCA1CTL1 |= UCSWRST;

                    UCA1BR1 = 0;
                    UCA1BR0 = 138;

                    UCA1MCTL = UCBRS_7 + UCBRF_0;
                    UCA1CTL1 &= ~UCSWRST;
                    UC1IE |= UCA1RXIE;    
        #elif defined(_DT01M_RF_) || defined(_DT01P_RF_) || defined(_PREPAID_METER_)
        #else
        #warning "check meter type define in uart.c"
        #endif               
                        break;
                        
            #ifdef DTP_DEVICE_COMMUNICATION              
                    case UART_DEVICE:
                        break;
            #endif
                    default:
                        return FALSE;
                }
            break;
        default:
            return FALSE;
            //break;
    }
#ifdef UART_CONFIG_7_EVEN_1
    #if (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__MSP430__)
    switch (uart)
    {
        case UART_OPTICAL:
//        UCA0CTL0 |= UC7BIT;
//        UCA0CTL0 |= (UCPEN | UCPAR);
//        UCA0CTL0 &= ~UCSPB;
        break;
        case UART_PC:
//        UCA1CTL0 |= UC7BIT;
//        UCA1CTL0 |= (UCPEN | UCPAR);
//        UCA1CTL0 &= ~UCSPB;
        break;
    }
    #elif (defined(_DT01M_RF_) || defined(_DT01P_RF_)) && defined(__MSP430__)
    switch (uart)
    {
        case UART_OPTICAL:
//            UCA0CTLW0 |= UCPEN | UCPAR | UC7BIT; 
//            UCA0CTLW0 &= ~UCSPB;
            break;
        case UART_PC:
//            UCA1CTLW0 |= UCPEN ;
//            UCA1CTLW0 |= (UCPAR | UC7BIT);
//            UCA1CTLW0 &= ~UCSPB;
            break;
    }
    #elif (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__PIC24FJxxxGA410__)
    #error "them code"
    #endif
#endif
    return TRUE;
}
#elif defined(_18F86J94)
int config_baudrate(int uart, int value)
{
    switch(value)
    {
        case BAUD_9600:
            switch(uart)
            {
                case UART_PC:
                    #if _XTAL_FREQ == 20000000
                    uart_pc_rx_baudrate_register() = 129;//129;                                     // 9600 20Mhz
                    #elif _XTAL_FREQ == 10000000
                    uart_pc_rx_baudrate_register() = 64;                                     // 9600 10Mhz
                    #endif
                    break;
                case UART_DEVICE:
                    #if _XTAL_FREQ == 20000000
                    uart_device_rx_baudrate_register() = 129;//129;                                     // 9600 20Mhz
                    #elif _XTAL_FREQ == 10000000
                    uart_pc_rx_baudrate_register() = 64;                                     // 9600 10Mhz
                    #endif
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
#elif defined(__PIC24FJxxxGA410__)
int config_baudrate(int uart, int value)
{
    //uart = 0: optical; uart = 1: pc
    switch (value)
    {
        case BAUD_300: 
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 1535;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 1535;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 1535;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 1535;
            #endif
            break;
        case BAUD_600:  
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 767;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 767; 
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 767;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 767;
            #endif
            break;
        case BAUD_1200:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 383;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 383;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 383;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 383;
            #endif
            break;
        case BAUD_2400:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 191;
            #endif
#ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 191;
#endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 191;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 191;
            #endif
            break;
        case BAUD_4800:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 95;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 95;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 95;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 95;
            #endif
            break;
        case BAUD_9600:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 47;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 47;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 47;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 47;
            #endif
            break;
        case BAUD_19200:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 23;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 23;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 23;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 23;
            #endif
            break;
        case BAUD_115200:
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            if (uart == UART_OPTICAL) UART_OPTICAL_BAUDRATE_REG = 3;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            if (uart == UART_PC) UART_PC_BAUDRATE_REG = 3;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            if (uart == UART_RFSLOT) UART_RFSLOT_BAUDRATE_REG = 3;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            if (uart == UART_GSMSLOT) UART_GSMSLOT_BAUDRATE_REG = 3;
            #endif
            break;
        default:
            return FALSE;
    }
    return TRUE;
}
#elif defined(_PIC16XX1919Y_)
int config_baudrate(int uart, int value)
{
    switch(value)
    {
        case BAUD_300:
            switch(uart)
            {
                case UART_PC:
                    #if _XTAL_FREQ == 16000000
                    uart_pc_rx_baudrate_register() = 13332;// 300 16Mhz
                    #elif _XTAL_FREQ == 10000000
                    uart_pc_rx_baudrate_register() = 8332;// 300 10Mhz
                    #elif _XTAL_FREQ == 32000000
                    uart_pc_rx_baudrate_register() = 2666666;// 300 10Mhz
                    #endif
                    break;
                default:
                    break;
            }
            break;
        case BAUD_9600:
            switch(uart)
            {
                case UART_PC:
                    #if _XTAL_FREQ == 16000000
                    uart_pc_rx_baudrate_register() = 416;// 9600 16Mhz
                    #elif _XTAL_FREQ == 10000000
                    uart_pc_rx_baudrate_register() = 259;// 9600 10Mhz
                    #elif _XTAL_FREQ == 32000000
                    uart_pc_rx_baudrate_register() = 832;// 9600 10Mhz
                    #endif
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
#else
#error "Check MCU type"
#endif //__MSP430__
////////////////////////////////////////////////////////////////////////////////
//  Function    :
//  Created     : 11/10/2014, by thaodtn
//  Description : ham send byte su dung trong calibration
//  Input		:
//  Output      :
////////////////////////////////////////////////////////////////////////////////
#if defined(DLT645_SUPPORT) //can chinh TI)
void send_message(int port, int len)
{
    tx_msg[port].ptr = 0;
    tx_msg[port].len = len;
    switch (port)
    {
//#if defined(UART_PORT_0_SUPPORT)
//    case 0:
//        UART0_INTR_REG |= UART0_INTR_TX;
//        break;
//#endif
#if defined(UART_PORT_1_SUPPORT)
    case 0:
    case 1:
        UART1_INTR_REG |= UART1_INTR_TX;
        break;
#endif
    }
}
#endif //DLT645_SUPPORT //can chinh TI

void UART0_init(void) //quy hoach lai, bo ham nay di
{
#if defined(_DT01M_RF_) || defined(_PREPAID_METER_) || defined(_IHD_)
    #if defined(__MSP430__)
    // Setup P1.2 UCA0RXD, P1.3 UCA0TXD
    UART0_SEL set (UART0_TX + UART0_RX);                  
    UART0_DIR set UART0_TX;             // Enable UCA0RXD, UCA0TXD
    UART0_DIR clr UART0_RX;
    //UC0IE |= UCA0RXIE;
    //UART0_INTR_REG |= UART0_INTR_RX;
    //thaodtn
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL0;                     /* UCLK = ACLK */

    UCA0BR1 = 0x0;
    UCA0BR0 = 0x3;
    UCA0MCTLW_H = 0x92;
    UCA0CTL1 &= ~UCSWRST;
//    UCA0IE |= UCRXIE;
    #elif defined(_18F86J94)
    
    #endif
    
    #ifdef UART_CONFIG_7_EVEN_1
//    UCA0CTLW0 = UCPEN; 
//    UCA0CTLW0 |= (UCPAR | UC7BIT);
//    UCA0CTLW0 &= ~UCSPB;
    #endif
#elif (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__MSP430__)
    // Setup P1.2 UCA0RXD, P1.3 UCA0TXD
    P2SEL |= BIT4 | BIT5;                   // Set P2.4, P2.5 to non-IO
    P2DIR |= BIT4 | BIT5;                   // Enable UCA0RXD, UCA0TXD

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 &= ~UCSSEL_0;
    UCA0CTL1 |= UCSSEL1;        /* UCLK = SMCLK */
    UCA0BR1 = 216;
    UCA0BR0 = 191;
    UCA0MCTL = UCBRS_1 + UCBRF_0;//2
    UCA0CTL1 &= ~UCSWRST; //reset de enable ngat
//    UC0IE |= UCA0RXIE;
    #ifdef UART_CONFIG_7_EVEN_1
//    UCA0CTL0 |= UC7BIT;
//    UCA0CTL0 |= (UCPEN | UCPAR);
//    UCA0CTL0 &= ~UCSPB;
    #endif
#elif (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__PIC24FJxxxGA410__)
    
#else
#warning "uart.c : Check meter type define"
#endif
}

void UART1_init(void) //quy hoach lai, bo ham nay di
{
#if defined(_DT01M_RF_)|| defined (_DT01P_RF_) || defined(_PREPAID_METER_)|| defined(_IHD_) || defined(_RELAY_CONTROL_)
    #if defined(__MSP430__)
    UART1_SEL set (BIT4 + BIT5);                   // Set P1.4, P1.5 to non-IO
    UART1_DIR set UART1_TX;                   // Enable UCA0RXD, UCA0TXD
    UART1_DIR clr UART1_RX;
    #ifdef _DT01P_RF_
    UART1_REN   set UART1_RX; // enable tro noi len tai uart rx
    UART1_PORT  set UART1_RX; // enable tro noi keo len
    #endif
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 |= UCSSEL__SMCLK;//UCSSEL__ACLK;                     /* UCLK = ACLK */
#ifdef UART_PC_BAUDRATE_300
    UCA1BR1 = 216;
    UCA1BR0 = 191;
    UCA1MCTLW = UCBRS1 + UCBRF_0;
#else
    //khuet mo tam thoi
    UCA1BR1 = 0x0;
    UCA1BR0 = 108;//0x3;
    UCA1MCTLW = UCBRS0 + UCBRF_6 + UCOS16;;//UCA1MCTLW_H = 0x92;
#endif
    UCA1CTL1 &= ~UCSWRST;
//    UCA1IE |= UCRXIE;
    #ifdef UART_CONFIG_7_EVEN_1
//    UCA1CTLW0 = UCPEN; 
//    UCA1CTLW0 |= (UCPAR | UC7BIT);
//    UCA1CTLW0 &= ~UCSPB;
    #endif
    #elif defined(_18F86J94)
    uart_pc_init();
    #endif
#elif (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__MSP430__)
    // Setup P1.6 UCA0RXD, P1.7 UCA0TXD
    P1SEL |= BIT6 | BIT7;                   // Set P1.6, P1 to non-IO
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 &= ~UCSSEL0;
    UCA1CTL1 |= UCSSEL1;  
    
    #ifdef CLOCK_12MHz
    UCA1BR1 = 0x04;
    UCA1BR0 = 0xE8;
    
    #else
    UCA1BR1 = 6;
    UCA1BR0 = 130;
    #endif
    UCA1MCTL = UCBRS_6 + UCBRF_0;
    UCA1CTL1 &= ~UCSWRST;
//    UC1IE |= UCA1RXIE;   
    #ifdef UART_CONFIG_7_EVEN_1
//    UCA1CTL0 |= UC7BIT;
//    UCA1CTL0 |= (UCPEN | UCPAR);
//    UCA1CTL0 &= ~UCSPB;
    #endif
#elif (defined(_DT03M_RF_) || defined(_DT03P_RF_)) && defined(__PIC24FJxxxGA410__)
    
#else
#warning "uart.c : Check meter type define"
#endif
}

#ifdef DTP_DEVICE_COMMUNICATION
void UART2_init(void)
{
    #if defined(_DT01M_RF_)|| defined (_DT01P_RF_) || defined(_IHD_)
    #if defined(__MSP430__)
    UART2_SEL set (UART2_TX + UART2_RX);
    UART2_DIR set UART2_TX;
    UART2_DIR clr UART2_RX;

    UCA2CTL1 |= UCSWRST;
    //9600bps
    UCA2CTL1 |= UCSSEL__SMCLK;
    UCA2BR1 = 0x00;
    UCA2BR0 = 108;
    UCA2MCTLW = UCBRS0 + UCBRF_6 + UCOS16;
    UCA2CTL1 &= ~UCSWRST;
//    UCA2IE |= UCRXIE;
    #elif defined(_18F86J94)
    uart_device_init();
    #endif
#elif defined(_PREPAID_METER_)
    UART2_SEL set (UART2_TX + UART2_RX);                   // Set P1.4, P1.5 to non-IO
    UART2_DIR set UART2_TX;                   // Enable UCA0RXD, UCA0TXD
    UART2_DIR clr UART2_RX;

    UCA2CTL1 |= UCSWRST;
    UCA2CTL1 |= UCSSEL__ACLK;                     /* UCLK = ACLK */

    UCA2BR1 = 0x0;
    UCA2BR0 = 0x3;
    UCA2MCTLW_H = 0x92;
    UCA2CTL1 &= ~UCSWRST;
//    UCA2IE |= UCRXIE;
#elif defined(_DT03M_RF_) || defined(_DT03P_RF_)
#else
#warning "uart.c : Check meter type define"
#endif
}
#endif

void uart_transmit_buffer(uint8_t *buffer, uint8_t length, int port)
{
    int i = 0;
    for (i = 0; i < length; i++)
    {
        switch (port)
        {
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            case UART_OPTICAL:
                uart_optical_send_byte(buffer[i]);//UART0_send_byte(buffer[i]);
                break;
            #endif    
            #ifdef COMMUNICATION_PC_SUPPORT
            case UART_PC:
                uart_pc_send_byte(buffer[i]);//UART1_send_byte(buffer[i]);
                break;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            case UART_RFSLOT:
                uart_rfslot_send_byte(buffer[i]);//UART2_send_byte(buffer[i]);
                break;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            case UART_GSMSLOT:
                uart_gsmslot_send_byte(buffer[i]);//UART2_send_byte(buffer[i]);
                break;
            #endif
        }    
    }
}

void uart_task(void)
{
#ifdef DTP_SUPPORT
    if (dtp.flags.tx_transmit == TRUE)
    {
    #ifdef DISPLAY_SUPPORT
        if (emeter.flags.low_voltage_detect == FALSE) display_anten(SHOW); //mat dien thi ko hien thi (PCF ton dong)
    #endif
        dtp.flags.tx_transmit = FALSE;
        //uart_transmit_buffer(dtp_buffer, dtp.tx_length + 2, dtp.port);
        uart_transmit_buffer(dtp_buffer, dtp.count, dtp.port); //thaodtn 21/5/2017
        switch (dtp.port)
        {
            #ifdef COMMUNICATION_OPTICAL_SUPPORT
            case UART_OPTICAL:
                uart_optical_rx_interupt_enable();
                break;
            #endif
            #ifdef COMMUNICATION_PC_SUPPORT
            case UART_PC:
                uart_pc_rx_interupt_enable();
                break;
            #endif
            #ifdef COMMUNICATION_RFSLOT_SUPPORT
            case UART_RFSLOT:
                uart_rfslot_rx_interupt_enable();
                break;
            #endif
            #ifdef COMMUNICATION_GSMSLOT_SUPPORT
            case UART_GSMSLOT:
                uart_gsmslot_rx_interupt_enable();
                break;
            #endif
        }        
    #ifdef DISPLAY_SUPPORT
        if (emeter.flags.low_voltage_detect == FALSE) display_anten(HIDE); //mat dien thi ko hien thi (PCF ton dong)
    #endif
    }
#endif
    if (flag_increase_prog_times == TRUE)
    {
        flag_increase_prog_times = FALSE;
        emeter.progtimes++;
        #if defined(_DT01P_RF_)
        eeprom_wp_output_low();
        delay_ms(3);
        eeprom_write_byte(ADDR_PROG_TIMES, emeter.progtimes, EEPROM_1);
        eeprom_write_byte(ADDR_PROG_TIMES + 2, emeter.progtimes, EEPROM_1);
        eeprom_wp_output_high();
        #else
        eeprom_write_byte(EEPROM_PROG_TIMES,emeter.progtimes, EEPROM_1);
        #endif
    }        
}


//chuyen sang hardware.c nhe
//
//#if defined(__MSP430__)
//void uart_pc_send_byte(char c)
//{   
//    while (!(UART1_FLAG_REG & UART1_FLAG_TX));
//    UART1_BUFFER_TX  = c;
//    while (!(UART1_FLAG_REG & UART1_FLAG_TX));
//}
//void uart_optical_send_byte(char c)
//{   
//    while (!(UART0_FLAG_REG & UART0_FLAG_TX));
//    UART0_BUFFER_TX  = c;
//    while (!(UART0_FLAG_REG & UART0_FLAG_TX));
//}
//#elif defined(__PIC24FJxxxGA410__)
//void uart_pc_send_byte(char c)
//{
//    while (U2STAbits.UTXBF == 1);
//    while (U2STAbits.TRMT == 0);
//    U2TXREG = c;
//    while (U2STAbits.UTXBF == 1);
//    while (U2STAbits.TRMT == 0);
//}
//#endif

#if defined(__MSP430__) || defined(_18F86J94)
//void uart_pc_process(void)
//{
//    uint8_t ch;
//    if (uart_pc_get_error()) 
//    {
//        uart_pc_rx_interrupt_flag_clear();
//        uart_pc_clear_all_error(); 
//        ch = uart_pc_rx_data_register();  // doc thanh ghi RCREG3 de xoa co FERR
//        uart_pc_rx_interupt_enable();
//        return;
//    }
//    ch = uart_pc_rx_data_register();    
//    if (uart_pc_rx_interrupt_flag_status() && uart_pc_rx_interrupt_status()) 
//    {
//        uart_pc_rx_interrupt_flag_clear();
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc dtp    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef DTP_SUPPORT
//        dtp_receice(ch, UART_PC);        
//        #endif        
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc sts    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef STS_SUPPORT
//        uart_sts_receive(ch);
//        #endif            
//        ////////////////////////////////////////////////////////////////////////////
//        /*   giao thuc IEC62056-21    */
//        ////////////////////////////////////////////////////////////////////////////
//        #ifdef IEC62056_21_SUPPORT
//        iec62056_pc_receive(ch);
//        #endif //IEC62056_21_SUPPORT
//    }    
//}
#endif