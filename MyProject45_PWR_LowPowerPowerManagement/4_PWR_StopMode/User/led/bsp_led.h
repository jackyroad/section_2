#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"


/* ���Ŷ��� (ʹ�ñ�׼���) ---------------------------------*/
// RGB LED (PH10/11/12)
#define LED1_PIN                  GPIO_Pin_10     // ��
#define LED1_GPIO_PORT            GPIOH
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define LED2_PIN                  GPIO_Pin_11     // ��
#define LED2_GPIO_PORT            GPIOH
#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define LED3_PIN                  GPIO_Pin_12     // ��
#define LED3_GPIO_PORT            GPIOH
#define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOH

// ״ָ̬ʾ�� (PD11)
#define LED4_PIN                  GPIO_Pin_11
#define LED4_GPIO_PORT            GPIOD
#define LED4_GPIO_CLK             RCC_AHB1Periph_GPIOD

/* ��ƽ���� ------------------------------------------------*/
#define ON  0    // �͵�ƽ����LED
#define OFF 1    // �ߵ�ƽϨ��LED

/* �Ĵ��������� (����F4��׼��) -----------------------------*/
// ��ȫд����ʹ��do-while(0)�����﷨����
#define digitalHi(p,i)    do{ (p)->BSRR = ((i) << 0);  }while(0)  // �ø�
#define digitalLo(p,i)    do{ (p)->BSRR = ((i) << 16); }while(0)  // �õ�
#define digitalToggle(p,i) do{ (p)->ODR ^= (i); }while(0)         // ��ת

/* ���ο��ƺ� ----------------------------------------------*/
#define LED1(a) do{ \
    (a) ? GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN) : GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN); \
}while(0)

#define LED2(a) do{ \
    (a) ? GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN) : GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN); \
}while(0)

#define LED3(a) do{ \
    (a) ? GPIO_SetBits(LED3_GPIO_PORT, LED3_PIN) : GPIO_ResetBits(LED3_GPIO_PORT, LED3_PIN); \
}while(0)

#define LED4(a) do{ \
    (a) ? GPIO_SetBits(LED4_GPIO_PORT, LED4_PIN) : GPIO_ResetBits(LED4_GPIO_PORT, LED4_PIN); \
}while(0)

/* ֱ�ӿ��ƺ� ----------------------------------------------*/
#define LED1_ON      digitalLo(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF     digitalHi(LED1_GPIO_PORT, LED1_PIN)
#define LED1_TOGGLE  digitalToggle(LED1_GPIO_PORT, LED1_PIN)

#define LED2_ON      digitalLo(LED2_GPIO_PORT, LED2_PIN)
#define LED2_OFF     digitalHi(LED2_GPIO_PORT, LED2_PIN)
#define LED2_TOGGLE  digitalToggle(LED2_GPIO_PORT, LED2_PIN)

#define LED3_ON      digitalLo(LED3_GPIO_PORT, LED3_PIN)
#define LED3_OFF     digitalHi(LED3_GPIO_PORT, LED3_PIN)
#define LED3_TOGGLE  digitalToggle(LED3_GPIO_PORT, LED3_PIN)

#define LED4_ON      digitalLo(LED4_GPIO_PORT, LED4_PIN)
#define LED4_OFF     digitalHi(LED4_GPIO_PORT, LED4_PIN)
#define LED4_TOGGLE  digitalToggle(LED4_GPIO_PORT, LED4_PIN)

/* RGB��ɫ���ƺ� -------------------------------------------*/
// ʹ�÷�б�ܻ���ʱȷ�����һ�������з�
#define LED_RED     do{ LED1_ON;  LED2_OFF; LED3_OFF; }while(0)
#define LED_GREEN   do{ LED1_OFF; LED2_ON;  LED3_OFF; }while(0)
#define LED_BLUE    do{ LED1_OFF; LED2_OFF; LED3_ON;  }while(0)
#define LED_YELLOW  do{ LED1_ON;  LED2_ON;  LED3_OFF; }while(0)
#define LED_PURPLE  do{ LED1_ON;  LED2_OFF; LED3_ON;  }while(0)
#define LED_CYAN    do{ LED1_OFF; LED2_ON;  LED3_ON;  }while(0)
#define LED_WHITE   do{ LED1_ON;  LED2_ON;  LED3_ON;  }while(0)
#define LED_RGBOFF  do{ LED1_OFF; LED2_OFF; LED3_OFF; }while(0)

/* �������� ------------------------------------------------*/
void LED_GPIO_Config(void);

#endif /* __LED_H */
