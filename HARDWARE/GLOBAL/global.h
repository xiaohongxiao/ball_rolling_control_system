#ifndef __GLOBAL_H_
#define __GLOBAL_H_	 

#include "stm32f10x.h"
//
#define OK  1
#define No  0

//任务标志位
typedef struct {
	volatile uint16_t task1_flag:		1;
	volatile uint16_t task2_flag:	  1;
	volatile uint16_t task3_flag:		1;
	volatile uint16_t task4_flag:		1;
	volatile uint16_t task5_flag:		1;
	volatile uint16_t Other_Task:	  11;
}TaskMask_s_;

//给定位置和初始脉冲
typedef struct {
  vs16 X;               //设定X轴的位置
	vs16 Y;               //设定Y轴的位置
  vs16 X_PWM;           //给定X轴PWM
	vs16 Y_PWM;           //给定Y轴PWM
}_SetValue_s_;           

//PID运算后输出的PWM
typedef struct _PID_PwmOutput_s_{
	vs16 pwm_X;							//X轴的数据
	vs16 pwm_Y;							//Y轴的数据
}_PID_PwmOutput_s_;

extern _SetValue_s_ SV;

enum PWM{
	PWM_Min = 1020,
PWM_Max_X = 2470,
PWM_Min_X = 820,
PWM_Mid_X = 1610,
PWM_Max_Y = 2410,
 PWM_Min_Y = 500,
 PWM_Mid_Y = 1440,
};


#endif
