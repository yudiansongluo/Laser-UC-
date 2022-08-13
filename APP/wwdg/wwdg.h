#ifndef _wwdg_H
#define _wwdg_H
#define send_buf_len 10


#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "picture.h"
#include "hwjs.h"
#include "string.h"
#include "hexify.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "nrf24l01.h"
#include "spi.h"
#include "dma.h"
#include "stdarg.h"	


extern u8 send_buf[send_buf_len];


extern unsigned char s0[5];
extern unsigned char s1[5];
extern unsigned char s3[5];
extern unsigned char s2[5];

extern char * dest_string;
extern uint32_t jk;
// Global
extern uint8_t USART1_FINISH;

extern uint32_t jkl;
//uint8_t* recv_bytes;
extern uint8_t* recv_bytes;



  void strHexToUint8Array(const char *str, uint8_t *bytes, size_t bytes_len);
 void atRecvDecode( char const * recv_str,  uint8_t * decoded_bytes, int * fport);
 int getAtRecvFport( char const * recv_str);
 uint8_t * getAtRecvBytes(char const * recv_str);
 int getAtRecvBytesLength(char const * recv_str);
 int getDataSize(int bufferSize);
 void mergeData(char* merged, unsigned char* buffer, int bufferSize);	
 void printHex(unsigned char * str,int bufferSize);
 char * newMergedData(unsigned char * buffer, int bufferSize);
 void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data);
 void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str);
 void UART_PutBytes (USART_TypeDef* USARTx, uint8_t *str, uint8_t length);	
 int reverse_bit8(int x);
void itoa(int num,char *str);
unsigned char cal_table_high_first(unsigned char value);
#endif
