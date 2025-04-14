#include "bsp_pwm_table.h"
#include <math.h>  // 包含round()和powf()的声明
#include <stdio.h> // 文件操作需要

// PWM表定义
uint16_t bsp_pwmTable[BSP_PWM_TABLE_TOTAL_POINTS];

/* 内联函数定义选择小值函数 */
static inline int min_int(int a, int b)
{
    return (a < b) ? a : b;
}

/**
 * @brief 通用PWM表生成函数
 * @note 时间复杂度O(n)，空间复杂度O(1)
 */
void BSP_PWMTable_Init(void)
{
    const int half_points = BSP_PWM_TABLE_TOTAL_POINTS / 2;

    // 防止数组越界访问，线性过渡区覆盖有效数据，通过min_int可自动修正这类配置错误，增强鲁棒性
    const int smooth_end = min_int(BSP_PWM_TABLE_SMOOTH_POINTS, half_points);

    /* 生成上升阶段曲线（Gamma校正核心段） */
    for (int i = smooth_end; i < half_points; i++)
    {
        // 归一化位置[0,1]，越往后步长越小（非线性）
        float t = (float)(i - smooth_end) / (half_points - smooth_end - 1);

        // Gamma公式：output = max_value * input^gamma
        // 当gamma>1时产生亮度补偿曲线（人眼对低亮度更敏感）
        bsp_pwmTable[i] = (uint16_t)roundf(BSP_PWM_TABLE_MAX_VALUE * powf(t, BSP_PWM_TABLE_GAMMA));
    }

    /* 低区线性过渡（解决LED/Motor在低PWM时的非线性问题） */
    if (smooth_end > 0)
    {
        const uint16_t base = bsp_pwmTable[smooth_end];
        for (int i = 0; i < smooth_end; i++)
        {
            // 线性插值：从0平滑过渡到Gamma曲线起点
            bsp_pwmTable[i] = (uint16_t)roundf((float)base * i / smooth_end);
        }
    }

    /* 生成对称下降阶段（内存优化：直接镜像上升段） */
    for (int i = 0; i < half_points; i++)
    {
        bsp_pwmTable[half_points + i] = bsp_pwmTable[half_points - 1 - i];
    }
}

void ExportPWMTable_Keil(void)
{
    printf("======== PWM Table ========\n");
    for (int i = 0; i < BSP_PWM_TABLE_TOTAL_POINTS; i++)
    {
        // 输出数值（宽度4，右对齐）
        printf("%4d", bsp_pwmTable[i]);

        // 所有数值后都加逗号（包括最后一个）
        printf(",");

        // 每10个数值换行
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }
    printf("\n"); // 整个表格结束后换行
}

#ifdef __MAIN__C
int main()
{
    BSP_PWMTable_Init();

    ExportPWMTable_Keil();
}
#endif /*__MAIN__C */
