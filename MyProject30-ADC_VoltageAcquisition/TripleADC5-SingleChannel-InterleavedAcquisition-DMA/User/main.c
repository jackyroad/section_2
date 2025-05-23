/**
 * @file   main.c
 * @brief  三重模式单通道交替采样ADC实验
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-04-03
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./systick/bsp_SysTick.h"

// ADC转换的电压值通过MDA方式传到SRAM
// 传输过来的三字的数据排放顺序为[ADC2[15:0],ADC1[15:0]]
//                             [ADC1[15:0],ADC3[15:0]]
//                             [ADC3[15:0],ADC2[15:0]]
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint32_t ADC_ConvertedValue[3];

// 局部变量，用于保存转换计算后的电压值
float ADC_ConvertedValueLocal[3];

static void Delay(__IO uint32_t nCount) // 简单的延时函数
{
  for (; nCount != 0; nCount--)
    ;
}

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  Debug_USART_Config();
  Rheostat_Init();

  while (1)
  {
    Delay(0xffffee);

    ADC_ConvertedValueLocal[0] = (float)((uint16_t)ADC_ConvertedValue[0] * 3.3 / 4096);
    ADC_ConvertedValueLocal[1] = (float)((uint16_t)ADC_ConvertedValue[1] * 3.3 / 4096);
    ADC_ConvertedValueLocal[2] = (float)((uint16_t)ADC_ConvertedValue[2] * 3.3 / 4096);

    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[0]);
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[1]);
    printf("\r\n The current AD value = 0x%08X \r\n", ADC_ConvertedValue[2]);

    printf("\r\n The current ADC1 value = %f V \r\n", ADC_ConvertedValueLocal[0]);
    printf("\r\n The current ADC2 value = %f V \r\n", ADC_ConvertedValueLocal[1]);
    printf("\r\n The current ADC3 value = %f V \r\n", ADC_ConvertedValueLocal[2]);
  }
}

    /*********************************************END OF FILE**********************/
