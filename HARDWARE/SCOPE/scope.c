/************************����ʾ�������ֳ���******************/
#include "sys.h"
#include "scope.h"
#include "usart.h"

float OutData[4];

/*
*********************************************************************************************************
*	�� �� ��: CRC_CHECK()
*	����˵��: ����Ӧ��������CRCУ��λ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

/*
*********************************************************************************************************
*	�� �� ��: OCS_displayData()
*	����˵��: ��ʾ�����ϱ��֡��ʽ������ӡ����
*	��    �Σ��ĸ�ͨ����ֵ��Ϊfloat��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void OCS_displayData(float CH1,float CH2,float CH3,float CH4)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;

  OutData[0]=CH1;
  OutData[1]=CH2;
  OutData[2]=CH3;
  OutData[3]=CH4;
  
  for(i=0;i<4;i++)
   {
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];  
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);//CRCУ��
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
  {
   	USART_SendData(USART1, databuf[i]); 
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
  }
    //uart_putchar(databuf[i]);
}



