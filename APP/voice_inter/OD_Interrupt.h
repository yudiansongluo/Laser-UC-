
#ifndef __BSP_FUN_H
#define __BSP_FUN_H
#include "stm32f10x.h"


#define TXD_high() GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define TXD_low() GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define BaudRateUsed 9600
#define SendingDelay 104

extern u32 TimingDelay;
extern u8 dataReceived[100];
extern u8 receivedFlag, SendingFlag;
extern u8 receivedNum, tmpreceivedNum;
extern u8 cnt;


extern void GPIO_Configuration(void);
extern void EXTI_Configuration(void);
extern void TIM1_Configuration(void);
extern void TIM2_Configuration(void);
extern void TIM3_Configuration(void);


extern void Delay_Ms(volatile u32 nTime);
extern void SendOneByte(u8 datatoSend);
#endif





