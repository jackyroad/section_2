/**
 * @file   main.c
 * @brief  ˫��ģʽ��ͨ��ADCʵ��
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-04-02
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./systick/bsp_SysTick.h"

// ADCת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ
float ADC_ConvertedValueLocal[RHEOSTAT_NOFCHANEL] = {0};

static void Delay(__IO uint32_t nCount) // �򵥵���ʱ����
{
  for (; nCount != 0; nCount--)
    ;
}

/**
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
  /* ��ʼ��USART����ģʽΪ 115200 8-N-1 */
  Debug_USART_Config();
  Rheostat_Init();

  while (1)
  {
    ADC_ConvertedValueLocal[0] = (float)ADC_ConvertedValue[0] / 4096 * (float)3.3;
    ADC_ConvertedValueLocal[1] = (float)ADC_ConvertedValue[1] / 4096 * (float)3.3;

    printf("\r\n CH1_PC3 value = %f V \r\n", ADC_ConvertedValueLocal[0]);
    printf("\r\n CH2_PA4 value = %f V \r\n", ADC_ConvertedValueLocal[1]);

    printf("\r\n\r\n");
    Delay(0xffffff);
  }
}

/*********************************************END OF FILE**********************/
