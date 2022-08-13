#ifndef _time_H
#define _time_H

#include "system.h"

void TIM4_Init(u16 per,u16 psc);
void TIM3_Configuration(u16 per,u16 psc);
void Delay_Us( u16 xus);
void Delay_Ms( u16 xms);
#endif
