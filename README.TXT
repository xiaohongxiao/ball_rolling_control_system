实验器材:
	正点原子精英STM32F103ZET6开发板

硬件资源:
	1,DS0(连接在PB5)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过FSMC驱动,FSMC_NE4接LCD片选/A10接RS)
	4,按键KEY0(PE4)/KEY1(PE3)/KEY_UP(PA0,也称之为WK_UP)
	5,TPAD电容触摸按键(右下角LOGO,即TPAD,连接在PA1)
	6,外部中断8(PA8,用于检测OV7670的帧信号)
	7,定时器6(用于打印摄像头帧率)
	8,ALIENTEK OV7670摄像头模块,连接关系为:
		OV7670模块 ----------- STM32开发板
		 OV_D0~D7  -----------   PC0~7
		 OV_SCL    ------------  PD3
		 OV_SDA    ------------  PG13
		 OV_VSYNC  ------------  PA8
		 FIFO_RRST -----------   PG14
		 FIFO_OE   -----------   PG15
		 FIFO_WRST ------------  PD6
		 FIFO_WEN  ------------  PB3
		 FIFO_RCLK ------------  PB4	  
	
---------------------板球系统说明---------

概述：
以STM32单片机为控制器，舵机为执行器，摄像头为检测仪表（测量元件）的并基于积分分离算法的板球控制系统，实现对小球在平板上精确定位的控制功能

确定系统方案：
1.确定控制目标，选择被控变量
	实现滚球在平板上的定位。被控变量选择滚球在平板上的坐标
2.选择测量参数与测量仪表
    测量参数为滚球在平板上的坐标。  
    测量仪表：选择摄像头
        原因：检测成本低，控制质量要求在可接受范围内，  对于平板的长宽，摄像头量程基本满足需要，可靠性较低
    检测工作原理：像素获取，（未进行滤波操作）直接二值化，小球轮廓检测，获取小球中心    
    检测小球位置的算法：
    	通过遍历，寻找小球的上下左右四个顶点位置，求小球质心位置
    检测参数的滤波处理操作：
        比赛中，无，直接进行图像二值化处理，图像在获取和传输时容易产生噪声，影响目标特征的准确性，阈值人为选择，容易受到光线的干扰，比赛时使用台灯进行补光操作。应该进行图像处理
    测量误差 60cm/320 = 1.8mm  60cm/240  = 2.5mm  题目给出小球圆边界与最大范围圆边界有15mm的允许误差，满足精度要求，对控制要求较高 
3.操作变量的选择与主要扰动的分析
操作变量：平板的俯仰角和横滚角。
扰动变量：光源对于平板的漫反射
4.操作变量与被控变量的配对
   在本比赛中无需考虑，比较明显能够找到，其他变量对于被控变量的增益都过小
5.控制方案与控制算法的选择
	控制方案：最简单的单回路控制    or  串级控制  选择单回路控制
	控制算法：积分分离的PID算法
	   测量值与设定值之间的偏差超出某一个阈值，使用PD算法，在阈值范围之内加入积分效果
	   control value = last controled value + P * error + flag * I * error integral value + d * (now error - last error)
6.执行器的选型
    根据操作变量的工艺条件以及对执行器位移特性的要求的选择
    选择控制变量与操作变量线性度较好的舵机
   驱动舵机功率放大电路：

7.控制系统的现场安装、调试
	检查和调整个控制仪表和设备的工作状况。
	执行器是舵机的时候工作在合理电压下才会满足线性特性
	控制器参数整定过程：临界振荡法


控制算法：
摄像头检测：小球运动检测及处理
舵机驱动电路：
测试方法： 
测试结果分析：

遇到的情况及解决方法：

1.常规PID 控制系统长时间震荡？
大幅度提降设定值，解决方法是积分分离算法，

2.存在外部扰动，尤其是外部光源
使用可调节亮度的台灯进行补光，并且将平台通过
显示屏显示出来


3.摄像头无法完整覆盖整个平板
通过适当的太高摄像头，牺牲一部分小球位置精确度。

4.舵机无法准确驱动，控制变量与操作变量不成线性关系
测量电池电压，发现电压过低，更换电池

5.摄像头采样率（帧数） 测量滞后现象
实际效果还可以，30帧的采样率
滞后会影响系统稳定性，系统相位裕度会减小

6.物理元件的死区和饱和的现象怎么解决，运动范围
通过限幅


7.被控对象是一个非线性系统，你是如何控制的
    通过反馈，可以将被控对象视作黑箱，非线性系统可以使用PID去控制被控对象

8.调试PID 参数问题，系统一直得不到临界震荡的效果或者震荡不明显？
使用的临界比例度法，他适用于系统时间常数较小的情况。  时间常数较大下，纯比例作用下会很稳定
改进：增长执行器舵机的舵臂，提升广义对象系统的响应性能，
临界比例度法：先去掉积分和微分部分，将P值逐渐增大到系统临界震荡，然后加入积分，对P适当缩小






































