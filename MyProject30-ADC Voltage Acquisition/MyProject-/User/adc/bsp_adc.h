#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

/* ͨ������ */
#define RHEOSTAT_NOFCHANEL         2

/* ------------------GPIO����--------------------------------*/
// ADC1 ͨ��13 GPIO-PC3
#define RHEOSTAT_ADC1_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC1_GPIO_PIN1     GPIO_Pin_3
#define RHEOSTAT_ADC1_GPIO_CLK1     RCC_AHB1Periph_GPIOC

#define RHEOSTAT_ADC1               ADC1
#define RHEOSTAT_ADC1_CHANNEL1      ADC_Channel_13
#define RHEOSTAT_ADC1_CLK           RCC_APB2Periph_ADC1


// ADC2 ͨ��4 GPIO-PA4
#define RHEOSTAT_ADC2_GPIO_PORT1    GPIOA
#define RHEOSTAT_ADC2_GPIO_PIN1     GPIO_Pin_4
#define RHEOSTAT_ADC2_GPIO_CLK1     RCC_AHB1Periph_GPIOA

#define RHEOSTAT_ADC2               ADC2
#define RHEOSTAT_ADC2_CHANNEL1      ADC_Channel_4
#define RHEOSTAT_ADC2_CLK           RCC_APB2Periph_ADC2

/* --------------------------------------------------------*/
// ADC CDR�Ĵ����궨�壬����ģʽ��ADCת���������ֵ���������
// ˫��ģʽ�¸�16λ����ADC2��ֵ����16λ����ADC1��ֵ
#define RHEOSTAT_ADC_CDR_ADDR      ((uint32_t)0x40012308)

/* ------------------DMA����------------------------------*/
// ������ʹ��ADC�Ķ���ģʽʱ��ADC1��DMA�������������Ǹ���������ҪҪ����DMA
// ����ѡ��DMA2-��0-ͨ��0�ǲ��ADC1��Ӧ��DMA����
#define RHEOSTAT_ADC1_DMA_CLK      RCC_AHB1Periph_DMA2
// ADC1ר���DMA����ͨ��0����0
#define RHEOSTAT_ADC1_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC1_DMA_STREAM   DMA2_Stream0
/* -------------------------------------------------------*/

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



