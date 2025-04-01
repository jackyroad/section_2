/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2015-xx-xx
 * @brief   ADC����
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
#include "./systick/bsp_SysTick.h"

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ
float ADC_Vol;

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
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  Debug_USART_Config();
  Rheostat_Init();

  while (1)
  {

    printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue);
    printf("\r\n The current AD value = %f V \r\n", ADC_Vol);

    /* �ֱ���Ϊ12λ������ÿһ�ݴ�СΪ3.3/(2^12)=0.80566mV�����Ǻ�΢С��*/
    ADC_Vol = (float)ADC_ConvertedValue / 4096 * (float)3.3; // ��ȡת����ADֵ

    Delay(0xffffff);
  }
}

/*********************************************END OF FILE**********************/
