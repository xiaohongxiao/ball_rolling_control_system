#include "task.h"
#include "usart.h"
#include "GAI.h"
u8 taskFlag = 0;
u8 taskEnd;
FLAG tflag={0,0,0,0,0,0};










void task1(void)
{
		X.kp = 2;   //6 10 0.2 0.2 20 40 
		Y.kp = 2;  
		X.ki = 0.25;   ///调整去6 0 18.8
			Y.ki = 0.25;
			X.kd = 4;
			Y.kd = 4;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 96;  //修改区目标 XY  118 179  244 136  离我近的舵机控制
			SV.Y = 109;  //
	camera_refresh();
	PID();
	Set_PWM();
	if(X_NOW > SV.X-8 && X_NOW < SV.X + 8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
	{
		taskEnd++;
		if(taskEnd==40)
		{
			//taskFlag = 0;
			Set_PWM_X(SV.X_PWM);    //设置水平
			Set_PWM_Y(SV.Y_PWM);
		}
	}
}

void task2(void)
{
	camera_refresh();
	PID();
	Set_PWM();
	if(X_NOW > SV.X-8 && X_NOW < SV.X + 8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
	{
		taskEnd++;
		if(taskEnd==40)
		{
			taskFlag = 0;
			Set_PWM_X(SV.X_PWM);    //设置水平
			Set_PWM_Y(SV.Y_PWM);
		}
	}
}

void task3(void)
{
	if(tflag.flag9 == 0)
	{
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-20 && X_NOW<SV.X+20 && Y_NOW>SV.Y-20 && Y_NOW<SV.Y+20)
				{
						taskEnd++;
						if(taskEnd==40)
						{
				//			taskFlag = 0;
							Set_PWM_X(SV.X_PWM);  //调整水平
							Set_PWM_Y(SV.Y_PWM);
							tflag.flag9 = 1;
							
						}
				}
	}else if(tflag.flag9 == 1)
	{
		X.kp = 3.5;			Y.kp = 3.5;			X.ki = 0.085;
		Y.ki = 0.085;		X.kd = 18.8;		Y.kd = 18.8;
		X.e_I = 0;	    Y.e_I = 0;			
		SV.X = 180; 			SV.Y = 120; //109
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
						taskEnd++;
						if(taskEnd==40)
						{
					//		taskFlag = 0;
							Set_PWM_X(SV.X_PWM);  ///调整水平
							Set_PWM_Y(SV.Y_PWM);
						}
				}
	}
	
}

///168 77

//任务执行4
void task4(void)
{
		if(tflag.flag4 == 0)
	{
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-80 && X_NOW<SV.X+80 && Y_NOW>SV.Y-80 && Y_NOW<SV.Y + 80)
				{
						taskEnd++;
						if(taskEnd==40)
						{
						//	taskFlag = 0;
							Set_PWM_X(SV.X_PWM);  //调整水平
							Set_PWM_Y(SV.Y_PWM);
							tflag.flag4 = 1;
							
						}
				}
	}else if(tflag.flag4 == 1)
	{
		X.kp = 3.5;			Y.kp = 3.5;			X.ki = 0.085;
		Y.ki = 0.085;		X.kd = 18.8;		Y.kd = 18.8;
		X.e_I = 0;	    Y.e_I = 0;			SV.X = 255; 			SV.Y = 219; //109
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-10 && X_NOW<SV.X+10 && Y_NOW>SV.Y-10 && Y_NOW<SV.Y+10)
				{
						taskEnd++;
						if(taskEnd==40)
						{
							
							Set_PWM_X(SV.X_PWM);  ///调整水平
							Set_PWM_Y(SV.Y_PWM);
						}
				}
	}
	
	
}


//任务执行
void task5(void)
{
		if(tflag.flag5 == 0)
		{
				
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-10 && X_NOW<SV.X+10 && Y_NOW>SV.Y-10 && Y_NOW<SV.Y+10)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						//taskFlag = 0;
						Set_PWM_X(SV.X_PWM);
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag5 = 1;
					}
				}
			}else if(tflag.flag5 ==  1)
		{
			X.kp = 3.1;  	Y.kp = 3.1;	X.ki = 0.085;	Y.ki = 0.085;
			X.kd = 18.8;  Y.kd = 18.8;X.e_I = 0;		Y.e_I = 0;	
			SV.X = 171;		SV.Y = 207;
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-20 && X_NOW<SV.X+20 && Y_NOW>SV.Y-20 && Y_NOW<SV.Y+20)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag5 = 2;
						
					}
				}
		}else if(tflag.flag5 ==  2)
		{
			X.kp = 3.1;  	Y.kp = 3.1;	X.ki = 0.085;	Y.ki = 0.085;
			X.kd = 18.8;  Y.kd = 18.8;X.e_I = 0;		Y.e_I = 0;	
			SV.X = 254;		SV.Y = 217;
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);						
					}
				}
		}
}







void task6(void)
{
	if(tflag.flag6 == 0)
		{
			X.kp = 3;  	Y.kp = 3;	X.ki = 0.085;	Y.ki = 0.085;
			X.kd = 18.8;  Y.kd = 18.8;X.e_I = 0;		Y.e_I = 0;	
			select_choice(1);
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag6 = 1;						
					}
				}
		}else if(tflag.flag6 ==  1)
		{
			select_choice(2);
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag6 = 2;						
					}
				}
		}
		else if(tflag.flag6 ==  2)
		{
				select_choice(3);
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag6 = 3;						
					}
				}
		}
		else if(tflag.flag6 == 3)
		{
				select_choice(4);
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);				
					}
				}
		}
		
		
		
		
		
		
		
}


void task7(void)
{
		if(tflag.flag7 == 0)
		{
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-40 && X_NOW<SV.X+40 && Y_NOW>SV.Y-40 && Y_NOW<SV.Y + 40)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						//taskFlag = 0;
						Set_PWM_X(SV.X_PWM);
						Set_PWM_Y(SV.Y_PWM);
						tflag.flag7 = 1;
					}
				}
			}else if(tflag.flag7 ==  1)
		{
			X.kp = 3.1;  	Y.kp = 3.1;	X.ki = 0.085;	Y.ki = 0.085;
			X.kd = 18.8;  Y.kd = 18.8;X.e_I = 0;		Y.e_I = 0;	
			SV.X = 256;		SV.Y = 217;
				camera_refresh();
				PID();
				Set_PWM();
				if(X_NOW>SV.X-8 && X_NOW<SV.X+8 && Y_NOW>SV.Y-8 && Y_NOW<SV.Y+8)
				{
					taskEnd++;
					if(taskEnd==40)
					{
						Set_PWM_X(SV.X_PWM);   //调水平
						Set_PWM_Y(SV.Y_PWM);						
					}
				}
		}
}
