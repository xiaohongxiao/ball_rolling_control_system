#include "control.h"
#include "lcd.h"
#include "usart.h"	 
#include "ov7670.h"
#include "global.h"
#include "timer.h"
#include "scope.h"

//��Ҫ�ļ�

extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	
extern _SetValue_s_ SV;
extern _PID_PwmOutput_s_ PwmOutPut;
//float e_I_Y,e_I_X;					            //����X���Y���ۼ�ƫ��
u16 Motor_X_PWM,Motor_Y_PWM;                    //����PWM���ֵ
aparPID X={0}, Y={0};

//--------------------------------------------------------------------------------
/*
    �������ݱ������ڴ������궨λ
*/

u16 X_MAX = 0, X_MIN = Wide, X_NOW = 0,X_MIN_LSAT = 0,X_MAX_LSAT = 0;   //X����������ݷ�ΧΪ:0~Wide 
u16 Y_MAX = 0, Y_MIN = Height, Y_NOW = 0, Y_MIN_LSAT = 0,Y_MAX_LSAT = 0;   //Y����������ݷ�ΧΪ��0~Height

//--------------------------------------------------------------------------------

//����LCD��ʾ
void camera_refresh(void)
{
	u16 i, j;
	u16 color;
	u16 gray;
	if(ov_sta)//��֡�жϸ���
	{
//		LCD_Scan_Dir(D2U_R2L);		//���ϵ���,������
//        if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-Height)/2,(lcddev.height-Wide)/2,Height,Wide);//����ʾ�������õ���Ļ����
//	    else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-Wide)/2,(lcddev.height-Height)/2,Wide,Height);//����ʾ�������õ���Ļ����	 	
	//LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		//////Ѱ��С���㷨������
		for(i = 0;i < Height;i++)         //Y���ϵ�  
		//�������ص�����
		{
			for(j=0;j<Wide;j++)   //x���ϵ������
			{
				OV7670_RCK_L;
				color = GPIOC -> IDR & 0XFF;	//������
				OV7670_RCK_H; 
				color <<= 8;  
				OV7670_RCK_L;
				color |= GPIOC -> IDR & 0XFF;	//������
				OV7670_RCK_H; 
				//��ɫͼ��ת�ɻҶ�ͼ��������λ�㷨
				//�㷨���۹�ʽ�����ڲ�ɫת�Ҷȣ���һ��������������ѧ��ʽ��
					//Gray = R*0.299 + G*0.587 + B*0.114
				gray = ( (color >> 11) * 19595 + ( ( color >> 5 ) & 0x3f ) * 38469 + ( color & 0x1f ) * 7472 ) >> 16; 
				//���ڼ������ص�Ҷ�
				
				if( gray >= 23 )  //�޶�����ͷͼ�λҶ���ֵ������˵���ǰ���
					{
					if((i > 5 ) && (i < ( Height - 5 ) ) && ( j < Wide - 5 ) && (j > 5)) 
						{   //����ȷ���߿򣬱���С����ȥ  ���仰��˵���Ǹ���Ե����
						  if(j > X_MAX) X_MAX = j;//x������������
							if(j < X_MIN) X_MIN = j;  //x����������С��
						
							if(i > Y_MAX) Y_MAX = i;//y������������ 
							if(i < Y_MIN) Y_MIN = i; //y����������С��
					 }
								color = WHITE;   //�Ҷ�ֵ
				 }
				else{
							color = BLACK;
						}
								LCD->LCD_RAM = color;   //����ɫ��Ϣ����LCD��
			}
		}
		if((X_MAX != 0) && (X_MIN != Wide) && (Y_MAX != 0) && (Y_MIN != Height )) 
			//˵��С���ڰ�������
		{
			X_NOW = (X_MAX + X_MIN) / 2;  
			//����pid��real������Ϣ ������������������ٴα������ֵ ��Сֵ
			Y_NOW = (Y_MAX + Y_MIN) / 2;
		}
		else
		{
			X_NOW = 322;   //����pid��real������Ϣ ������������������ٴα������ֵ ��Сֵ
		  Y_NOW = 242;   //��������ֵ����
		}
		
		X_MAX = 0;
		X_MIN = Wide;
		Y_MAX = 0;
		Y_MIN = Height;


//		LCD_ShowNum(102,150,X_NOW,3,24);  ����ʾ������ʾ����
//		LCD_ShowNum(102,180,Y_NOW,3,24);		
		
  	ov_sta=0;					//����֡�жϱ��
		ov_frame++;      //֡�ļ�������λʱ���ڣ�ͳ��֡�����ݣ����ڲ���֡������� 
	//	LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
		
	}
}

