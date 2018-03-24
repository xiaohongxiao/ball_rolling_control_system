#include "delay.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "control.h"
#include "task.h"
#include "global.h"
//#include "scope.h"
#include "GAI.h"

extern u8 taskFlag;

int main(void)
{
	u8 lightmode = 0,saturation = 2,contrast = 0,effect = 0; //用于设置摄像头模式的   //原子历程对比度为contrast=2          	 
 	u8 key = 0;
	
	delay_init();	    	 //延时函数初始化
	LED_Init();
	KEY_Init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200		
	
#if 1
	LCD_Init();			   		//初始化LCD
	TIM3_PWM_Init(3002,71);   //PWM初始化
	TIM6_Int_Init(499,7199);	//10Khz计数频率,1秒钟中断  //用于中断打印帧数							  
	EXTI8_Init();						//使能定时器捕获
	
//----------------------------------------------------------------
	LCD_Clear(BLACK);   //黑色屏幕
//	LCD_ShowString(30,120,200,16,24,"Time :   s");
	LCD_ShowString(30,150,200,16,24,"  X  :");
	LCD_ShowString(30,180,200,16,24,"  Y  :");
	while(OV7670_Init())//初始化OV7670
	{
	}

	delay_ms(1500);
  //---------------------------------------------------------------------------------
	/*
	  测试
			 灯光模式 Light Mode  
			 饱和度  Saturation
			 对比度 Contration
	  对图像处理的影响(特别是光线的干扰 每种模式都只有四种选择，0~3)
	*/
	OV7670_Light_Mode(lightmode);
	//灯光模式   //自动模式比较保险
	OV7670_Color_Saturation(saturation); 
	//饱和模式   //饱和度没发现有什么模式较好的
	OV7670_Contrast(contrast);           
	//对比度     //发现对比度为0时效果较好
 	OV7670_Special_Effects(effect);	   
  //特效设置 例如：模式复古、偏蓝等  //没进行测试
//---------------------------------------------------------------------------------	
	OV7670_Window_Set(12,176,Height,Wide);	
	//设置窗口	 大小 
  OV7670_CS=0;
	
	LCD_ShowNum(102,90,1,1,24);
#endif
	
	taskFlag = 1;    //任务标志位  设置成1 ，默认任务1
	LCD_ShowNum(200,150,1,3,24);
	
	
	SV.X_PWM = 1200;     //目标水平 初始目标
	SV.Y_PWM = 1400;   //再调Y  //调整区域
		
 	while(1)
	{
		key = KEY_Scan(0);
		
		if(key==KEY0_PRES && taskFlag == 1) //task2
		{
	
			X.kp = 2.9;   //6 10 0.2 0.2 20 40 
			Y.kp = 2.9;  
			X.ki = 0.25;   ///调整去6 0 18.8
			Y.ki = 0.085;
			X.kd = 18;
			Y.kd = 18;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 180;  //修改区目标 XY  118 179  244 136  离我近的舵机控制
			SV.Y = 120;  //
			
			times = 0;
			taskFlag = 2;               ////
			LED0 = !LED0;
		}
		else if(key==KEY1_PRES  && taskFlag == 2) //task3  只是将小球放入4区域；
		{
				X.kp = 4.1;
			Y.kp = 4.1;
			X.ki = 0.085;
			Y.ki = 0.085;
			X.kd = 18.8;
			Y.kd = 18.8;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 189; //95
			SV.Y = 36; //109 
			
		//	times = 0;
			taskFlag = 3;
			LED0 = !LED0;
		}
		else if(key==WKUP_PRES  && taskFlag == 3) //task4  任务表第一个点
		{
				X.kp = 3.1;  //2.5  0.021  4.8
			Y.kp = 3.1;
			X.ki = 0.085;
			Y.ki = 0.085;
			X.kd = 18.8;
			Y.kd = 18.8;
			X.e_I = 0;
			Y.e_I = 0;
					
			SV.X = 180;  /// 间接目标点
			SV.Y = 120;
			
			
			times = 0;
			taskFlag = 4;
			LED0 = !LED0;
			} 
		else if(key==KEY0_PRES && taskFlag == 4)   //task5  1-2-6-9
		{
				X.kp = 3.1;  //2.5  0.021  4.8
			Y.kp = 3.1;
			X.ki = 0.085;
			Y.ki = 0.085;
			X.kd = 18.8;
			Y.kd = 18.8;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 95;  
			SV.Y = 111;  ///第二171 207  第三 254 217			
				times = 0;
				taskFlag = 5;
					LED0 = !LED0;
			}
		else if(key==KEY1_PRES && taskFlag == 5)    //task6
		{
				if(key_flag.flag1 == 0)
			{
				taskFlag = 10;   //按键选择ABCD区域
				key_flag.flag1 = 1;
			}
			
						///选取按键A B C D区域
			
			}
		else if(key==WK_UP && taskFlag == 6)
		{
				X.kp = 2.5;  //2.5  0.021  4.8
			Y.kp = 2.5;
			X.ki = 0.25;
			Y.ki = 0.085;
			X.kd = 18;
			Y.kd = 10;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 180;
			SV.Y = 120;
			
			times = 0;
			taskFlag = 7;
			LED0 = !LED0;
			}
			
			
			
			
			
			
			
			
			
			
			
			//////按键选取区域
			if(key_flag.flag1 == 1)
			{
				
				
				
				
	while(KEY0 == 0);  //anxia 			
	if(KEY0 == 0) //  KEY0 再按下一次
	{
		while(KEY0 == 0);
		
		while( WK_UP == 0)   //没按下进入
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				
				key_flag.flag2 ++;   //表示选几号区域，
				LED1=!LED1;				
			}	
					LED1 = !LED1;
					delay_ms(500);
					LED1 = !LED1;  //说明选完区域
		}
		
		task5_area.area1 = key_flag.flag2;
		key_flag.flag2 = 0;
	

		////lcd显示b ===
		//delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 //area2	
		while( WK_UP == 0)   //没按下进入
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //表示选几号区域，
				LED1=!LED1;				
			}			
		}
		task5_area.area2 = key_flag.flag2;
		key_flag.flag2 = 0;  //清0
		
		
		////lcd显示c ===
	//	delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 
		//area3	
		while( WK_UP == 0)   //没按下进入
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //表示选几号区域，
				LED1=!LED1;				
			}			
		}
		task5_area.area3 = key_flag.flag2;
		key_flag.flag2 = 0;  //清0
		
		
		////lcd显示a ===
		//delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 
		//area4	
		while( WK_UP == 0)   //没按下进入
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //表示选几号区域，
				LED1=!LED1;				
			}			
		}
		task5_area.area4 = key_flag.flag2;
		key_flag.flag2 = 0;  //清0
		
		
		X.kp = 3.1;  //2.5  0.021  4.8
			Y.kp = 3.1;
			X.ki = 0.085;
			Y.ki = 0.085;
			X.kd = 18.8;
			Y.kd = 18.8;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 254;
			SV.Y = 217;
			
			times = 0;
			taskFlag = 6;   ///返回执行任务6
		key_flag.flag1 = 2;  //跑出按键				
}
			
			
			
			
						
			
				}
		}
}