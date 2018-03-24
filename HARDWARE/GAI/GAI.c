#include "GAI.h"
#include "global.h"


KEY_FLAG key_flag = {0,0,0,0,0};
AIM_BALL_X_Y aim_ball_x_y ={
{103,24},
{96,109},
{86,195},
{189,33},
{180,120},
{171,206},
{272,44},
{266,125},
{256,217}
};


AREA task5_area = {0,0,0,0};
///选择第几个点


void select_choice(u8 i)
{
	switch(i)
	{
		case 1: SV.X = aim_ball_x_y.aim_1[0] ;  SV.Y = aim_ball_x_y.aim_1[1] ;   break;
		case 2:SV.X = aim_ball_x_y.aim_2[0] ;  SV.Y = aim_ball_x_y.aim_2[1] ; break;
		case 3:SV.X = aim_ball_x_y.aim_3[0] ;  SV.Y = aim_ball_x_y.aim_3[1] ; break;
		case 4:SV.X = aim_ball_x_y.aim_4[0] ;  SV.Y = aim_ball_x_y.aim_4[1] ; break;
		case 5:SV.X = aim_ball_x_y.aim_5[0] ;  SV.Y = aim_ball_x_y.aim_5[1] ; break;
		case 6:SV.X = aim_ball_x_y.aim_6[0] ;  SV.Y = aim_ball_x_y.aim_6[1] ; break;
		case 7:SV.X = aim_ball_x_y.aim_7[0] ;  SV.Y = aim_ball_x_y.aim_7[1] ; break;
		case 8:SV.X = aim_ball_x_y.aim_8[0] ;  SV.Y = aim_ball_x_y.aim_8[1] ; break;
		case 9:SV.X = aim_ball_x_y.aim_9[0] ;  SV.Y = aim_ball_x_y.aim_9[1] ; break;
		
	}
}







