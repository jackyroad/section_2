/**
 * @file   bsp_breath_led.c
 * @brief  ��ɫ����������
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-11
 * @copyright Copyright (c) 2025
 */

#include "./led/bsp_breath_led.h"
#include "./table/bsp_pwm_table.h"
#include "./usart/bsp_debug_usart.h"

// ����������ε�Ƶ��
__IO uint16_t period_class = 10;

/* LED���ȵȼ� PWM��,ָ������ ���˱�ʹ�ù���Ŀ¼�µ�matlab����*/
/* led�Ƶ����ȱ仯ʵ�����Ǹ�Ƶled������ɵ�һ�����󣬲����ǵƵ����������� */

// ����PWM���ж��ٸ�Ԫ��
uint16_t POINT_NUM = sizeof(bsp_pwmTable) / sizeof(bsp_pwmTable[0]);

/**
 * @brief  ����TIM�������PWMʱ�õ���I/O
 * @param  None
 * @retval None
 */
static void TIMx_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd(BRE_LED_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(BRE_LED_GPIO_PORT, BRE_LED_PINSOURCE, BRE_LED_AF);

	/*BREATH_LED1*/
	GPIO_InitStructure.GPIO_Pin = BRE_LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(BRE_LED_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ�����
 * @param  None
 * @retval None
 */
static void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// �����ж���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = BRE_TIM_IRQn;

	// ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ����TIM�����PWM�źŵ�ģʽ�������ڡ�����
 * @param  None
 * @retval None
 */

static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// ����TIMx_CLK,x[2,3,4,5]������APB1������
	BRE_TIM_APBxClock_FUN(BRE_TIM_CLK, ENABLE);

	/* ������ʱ������ ,���PWM��������жϷ������е�period_cntѭ����������*/

	/* ����ʹ��������������Ϊ3�����Ҽ��ɴﵽ�ܺõ�Ч�� */

	// Ҫ��
	// TIM_Period����PWM������ֵ��Χһ��
	// TIM_Prescaler��ԽСԽ�ã��ɼ�����˸����
	// PERIOD_CLASS���жϷ������п��Ƶ�����ѭ���Ĵ������������ɿ���������ߵ�����
	// POINT_NUM��PWM���Ԫ�أ�����PWM������ߵĲ�������

	/*************��ʵ���е�����***************/
	/***********************************************
		#PWM����
		POINT_NUM = 256

		#���ڱ���
		PERIOD_CLASS = 10

		#��ʱ����ʱ����
		TIMER_TIM_Period = 2**10
		#��ʱ����Ƶ
		TIMER_TIM_Prescaler = 100

		#STM32ϵͳʱ��Ƶ�ʺ�����
		f_hclk = 180MHZ

		#��ʱ����ʱƵ��
		f_pclk1 = f_hclk/2 = 90M
		t_pclk1 = 1/f_pclk1 = 0.011us

		#��ʱ��update�¼����ڣ�PrescalerԤ��Ƶ���Ӿ���������ʱ��һ�����ڵ�ʱ�䱶��������˵��Ƶ���ڱ䳤��
		t_timer = t_pclk1*TIMER_TIM_Prescaler*TIMER_TIM_Period = 0.00284s = 351.56HZ

		#ÿ��PWM���ʱ��
		T_Point = t_timer * PERIOD_CLASS

		#������������
		T_Up_Down_Cycle = T_Point * POINT_NUM

		print ("�������ڣ�",T_Up_Down_Cycle)

	#���н����

	�������ڣ�2.92s

	#####ע������ı��������ʹ�������ƽ�������Ըı䶨ʱ�����ڡ������������һ�������ڽ��͵�һ�뼴��######

	************************************************************/

	TIM_TimeBaseStructure.TIM_Period = (1024 - 1);				// ����ʱ����0������ TIM_Period+1 ��Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = (100 - 1);			// ����Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ

	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BRE_TIM, &TIM_TimeBaseStructure);

	/* PWMģʽ���� */
	/* CCRֵ��ʵʱ�޸ģ�PWMռ�ձȶ�̬�仯�������ڣ�TIM_Period��ʼ�չ̶� */
	/* ÿ�δﵽccr��ֵ�ͷ�ת��ƽ��Ȼ������жϣ��ı�ccr��ֱ���ﵽTIM_Period����ʱ���Ż����� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // ����ΪPWMģʽ1����ʱ��δ�ﵽCCRʱ�����Ч��ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ʹ�����
	TIM_OCInitStructure.TIM_Pulse = 0;							  // ���ó�ʼPWM������Ϊ0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	  // ��Ч��ƽΪ�͵�ƽ�� ��Ч��ƽ�ڼ�LED����

	BRE_TIM_OCxInit(BRE_TIM, &TIM_OCInitStructure); // ʹ��ͨ��

	/*ʹ��ͨ������*/
	BRE_TIM_OCxPreloadConfig(BRE_TIM, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(BRE_TIM, ENABLE); // ʹ��TIM���ؼĴ���ARR

	// �����������ж�
	TIM_ITConfig(BRE_TIM, TIM_IT_Update, ENABLE);

	// ʹ�ܼ�����
	TIM_Cmd(BRE_TIM, ENABLE);
}

/**
 * @brief  ��ʼ��������
 * @param  None
 * @retval None
 */
void BreathLED_Config(void)
{
	BSP_PWMTable_Init();

	ExportPWMTable_Keil();

	TIMx_GPIO_Config();

	TIM_Mode_Config();

	TIMx_NVIC_Configuration();
}

/*********************************************END OF FILE**********************/
