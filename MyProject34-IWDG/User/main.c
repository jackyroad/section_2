/**
 * @file   main.c
 * @brief 
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{gitCommitShort})
 * @date 2025-03-31
 * @copyright Copyright (c) 2025
 */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./iwdg/bsp_iwdg.h"
#include "./key/bsp_key.h"
#include "./tim/bsp_basic_time.h"


static void Delay(__IO uint32_t nCount);

int main(void)
{
  /* LED 端口初始化 */
  LED_GPIO_Config();

  Delay(0X8FFFFF);

  /* 检查是否为独立看门狗复位 */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* 独立看门狗复位 */
    /*  亮红灯 */
    LED_RED;

    /* 清除标志 */
    RCC_ClearFlag();

    /*如果一直不喂狗，会一直复位，加上前面的延时，会看到红灯闪烁
    在1s 时间内喂狗的话，则会持续亮绿灯*/
  }
  else
  {
    /*不是独立看门狗复位(可能为上电复位或者手动按键复位之类的) */
    /* 亮蓝灯 */
    LED_BLUE;
  }

  /*初始化按键*/
  Key_GPIO_Config();

  // IWDG 1s 超时溢出
  IWDG_Config(IWDG_Prescaler_64, 625);

  // while部分是我们在项目中具体需要写的代码，这部分的程序可以用独立看门狗来监控
  // 如果我们知道这部分代码的执行时间，比如是500ms，那么我们可以设置独立看门狗的
  // 溢出时间是600ms，比500ms多一点，如果要被监控的程序没有跑飞正常执行的话，那么
  // 执行完毕之后就会执行喂狗的程序，如果程序跑飞了那程序就会超时，到达不了喂狗的
  // 程序，此时就会产生系统复位。但是也不排除程序跑飞了又跑回来了，刚好喂狗了，
  // 歪打正着。所以要想更精确的监控程序，可以使用窗口看门狗，窗口看门狗规定必须
  // 在规定的窗口时间内喂狗。
  while (1)
  {
    // 这里添加需要被监控的代码，如果有就去掉按键模拟喂狗，把按键扫描程序去掉
    //--------------------------------------------------------------------------
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      // 喂狗，如果不喂狗，系统则会复位，复位后亮红灯，如果在1s
      // 时间内准时喂狗的话，则会亮绿灯
      IWDG_Feed();
      // 喂狗后亮绿灯
      LED_GREEN;
    }
  }
}

static void Delay(__IO uint32_t nCount) // 简单的延时函数
{
  for (; nCount != 0; nCount--)
    ;
}

/*********************************************END OF FILE**********************/

