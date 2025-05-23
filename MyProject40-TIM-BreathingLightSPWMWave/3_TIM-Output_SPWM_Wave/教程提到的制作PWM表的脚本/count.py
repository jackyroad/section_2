#! python3
#coding=utf-8

"""
Python版本：3.x

计算不同配置下呼吸灯的周期

运行方式：
在命令行中输入：python count.py 

运行结果：
打印出当前配置中一个呼吸周期的时间
"""


#PWM点数
POINT_NUM = 180

#周期倍数
PERIOD_CLASS = 1

#幅值等级
AMPLITUDE_CLASS = 256

#定时器定时周期
TIMER_TIM_Period = 2**9
#定时器分频
TIMER_TIM_Prescaler = 12

#STM32系统时钟频率和周期
f_hclk = 180000000

#定时器计时频率
f_pclk1 = f_hclk/2
t_pclk1 = 1/f_pclk1

#定时器update事件周期
t_timer = t_pclk1*TIMER_TIM_Prescaler*TIMER_TIM_Period

#每个PWM点的时间
T_Point = t_timer * PERIOD_CLASS * AMPLITUDE_CLASS

#整个呼吸周期
T_Up_Down_Cycle = T_Point * POINT_NUM

print ("呼吸周期：",T_Up_Down_Cycle)



