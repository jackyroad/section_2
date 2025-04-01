/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   оƬ�¶�
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
#include "./usart/bsp_debug_usart.h"
#include "./temp/bsp_tempad.h"


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

/*�������¶�ֵ*/
 u16 Current_Temp;

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

//��*��ע�����
//��ʵ���������оƬ�¶ȣ�һ�������Ҫ��

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
	
	Temp_ADC1_Init();
	
	printf("\r\n ����һ���ڲ��¶ȴ�����ʵ�� \r\n");
	printf( "\r\n Print current Temperature  \r\n");
	
    while (1)
    {
    Delay(0xffffee);      // ��ʱ 
	
	//���㷽���ο���STM32�����ֲᡷ
	//���㷽��1            	                 									  
 	 Current_Temp= (ADC_ConvertedValue*3.3/4096-0.76 )*1000 / 2.5+ 25 ;
	
	//���㷽��2
	//Current_Temp=(ADC_ConvertedValue-V25)/AVG_SLOPE+25;	

	//10������ʾ
	printf("\r\n The IC current tem= %3d ��\r\n", Current_Temp);	      
 	 
	//16������ʾ 						 
 	// printf("\r\n The current temperature= %04x \r\n", Current_Temp);		 
    }
}



/*********************************************END OF FILE**********************/

