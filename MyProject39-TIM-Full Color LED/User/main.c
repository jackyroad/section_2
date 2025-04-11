/**
 * @file   main.c
 * @brief  定时器PWM输出制作全彩LED灯
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-04-09
 * @copyright Copyright (c) 2025
 */
  
#include "stm32f4xx.h"
#include "./led/bsp_color_led.h"
#include "./usart/bsp_debug_usart.h"

static void Delay(__IO uint32_t nCount);

/**
 * @brief  主函数
 * @param  None
 * @retval None
 */
int main(void)
{
	uint32_t random_color = 0;

  /* 初始化呼吸灯 */
	ColorLED_Config(); 
	
  /*初始化串口*/
  Debug_USART_Config();
	
	/* 使能RNG时钟 */
  /* RNG是一个硬件随机数生成器,用于生成真随机数 */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);	
	/* 使能RNG外设 */
  RNG_Cmd(ENABLE);

	printf("\r\n 欢迎使用野火  STM32 F429 开发板。\r\n");		
	printf("\r\n 全彩LED灯例程\r\n");	  
  printf("\r\n 使用PWM控制RGB灯，可控制输出各种颜色\r\n ");

  while(1)
  {
		//SetRGBColor(COLOR_YELLOW); 
		//Delay(0xFFFFF); 
		
		/* 等待随机数产生完毕 */
		while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
		/*获取随机数*/       
		random_color = RNG_GetRandomNumber();
		
		printf("\r\n 随机颜色值：0x%06x",random_color&0xFFFFFF);
		/*显示随机颜色*/
		SetRGBColor(random_color&0xFFFFFF); 
		Delay(0x4FFFFF); 
  }
}


static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

