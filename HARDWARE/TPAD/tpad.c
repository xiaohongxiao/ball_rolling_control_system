#include "tpad.h"
#include "delay.h"		    
#include "usart.h"

#define TPAD_ARR_MAX_VAL 	0XFFFF	//����ARRֵ
vu16 tpad_default_val=0;//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��


//��ʼ����������
//��ÿ��ص�ʱ����������ȡֵ.
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
u8 TPAD_Init(u8 psc)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);//��1Mhz��Ƶ�ʼ��� 
	for(i=0;i<10;i++)//������ȡ10��
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//����
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//ȡ�м��8�����ݽ���ƽ��
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;//��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!
	return 0;		     	    					   
}
//��λһ��
void TPAD_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
	//����GPIOA.1Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 //PA.1���0,�ŵ�

	delay_ms(5);

	TIM_SetCounter(TIM5,0);		//��0
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    
}
//�õ���ʱ������ֵ
//�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
u16 TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM5, TIM_IT_CC2) == RESET)//�ȴ�����������
	{
		if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500)return TIM_GetCounter(TIM5);//��ʱ��,ֱ�ӷ���CNT��ֵ
	};	
	return TIM_GetCapture2(TIM5);	  
} 	 
//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//�õ�һ��ֵ
		if(temp>res)res=temp;
	};
	return res;
}  
//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;										  
#define TPAD_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;		//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;		//֧������	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//����tpad_default_val+TPAD_GATE_VAL,��Ч
	{							 
		if(keyen==0)res=1;		//keyen==0,��Ч 
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	
//��ʱ��2ͨ��2���벶������
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	 //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 //����Ϊ��������

   	//��ʼ��TIM5  
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	//��ʼ��ͨ��2 
  	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x03;//IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);//��ʼ��I5 IC2

    TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
}













