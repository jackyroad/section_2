#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32f4xx.h"

/*使用软件IIC，把SOFT_IIC宏设置为1，硬件IIC则设置为0
!!使用硬件IIC时非常容易出现错误，不推荐*/
#define SOFT_IIC      0

/* STM32 I2C 快速模式 */
#define I2C_Speed                        400000

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS7                 0x0A


/*设定使用的电容屏IIC设备地址*/

/* 如果用的是4.3寸/5寸的RGB屏，其触摸芯片可能是GT1151QM，它的I2C地址不同，需要定义这个宏切换一下地址 */
#define LCD_TOUCH_IC_GT1151QM


#ifndef LCD_TOUCH_IC_GT1151QM
#define GTP_ADDRESS                      0xBA
#else
#define GTP_ADDRESS                      0x28
/* 4.3寸和5寸电容屏都用 GT1151QM 这个新触摸芯片，
   只能选择这两个屏幕当中的一个，默认选5寸电容屏（定义宏为1选中）*/
  #define TOUCH_GT1151QM_LCD_4_3    0   //使用的是4.3寸电容屏(GT1151QM)
  #define TOUCH_GT1151QM_LCD_5      1   //使用的是5寸电容屏(GT1151QM)
#endif


#define I2CT_FLAG_TIMEOUT                ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT                ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#if 0
#define GTP_I2C                          I2C1
#define GTP_I2C_CLK                      RCC_APB1Periph_I2C1

#define GTP_I2C_SCL_PIN                  GPIO_Pin_6                  
#define GTP_I2C_SCL_GPIO_PORT            GPIOB                       
#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define GTP_I2C_SCL_SOURCE               GPIO_PinSource6
#define GTP_I2C_SCL_AF                   GPIO_AF_I2C1

#define GTP_I2C_SDA_PIN                  GPIO_Pin_7                  
#define GTP_I2C_SDA_GPIO_PORT            GPIOB                     
#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define GTP_I2C_SDA_SOURCE               GPIO_PinSource7
#define GTP_I2C_SDA_AF                   GPIO_AF_I2C1
#else
#define GTP_I2C                          I2C2
#define GTP_I2C_CLK                      RCC_APB1Periph_I2C2

#define GTP_I2C_SCL_PIN                  GPIO_Pin_4                  
#define GTP_I2C_SCL_GPIO_PORT            GPIOH                       
#define GTP_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define GTP_I2C_SCL_SOURCE               GPIO_PinSource4
#define GTP_I2C_SCL_AF                   GPIO_AF_I2C2

#define GTP_I2C_SDA_PIN                  GPIO_Pin_5                  
#define GTP_I2C_SDA_GPIO_PORT            GPIOH                     
#define GTP_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOH
#define GTP_I2C_SDA_SOURCE               GPIO_PinSource5
#define GTP_I2C_SDA_AF                   GPIO_AF_I2C2
#endif

#define GTP_RST_GPIO_PORT                GPIOI
#define GTP_RST_GPIO_CLK                 RCC_AHB1Periph_GPIOI
#define GTP_RST_GPIO_PIN                 GPIO_Pin_8

#define GTP_INT_GPIO_PORT                GPIOD
#define GTP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define GTP_INT_GPIO_PIN                 GPIO_Pin_13
#define GTP_INT_READ()                   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)

//#define GTP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
//#define GTP_INT_EXTI_PINSOURCE           EXTI_PinSource12




//软件IIC使用的宏
#define I2C_SCL_1()  GPIO_SetBits(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)		/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		/* SDA = 0 */

#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)	/* 读SDA口线状态 */


static uint32_t I2C_TIMEOUT_UserCallback(void);


uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);

void I2C_Touch_Init(void);
void I2C_ResetChip(void);


#endif /* __I2C_TOUCH_H */
