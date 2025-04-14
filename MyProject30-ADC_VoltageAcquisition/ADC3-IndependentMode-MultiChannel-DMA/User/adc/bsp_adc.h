#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

/* ͨ������ */
#define RHEOSTAT_NOFCHANEL         3

/* ------------------GPIO����--------------------------------*/
// ADC1 ͨ��1 GPIO-C3
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_3
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL1       ADC_Channel_13

// ADC1 ͨ��2 GPIO-A4
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_4
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL2       ADC_Channel_4

// ADC1 ͨ��3 GPIO-A6
#define RHEOSTAT_ADC_GPIO_PORT3    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN3     GPIO_Pin_6
#define RHEOSTAT_ADC_GPIO_CLK3     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL3       ADC_Channel_6

/* ----------------ADC����--------------------------------*/
// ADC ��ź궨��
#define RHEOSTAT_ADC               ADC1
#define RHEOSTAT_ADC_CLK           RCC_APB2Periph_ADC1
// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR       ((u32)ADC1 + 0x4c)

/* ------------------DMA����------------------------------*/
// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2

// ADC1ר���DMA����ͨ��0����0
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0
/* -------------------------------------------------------*/

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



