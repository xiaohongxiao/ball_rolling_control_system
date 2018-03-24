#include "control.h"
#include "lcd.h"
#include "usart.h"	 
#include "ov7670.h"
#include "global.h"
#include "timer.h"
#include "scope.h"

//重要文件

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	
extern _SetValue_s_ SV;
extern _PID_PwmOutput_s_ PwmOutPut;
//float e_I_Y,e_I_X;					            //计算X轴和Y轴累计偏差
u16 Motor_X_PWM,Motor_Y_PWM;                    //最终PWM输出值
aparPID X={0}, Y={0};

//--------------------------------------------------------------------------------
/*
    下面数据变量用于处理坐标定位
*/

u16 X_MAX = 0, X_MIN = Wide, X_NOW = 0,X_MIN_LSAT = 0,X_MAX_LSAT = 0;   //X轴上面的数据范围为:0~Wide 
u16 Y_MAX = 0, Y_MIN = Height, Y_NOW = 0, Y_MIN_LSAT = 0,Y_MAX_LSAT = 0;   //Y轴上面的数据范围为：0~Height

//--------------------------------------------------------------------------------

//更新LCD显示
void camera_refresh(void)
{
	u16 i, j;
	u16 color;
	u16 gray;
	if(ov_sta)//有帧中断更新
	{
//		LCD_Scan_Dir(D2U_R2L);		//从上到下,从左到右
//        if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-Height)/2,(lcddev.height-Wide)/2,Height,Wide);//将显示区域设置到屏幕中央
//	    else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-Wide)/2,(lcddev.height-Height)/2,Wide,Height);//将显示区域设置到屏幕中央	 	
	//LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		//////寻找小球算法，采用
		for(i = 0;i < Height;i++)         //Y轴上的  
		//遍历像素点坐标
		{
			for(j=0;j<Wide;j++)   //x轴上的坐标点
			{
				OV7670_RCK_L;
				color = GPIOC -> IDR & 0XFF;	//读数据
				OV7670_RCK_H; 
				color <<= 8;  
				OV7670_RCK_L;
				color |= GPIOC -> IDR & 0XFF;	//读数据
				OV7670_RCK_H; 
				//彩色图像转成灰度图像：整数移位算法
				//算法理论公式：对于彩色转灰度，有一个很著名的心理学公式：
					//Gray = R*0.299 + G*0.587 + B*0.114
				gray = ( (color >> 11) * 19595 + ( ( color >> 5 ) & 0x3f ) * 38469 + ( color & 0x1f ) * 7472 ) >> 16; 
				//用于计算像素点灰度
				
				if( gray >= 23 )  //限定摄像头图形灰度阈值，，，说明是白球
					{
					if((i > 5 ) && (i < ( Height - 5 ) ) && ( j < Wide - 5 ) && (j > 5)) 
						{   //用于确定边框，避免小球冲出去  换句话来说就是给边缘划线
						  if(j > X_MAX) X_MAX = j;//x方向坐标最大点
							if(j < X_MIN) X_MIN = j;  //x方向坐标最小点
						
							if(i > Y_MAX) Y_MAX = i;//y轴上坐标最大点 
							if(i < Y_MIN) Y_MIN = i; //y轴上坐标最小点
					 }
								color = WHITE;   //灰度值
				 }
				else{
							color = BLACK;
						}
								LCD->LCD_RAM = color;   //将颜色信息供给LCD屏
			}
		}
		if((X_MAX != 0) && (X_MIN != Wide) && (Y_MAX != 0) && (Y_MIN != Height )) 
			//说明小球还在板子上面
		{
			X_NOW = (X_MAX + X_MIN) / 2;  
			//更新pid的real坐标信息 清除掉本次坐标用于再次遍历最大值 最小值
			Y_NOW = (Y_MAX + Y_MIN) / 2;
		}
		else
		{
			X_NOW = 322;   //更新pid的real坐标信息 清除掉本次坐标用于再次遍历最大值 最小值
		  Y_NOW = 242;   //给超出的值即可
		}
		
		X_MAX = 0;
		X_MIN = Wide;
		Y_MAX = 0;
		Y_MIN = Height;


//		LCD_ShowNum(102,150,X_NOW,3,24);  在显示屏上显示坐标
//		LCD_ShowNum(102,180,Y_NOW,3,24);		
		
  	ov_sta=0;					//清零帧中断标记
		ov_frame++;      //帧的计数，单位时间内，统计帧的数据，用于测试帧更新与否 
	//	LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
		
	}
}

