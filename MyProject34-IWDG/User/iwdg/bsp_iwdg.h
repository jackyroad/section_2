#ifndef __BSP_IWDG_H
#define __BSP_IWDG_H

#include "stm32f4xx.h"

void IWDG_Config(uint8_t prv, uint16_t rlv);
void IWDG_Feed(void);

#endif /* __BSP_IWDG_H */