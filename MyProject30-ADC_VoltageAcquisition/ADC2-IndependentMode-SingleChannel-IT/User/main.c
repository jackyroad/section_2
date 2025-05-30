/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC例程
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
#include "./usart/bsp_usart.h"
#include "./adc/bsp_adc.h"


// ADC转换的电压值通过MDA方式传到SRAM
__IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_Vol; 

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	Rheostat_Init();	
	
    while (1)
    {
    	
			ADC_Vol =(float) ADC_ConvertedValue/4096*(float)3.3; // 读取转换的AD值

      printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
      printf("\r\n The current AD value = %f V \r\n",ADC_Vol);     



      Delay(0xffffee);  
    }
}



/*********************************************END OF FILE**********************/