//积分分离PID算法 且PID是不一样的
void PID (void)
{
//	static float kp1=0.5,ki1=0.155,kd1=0.2;				//X轴PID系数
//	static float kp2=0.5,ki2=0.155,kd2=0.2;				//Y轴PID系数
//	static float e_X[2],e_Y[2];						//前后两次偏差


	static u8 flag_Y = 0, flag_X = 0;					//分离积分标志位
	if((X_NOW > Wide) || ((Y_NOW > Height ) ))  
		//如果小球的坐标不属于像素范围内时,退出程序，不进行PID运算
	{
		PwmOutPut.pwm_Y = 0;  //差值PWM附0
		PwmOutPut.pwm_X = 0;  //不进行控制
	  return ;
	}
	//计算X轴和Y轴偏差
	X.err[0] = SV.X - X_NOW;    //XY位置目标和实际差值  setpoint value -  controlled value 
	Y.err[0] = SV.Y - Y_NOW;
	
	//printf("e_X:%f\r\n\r\n",e_X[0]);					//打印当前偏差
	
	//===========================Y轴PID运算========================================
	//积分分离, 以便在偏差较大时可以快速的减缩偏差，在偏差较小的时候，才加入积分，消除误差
	if(Y.err[0] >= 50.0 || Y.err[0] <=- 50.0)  //偏差像素点之间50个点左右
  {
		flag_Y = 0;    //不进行积分运算标志位
	}
	else
	{
		flag_Y = 1;   //进行积分运算标志位
		Y.e_I += Y.err[0]; //加入积分     error integral
	}
	
	//积分限幅  处理积分饱和现象
	if(Y.e_I > 600)
		Y.e_I = 600;
	if(Y.e_I < -600)
		Y.e_I = -600;
	
	//确切的微分没有
	
	
	
	//位置式PID
	PwmOutPut.pwm_Y = Y.kp * Y.err[0] + flag_Y * Y.ki * Y.e_I + Y.kd * ( Y.err[0] - Y.err[1] );
	//control value = p * error(0) + 积分标志位 * i * 积分误差 + d * （现在偏差-过去偏差）
	
	
	
	
	//===========================X轴PID运算============================================
	//积分分离,以便在偏差较大时可以快速的减缩偏差，在
	//偏差较小的时候，才加入积分，消除误差
	if(X.err[0]>=50.0||X.err[0]<=-50.0)
	//偏差进行判断操作
	{
		flag_X = 0;
	}else{
		flag_X = 1;
		X.e_I += X.err[0];
	}
	
	//积分限幅
	if(X.e_I > 600)
		X.e_I = 600;
	if(X.e_I < -600)
		X.e_I = -600;
	
	//位置式PID
	PwmOutPut.pwm_X = X.kp*X.err[0]+flag_X*X.ki*X.e_I+X.kd*(X.err[0]-X.err[1]);
	
	//=======================================================================================
	//记录本次偏差
	Y.err[1] = Y.err[0];								//用本次偏差值代替上次偏差
	X.err[1] = X.err[0];								//用本次偏差值代替上次偏差
	
	//OCS_displayData(X_NOW,X.err[0],Y.err[0],X.e_I);
//	X_NOW = PwmOutPut.pwm_X;
//	Y_NOW = PwmOutPut.pwm_Y;
}
void Set_PWM(void)
{
  static s16 PWM_Max = 1550;   //执行器在该系统可操作最大行程最大值
	static s16 PWM_Min = 850;
	static s16 PWM_Max_Y = 2000;
	static s16 PWM_Min_Y = 800;
	
	Motor_X_PWM = SV.X_PWM + PwmOutPut.pwm_X; //位置式PID  输出电机pwm
	Motor_Y_PWM = SV.Y_PWM + PwmOutPut.pwm_Y;
	if(Motor_X_PWM > PWM_Max) //控制信号输出限制幅值，保护执行器
	{
	    Motor_X_PWM = PWM_Max ;
	}
	else if(Motor_X_PWM < PWM_Min)
	{
		Motor_X_PWM = PWM_Min;
	}	
	if(Motor_Y_PWM > PWM_Max_Y)
	{
	    Motor_Y_PWM = PWM_Max_Y;
	}
	else if(Motor_Y_PWM < PWM_Min_Y)
	{
		Motor_Y_PWM = PWM_Min_Y;
	}
	
	Set_PWM_X(Motor_X_PWM);  //离我附近的是这个坐标轴
	Set_PWM_Y(Motor_Y_PWM);
	//printf("xpwm:%d\r\nypwm:%d\r\n",Motor_X_PWM,Motor_Y_PWM);
}



//调试函数
void tiaoshi(float X_kp, float X_ki,float X_kd, float Y_kp, float Y_ki,float Y_kd,float X_e_I,float Y_e_I)
{			
  		X.kp = X_kp;
	    Y.kp = Y_kp;
			X.ki = X_ki;
			Y.ki = Y_ki;
			X.kd = X_kd;
			Y.kd = Y_kd;
			X.e_I = X_e_I;
			Y.e_I = Y_e_I;
}



