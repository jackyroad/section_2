/**
 * @file   bsp_adc.c
 * @brief  ��ͨ��ADC����
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */
#include "./adc/bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;

static void Rheostat_ADC_GPIO_Config(void)
{
  /* ѡ��PC3��ΪADC1ͨ��13 */
		GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN;
  /* GPIO����Ϊģ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  /* ������������ */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; 
  
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Rheostat_ADC_Mode_Config(void)
{
  /* �ṹ�嶨�� */
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  /* ------------------DMA Init �ṹ����� ��ʼ��-------------------------- */
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 

	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;

  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
  // �ڲ� SRAM(�����ȫ�ֱ���,������.data��.bss ��),�����ַ��ϵͳ�Զ������
  // ע��������ȡADC_ConvertedValue���ֵ�ĵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_ConvertedValue;  

  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

  // ��������СΪ��ָһ�δ������������ע��������������ֽ�������DataSize���
  DMA_InitStructure.DMA_BufferSize = 1;	

	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶������϶�ȡ�������������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 

  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	

	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
  // ��ͬ���ȼ�����ͨ�����˳����Channel 1 > Channel 2��
  // ��ͬ���ȼ��������ȼ�ͨ��������ռ�����ȼ�ͨ��
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ������������ã�������Ч��	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

	// ѡ�� DMA ͨ����ͨ�����������У�ÿ������һ��ͨ��
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	
  /* -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------ */
  // ����ADCʱ��
  RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK, ENABLE);

  // ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;

  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;

  // ��ֹDMAֱ�ӷ���ģʽ����ADC����Ҫ��ADC���в���
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;

  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; 

  ADC_CommonInit(&ADC_CommonInitStructure);
	
  /* -------------------ADC Init �ṹ�� ���� ��ʼ��-------------------------- */
	ADC_StructInit(&ADC_InitStructure);

  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 

  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 

  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

  //�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = 1;    

  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);

  //---------------------------------------------------------------------------
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);

  // ʹ��ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// ʹ��ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  

  //��ʼadcת�����������
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}



void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}



