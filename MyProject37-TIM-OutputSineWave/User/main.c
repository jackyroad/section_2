/**
 * @file   main.c
 * @brief  DAC输出正弦波
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{DAC正弦波输出完成})
 * @date 2025-04-09
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./dac/bsp_dac.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
		
	LED_GPIO_Config();
	
	  /*初始化串口*/
  Debug_USART_Config();
 

	printf("\r\n 欢迎使用野火  STM32 F429 开发板。\r\n");	
	printf("\r\n DAC输出例程，输出正弦波\r\n");	
  printf("\r\n 使用示波器检测开发板的PA4、PA5引脚，可测得正弦波\r\n ");

	
	/*初始化DAC，开始DAC转换,使用示波器检测PA4/PA5，可观察到正弦波*/
	DAC_Mode_Init();

	while(1);	 

}


/*********************************************END OF FILE**********************/

