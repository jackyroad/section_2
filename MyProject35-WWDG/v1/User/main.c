/**
 * @file   main.c
 * @brief
 * @author leshen (13762713527@qq.com)
 * @version{master} (commit:{������wwdg�жϺ����������ظ���})
 * @date 2025-04-01
 * @copyright Copyright (c) 2025
 */

#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./wwdg/bsp_wwdg.h"

static void Delay(__IO uint32_t nCount);
/**
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
  uint8_t wwdg_tr, wwdg_wr;

  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();

  // BLUE ��ɫ����
  LED3(ON);
  Delay(0XFFFFFF);

  // WWDG����

  /* WWDG ���ú���
   * tr ���ݼ���ʱ����ֵ�� ȡֵ��ΧΪ��0x7f~0x40��������Χ��ֱ�Ӹ�λ
   * wr ������ֵ��ȡֵ��ΧΪ��0x7f~0x40
   * prv��Ԥ��Ƶ��ֵ��ȡֵ������
   * @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(45MHz)/4096)/1
     Լ10968Hz 91us
   * @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(45MHz)/4096)/2
     Լ5484Hz	182us
   * @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(45MHz)/4096)/4
     Լ2742Hz	364us
   * @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(45MHz)/4096)/8
     Լ1371Hz	728us
   *
   * ����tr = 127(0x7f��tr�����ֵ)
   *     wr = 80��0x50, 0x40Ϊ��Сwr��Сֵ��
   *		 prv =  WWDG_Prescaler_86
   * ϵͳ�Ḵλ����46.6ms��û��ι����ϵͳҲ�Ḵλ��
   * ��Ҫ��ˢ�´��ڵ�ʱ����ι����ϵͳ�Ų��Ḵλ��
  */
  // ��ʼ��WWDG�����ü�������ʼֵ�������ϴ���ֵ������WWDG��ʹ����ǰ�����ж�
  WWDG_Config(127, 80, WWDG_Prescaler_8);

  // ����ֵ�����ڳ�ʼ����ʱ�����ó�0X5F�����ֵ����ı�
  wwdg_wr = WWDG->CFR & 0X7F;
  Delay(0XFFFFFF);

  while (1)
  {
    // BLUE ��ɫ��
    LED3(OFF);
    //-----------------------------------------------------
    // �ⲿ��Ӧ��д��Ҫ��WWDG��صĳ�����γ������е�ʱ��
    // �����˴���ֵӦ�����óɶ��
    //-----------------------------------------------------

    // ��ʱ��ֵ����ʼ�������0X7F��������WWDGʱ�����ֵ�᲻�ϼ�С
    // ����������ֵ���ڴ���ֵʱι���Ļ����Ḵλ�������������ٵ�0X40
    // ��û��ι���Ļ��ͷǳ��ǳ�Σ���ˣ��������ټ�һ�ε���0X3Fʱ�͸�λ
    // ����Ҫ����������ֵ�ڴ���ֵ��0X40֮���ʱ��ι��������0X40�ǹ̶��ġ�
    wwdg_tr = WWDG->CR & 0X7F;

    // ��Ҫע�����if���ж�����һ�������ڵģ����û���ڴ�����Ҳ����34.2ms~46.6ms֮������ж�
    // ��ô�����ѭ�������һֱ�����жϴ������Ҳ����˵�Գ���ִ��ʱ��Ҫ��һ����ȷ�ж�
    // �����ſ���׼ȷ����ʱ�䴰���ڼ�ι��
    // ���⻹��ע�⣬�ڴ����жϺ��������û��ι����������ô���򽫻Ḵλ
    if (wwdg_tr < wwdg_wr)
    {
      // ι�����������ü�������ֵΪ���0X7F
      WWDG_Feed();
    }
  }
}

static void Delay(__IO uint32_t nCount) // �򵥵���ʱ����
{
  for (; nCount != 0; nCount--)
    ;
}

/*********************************************END OF FILE**********************/
