#include "usmart.h"
#include "usmart_str.h" 

////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	
#include "usart.h"		
#include "sys.h"
#include "lcd.h"
#include "ov7670.h"
#include "control.h"

extern void led_set(u8 sta);
//extern void pidChange(float p, float i, float d);
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",	 

	(void*)SCCB_WR_Reg,"u8 SCCB_WR_Reg(u8 reg,u8 data)", 
	(void*)SCCB_RD_Reg,"u8 SCCB_RD_Reg(u8 reg)",
	(void*)OV7670_Window_Set,"void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)",
	//(void*)pidChange,"void pidChange(float p,float i,float d)",
(void*)tiaoshi,"void tiaoshi(float X_kp, float X_ki,float X_kd, float Y_kp, float Y_ki,float Y_kd,float X_e_I,float Y_e_I)"

};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















