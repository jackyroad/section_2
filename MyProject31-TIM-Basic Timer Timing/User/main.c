/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.5.1版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./time/bsp_basic_time.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  LED_GPIO_Config();

  TIMx_Configuration();

  while(1);

}/*********************************************END OF FILE**********************/

