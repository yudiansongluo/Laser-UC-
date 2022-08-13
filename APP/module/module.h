#ifndef _module_H
#define _module_H
#include "system.h"

//#define ID_Num1  0x00
//#define ID_Num2  0x7b  //123

#define ID_Num1  0x01
#define ID_Num2  0xc8

//#define ID_Num1  0x00
//#define ID_Num2  0x0C //12 

//#define ID_Num1  0x00
//#define ID_Num2  0x18

//#define ID_Num1  0x00
//#define ID_Num2  0x5E



extern char *merged;
extern char * dest_string;
extern char * dest_string1;
extern u8 lorasendflag;
extern u8 netflag2;
extern u8 beat_count;
extern u8 flag_que[];
extern u8 voice_que[];


////1 baud  2=10s/oport  3=deserve  4turn off vtg  5.gsa 6.gll 7.gsv
//unsigned char gpstest1[37];
//unsigned char gpstest2[22];
//unsigned char gpstest3[21];
//unsigned char gpstest4[26];
//unsigned char gpstest5[26];
//unsigned char gpstest6[26];
//unsigned char gpstest7[26];



void Jion_Net(void);
void Usart_Receive(void);
void IR_Poll(void);	
void Gps_RecToSend(void);
//void Nrf_RecToSend(void);
//void Nrf_Send(void);
void GPS_LogIn(void);
void Usart3_Receive(void);
void USART3_Send(void);
void Lc12s_Init(void);
void Lc12s(u8 data[]);
void Lc12s_test(uint8_t data[]);
void Lc12s_test1(u8 data);
extern void updating(void);

#endif
