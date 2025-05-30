/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC液晶显示汉字(字库在外部SD卡)
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/sdram/bsp_sdram.h"
#include "./Bsp/lcd/bsp_lcd.h"
#include "./Bsp/touch/bsp_touch_gtxx.h"
#include <string.h>

void Delay(__IO u32 nCount); 
void LCD_Test(void);

// WIFI和SD卡公用SDIO总线，当使用SD卡时，避免WIFI的冲突
// 需要把WIFI的使能引脚PDN拉低。
void WIFI_PDNPIN_SET(void)
{
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //禁用WiFi模块
} 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
	/*串口初始化*/
	Debug_USART_Config();
  
	// 失能WIFI
	WIFI_PDNPIN_SET();
	
  /* 必须先初始化触摸屏, 读取触摸芯片ID以判断不同尺寸类型的屏幕 */
  GTP_Init_ReadID();
  
  printf("\r\n提示：\r\n"
         "如果是4.3寸/5寸的RGB屏幕，使用的触摸芯片可能是GT1151QM。\r\n"
         "如果触摸或显示异常，请确认使用的屏幕触摸芯片型号，"
         "在“bsp_i2c_touch.h”文件修改宏定义 LCD_TOUCH_IC_GT1151QM。\r\n\r\n");
  
  /*初始化液晶屏*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*把背景层刷黑色*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/
		
	
  LED_BLUE;    

  Delay(0xfff);  

  while(1)
	{
		LCD_Test();
	}  
 

}



/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	static uint8_t testCNT=0;
	char dispBuff[100];
	
	testCNT++;
	
	/*使用不透明前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  LCD_SetTransparency(0xff);
	
  LCD_Clear(LCD_COLOR_BLACK);	/* 清屏，显示全黑 */

	/*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*选择字体，使用中英文显示时，尽量把英文选择成16*24的字体，
	*中文字体大小是24*24的，需要其它字体请自行制作字模*/
	/*这个函数只对英文字体起作用*/
    LCD_SetFont(&Font16x24); 	
    /* 请阅读例程列表下  屏幕相关必读说明.txt里有7、5、4.3寸屏幕选择的方法 */
    /* 默认使用5.0寸，具体配置选择在 bsp_i2c_touch.h文件里 */
 	LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t* )"野火5.0英寸液晶屏参数，");
	LCD_DisplayStringLine_EN_CH(LINE(2),(uint8_t* )"分辨率:800x480 像素");
  LCD_DisplayStringLine_EN_CH(LINE(3),(uint8_t* )"触摸屏:5点电容触摸屏");
  LCD_DisplayStringLine_EN_CH(LINE(4),(uint8_t* )"使用STM32-LTDC直接驱动,无需外部液晶驱动器");
	LCD_DisplayStringLine_EN_CH(LINE(5),(uint8_t* )"支持RGB888/565,24位数据总线");
  LCD_DisplayStringLine_EN_CH(LINE(6),(uint8_t* )"触摸屏使用IIC总线驱动");
	
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示变量例子: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(7));
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD_DisplayStringLine_EN_CH(LINE(7),(uint8_t* )dispBuff);


  /* 画直线 */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  
	LCD_ClearLine(LINE(8));
  LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"画线:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawUniLine(50,250,750,250);  
  LCD_DrawUniLine(50,300,750,300);
  
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawUniLine(300,250,400,400);  
  LCD_DrawUniLine(600,250,600,400);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  
  
  /*画矩形*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
  LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"画矩形:");
	
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawRect(200,350,50,200);
  
  Delay(0xFFFFFF);
  
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
  

  /*填充矩形*/
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"填充矩形:");

 	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,250,200,100);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullRect(350,250,200,50);
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
  LCD_DrawFullRect(200,350,50,200);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
  /* 画圆 */
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"画圆:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
  LCD_DrawCircle(200,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
  LCD_DrawCircle(350,350,75);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


  /*填充圆*/
	LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_ClearLine(LINE(8));
	LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"填充圆:");
  
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(300,350,50);
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
  LCD_DrawFullCircle(450,350,75);
  
  Delay(0xFFFFFF);
  
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
	
	LCD_ClearLine(LINE(8));

//这种操作效果会有花屏现象，不推荐使用。一般背景混合使用ARGB格式的颜色来混合		
//	/*透明效果 前景层操作*/
//	{
//		LCD_SetTextColor(LCD_COLOR_BLUE);
//		LCD_ClearLine(LINE(8));
//		LCD_DisplayStringLine_EN_CH(LINE(8),(uint8_t* )"双层透明效果：");
//		
//		/*设置前景层不透明度*/
//		LCD_SetLayer(LCD_FOREGROUND_LAYER); 	
//		LCD_SetTransparency(200);
//		
//		/*在前景画一个红色圆*/
//		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
//		LCD_DrawFullCircle(400,350,75);
//	
//	}
//	
//	/*透明效果 背景层操作*/
//	{		
//		/*设置背景层不透明*/
//		LCD_SetLayer(LCD_BACKGROUND_LAYER); 	
//		LCD_SetTransparency(0xff);
//		LCD_Clear(LCD_COLOR_BLACK);

//		/*在背景画一个绿色圆*/
//		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//		LCD_DrawFullCircle(450,350,75);
//		
//		/*在背景画一个蓝色圆*/
//		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
//		LCD_DrawFullCircle(350,350,75);
//	}
  
  Delay(0xFFFFFF);
	
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

