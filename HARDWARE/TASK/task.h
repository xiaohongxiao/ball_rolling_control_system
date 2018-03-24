#ifndef __TASK_H_
#define __TASK_H_	 

#include "stm32f10x.h"
#include "timer.h"
#include "control.h"
#include "global.h"
#include "lcd.h"

extern u8 taskFlag;

void task1(void);
void task2(void);
void task3(void);
void task4(void);
void task5(void);
void task6(void);
void task7(void);




typedef struct{
	int flag1;
	int flag2;
	int flag3;
	int flag4;
	int flag5;
	int flag6;	
	int flag7;
	int flag8;
		int flag9;
}FLAG;
extern FLAG tflag;


#endif
