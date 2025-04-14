/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ʾʹ��DMA���д��ڴ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_usart_dma.h"
#include "./led/bsp_led.h"


static void Delay(__IO u32 nCount); 


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  /* ��ʼ��USART */
  Debug_USART_Config(); 

  /* ����ʹ��DMAģʽ */
  USART_DMA_Config();
  
  /* ����RGB��ɫ�� */
  LED_GPIO_Config();

  printf("\r\n USART1 DMA RX ���� \r\n");
  
  USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);


  while(1)
  {
    LED1_TOGGLE
    Delay(0xFFFFF);
  }
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

