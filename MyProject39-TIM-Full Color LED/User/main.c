/**
 * @file   main.c
 * @brief  ��ʱ��PWM�������ȫ��LED��
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
 * @brief  ������
 * @param  None
 * @retval None
 */
int main(void)
{
	uint32_t random_color = 0;

  /* ��ʼ�������� */
	ColorLED_Config(); 
	
  /*��ʼ������*/
  Debug_USART_Config();
	
	/* ʹ��RNGʱ�� */
  /* RNG��һ��Ӳ�������������,��������������� */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);	
	/* ʹ��RNG���� */
  RNG_Cmd(ENABLE);

	printf("\r\n ��ӭʹ��Ұ��  STM32 F429 �����塣\r\n");		
	printf("\r\n ȫ��LED������\r\n");	  
  printf("\r\n ʹ��PWM����RGB�ƣ��ɿ������������ɫ\r\n ");

  while(1)
  {
		//SetRGBColor(COLOR_YELLOW); 
		//Delay(0xFFFFF); 
		
		/* �ȴ������������� */
		while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
		/*��ȡ�����*/       
		random_color = RNG_GetRandomNumber();
		
		printf("\r\n �����ɫֵ��0x%06x",random_color&0xFFFFFF);
		/*��ʾ�����ɫ*/
		SetRGBColor(random_color&0xFFFFFF); 
		Delay(0x4FFFFF); 
  }
}


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

