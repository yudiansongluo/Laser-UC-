#ifndef _hwjs_H
#define _hwjs_H

#include "system.h"



void Hwjs_InitA15(void);
void Hwjs_InitC13(void);
void Hwjs_InitB9(void );
void Hwjs_InitA12(void);
void Hwjs_InitA11(void);
void Hwjs_InitB14(void);
void Hwjs_InitA8(void);
void Hwjs_InitA7(void);
void ShakeB0(void);
void Lc12s_GPIO_Init(void);
void	Lead_Low_Time( GPIO_TypeDef* GPIO,uint16_t GPIO_Pin);

void EXIT_10(void);
void EXIT_12(void);
void EXIT_13(void);
void EXIT_14(void);
void EXIT_15(u8 Lead_High_Flag);
//void Hwjs_function(void);
u8 HW_jssj(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin );
u8 HW_jssj_low(uint16_t GPIO_Pin );

void IR_Poll(void);

u8 HW_jssj1(uint16_t GPIO_Pin);
//定义全局变量
extern u32 hw_jsm;
extern u8  hw_jsbz;
extern u8 irflag;
extern u8 data_u8[4];
extern u8 readflg;


#endif
