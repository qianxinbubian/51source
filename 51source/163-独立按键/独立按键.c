/********************************************************************
* 文件名  ： 独立按键.c
* 描述    :  该程序实现独立按键去控制 LED灯 的亮灭，并讲诉了对按键的处理方法。
			 独立按键相应的IO口平时为高电平，一旦按键按下，单片机便检测到低电平。
			 按下P3.2，P0.0口对应的LED亮灭交替变化。
			 按下P3.3，P0.1口对应的LED亮灭交替变化。
			 按下P3.4，P0.2口对应的LED亮灭交替变化。
* 创建人  ： 东流，2009年8月27日
* 版本号  ： 1.0
***********************************************************************/
#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int

sbit KEY1 = P3^2;    
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;

sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;

/********************************************************************
* 名称 : Delay()
* 功能 : 延时,延时时间为 10ms * del
* 输入 : del
* 输出 : 无
***********************************************************************/
void Delay(uint del)
{
	uint i,j;
	for(i=0; i<del; i++)
	for(j=0; j<1827; j++)   
	;
}

/********************************************************************
* 名称 : Delay()
* 功能 : 实现按键功能，当按键按下时，相应的LED亮灭交替
* 输入 : 无
* 输出 : 无
***********************************************************************/
void KEY()
{
	if(KEY1==0 || KEY2==0 || KEY3==0)	
	{
		Delay(2);   		//20毫秒软件防抖
		if(KEY1 == 0)
		{ 
			LED1 = !LED1;		//LED显示取反
		}
		else if(KEY2 == 0)
		{			
			LED2 = !LED2;
		}
		else 
		{			
			LED3 = !LED3;
		}
		Delay(50);			//延时0.5秒再进行下次按键的检测
	}	
}
/********************************************************************
* 名称 : Main()
* 功能 : 实现按键控制LED的亮灭
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Main(void)
{
	while(1)
	{
		 KEY();
	}
}