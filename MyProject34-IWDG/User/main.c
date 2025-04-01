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
  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();

  Delay(0X8FFFFF);

  /* ����Ƿ�Ϊ�������Ź���λ */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* �������Ź���λ */
    /*  ����� */
    LED_RED;

    /* �����־ */
    RCC_ClearFlag();

    /*���һֱ��ι������һֱ��λ������ǰ�����ʱ���ῴ�������˸
    ��1s ʱ����ι���Ļ������������̵�*/
  }
  else
  {
    /*���Ƕ������Ź���λ(����Ϊ�ϵ縴λ�����ֶ�������λ֮���) */
    /* ������ */
    LED_BLUE;
  }

  /*��ʼ������*/
  Key_GPIO_Config();

  // IWDG 1s ��ʱ���
  IWDG_Config(IWDG_Prescaler_64, 625);

  // while��������������Ŀ�о�����Ҫд�Ĵ��룬�ⲿ�ֵĳ�������ö������Ź������
  // �������֪���ⲿ�ִ����ִ��ʱ�䣬������500ms����ô���ǿ������ö������Ź���
  // ���ʱ����600ms����500ms��һ�㣬���Ҫ����صĳ���û���ܷ�����ִ�еĻ�����ô
  // ִ�����֮��ͻ�ִ��ι���ĳ�����������ܷ����ǳ���ͻᳬʱ�����ﲻ��ι����
  // ���򣬴�ʱ�ͻ����ϵͳ��λ������Ҳ���ų������ܷ������ܻ����ˣ��պ�ι���ˣ�
  // ������š�����Ҫ�����ȷ�ļ�س��򣬿���ʹ�ô��ڿ��Ź������ڿ��Ź��涨����
  // �ڹ涨�Ĵ���ʱ����ι����
  while (1)
  {
    // ���������Ҫ����صĴ��룬����о�ȥ������ģ��ι�����Ѱ���ɨ�����ȥ��
    //--------------------------------------------------------------------------
    if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      // ι���������ι����ϵͳ��Ḵλ����λ������ƣ������1s
      // ʱ����׼ʱι���Ļ���������̵�
      IWDG_Feed();
      // ι�������̵�
      LED_GREEN;
    }
  }
}

static void Delay(__IO uint32_t nCount) // �򵥵���ʱ����
{
  for (; nCount != 0; nCount--)
    ;
}

/*********************************************END OF FILE**********************/

