#ifndef __BSP_BASIC_TIME_H
#define __BSP_BASIC_TIME_H

#include "stm32f4xx.h"

#define BASIC_TIM                   TIM6
#define BASIC_TIM_CLK               RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn              TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler        TIM6_DAC_IRQHandler //自己编写的中断函数       


#endif /* __BSP_BASIC_TIME_H */
