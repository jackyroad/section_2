/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   CAN通讯实验(回环模式)
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
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./can/bsp_can.h"
#include "./key/bsp_key.h"

__IO uint32_t flag = 0;		 //用于标志是否接收到数据，在中断函数中赋值
CanTxMsg TxMessage;			     //发送缓冲区
CanRxMsg RxMessage;				 //接收缓冲区

/// 不精确的延时
static void can_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	LED_GPIO_Config();
	
  /*初始化USART1*/
  Debug_USART_Config();
	
	/*初始化按键*/
	Key_GPIO_Config();
 
	/*初始化can,在中断接收CAN数据包*/
	CAN_Config();
	
	printf("\r\n 欢迎使用野火  STM32 F429 开发板。\r\n");
  printf("\r\n 野火F429 CAN通讯回环实验例程\r\n");
	
	printf("\r\n 实验步骤：\r\n");

	printf("\r\n 1.使用回环模式，不需要连接外部芯片\r\n");
	printf("\r\n 2.按下开发板的KEY1键，会使用CAN向外发送0-7的数据包，包的扩展ID为0x1314 (由于回环模式，是自己发送给自己)\r\n");
	printf("\r\n 3.若开发板的CAN接收到扩展ID为0x1314的数据包，会把数据以打印到串口。 \r\n");
	printf("\r\n 4.本例中的can波特率为1MBps，为stm32的can最高速率。 \r\n");

  while(1)
	{
		/*按一次按键发送一次数据*/
		if(	Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
		{
			LED_BLUE;
			CAN_SetMsg(&TxMessage);
			
			CAN_Transmit(CANx, &TxMessage);
			
			can_delay(10000);//等待发送完毕，可使用CAN_TransmitStatus查看状态
			
			LED_GREEN;
			
			printf("\r\n已使用CAN发送数据包！\r\n"); 			
			printf("\r\n发送的报文内容为：\r\n");
			printf("\r\n 扩展ID号ExtId：0x%x \r\n",TxMessage.ExtId);
			CAN_DEBUG_ARRAY(TxMessage.Data,8); 


		}
		if(flag==1)
		{		
			LED_GREEN;
			printf("\r\nCAN接收到数据：\r\n");	

			CAN_DEBUG_ARRAY(RxMessage.Data,8); 
			
			flag=0;
		}
	
	
	}

}



/*********************************************END OF FILE**********************/

