#include "timer.h"
#include "led.h"
#include "usart.h"
#include "control.h"
#include "lcd.h"
#include "task.h"

/*---------------------------------------------
   用于计数帧的数据，所以定时器是用来设定频率的
----------------------------------------------*/

u8 ov_frame; 	//统计帧数
u32 times;
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
			if(taskFlag==1)
		{
			task1();
			times++;
		}
		
		
		if(taskFlag==2)
		{
			task2();
			times++;
		}
		else if(taskFlag==3)
		{
			task3();
			times++;
		}
		else if(taskFlag == 4)
		{
			task4();
			times++;
		}else if(taskFlag == 5)
		{
			task5();
			times++;
		}
		else if(taskFlag == 6)
		{
			task6();
			times++;
		}
		else if(taskFlag == 7)
		{
			task7();
			times++;
		}
		
		
		
		
		
		LCD_ShowNum(102,60,15,2,24);
		//printf("frame:%dfps\r\n",ov_frame);	//打印帧率
		ov_frame=0;   ///清除帧数
		LED0 = !LED0;
	}				   
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 	    
}





//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断
 
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 									 
}

//---------------------------------------------------------------------------------------------
/*
    脉冲输出：引脚PA6  {TIM_CH1}   X轴是CH1
              引脚PA7  {TIM_CH2}   Y轴是CH2
*/
//---------------------------------------------------------------------------------------------
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 1520; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM1
}



//封装区域
void Set_PWM_X(u16 PWM)
{
    TIM_SetCompare2(TIM3,PWM);	  
}
void Set_PWM_Y(u16 PWM)
{
     TIM_SetCompare1(TIM3,PWM);    
}
