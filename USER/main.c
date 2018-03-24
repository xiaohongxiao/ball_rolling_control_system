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
	u8 lightmode = 0,saturation = 2,contrast = 0,effect = 0; //������������ͷģʽ��   //ԭ�����̶Աȶ�Ϊcontrast=2          	 
 	u8 key = 0;
	
	delay_init();	    	 //��ʱ������ʼ��
	LED_Init();
	KEY_Init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200		
	
#if 1
	LCD_Init();			   		//��ʼ��LCD
	TIM3_PWM_Init(3002,71);   //PWM��ʼ��
	TIM6_Int_Init(499,7199);	//10Khz����Ƶ��,1�����ж�  //�����жϴ�ӡ֡��							  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	
//----------------------------------------------------------------
	LCD_Clear(BLACK);   //��ɫ��Ļ
//	LCD_ShowString(30,120,200,16,24,"Time :   s");
	LCD_ShowString(30,150,200,16,24,"  X  :");
	LCD_ShowString(30,180,200,16,24,"  Y  :");
	while(OV7670_Init())//��ʼ��OV7670
	{
	}

	delay_ms(1500);
  //---------------------------------------------------------------------------------
	/*
	  ����
			 �ƹ�ģʽ Light Mode  
			 ���Ͷ�  Saturation
			 �Աȶ� Contration
	  ��ͼ�����Ӱ��(�ر��ǹ��ߵĸ��� ÿ��ģʽ��ֻ������ѡ��0~3)
	*/
	OV7670_Light_Mode(lightmode);
	//�ƹ�ģʽ   //�Զ�ģʽ�Ƚϱ���
	OV7670_Color_Saturation(saturation); 
	//����ģʽ   //���Ͷ�û������ʲôģʽ�Ϻõ�
	OV7670_Contrast(contrast);           
	//�Աȶ�     //���ֶԱȶ�Ϊ0ʱЧ���Ϻ�
 	OV7670_Special_Effects(effect);	   
  //��Ч���� ���磺ģʽ���š�ƫ����  //û���в���
//---------------------------------------------------------------------------------	
	OV7670_Window_Set(12,176,Height,Wide);	
	//���ô���	 ��С 
  OV7670_CS=0;
	
	LCD_ShowNum(102,90,1,1,24);
#endif
	
	taskFlag = 1;    //�����־λ  ���ó�1 ��Ĭ������1
	LCD_ShowNum(200,150,1,3,24);
	
	
	SV.X_PWM = 1200;     //Ŀ��ˮƽ ��ʼĿ��
	SV.Y_PWM = 1400;   //�ٵ�Y  //��������
		
 	while(1)
	{
		key = KEY_Scan(0);
		
		if(key==KEY0_PRES && taskFlag == 1) //task2
		{
	
			X.kp = 2.9;   //6 10 0.2 0.2 20 40 
			Y.kp = 2.9;  
			X.ki = 0.25;   ///����ȥ6 0 18.8
			Y.ki = 0.085;
			X.kd = 18;
			Y.kd = 18;
			X.e_I = 0;
			Y.e_I = 0;
			
			SV.X = 180;  //�޸���Ŀ�� XY  118 179  244 136  ���ҽ��Ķ������
			SV.Y = 120;  //
			
			times = 0;
			taskFlag = 2;               ////
			LED0 = !LED0;
		}
		else if(key==KEY1_PRES  && taskFlag == 2) //task3  ֻ�ǽ�С�����4����
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
		else if(key==WKUP_PRES  && taskFlag == 3) //task4  ������һ����
		{
				X.kp = 3.1;  //2.5  0.021  4.8
			Y.kp = 3.1;
			X.ki = 0.085;
			Y.ki = 0.085;
			X.kd = 18.8;
			Y.kd = 18.8;
			X.e_I = 0;
			Y.e_I = 0;
					
			SV.X = 180;  /// ���Ŀ���
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
			SV.Y = 111;  ///�ڶ�171 207  ���� 254 217			
				times = 0;
				taskFlag = 5;
					LED0 = !LED0;
			}
		else if(key==KEY1_PRES && taskFlag == 5)    //task6
		{
				if(key_flag.flag1 == 0)
			{
				taskFlag = 10;   //����ѡ��ABCD����
				key_flag.flag1 = 1;
			}
			
						///ѡȡ����A B C D����
			
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
			
			
			
			
			
			
			
			
			
			
			
			//////����ѡȡ����
			if(key_flag.flag1 == 1)
			{
				
				
				
				
	while(KEY0 == 0);  //anxia 			
	if(KEY0 == 0) //  KEY0 �ٰ���һ��
	{
		while(KEY0 == 0);
		
		while( WK_UP == 0)   //û���½���
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				
				key_flag.flag2 ++;   //��ʾѡ��������
				LED1=!LED1;				
			}	
					LED1 = !LED1;
					delay_ms(500);
					LED1 = !LED1;  //˵��ѡ������
		}
		
		task5_area.area1 = key_flag.flag2;
		key_flag.flag2 = 0;
	

		////lcd��ʾb ===
		//delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 //area2	
		while( WK_UP == 0)   //û���½���
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //��ʾѡ��������
				LED1=!LED1;				
			}			
		}
		task5_area.area2 = key_flag.flag2;
		key_flag.flag2 = 0;  //��0
		
		
		////lcd��ʾc ===
	//	delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 
		//area3	
		while( WK_UP == 0)   //û���½���
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //��ʾѡ��������
				LED1=!LED1;				
			}			
		}
		task5_area.area3 = key_flag.flag2;
		key_flag.flag2 = 0;  //��0
		
		
		////lcd��ʾa ===
		//delay_ms(500);  //yansh
		while(WK_UP == 1);  
	 
		//area4	
		while( WK_UP == 0)   //û���½���
		{
			if(KEY1== 0)
			{
				while(KEY1 == 0);
				key_flag.flag2 ++;   //��ʾѡ��������
				LED1=!LED1;				
			}			
		}
		task5_area.area4 = key_flag.flag2;
		key_flag.flag2 = 0;  //��0
		
		
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
			taskFlag = 6;   ///����ִ������6
		key_flag.flag1 = 2;  //�ܳ�����				
}
			
			
			
			
						
			
				}
		}
}