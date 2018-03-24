#include "global.h"

//指示任务是否开启结构体
TaskMask_s_ TaskMask={0};

//给定数据(例如给电机初始PWM,设置小球X轴和Y轴数据)
_SetValue_s_ SV={0};

//PID运算后输出值
_PID_PwmOutput_s_ PwmOutPut = {0};

