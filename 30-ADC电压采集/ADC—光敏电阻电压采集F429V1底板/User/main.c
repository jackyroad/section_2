/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC�ɼ���ѹ��DMAģʽ
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
#include "./adc/bsp_adc.h"


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal; 

static void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

    /*��ʼ��USART1*/
    Debug_USART_Config();
 
		/* ��ʼ�������������õ���DAC��ADC���ݲɼ���ɺ�ֱ����DMA�������ݵ�ADC_ConvertedValue����
				DMAֱ�Ӹı�ADC_ConvertedValue��ֵ*/
    Rheostat_Init();
  
    printf("\r\n ----����һ�����������ѹ�ɼ�ʵ��(DMA����)----\r\n");
    
    while (1)
    {
      ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*(float)3.3; // ��ȡת����ADֵ
    
      printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
      printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 

      Delay(0xffffee);  
    }
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

