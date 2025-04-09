/**
 * @file   bsp_dac.c
 * @brief  DAC输出正弦波相关配置
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-09
 * @copyright Copyright (c) 2025
 */
  
#include "./dac/bsp_dac.h"


/* 波形数据 ---------------------------------------------------------*/
/* 可以增加数据，使得输出波形更接近正弦波，但是如果数据增加一倍，定时器产生的update事件时间应该缩短1倍 */
const uint16_t Sine12bit[32] = {
	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,	2661	, 2255	, 
	1841	, 1435	, 1054	, 714		, 428		, 209		, 65		, 3			,
	24		, 127		, 310		, 564		, 878		, 1240	, 1636	, 2048
};

uint32_t DualSine12bit[32];


/**
  * @brief  使能DAC的时钟，初始化GPIO
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* 使能GPIOA时钟 */
  RCC_AHB1PeriphClockCmd(DAC_CH1_GPIO_CLK|DAC_CH2_GPIO_CLK, ENABLE);	
	
	/* 使能DAC时钟 */	
  RCC_APB1PeriphClockCmd(DAC_CLK, ENABLE);
  
  /* DAC的GPIO配置，模拟输入 */
  GPIO_InitStructure.GPIO_Pin =  DAC_CH1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(DAC_CH1_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin =  DAC_CH2_GPIO_PIN;
  GPIO_Init(DAC_CH2_GPIO_PORT, &GPIO_InitStructure);
	
		  /* 配置DAC 通道1 */
  DAC_InitStructure.DAC_Trigger = DAC_TRIGGER;						                //使用TIM2作为触发源
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	        //不使用波形发生器
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	          //不使用DAC输出缓冲
	//三角波振幅（本实验没有用到，可配置成任意值，但本结构体成员不能为空）
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_Init(DAC_CH1_CHANNEL, &DAC_InitStructure);

  /* 配置DAC 通道2 */
  DAC_Init(DAC_CH2_CHANNEL, &DAC_InitStructure);
  
	/* 使能DAC通道1、2 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_2, ENABLE);
	
	/* 使能DAC的DMA请求 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);

}

/**
 * @brief  配置TIM2，这是一个通用定时器
 * @param  None
 * @retval None
 * @note   定时器时钟为90MHZ,20个周期一个循环，用时1/90M*20
           当定时器计数器向上计数至指定的值时，产生Update事件，同时触发DAC把DHRx寄存器的数据转移到DORx，从而开始进行转换
           因为我们定义的正弦波数组有32个成员，也就是一个完整的正弦波周期要进行32次DMA转运，波形周期为32*20*1/90M=7.11us，频率为140625HZ
 */
static void DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* 使能TIM2时钟，TIM2CLK 为90M */
  RCC_APB1PeriphClockCmd(DAC_TIM_CLK, ENABLE);
	
  /* TIM2基本定时器配置 */
  /* 定时器周期1/90M*20 */
  TIM_TimeBaseStructure.TIM_Period = 19; 

  // 实际分频系数=TIM_Prescaler+1，不分频
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;

  // 时钟分频系数，不分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(DAC_TIM, &TIM_TimeBaseStructure);

  /* 配置TIM2触发源，递增计数到20就触发中断，重新计数 */
  TIM_SelectOutputTrigger(DAC_TIM, TIM_TRGOSource_Update);

	/* 使能TIM2 */
  TIM_Cmd(DAC_TIM, ENABLE);
}


/**
  * @brief  配置DMA
  * @param  None
  * @retval None
  */
static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* DAC1使用DMA1 通道7 数据流5时钟 */
	RCC_AHB1PeriphClockCmd(DAC_DMA_CLK, ENABLE);
	
	/* 配置DMA2 */
  DMA_InitStructure.DMA_Channel = DAC_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;					//外设数据地址 寄存器 DHR12RD 的地址12位、右对齐、双通道
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSine12bit;			  //内存数据地址 DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									//数据传输方向内存至外设
  DMA_InitStructure.DMA_BufferSize = 32;																	//缓存大小为32字节	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//内存数据地址自增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//外设数据以字为单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//内存数据以字为单位	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DAC_DMA_STREAM, &DMA_InitStructure);
  
  /* 使能 DMA_Stream */
  DMA_Cmd(DAC_DMA_STREAM, ENABLE);
}


/**
  * @brief  DAC初始化函数
  * @param  无
  * @retval 无
  */
void DAC_Mode_Init(void)
{
	uint32_t Idx = 0;  

	DAC_Config();
	DAC_TIM_Config();
	
	/* 填充正弦波形数据，双通道右对齐*/
  /* 应为DAC1和DAC2都要输出，所以把正弦波波形数据复制一份到DAC_DHR12(32位，高16位DAC2，低16位DAC1) */
  for (Idx = 0; Idx < 32; Idx++)
  {
    DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
  }
	
	DAC_DMA_Config();
}










