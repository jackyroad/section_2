#include "bsp_basic_time.h"

/**
 * @brief 基本定时器初始化
 * @note  TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 * 
 * TIM_Prescaler             都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period                都有
 * TIM_ClockDivision         TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter     TIMx,x[1,8]才有(高级定时器)
 * 
 */
static void TIMx_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 开启APB1基本定时器外设时钟*/
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);

    //定时器时钟源TIMxCLK = 2 * PCLK1  
    //				PCLK1 = HCLK / 4 
    //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 9000 - 1;
    
    /* 累计 TIM_Period个后产生一个更新或者中断*/		
    //当定时器从0计数到4999，即为5000次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Period = 5000 - 1;

    /* 初始化定时器 */
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    /* 清除定时器更新中断标志 */
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    /* 开启定时器更新中断.当定时器计数器（TIMx_CNT）达到重装载值（TIMx_ARR）时，会发生"更新事件" */
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);

    /* 使能定时器 */
    TIM_Cmd(BASIC_TIM,ENABLE);
}

/**
 * @brief 基本定时器中断配置
 */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

     /* 设置优先级分组0 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* 设置中断来源 */
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn;

    /* 设置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

    /* 设置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 基本定时器初始化，定时结束产生0.5中断
 */
void TIMx_Configuration(void)
{
    TIMx_NVIC_Configuration();

    TIMx_Mode_Config();
}
