//实例64：用数码管显示动态检测结果
#include<reg51.h>    //  包含51单片机寄存器定义的头文件
#include<stdlib.h>   //包含随机函数rand()的定义文件
unsigned char i;   //记录中断次数
unsigned int x;    //随机检测的数据
unsigned char code Tab[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};   //数码管显示0～9的段码表

/***********************************************************************
函数功能：快速动态扫描延时，延时约0.9毫秒
************************************************************************/
void delay(void)
{
  unsigned int i;
  for(i=0;i<300;i++)
    ;
}
/***********************************************************************
函数功能：4位数的数码显示器显示
入口参数：k
出口参数：无
************************************************************************/
void display(unsigned int k)    
{
  
	P2=0xfe;   //即P2=1111 1110B，P2.0引脚输出低电平，数码显示器DS0接通电源
   P0=Tab[k/1000];        //显示千位
	delay();   
   P2=0xfd ;  //即P2=1111 1101B，P2.1引脚输出低电平，数码显示器DS1接通电源
   P0=Tab[(k%1000)/100];          //显示百位
	delay();
   P2=0xfb;   //即P2=1111 1011B，P2.2引脚输出低电平，数码显示器DS2接通电源          
   P0=Tab[(k%100)/10];  //显示十位
	 delay();
   P2=0xf7;   //即P2=1111 0111B ，P2.3引脚输出低电平，数码显示器DS3接通电源               
	P0=Tab[k%10];//显示个位
	delay();
   P2=0xff;  //关闭所有显示器
 }

void main(void)     //主函数
{
  	      TMOD=0x01;                 //使用定时器T0
			TH0=(65536-46083)/256;    //将定时器计时时间设定为46083×1.085微秒=50000微秒=50毫秒
			TL0=(65536-46083)%256;
			EA=1;                    //开启总中断
			ET0=1;                   //定时器T0中断允许
			TR0=1;                   //启动定时器T0开始运行
			
	while(1)
	{
	    display(x);   //调用检测结果的显示程序
	 }  
	
}
/********************************************************
函数功能：定时器T0的中断服务程序
*******************************************************/

void Time0(void) interrupt 1 using 1
{
    TR0=0;    //关闭定时器T0
   i++;        //每来一次中断，i自加1	
	if(i==20)   //够20次中断，即1秒钟进行一次检测结果采样
	  {
        x=rand()/10;     //随机产生一个从0到32767的整数，再将其除以10，获得一个随机4位数，模拟检测结果
		  i=0;             //将i清0，重新统计中断次数
	  }	   
  	TH0=(65536-46083)/256;   //重新给计数器T0赋初值
	TL0=(65536-46083)%256;
	TR0=1;     //启动定时器T0
	
 }

