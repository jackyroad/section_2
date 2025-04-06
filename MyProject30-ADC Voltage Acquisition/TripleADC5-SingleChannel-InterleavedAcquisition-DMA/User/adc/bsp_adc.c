/**
 * @file   bsp_adc.c
 * @brief  ����ģʽ��ͨ���������ADC����(DMA)
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-03
 * @copyright Copyright (c) 2025
 * @note   ͨ����������ɲ�׽����Ƶ�ʵ��źţ����ⵥADC���ο�˹������
 */
#include "./adc/bsp_adc.h"

__IO uint32_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};

static void Rheostat_ADC_GPIO_Config(void)
{
  /* ADCͨ����GPIO���ŵĶ�Ӧ��ϵ���趨�õģ������������ã������ */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* -------------ADC1/2/3-ͨ��13-PC3------------------------------ */
  // ʹ�� GPIO ʱ��
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC1_GPIO_CLK1, ENABLE);
  // ���� IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC1_GPIO_PIN1;
  /* GPIO����Ϊģ�������� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  /* ������������ */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RHEOSTAT_ADC1_GPIO_PORT1, &GPIO_InitStructure);
}


/* ADC��DMAģʽ���� */
static void Rheostat_ADC_Mode_Config(void)
{
  /* �ṹ�嶨�� */
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;

  /* ------------------DMA Init �ṹ����� ��ʼ��-------------------------- */
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC1_DMA_CLK, ENABLE);

  // �����ַΪ��ADC ���ݼĴ�����ַ
  DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_CDR_ADDR;

  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
  // �ڲ� SRAM(�����ȫ�ֱ���,������.data��.bss ��),�����ַ��ϵͳ�Զ������
  // ע��������ȡADC_ConvertedValue���ֵ�ĵ�ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;

  // ���ݴ��䷽��Ϊ���赽�洢��
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

  // ��������СΪ��ָһ�δ������������ע��������������ֽ�������DataSize���
  // ����3�ζ�����32λ����
  DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL;

  // ����Ĵ���ֻ��һ������ַ���õ���
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢���ǰ�����洢�����϶�ȡ�������������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

  // // �������ݴ�СΪ���֣��������ֽ�
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;

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
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC1_DMA_CHANNEL;
  // ��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
  DMA_Init(RHEOSTAT_ADC1_DMA_STREAM, &DMA_InitStructure);
  // ʹ��DMA��
  DMA_Cmd(RHEOSTAT_ADC1_DMA_STREAM, ENABLE);

  /* -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------ */
  // ����ADCʱ��
  RCC_APB2PeriphClockCmd(RHEOSTAT_ADC1_CLK | RHEOSTAT_ADC2_CLK | RHEOSTAT_ADC3_CLK, ENABLE);

  // ����ģʽ��ͨ���������
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_Interl;

  // ʱ��Ϊfpclk x��Ƶ
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;

  // DMA1ֱ�ӷ���ģʽ��ʹ�ö���ͬ��ģʽʱѡ��DMA1
  // ÿ����һ��DMA����ʹ���һ��ADCͨ��16λ����(CDR�Ĵ����԰��ִ���)��˳������1��2��3
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2;

  // ����ʱ������ͬ��ģʽ������������ν
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;

  ADC_CommonInit(&ADC_CommonInitStructure);

  /* -------------------ADC Init �ṹ�� ���� ��ʼ��-------------------------- */
  ADC_StructInit(&ADC_InitStructure);

  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ
  // �������ADC����ADC1��ADC2�����Խ��ɼ�һ��������ͨ�����Ҳ���Ҫ�Զ���ѯ���ͨ��������Ҫ����ɨ��ģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;

  // ����ת��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

  // ��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

  // �ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;

  // �����Ҷ���
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

  // ת��ͨ��1��
  // ��Ե���ADCģ���ת��ͨ����������
  ADC_InitStructure.ADC_NbrOfConversion = 1;

  ADC_Init(RHEOSTAT_ADC1, &ADC_InitStructure);
  ADC_Init(RHEOSTAT_ADC2, &ADC_InitStructure);
  ADC_Init(RHEOSTAT_ADC3, &ADC_InitStructure);

  //---------------------------------------------------------------------------
  // ���� ADC ͨ��ת��˳�򣬲���ʱ��Ϊ56��ʱ������
  ADC_RegularChannelConfig(RHEOSTAT_ADC1, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC2, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC3, RHEOSTAT_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

  // ʹ��DMA���� after last transfer (Multi-ADC mode)
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

  // ʹ��ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC1, ENABLE);

  // ʹ��ADC
  ADC_Cmd(RHEOSTAT_ADC1, ENABLE);
  ADC_Cmd(RHEOSTAT_ADC2, ENABLE);
  ADC_Cmd(RHEOSTAT_ADC3, ENABLE);

  // ��ʼadcת�����������,����ģʽADC1����
  ADC_SoftwareStartConv(RHEOSTAT_ADC1);
  //ADC_SoftwareStartConv(RHEOSTAT_ADC2);
}

void Rheostat_Init(void)
{
  Rheostat_ADC_GPIO_Config();
  Rheostat_ADC_Mode_Config();
}
