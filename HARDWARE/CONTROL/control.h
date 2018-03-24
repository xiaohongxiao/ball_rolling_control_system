#ifndef __CONTROL_H_
#define __CONTROL_H_	 

#include "stm32f10x.h"
#include "task.h"

#define Wide       320  //x轴宽度  用于定义图像窗口大小的   修改时可以直接修改该值就可以实现功能
#define Height     240//Y轴宽度  用于定义图像窗口大小的   修改时可以直接修改该值就可以实现功能

typedef struct
{
	float kp, ki, kd;   //比例，积分，微分
	float err[2]; //当前偏差、上一次偏差
	float e_I;    ///偏差积分结果
}aparPID;

extern u16 X_NOW;   //X轴上面的数据范围为:0~Wide，小球现在的坐标X 
extern u16 Y_NOW;   //Y轴上面的数据范围为：0~Height
extern aparPID X, Y;

void pidChange(u16 p,u16 i,u16 d);             //pid变化值
void camera_refresh(void);               //摄像头刷新
void PD (void);                    //使用PD函数
void PID (void);          //该PID XY轴PID参数不一样
void Set_PWM(void);             //设置舵机PWM


void tiaoshi(float X_kp, float X_ki,float X_kd, float Y_kp, float Y_ki,float Y_kd,float X_e_I,float Y_e_I);



#endif
