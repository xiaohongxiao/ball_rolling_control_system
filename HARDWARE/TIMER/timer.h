#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern u32 times;

void TIM6_Int_Init(u16 arr,u16 psc);   //��ʱ�����ڴ�ӡ֡��
void TIM3_PWM_Init(u16 arr,u16 psc);   //PWM��� ��·���
void Set_PWM_X(u16 PWM);               //X���������
void Set_PWM_Y(u16 PWM);               //Y���������

#endif




