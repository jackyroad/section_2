/**
 * @file   bsp_dac.c
 * @brief  DAC������Ҳ��������
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-09
 * @copyright Copyright (c) 2025
 */
  
#include "./dac/bsp_dac.h"


/* �������� ---------------------------------------------------------*/
/* �����������ݣ�ʹ��������θ��ӽ����Ҳ������������������һ������ʱ��������update�¼�ʱ��Ӧ������1�� */
const uint16_t Sine12bit[32] = {
	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,	2661	, 2255	, 
	1841	, 1435	, 1054	, 714		, 428		, 209		, 65		, 3			,
	24		, 127		, 310		, 564		, 878		, 1240	, 1636	, 2048
};

uint32_t DualSine12bit[32];


/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_AHB1PeriphClockCmd(DAC_CH1_GPIO_CLK|DAC_CH2_GPIO_CLK, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(DAC_CLK, ENABLE);
  
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin =  DAC_CH1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(DAC_CH1_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin =  DAC_CH2_GPIO_PIN;
  GPIO_Init(DAC_CH2_GPIO_PORT, &GPIO_InitStructure);
	
		  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_TRIGGER;						                //ʹ��TIM2��Ϊ����Դ
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	        //��ʹ�ò��η�����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;	          //��ʹ��DAC�������
	//���ǲ��������ʵ��û���õ��������ó�����ֵ�������ṹ���Ա����Ϊ�գ�
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
  DAC_Init(DAC_CH1_CHANNEL, &DAC_InitStructure);

  /* ����DAC ͨ��2 */
  DAC_Init(DAC_CH2_CHANNEL, &DAC_InitStructure);
  
	/* ʹ��DACͨ��1��2 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_2, ENABLE);
	
	/* ʹ��DAC��DMA���� */
  DAC_DMACmd(DAC_Channel_1, ENABLE);

}

/**
 * @brief  ����TIM2������һ��ͨ�ö�ʱ��
 * @param  None
 * @retval None
 * @note   ��ʱ��ʱ��Ϊ90MHZ,20������һ��ѭ������ʱ1/90M*20
           ����ʱ�����������ϼ�����ָ����ֵʱ������Update�¼���ͬʱ����DAC��DHRx�Ĵ���������ת�Ƶ�DORx���Ӷ���ʼ����ת��
           ��Ϊ���Ƕ�������Ҳ�������32����Ա��Ҳ����һ�����������Ҳ�����Ҫ����32��DMAת�ˣ���������Ϊ32*20*1/90M=7.11us��Ƶ��Ϊ140625HZ
 */
static void DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ90M */
  RCC_APB1PeriphClockCmd(DAC_TIM_CLK, ENABLE);
	
  /* TIM2������ʱ������ */
  /* ��ʱ������1/90M*20 */
  TIM_TimeBaseStructure.TIM_Period = 19; 

  // ʵ�ʷ�Ƶϵ��=TIM_Prescaler+1������Ƶ
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;

  // ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  

  TIM_TimeBaseInit(DAC_TIM, &TIM_TimeBaseStructure);

  /* ����TIM2����Դ������������20�ʹ����жϣ����¼��� */
  TIM_SelectOutputTrigger(DAC_TIM, TIM_TRGOSource_Update);

	/* ʹ��TIM2 */
  TIM_Cmd(DAC_TIM, ENABLE);
}


/**
  * @brief  ����DMA
  * @param  None
  * @retval None
  */
static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* DAC1ʹ��DMA1 ͨ��7 ������5ʱ�� */
	RCC_AHB1PeriphClockCmd(DAC_DMA_CLK, ENABLE);
	
	/* ����DMA2 */
  DMA_InitStructure.DMA_Channel = DAC_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;					//�������ݵ�ַ �Ĵ��� DHR12RD �ĵ�ַ12λ���Ҷ��롢˫ͨ��
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSine12bit;			  //�ڴ����ݵ�ַ DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									//���ݴ��䷽���ڴ�������
  DMA_InitStructure.DMA_BufferSize = 32;																	//�����СΪ32�ֽ�	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_Init(DAC_DMA_STREAM, &DMA_InitStructure);
  
  /* ʹ�� DMA_Stream */
  DMA_Cmd(DAC_DMA_STREAM, ENABLE);
}


/**
  * @brief  DAC��ʼ������
  * @param  ��
  * @retval ��
  */
void DAC_Mode_Init(void)
{
	uint32_t Idx = 0;  

	DAC_Config();
	DAC_TIM_Config();
	
	/* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
  /* ӦΪDAC1��DAC2��Ҫ��������԰����Ҳ��������ݸ���һ�ݵ�DAC_DHR12(32λ����16λDAC2����16λDAC1) */
  for (Idx = 0; Idx < 32; Idx++)
  {
    DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
  }
	
	DAC_DMA_Config();
}










