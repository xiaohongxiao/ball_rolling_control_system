#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern u32 times;

void TIM6_Int_Init(u16 arr,u16 psc);   //定时器用于打印帧率
void TIM3_PWM_Init(u16 arr,u16 psc);   //PWM输出 两路输出
void Set_PWM_X(u16 PWM);               //X轴脉冲输出
void Set_PWM_Y(u16 PWM);               //Y轴脉冲输出

#endif




