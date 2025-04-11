/**
 * @file   main.c
 * @brief  ����TIM��PWM����ɫ������ʵ��
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-04-11
 * @copyright Copyright (c) 2025
 */
  
#include "stm32f4xx.h"
#include "./led/bsp_breath_led.h"
#include "./usart/bsp_debug_usart.h"



/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{

  /*��ʼ������*/
  Debug_USART_Config();
 
	printf("\r\n ��ӭʹ��Ұ��  STM32 F429 �����塣\r\n");		
	printf("\r\n ����������\r\n");	  
  printf("\r\n RGB LED �Ժ����Ƶ���ʽ��˸\r\n ");

  /* ��ʼ�������� */
	BreathLED_Config(); 
  
  while(1)
  {
  }

}



/*********************************************END OF FILE**********************/

