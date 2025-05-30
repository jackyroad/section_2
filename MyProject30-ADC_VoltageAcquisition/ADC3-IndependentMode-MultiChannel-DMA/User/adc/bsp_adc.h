#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

/* 通道数量 */
#define RHEOSTAT_NOFCHANEL         3

/* ------------------GPIO定义--------------------------------*/
// ADC1 通道1 GPIO-C3
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_3
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL1       ADC_Channel_13

// ADC1 通道2 GPIO-A4
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_4
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL2       ADC_Channel_4

// ADC1 通道3 GPIO-A6
#define RHEOSTAT_ADC_GPIO_PORT3    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN3     GPIO_Pin_6
#define RHEOSTAT_ADC_GPIO_CLK3     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL3       ADC_Channel_6

/* ----------------ADC定义--------------------------------*/
// ADC 序号宏定义
#define RHEOSTAT_ADC               ADC1
#define RHEOSTAT_ADC_CLK           RCC_APB2Periph_ADC1
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR       ((u32)ADC1 + 0x4c)

/* ------------------DMA定义------------------------------*/
// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2

// ADC1专配的DMA就是通道0，流0
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0
/* -------------------------------------------------------*/

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



