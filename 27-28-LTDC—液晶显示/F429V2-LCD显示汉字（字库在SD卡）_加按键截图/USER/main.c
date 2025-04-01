/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDCҺ����ʾ����(�ֿ����ⲿSD��)_�Ӱ�����ͼ
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
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/sdram/bsp_sdram.h"
#include "./Bsp/lcd/bsp_lcd.h"
#include "./Bsp/touch/bsp_touch_gtxx.h"
#include "./Bsp/bmp/bmp_ScreenShot.h"
#include "./Bsp/key/bsp_key.h"

#include <string.h>

void Delay(__IO u32 nCount); 
void LCD_Test(void);

// WIFI��SD������SDIO���ߣ���ʹ��SD��ʱ������WIFI�ĳ�ͻ
// ��Ҫ��WIFI��ʹ������PDN���͡�
void WIFI_PDNPIN_SET(void)
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //����WiFiģ��
} 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
	/*���ڳ�ʼ��*/
	Debug_USART_Config();
  
	// ʧ��WIFI
	WIFI_PDNPIN_SET();
	
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
	LCD_Clear(LCD_COLOR_BLACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
		
	
  /* �ļ�ϵͳ������fonts.c��GetGBKCode_from_sd������ */
  LED_BLUE;    

  Delay(0xfff);
  
  LCD_Test();
  
  while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      uint8_t res = 0;
      
      printf("��ʼ��ͼ\r\n");

      res = screen_shot(LCD_FOREGROUND_LAYER);
      
      if(res == 0)
      {
        printf("��ͼ�ɹ�\r\n");
      }
      else
      {
        printf("��ͼʧ��\r\n");
      }
    }
	}
}



/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	static uint8_t testCNT=0;
	char dispBuff[100];
	
	testCNT++;
	
	/*ʹ�ò�͸��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  LCD_SetTransparency(0xff);
	
  LCD_Clear(LCD_COLOR_BLACK);	/* ��������ʾȫ�� */

	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*ѡ�����壬ʹ����Ӣ����ʾʱ��������Ӣ��ѡ���16*24�����壬
	*���������С��24*24�ģ���Ҫ��������������������ģ*/
	/*�������ֻ��Ӣ������������*/
    LCD_SetFont(&Font16x24); 	
    /* ���Ķ������б���  ��Ļ��رض�˵��.txt����7��5��4.3����Ļѡ��ķ��� */
    /* Ĭ��ʹ��5.0�磬��������ѡ���� bsp_i2c_touch.h�ļ��� */
 	LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t* )"Ұ��5.0Ӣ��Һ����������");
	LCD_DisplayStringLine_EN_CH(LINE(2),(uint8_t* )"�ֱ���:800x480 ����");
  LCD_DisplayStringLine_EN_CH(LINE(3),(uint8_t* )"������:5����ݴ�����");
  LCD_DisplayStringLine_EN_CH(LINE(4),(uint8_t* )"ʹ��STM32-LTDCֱ������,�����ⲿҺ��������");
	LCD_DisplayStringLine_EN_CH(LINE(5),(uint8_t* )"֧��RGB888/565,24λ��������");
  LCD_DisplayStringLine_EN_CH(LINE(6),(uint8_t* )"������ʹ��IIC��������");
	
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"��ʾ��������: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(7));
	
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine_EN_CH(LINE(7),(uint8_t* )dispBuff);

  /*������*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"������:");

 	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,350,50,200);
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

