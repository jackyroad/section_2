#ifndef BSP_PWM_TABLE_H
#define BSP_PWM_TABLE_H

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief PWM表生成配置参数
 * @param BSP_PWM_TABLE_TOTAL_POINTS  总点数（建议256-1024，需为偶数）
 * @param BSP_PWM_TABLE_MAX_VALUE     最大值（根据PWM分辨率选择，如10bit为1024）
 * @param BSP_PWM_TABLE_GAMMA         Gamma值（控制曲线形态，典型2.2-3.0）
 * @param BSP_PWM_TABLE_SMOOTH_POINTS 平滑区点数（建议总点数的5-10%）
 *
 * 核心原理：
 * 1. 使用Gamma校正模拟人眼对亮度的非线性感知
 * 2. 对称结构实现平滑的呼吸效果
 * 3. 线性过渡区消除低亮度时的闪烁
 */
#define BSP_PWM_TABLE_TOTAL_POINTS          256     // 总点数(内存占用=点数*2 bytes)
#define BSP_PWM_TABLE_MAX_VALUE             1024    // 匹配PWM寄存器位数(如10bit=1024)
#define BSP_PWM_TABLE_GAMMA                 2.2f    // 2.2-2.5适合LED，2.8-3.0适合电机
#define BSP_PWM_TABLE_SMOOTH_POINTS         13      // 建议值：max(15, 总点数*0.05)

// 外部可访问的PWM表
extern uint16_t bsp_pwmTable[BSP_PWM_TABLE_TOTAL_POINTS];

void BSP_PWMTable_Init(void);
void ExportPWMTable_Keil(void);

#endif /* BSP_PWM_TABLE_H */

/******************************************************************************
 * PWM波形表生成器 - 核心算法详解
 *
 * 一、Gamma校正核心算法（数学建模）
 *
 * 1. 标准Gamma曲线公式：
 *
 *        PWM[n] = Vmax * (n/N)^γ
 *
 *    其中：
 *        n ∈ [0,N] : 离散化时间点索引
 *        N        : 总半程点数 = TOTAL_POINTS/2
 *        Vmax     : PWM最大值 = BSP_PWM_TABLE_MAX_VALUE
 *        γ        : Gamma系数 = BSP_PWM_TABLE_GAMMA
 *
 * 2. 改进的分段算法：
 *
 *    (1) 低区线性过渡段（0 ≤ n < S）：
 *
 *                PWM[n] = Vmax * (S/N)^γ * (n/S)
 *                        └─基准值┘   └─线性插值┘
 *
 *    (2) Gamma主曲线段（S ≤ n ≤ N）：
 *
 *                PWM[n] = Vmax * [(n-S)/(N-S)]^γ
 *
 *    (3) 对称下降段（N < n ≤ 2N）：
 *
 *                PWM[n] = PWM[2N - n]
 *
 * 二、参数物理意义详解
 *
 * 1. 平滑过渡点S的计算：
 *
 *        S = min(用户设定值, N/2)
 *        典型取值：S = max(15, N*0.05)
 *
 * 2. Gamma系数γ的选择依据：
 *
 *    +----------------+------------------------+-----------------------------+
 *    | γ值范围        | 曲线特性               | 适用场景                    |
 *    +----------------+------------------------+-----------------------------+
 *    | 1.8 ≤ γ < 2.0 | 接近线性，低区变化快    | 电机控制/需要快速响应        |
 *    | 2.0 ≤ γ < 2.3 | 标准视觉补偿            | 通用LED调光                 |
 *    | 2.3 ≤ γ ≤ 3.0 | 低区极度平缓            | 高精度亮度调节              |
 *    +----------------+------------------------+-----------------------------+
 *
 * 3. 离散化误差控制：
 *
 *        相对误差 = |理论值 - 离散值| / Vmax
 *        典型误差控制在 < 0.5% 需要：
 *            TOTAL_POINTS ≥ 256 (当γ=2.3时)
 *
 * 三、算法实现流程图
 *
 *       开始
 *        │
 *        ├─ 计算半程点数 N = TOTAL_POINTS/2
 *        ├─ 计算平滑终点 S = min(SMOOTH_POINTS, N)
 *        │
 *        ├─ 生成上升阶段 ───┬─ 低区线性段：PWM[i]=Vmax*(S/N)^γ*(i/S)
 *        │                 └─ Gamma段：PWM[i]=Vmax*[(i-S)/(N-S)]^γ
 *        │
 *        └─ 生成下降阶段 ─── 镜像对称：PWM[N+i] = PWM[N-1-i]
 *
 * 四、关键公式推导
 *
 * 1. 基准值计算（平滑段终点）：
 *
 *        Pbase = Vmax * (S/N)^γ
 *
 * 2. 归一化时间参数：
 *
 *        t = (n - S)/(N - S)  (确保t ∈ [0,1])
 *
 * 3. 离散化修正：
 *
 *        实际实现时n需偏移0.5保证对称性：
 *        t = (n + 0.5 - S)/(N - S)
 *
 ******************************************************************************/
