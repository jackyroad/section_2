/**
 * @file   bsp_color_led.c
 * @brief  定时器和LED设置
 * @author leshen (13762713527@qq.com)
 * @date 2025-04-09
 * @copyright Copyright (c) 2025
 */
  
#include "./led/bsp_color_led.h"


 /**
  * @brief  配置TIM3复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( COLOR_TIM_GPIO_CLK, ENABLE); 

		GPIO_PinAFConfig(COLOR_RED_GPIO_PORT,COLOR_RED_PINSOURCE,COLOR_RED_AF); 
		GPIO_PinAFConfig(COLOR_GREEN_GPIO_PORT,COLOR_GREEN_PINSOURCE,COLOR_GREEN_AF); 
		GPIO_PinAFConfig(COLOR_BLUE_GPIO_PORT,COLOR_BLUE_PINSOURCE,COLOR_BLUE_AF);  
	
		/*COLOR_LED1*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_RED_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
		GPIO_Init(COLOR_RED_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED2*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_GREEN_PIN;	
    GPIO_Init(COLOR_GREEN_GPIO_PORT, &GPIO_InitStructure);	
    
    /*COLOR_LED3*/															   
		GPIO_InitStructure.GPIO_Pin = COLOR_BLUE_PIN;	
    GPIO_Init(COLOR_BLUE_GPIO_PORT, &GPIO_InitStructure);		
}


/**
  * @brief  配置COLOR_TIMx输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	// 开启TIMx_CLK,x[2,3,4,5] 
  COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK, ENABLE); 
	
  /* 累计 TIM_Period个后产生一个更新或者中断*/		 
  TIM_TimeBaseStructure.TIM_Period = 256-1;       //当定时器从0计数到255，即为256次，为一个定时周期
	
	//定时器时钟源TIMxCLK = 2 * PCLK1  
  //				PCLK1 = HCLK / 4 
  //				=> TIMxCLK = HCLK / 2 = SystemCoreClock /2
	// 定时器频率为 = TIMxCLK/(TIM_Prescaler+1) = (SystemCoreClock /2)/((SystemCoreClock/2)/1000000)*30 = 1000000/30 = 1/30MHz
  
	/*基本定时器配置TIM_Prescaler根据效果来设置即可，中断周期小 灯闪烁快，大则闪烁缓慢*/
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/2)/1000000)*30-1;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;		 //设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
  	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(COLOR_TIM, &TIM_TimeBaseStructure);	
	
	
	/*PWM模式配置，TIM5只需要配置这4个，其他的内容属于高级定时器 */
	/* PWM1 Mode configuration: Channel1 */
  /*                宏定义            值    模式说明
              TIM_OCMode_Timing    0x0000  定时模式  （不改变输出引脚电平，仅触发中断 /DMA）
              TIM_OCMode_Active    0x0010  强制高电平（匹配时输出高电平）
              TIM_OCMode_Inactive  0x0020  强制低电平（匹配时输出低电平）
              TIM_OCMode_Toggle    0x0030  翻转模式  （匹配时翻转输出电平）
              TIM_OCMode_PWM1      0x0060  PWM 模式 1（CNT <CCRx 时有效电平，CNT ≥ CCRx 时无效电平）
              TIM_OCMode_PWM2      0x0070  PWM 模式 2（CNT < CCRx 时无效电平，CNT ≥ CCRx 时有效电平）
  */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;	  												//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR_Val时为低电平 LED灯亮
  
  /* TIM5定时器的三个输出通道CH1,CH2,CH3分别对应R-LED,G-LED,B-LED，
  就是说对应三个GPIO根据配置的RCC寄存器里面的值输出对应的PWM信号 */
  //使能通道  
  COLOR_RED_TIM_OCxInit(COLOR_TIM, &TIM_OCInitStructure);	 							
	/*使能通道重载*/
	COLOR_RED_TIM_OCxPreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);
	
	//使能通道  
	COLOR_GREEN_TIM_OCxInit(COLOR_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	COLOR_GREEN_TIM_OCxPreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);

  //使能通道  
  COLOR_BLUE_TIM_OCxInit(COLOR_TIM, &TIM_OCInitStructure);	 
	/*使能通道重载*/
	COLOR_BLUE_TIM_OCxPreloadConfig(COLOR_TIM, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(COLOR_TIM, ENABLE);			//使能TIM重载寄存器ARR
	
	// 使能计数器
	TIM_Cmd(COLOR_TIM, ENABLE);																		
}


 /**
  * @brief  设置RGB LED的颜色
	* @param  rgb:要设置LED显示的颜色值格式RGB888(实际上只用到了24位，有意义的范围为[0,2^24-1])
  * @retval 无
  */
void SetRGBColor(uint32_t rgb)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIM->COLOR_RED_CCRx = (uint8_t)(rgb>>16);			//R
	COLOR_TIM->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);	  //G     
	COLOR_TIM->COLOR_BLUE_CCRx = (uint8_t)rgb;				  //B
}


/**
 * @brief  设置RGB LED的颜色
 * @param  rgb:要设置LED显示的颜色值[0:255]
 * @retval None
 * @note   这个函数的功能与上面函数SetRGBColor一致，不过输入参数不一样
 */
void SetColorValue(uint8_t r,uint8_t g,uint8_t b)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIM->COLOR_RED_CCRx = r;	
	COLOR_TIM->COLOR_GREEN_CCRx = g;	  
	COLOR_TIM->COLOR_BLUE_CCRx = b;	
}


/**
  * @brief  初始化呼吸灯
  * @param  无
  * @retval 无
  */
void ColorLED_Config(void)
{
	TIMx_GPIO_Config();
	
	TIM_Mode_Config();
	
	//上电后默认显示
	SetColorValue(0xff,0xff,0xff);
}

/*********************************************END OF FILE**********************/
