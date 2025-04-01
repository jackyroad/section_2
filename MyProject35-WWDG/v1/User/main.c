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
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
  uint8_t wwdg_tr, wwdg_wr;

  LED_GPIO_Config();

  LED1_ON;
  Delay(0xFFFFFF);

  WWDG_Config(127, 80, WWDG_Prescaler_1);
  /* ��¼�趨�Ĵ���ֵ */
  wwdg_wr = WWDG->CFR & 0X7F;

  while (1)
  {
    LED1_OFF;
    Delay(0xFFFFFF);
    /* ��¼�ݼ���������ֵ�����ֵ�����趨��trֵ�ϵݼ���0x40 */
    wwdg_tr = WWDG->CR & 0x7F;

    if (wwdg_tr > wwdg_wr)
    {
      /* ι������trֵ��ʼ���µݼ�*/
      WWDG_Feed();
    }
  }
}

static void Delay(__IO uint32_t nCount) // �򵥵���ʱ����
{
  for (; nCount != 0; nCount--)
    ;
}

/*********************************************END OF FILE**********************/
