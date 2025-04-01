#include "bsp_iwdg.h"

/**
 * @brief  独立看门狗配置
 * IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 * @param  prv:预分频器值，取值如下：
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 *		独立看门狗使用LSI作为时钟。
 *		LSI 的频率一般在 30~60KHZ 之间，根据温度和工作场合会有一定的漂移，我
 *		们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度
 *		要求比较低的场合。
 *
 * @param  rlv:重装载寄存器的值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出
 *						（64/40）*625 = 1s
 * @retval  None
 */
void IWDG_Config(uint8_t prv, uint16_t rlv)
{
    /* 使能预分频寄存器PR和重装寄存器RLR可写*/
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* 设置预分频器因子 */
    IWDG_SetPrescaler(prv);

    /* 设置重装寄存器值 */
    IWDG_SetReload(rlv);

    /* 把重装寄存器的值放入计数器中 */
    IWDG_ReloadCounter();

    /* 使能IWDG */
    IWDG_Enable();
}

// 喂狗函数
void IWDG_Feed(void)
{
    /* 把重装寄存器的值放入计数器中 */
    IWDG_ReloadCounter();
}




