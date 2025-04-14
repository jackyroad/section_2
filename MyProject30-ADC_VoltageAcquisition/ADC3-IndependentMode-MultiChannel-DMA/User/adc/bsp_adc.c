/**
 * @file   bsp_adc.c
 * @brief  多通道ADC驱动(DMA)
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#include "./adc/bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};

static void Rheostat_ADC_GPIO_Config(void)
{
  /* ADC通道和GPIO引脚的对应关系是设定好的，无需我们配置，查表即可 */
		GPIO_InitTypeDef GPIO_InitStructure;
	
  /* -------------ADC1-通道13-PC3------------------------------ */
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK1, ENABLE);
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN1;
  /* GPIO设置为模拟量输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  /* 不上拉不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; 
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);

  /* -------------ADC1-通道4-PA4------------------------------ */
  // 使能 GPIO 时钟
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK2, ENABLE);
  // 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN2;
  /* GPIO设置为模拟量输入 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  /* 不上拉不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);

  /* -------------ADC1-通道6-PA6------------------------------ */
  // 使能 GPIO 时钟
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK3, ENABLE);
  // 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN3;
  /* GPIO设置为模拟量输入 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  /* 不上拉不下拉 */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);
}

static void Rheostat_ADC_Mode_Config(void)
{
  /* 结构体定义 */
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  /* ------------------DMA Init 结构体参数 初始化-------------------------- */
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 

	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;

  // 存储器地址，实际上就是一个内部SRAM的变量
  // 内部 SRAM(如果是全局变量,可能在.data或.bss 段),具体地址是系统自动分配的
  // 注意这里是取ADC_ConvertedValue这个值的地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  

  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

  // 缓冲区大小为，指一次传输的数据量，注意这个数量不是字节数，与DataSize相关
  DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL;

  // 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器是按数组存储，不断读取传输过来的数据
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 

  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	

	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
  // 相同优先级：按通道编号顺序（如Channel 1 > Channel 2）
  // 不同优先级：高优先级通道立即抢占低优先级通道
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置（配置无效）	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

	// 选择 DMA 通道，通道存在于流中，每个流绑定一个通道
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	
  /* -------------------ADC Common 结构体 参数 初始化------------------------ */
  // 开启ADC时钟
  RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK, ENABLE);

  // 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;

  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;

  // 禁止DMA直接访问模式，单ADC不需要多ADC并行采样
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;

  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; 

  ADC_CommonInit(&ADC_CommonInitStructure);
	
  /* -------------------ADC Init 结构体 参数 初始化-------------------------- */
	ADC_StructInit(&ADC_InitStructure);

  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

  // 禁止扫描模式，多通道采集才需要	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 

  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

  //外部触发通道，本例子使用软件触发，此值随便赋值即可
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

  //转换通道 3个
  ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL;

  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);

  //---------------------------------------------------------------------------
  // 配置 ADC 通道转换顺序，采样时间为56个时钟周期
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL1, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL2, 2, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL3, 3, ADC_SampleTime_56Cycles);

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);

  // 使能ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// 使能ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  

  //开始adc转换，软件触发
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}



void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}



