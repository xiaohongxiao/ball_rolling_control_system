#include "sys.h"
#include "ov7670.h"
#include "ov7670cfg.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"	
 	    
//��ʼ��OV7670
//����0:�ɹ�
//��������ֵ:�������
u8 OV7670_Init(void)
{
	u8 temp;
	u16 i=0;	  
	//����IO
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	                       RCC_APB2Periph_GPIOB |
	                       RCC_APB2Periph_GPIOC |
	                       RCC_APB2Periph_GPIOD |
	                       RCC_APB2Periph_GPIOG |
	                       RCC_APB2Periph_AFIO, 
	                       ENABLE);	 //ʹ����ض˿�ʱ��
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; 	//PA8 ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4);	

	
	GPIO_InitStructure.GPIO_Pin  = 0xff; //PC0~7 ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_SetBits(GPIOD,GPIO_Pin_6);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_14|GPIO_Pin_15);
	
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
		//������ӳ��
 	SCCB_Init();        		//��ʼ��SCCB ��IO��	
	
 	if(SCCB_WR_Reg(0x12,0x80))return 1;	//��λSCCB	  
	delay_ms(50);  
	//��ȡ��Ʒ�ͺ�
 	temp=SCCB_RD_Reg(0x0b);   
	if(temp!=0x73)return 2;  
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76)return 2;
	//��ʼ������	  
	for(i = 0; i < sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]);i++)
	{
	   	SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
  	}
   	return 0x00; 	//ok
} 
////////////////////////////////////////////////////////////////////////////
//OV7670��������
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//Ĭ�Ͼ�������Ϊ�Զ���ƽ��
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	SCCB_WR_Reg(0X13,reg13val);//COM8���� 
	SCCB_WR_Reg(0X01,reg01val);//AWB��ɫͨ������ 
	SCCB_WR_Reg(0X02,reg02val);//AWB��ɫͨ������ 
}				  
//ɫ������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//Ĭ�Ͼ���sat=2,��������ɫ�ȵ�����
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//ɫ�ʾ���ϵ��1
	SCCB_WR_Reg(0X50,reg4f5054val);	//ɫ�ʾ���ϵ��2 
	SCCB_WR_Reg(0X51,0X00);			//ɫ�ʾ���ϵ��3  
	SCCB_WR_Reg(0X52,reg52val);		//ɫ�ʾ���ϵ��4 
	SCCB_WR_Reg(0X53,reg53val);		//ɫ�ʾ���ϵ��5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//ɫ�ʾ���ϵ��6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//��������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//Ĭ�Ͼ���bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	SCCB_WR_Reg(0X55,reg55val);	//���ȵ��� 
}
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//Ĭ�Ͼ���contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	SCCB_WR_Reg(0X56,reg56val);	//�Աȶȵ��� 
}
//��Ч����
//0:��ͨģʽ    
//1,��Ƭ
//2,�ڰ�   
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//Ĭ��Ϊ��ͨģʽ
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://�ڰ�
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://����
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB���� 
	SCCB_WR_Reg(0X68,reg67val);//MANU,�ֶ�Uֵ 
	SCCB_WR_Reg(0X67,reg68val);//MANV,�ֶ�Vֵ 
}	
//����ͼ���������
//��QVGA���á�
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sx>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endx>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endy>>3);			//����Href��end�ĸ�8λ
}

 







