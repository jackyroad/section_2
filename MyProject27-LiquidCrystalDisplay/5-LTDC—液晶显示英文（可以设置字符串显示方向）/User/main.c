/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC-Һ����ʾӢ�ģ��ֿ����ڲ�FLASH
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"
#include <string.h>

void Delay(__IO u32 nCount); 
void LCD_Test(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
  
  /* ���Դ��ڳ�ʼ�� */
  Debug_USART_Config();
  /* �����ȳ�ʼ��������, ��ȡ����оƬID���жϲ�ͬ�ߴ����͵���Ļ */
  GTP_Init_ReadID();
  
  printf("\r\n��ʾ��\r\n"
         "�����4.3��/5���RGB��Ļ��ʹ�õĴ���оƬ������GT1151QM��\r\n"
         "�����������ʾ�쳣����ȷ��ʹ�õ���Ļ����оƬ�ͺţ�"
         "�ڡ�bsp_i2c_touch.h���ļ��޸ĺ궨�� LCD_TOUCH_IC_GT1151QM��\r\n\r\n");
  
  /*��ʼ��Һ����*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
  LCD_Clear(LCD_COLOR_BLACK);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
  LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  LCD_SetTransparency(0xFF);
  LCD_Clear(LCD_COLOR_YELLOW);
  //LCD_Clear(LCD_COLOR_RED);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/		
	
  LED_BLUE;    

  Delay(0xfff);  
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_GREEN);
  while(1)
	{

	  LCD_DisplayStringLine(LINE(0),(uint8_t* )"BH 5.0 inch LCD para:");
	  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Image resolution:800x480 px");
	  LCD_DisplayStringLine(LINE(4),(uint8_t* )"Touch pad:5 point touch supported");
	  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Use STM32-LTDC directed driver,");
	  LCD_DisplayStringLine(LINE(8),(uint8_t* )"no extern lcd driver needed,RGB888,24bits data bus");
	  LCD_DisplayStringLine(LINE(10),(uint8_t* )"Touch pad use IIC to communicate");
	}  

}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

