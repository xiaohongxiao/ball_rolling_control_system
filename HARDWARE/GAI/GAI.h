#ifndef _GAI_H_
#define _GAI_H_

#include "sys.h"



typedef struct
{
	u8 flag1;
	u8 flag2;
	u8 flag3;
	u8 flag4;
	u8 flag5;
}KEY_FLAG;
extern KEY_FLAG key_flag;


typedef struct{

u8 area1;
u8 area2;	
u8 area3;
u8 area4;

}AREA;


extern AREA task5_area;




typedef struct{
	  int aim_1[2];
		int aim_2[2];
		int aim_3[2];
		int aim_4[2];
		int aim_5[2];
		int aim_6[2];
		int aim_7[2];
		int aim_8[2];
		int aim_9[2];
	
}AIM_BALL_X_Y;
extern AIM_BALL_X_Y  aim_ball_x_y;

void select_choice(u8 i);



#endif