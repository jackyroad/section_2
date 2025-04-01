#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void SysTick_Init(void);
void Delay_s(__IO u32 nTime);
//#define Delay_ms(x) Delay_s(x/1000)	 

#endif /* __SYSTICK_H */