//���ַ���PID�㷨 ��PID�ǲ�һ����
void PID (void)
{
//	static float kp1=0.5,ki1=0.155,kd1=0.2;				//X��PIDϵ��
//	static float kp2=0.5,ki2=0.155,kd2=0.2;				//Y��PIDϵ��
//	static float e_X[2],e_Y[2];						//ǰ������ƫ��


	static u8 flag_Y = 0, flag_X = 0;					//������ֱ�־λ
	if((X_NOW > Wide) || ((Y_NOW > Height ) ))  
		//���С������겻�������ط�Χ��ʱ,�˳����򣬲�����PID����
	{
		PwmOutPut.pwm_Y = 0;  //��ֵPWM��0
		PwmOutPut.pwm_X = 0;  //�����п���
	  return ;
	}
	//����X���Y��ƫ��
	X.err[0] = SV.X - X_NOW;    //XYλ��Ŀ���ʵ�ʲ�ֵ  setpoint value -  controlled value 
	Y.err[0] = SV.Y - Y_NOW;
	
	//printf("e_X:%f\r\n\r\n",e_X[0]);					//��ӡ��ǰƫ��
	
	//===========================Y��PID����========================================
	//���ַ���, �Ա���ƫ��ϴ�ʱ���Կ��ٵļ���ƫ���ƫ���С��ʱ�򣬲ż�����֣��������
	if(Y.err[0] >= 50.0 || Y.err[0] <=- 50.0)  //ƫ�����ص�֮��50��������
  {
		flag_Y = 0;    //�����л��������־λ
	}
	else
	{
		flag_Y = 1;   //���л��������־λ
		Y.e_I += Y.err[0]; //�������     error integral
	}
	
	//�����޷�  ������ֱ�������
	if(Y.e_I > 600)
		Y.e_I = 600;
	if(Y.e_I < -600)
		Y.e_I = -600;
	
	//ȷ�е�΢��û��
	
	
	
	//λ��ʽPID
	PwmOutPut.pwm_Y = Y.kp * Y.err[0] + flag_Y * Y.ki * Y.e_I + Y.kd * ( Y.err[0] - Y.err[1] );
	//control value = p * error(0) + ���ֱ�־λ * i * ������� + d * ������ƫ��-��ȥƫ�
	
	
	
	
	//===========================X��PID����============================================
	//���ַ���,�Ա���ƫ��ϴ�ʱ���Կ��ٵļ���ƫ���
	//ƫ���С��ʱ�򣬲ż�����֣��������
	if(X.err[0]>=50.0||X.err[0]<=-50.0)
	//ƫ������жϲ���
	{
		flag_X = 0;
	}else{
		flag_X = 1;
		X.e_I += X.err[0];
	}
	
	//�����޷�
	if(X.e_I > 600)
		X.e_I = 600;
	if(X.e_I < -600)
		X.e_I = -600;
	
	//λ��ʽPID
	PwmOutPut.pwm_X = X.kp*X.err[0]+flag_X*X.ki*X.e_I+X.kd*(X.err[0]-X.err[1]);
	
	//=======================================================================================
	//��¼����ƫ��
	Y.err[1] = Y.err[0];								//�ñ���ƫ��ֵ�����ϴ�ƫ��
	X.err[1] = X.err[0];								//�ñ���ƫ��ֵ�����ϴ�ƫ��
	
	//OCS_displayData(X_NOW,X.err[0],Y.err[0],X.e_I);
//	X_NOW = PwmOutPut.pwm_X;
//	Y_NOW = PwmOutPut.pwm_Y;
}
void Set_PWM(void)
{
  static s16 PWM_Max = 1550;   //ִ�����ڸ�ϵͳ�ɲ�������г����ֵ
	static s16 PWM_Min = 850;
	static s16 PWM_Max_Y = 2000;
	static s16 PWM_Min_Y = 800;
	
	Motor_X_PWM = SV.X_PWM + PwmOutPut.pwm_X; //λ��ʽPID  ������pwm
	Motor_Y_PWM = SV.Y_PWM + PwmOutPut.pwm_Y;
	if(Motor_X_PWM > PWM_Max) //�����ź�������Ʒ�ֵ������ִ����
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
	
	Set_PWM_X(Motor_X_PWM);  //���Ҹ����������������
	Set_PWM_Y(Motor_Y_PWM);
	//printf("xpwm:%d\r\nypwm:%d\r\n",Motor_X_PWM,Motor_Y_PWM);
}



//���Ժ���
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



