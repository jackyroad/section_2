/**
  ******************************************************************************
  * @file    bsp_tempad.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ��ȡоƬ�¶�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

//�¶ȴ��������ڲ���ADC1_IN16����ͨ�������ӣ�
#include "./temp/bsp_tempad.h"

//ADC1_BASE��ַ��0x4000 0000+0x2400 ��ADC1�Ļ���ַ�������Ĺ������ݼĴ�����ƫ�Ƶ�ַ�ǣ�0x4c
#define ADC1_DR_Address    ((u32)ADC1_BASE+0x4c)

__IO u16 ADC_ConvertedValue;
//__IO u16 ADC_ConvertedValueLocal;


/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA2��ʱ��
 * ����  :��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)		       
{

	/* Enable DMA clock */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 

	/* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

 }


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	/* DMA channel1 configuration */
		   
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		        // �������ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_ConvertedValue;	        // ADת��ֵ����ŵ��ڴ����ַ	�����Ǹ�����ַ��
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                        // ������Ϊ���ݴ������Դ	
  DMA_InitStructure.DMA_BufferSize = 1;                                     // ����ָ��DMAͨ�� DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          // �����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;			        // �ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // HalfWord
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		                    //������ѭ��ģʽ��
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                    //�����ȼ�
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  		                        //û������Ϊ�ڴ浽�ڴ�Ĵ���
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(DMA2_Stream0, ENABLE);

// -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
  ADC_StructInit(&ADC_InitStructure);
	// ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;			                    //��ͨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			            //����ת��
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;       //�������������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//Right
  ADC_InitStructure.ADC_NbrOfConversion = 1;				                    //��һ��ͨ��ת��
  ADC_Init(ADC1, &ADC_InitStructure);
	

  /* ADC1 regular channel16 configuration */ 
  //���ò���ͨ��IN16, ���ò���ʱ��
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_144Cycles);	

  //ʹ���¶ȴ��������ڲ��ο���ѹ   
  ADC_TempSensorVrefintCmd(ENABLE);                                    


  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
   /* Enable ADC1 DMA */	  
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	//��ʼadcת�����������
	ADC_SoftwareStartConv(ADC1);
}												

/*
 * ��������Temp_ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Temp_ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
