#define PERIPH_BASE           ((uint32_t)0x40000000)                    /外设寄存器总基地址
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)                /AHB1总线外设基地址
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)                /RCC外设寄存器基地址

/*
 *RCC是一个宏，用于将 RCC 外设的基地址转换为 RCC_TypeDef 结构体指针
 *通过该宏，可以方便地访问 RCC 外设的寄存器，简化代码并提高可读性
 *直接通过 RCC->CR、RCC->AHB1ENR 等方式访问 RCC 寄存器，无需手动计算寄存器地址
 */
#define RCC                 ((RCC_TypeDef *) RCC_BASE)


#define IS_RCC_AHB1_CLOCK_PERIPH(PERIPH) ((((PERIPH) & 0x810BE800) == 0x00) && ((PERIPH) != 0x00))
