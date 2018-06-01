/********************************************************************
* 名称 : 蜂鸣器发声.c
* 功能 : 通过P1.2口电平的交替变化，来控制蜂鸣器唱歌
/***********************************************************************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int	  
sbit BELL = P1^2;		  	//设置P1.2口，为控制蜂鸣器发声的引脚									  

/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms * x
* 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay(uint i)
{
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}

/********************************************************************
* 名称 : Main()
* 功能 : 实现灯的闪烁
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Main(void)
{
	uint i;
	while(1)
	{
		for(i = 0; i < 1000; i++)
		{
			BELL = 0;
			Delay(1);	   //延时10毫秒
			BELL = 1;
			Delay(1);	   //延时10毫秒	
		}
		Delay(1000);
	}
}