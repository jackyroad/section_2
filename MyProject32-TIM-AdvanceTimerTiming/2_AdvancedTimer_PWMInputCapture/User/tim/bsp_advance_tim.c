/**
 * @file   bsp_advance_tim.c
 * @brief  通用定时器PWM输出和输入捕获
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-08
 * @copyright Copyright (c) 2025
 */
  
#include "./tim/bsp_advance_tim.h"

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
  RCC_AHB1PeriphClockCmd(GENERAL_OCPWM_GPIO_CLK | 
                         ADVANCE_ICPWM_GPIO_CLK, ENABLE);

	/* 定时器复用引脚 */
	GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT,GENERAL_OCPWM_PINSOURCE,GENERAL_OCPWM_AF); 
	GPIO_PinAFConfig(ADVANCE_ICPWM_GPIO_PORT,ADVANCE_ICPWM_PINSOURCE,ADVANCE_ICPWM_AF); 
	
	/* 通用定时器PWM输出引脚 */															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	/* 高级控制定时器PWM输入捕获引脚 */
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_PIN;	
	GPIO_Init(ADVANCE_ICPWM_GPIO_PORT, &GPIO_InitStructure);
}

 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  None
  * @retval None
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  /* 配置输出比较 */
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14]---通用定时器里面的这些定时器挂载在APB1上
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* --------------------定时器共用的配置--------------------------------- */
  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到9999，即为10000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	
	// 通用定时器2时钟源TIMxCLK = HCLK/2=90MHz，虽然APB1是45M，单最终定时器的时钟频率是45x2=90MHZ
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  // 0.1s产生一个中断
  TIM_TimeBaseStructure.TIM_Prescaler = 900-1;	

  // 采样时钟分频
  // 影响输入通道的采样时钟，用于输入捕获配置
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
  /* -------------------------------------------------------------------- */

  /* PWM输出模式配置 */
  /* 配置为PWM模式1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	

	/* PWM脉冲宽度 */
  TIM_OCInitStructure.TIM_Pulse = 3000-1;

  /* 有效电平为高电平 */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* 使能通道1 */
  TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);	

	/*使能通道1重载*/
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

	// 使能定时器
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

static void TIM_PWMINPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	// 开启TIMx_CLK,x[1,8] 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 

  /* 捕获定时器的时钟周期应该要比要捕获的信号周期长，这里直接取最大值(16位) */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF-1; 	

	// 高级控制定时器时钟源TIMxCLK = HCLK=180MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 1800-1;	

  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
  
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
	/* IC1捕获：上升沿触发 TI1FP1 */
  /* 计算信号周期 */
  TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	/* IC2捕获：下降沿触发 TI1FP2 */	
  /* 通过硬件自动切换捕获边沿并触发计数器清零 */
  /* 计算占空比 */
	TIM_ICInitStructure.TIM_Channel = ADVANCE_IC2PWM_CHANNEL;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	/* 选择定时器输入触发: TI1FP1 */
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

  /* 选择从模式: 复位模式 */
  /* 当TI1上升沿时，计数器自动清零 */
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);

  /* 使能高级控制定时器 */
  TIM_Cmd(ADVANCE_TIM, ENABLE);

  /* 使能捕获/比较2中断请求 */
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
}


/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_GPIO_Config();
	
	TIMx_NVIC_Configuration();	
  
  TIM_PWMOUTPUT_Config();
	
	TIM_PWMINPUT_Config();
}

/*********************************************END OF FILE**********************/
