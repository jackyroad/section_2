/**
 * @file   bsp_breath_led.c
 * @brief  单色呼吸灯驱动
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-11
 * @copyright Copyright (c) 2025
 */

#include "./led/bsp_breath_led.h"
#include "./table/bsp_pwm_table.h"
#include "./usart/bsp_debug_usart.h"

// 控制输出波形的频率
__IO uint16_t period_class = 10;

/* LED亮度等级 PWM表,指数曲线 ，此表使用工程目录下的matlab生成*/
/* led灯的亮度变化实际上是高频led开关造成的一种现象，并不是灯的亮度提升了 */

// 计算PWM表有多少个元素
uint16_t POINT_NUM = sizeof(bsp_pwmTable) / sizeof(bsp_pwmTable[0]);

/**
 * @brief  配置TIM复用输出PWM时用到的I/O
 * @param  None
 * @retval None
 */
static void TIMx_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启LED相关的GPIO外设时钟*/
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
 * @brief  通用定时器 TIMx,x[2,3,4,5]中断优先级配置
 * @param  None
 * @retval None
 */
static void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// 设置中断组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = BRE_TIM_IRQn;

	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  配置TIM输出的PWM信号的模式，如周期、极性
 * @param  None
 * @retval None
 */

static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 开启TIMx_CLK,x[2,3,4,5]，都在APB1总线上
	BRE_TIM_APBxClock_FUN(BRE_TIM_CLK, ENABLE);

	/* 基本定时器配置 ,配合PWM表点数、中断服务函数中的period_cnt循环次数设置*/

	/* 设置使得整个呼吸过程为3秒左右即可达到很好的效果 */

	// 要求：
	// TIM_Period：与PWM表中数值范围一致
	// TIM_Prescaler：越小越好，可减轻闪烁现象
	// PERIOD_CLASS：中断服务函数中控制单个点循环的次数，调整它可控制拟合曲线的周期
	// POINT_NUM：PWM表的元素，它是PWM拟合曲线的采样点数

	/*************本实验中的配置***************/
	/***********************************************
		#PWM点数
		POINT_NUM = 256

		#周期倍数
		PERIOD_CLASS = 10

		#定时器定时周期
		TIMER_TIM_Period = 2**10
		#定时器分频
		TIMER_TIM_Prescaler = 100

		#STM32系统时钟频率和周期
		f_hclk = 180MHZ

		#定时器计时频率
		f_pclk1 = f_hclk/2 = 90M
		t_pclk1 = 1/f_pclk1 = 0.011us

		#定时器update事件周期，Prescaler预分频因子就是提升定时器一个周期的时间倍数，就是说降频周期变长了
		t_timer = t_pclk1*TIMER_TIM_Prescaler*TIMER_TIM_Period = 0.00284s = 351.56HZ

		#每个PWM点的时间
		T_Point = t_timer * PERIOD_CLASS

		#整个呼吸周期
		T_Up_Down_Cycle = T_Point * POINT_NUM

		print ("呼吸周期：",T_Up_Down_Cycle)

	#运行结果：

	呼吸周期：2.92s

	#####注意如果改变数组点数使得输出更平滑，可以改变定时器周期。比如点数提升一倍，周期降低到一半即可######

	************************************************************/

	TIM_TimeBaseStructure.TIM_Period = (1024 - 1);				// 当定时器从0计数到 TIM_Period+1 ，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = (100 - 1);			// 设置预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式

	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BRE_TIM, &TIM_TimeBaseStructure);

	/* PWM模式配置 */
	/* CCR值可实时修改，PWM占空比动态变化，但周期（TIM_Period）始终固定 */
	/* 每次达到ccr的值就反转电平，然后进入中断，改变ccr，直到达到TIM_Period，计时器才会清零 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // 配置为PWM模式1，计时器未达到CCR时输出有效电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
	TIM_OCInitStructure.TIM_Pulse = 0;							  // 设置初始PWM脉冲宽度为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	  // 有效电平为低电平， 有效电平期间LED灯亮

	BRE_TIM_OCxInit(BRE_TIM, &TIM_OCInitStructure); // 使能通道

	/*使能通道重载*/
	BRE_TIM_OCxPreloadConfig(BRE_TIM, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(BRE_TIM, ENABLE); // 使能TIM重载寄存器ARR

	// 开启计数器中断
	TIM_ITConfig(BRE_TIM, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(BRE_TIM, ENABLE);
}

/**
 * @brief  初始化呼吸灯
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
