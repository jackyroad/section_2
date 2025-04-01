/**
 * @file   main.c
 * @brief 
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./wwdg/bsp_wwdg.h"

static void Delay(__IO uint32_t nCount);
/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
  uint8_t wwdg_tr, wwdg_wr;

  LED_GPIO_Config();

  LED1_ON;
  Delay(0xFFFFFF);

  WWDG_Config(127, 80, WWDG_Prescaler_1);
  /* 记录设定的窗口值 */
  wwdg_wr = WWDG->CFR & 0X7F;

  while (1)
  {
    LED1_OFF;
    Delay(0xFFFFFF);
    /* 记录递减计数器的值，这个值会在设定的tr值上递减到0x40 */
    wwdg_tr = WWDG->CR & 0x7F;

    if (wwdg_tr > wwdg_wr)
    {
      /* 喂狗，从tr值开始重新递减*/
      WWDG_Feed();
    }
  }
}

static void Delay(__IO uint32_t nCount) // 简单的延时函数
{
  for (; nCount != 0; nCount--)
    ;
}

/*********************************************END OF FILE**********************/
