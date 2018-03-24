#ifndef __CONTROL_H_
#define __CONTROL_H_	 

#include "stm32f10x.h"
#include "task.h"

#define Wide       320  //x����  ���ڶ���ͼ�񴰿ڴ�С��   �޸�ʱ����ֱ���޸ĸ�ֵ�Ϳ���ʵ�ֹ���
#define Height     240//Y����  ���ڶ���ͼ�񴰿ڴ�С��   �޸�ʱ����ֱ���޸ĸ�ֵ�Ϳ���ʵ�ֹ���

typedef struct
{
	float kp, ki, kd;   //���������֣�΢��
	float err[2]; //��ǰƫ���һ��ƫ��
	float e_I;    ///ƫ����ֽ��
}aparPID;

extern u16 X_NOW;   //X����������ݷ�ΧΪ:0~Wide��С�����ڵ�����X 
extern u16 Y_NOW;   //Y����������ݷ�ΧΪ��0~Height
extern aparPID X, Y;

void pidChange(u16 p,u16 i,u16 d);             //pid�仯ֵ
void camera_refresh(void);               //����ͷˢ��
void PD (void);                    //ʹ��PD����
void PID (void);          //��PID XY��PID������һ��
void Set_PWM(void);             //���ö��PWM


void tiaoshi(float X_kp, float X_ki,float X_kd, float Y_kp, float Y_ki,float Y_kd,float X_e_I,float Y_e_I);



#endif
