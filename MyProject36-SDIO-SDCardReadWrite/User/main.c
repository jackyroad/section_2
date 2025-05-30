/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SDIO sd卡测试实验（不含文件系统）。
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
#include "usart/bsp_debug_usart.h"
#include "sdio/sdio_test.h"
#include "led/bsp_led.h"
#include "key/bsp_key.h" 


/**
	*********************************************************************************************************
	* Description : 初始化BL8782 WiFi模块使能引脚，并禁用WiFi模块
	* Argument(s) : none.
	* Return(s)   : none.
	*********************************************************************************************************
	*/
static void BL8782_PDN_INIT(void)
{
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //禁用WiFi模块
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /* 禁用WiFi模块 */
	BL8782_PDN_INIT();
	
	/* 初始化LED灯 */
  LED_GPIO_Config();
	LED_BLUE;	
	/* 初始化独立按键 */
	Key_GPIO_Config();
  
  /*初始化USART1*/
  Debug_USART_Config();
  
	printf("\r\n欢迎使用野火  STM32 F429 开发板。\r\n");	
	
	printf("在开始进行SD卡基本测试前，请给开发板插入32G以内的SD卡\r\n");			
	printf("本程序会对SD卡进行 非文件系统 方式读写，会删除SD卡的文件系统\r\n");		
	printf("实验后可通过电脑格式化或使用SD卡文件系统的例程恢复SD卡文件系统\r\n");		
	printf("\r\n 但sd卡内的原文件不可恢复，实验前务必备份SD卡内的原文件！！！\r\n");		
	
	printf("\r\n 若已确认，请按开发板的KEY1按键，开始SD卡测试实验....\r\n");	
  
  /* Infinite loop */
  while (1)
  {	
		/*按下按键开始进行SD卡读写实验，会损坏SD卡原文件*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
		{
			printf("\r\n开始进行SD卡读写实验\r\n");	
		  SD_Test();			
		}
	} 
}

/*********************************************END OF FILE**********************/

