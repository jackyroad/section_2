/**
 * @file   main.c
 * @brief 
 * @author leshen (13762713527@qq.com)
 * @version{gitBranch} (commit:�޸���ע��)
 * @date 2025-03-30
 * @copyright Copyright (c) 2025
 */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./time/bsp_basic_time.h"
#include "./beep/bsp_beep.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  LED_GPIO_Config();

  BEEP_GPIO_Config();

  TIMx_Configuration();

  while(1);

}/*********************************************END OF FILE**********************/

