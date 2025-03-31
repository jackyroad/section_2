/**
 * @file   main.c
 * @brief
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit2:{添加了led和beep功能})
 * @date 2025-03-31
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./tim/bsp_advance_tim.h"
#include "./key/bsp_key.h"
#include "./beep/bsp_beep.h"
#include "./led/bsp_led.h"

extern __IO uint16_t ChannelPulse;

void delay_ms(uint32_t nCount);

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */

int main(void)
{
  /* 外设初始化 */
  Key_GPIO_Config();
  BEEP_GPIO_Config();
  LED_GPIO_Config();
  TIMx_Configuration();

  while (1)
  {
    /* 占空比公式：ChannelPulse/1024 100% */
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      if (ChannelPulse < 960)
      {
        ChannelPulse += 64;
      }
      else
      {
        ChannelPulse = 1024;
      }

      TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);

      BEEP_ON;
      delay_ms(100);
      BEEP_OFF;
    }

    if (Key_Scan(KEY1_GPIO_PORT, KEY2_PIN) == KEY_ON)
    {
      if (ChannelPulse >= 64)
      {
        ChannelPulse -= 64;
      }
      else
      {
        ChannelPulse = 0;
      }

      TIM_SetCompare1(ADVANCE_TIM, ChannelPulse);

      BEEP_OFF;
      delay_ms(50);
      BEEP_ON;
      delay_ms(50);
      BEEP_OFF;
    }

    if (GPIO_ReadOutputDataBit(ADVANCE_OCPWM_GPIO_PORT, ADVANCE_OCPWM_PIN))
    {
      LED1_ON;
    }
    else
    {
      LED1_OFF;
    }

    if (GPIO_ReadOutputDataBit(ADVANCE_OCNPWM_GPIO_PORT, ADVANCE_OCNPWM_PIN))
    {
      LED2_ON;
    }
    else
    {
      LED2_OFF;
    }
  }
}

void delay_ms(uint32_t nCount)
{
  for (; nCount != 0; nCount--)
  {
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
  }
}

/*********************************************END OF FILE**********************/
