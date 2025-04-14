#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

/* 通道数量 */
#define RHEOSTAT_NOFCHANEL         2

/* ------------------GPIO定义--------------------------------*/
// ADC1 通道13 GPIO-PC3
#define RHEOSTAT_ADC1_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC1_GPIO_PIN1     GPIO_Pin_3
#define RHEOSTAT_ADC1_GPIO_CLK1     RCC_AHB1Periph_GPIOC

#define RHEOSTAT_ADC1               ADC1
#define RHEOSTAT_ADC1_CHANNEL1      ADC_Channel_13
#define RHEOSTAT_ADC1_CLK           RCC_APB2Periph_ADC1


// ADC2 通道4 GPIO-PA4
#define RHEOSTAT_ADC2_GPIO_PORT1    GPIOA
#define RHEOSTAT_ADC2_GPIO_PIN1     GPIO_Pin_4
#define RHEOSTAT_ADC2_GPIO_CLK1     RCC_AHB1Periph_GPIOA

#define RHEOSTAT_ADC2               ADC2
#define RHEOSTAT_ADC2_CHANNEL1      ADC_Channel_4
#define RHEOSTAT_ADC2_CLK           RCC_APB2Periph_ADC2

/* --------------------------------------------------------*/
// ADC CDR寄存器宏定义，多重模式下ADC转换后的数字值存放在这里
// 双重模式下高16位保存ADC2的值，低16位保存ADC1的值
#define RHEOSTAT_ADC_CDR_ADDR      ((uint32_t)0x40012308)

/* ------------------DMA定义------------------------------*/
// 当我们使用ADC的多重模式时，ADC1的DMA是主，其他的是辅助，不需要要配置DMA
// 此外选用DMA2-流0-通道0是查表看ADC1对应的DMA而来
#define RHEOSTAT_ADC1_DMA_CLK      RCC_AHB1Periph_DMA2
// ADC1专配的DMA就是通道0，流0
#define RHEOSTAT_ADC1_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC1_DMA_STREAM   DMA2_Stream0
/* -------------------------------------------------------*/

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



