#include "reg52.h"
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
uchar i,j;
sbit P2_7=P2^7;
sbit fmq=P3^6;
sbit P3_7=P3^7;
sbit D0=P1^0;
sbit D1=P1^1;
sbit D2=P1^2;
sbit D3=P1^3;
void delay1ms(unsigned int i)
{

unsigned char j;
 while(i--)
 {
 for(j=0;j<115;j++)	  //1ms基准延时程序 
 {
  ;
 }
 
 
 }
  
}

void delay500us()
{

unsigned char j;

 for(j=0;j<57;j++)	  //500us基准延时程序 
 {
  ;
 } 
}

void beep()	//产生1KHZ频率声音的函数
{
 	fmq=0;
	delay500us();
	fmq=1;
	delay500us();	

}
void beep1()
{
uint k;
for(k=0;k<500;k++)
{
beep();

}
fmq=1;
delay1ms(500);

}





void main(void)
{
P2_7=0;//开数码管显示 选通左右侧的数码管
P0=0xc0; //初始显示数字0
P3=0xff; //P3口设置为输入状态 先全部写1 

 while(1){



//i=i&0x0f; //和0x0f相与 屏蔽高四位,读取无线遥控码

if (D0==1)//如果是0x04，则代表无线遥控器的按键A按下，执行显示
{
P0=0x88; //字母A的字型码
beep1();
}
if (D1==1)//如果是0x01，则代表无线遥控器的按键b按下，执行显示
{
P0=0x83;//字母b的字型码
 fmq=1;
}
if (D2==1)//如果是0x08，则代表无线遥控器的按键C按下，执行显示 
{ 
P0=0xc6;//字母C的字型码
 P3_7=0;//继电器吸合
}
if (D3==1)//如果是0x02，则代表无线遥控器的按键d按下，执行显示
{
P0=0xa1;//字母d的字型码
 P3_7=1; //继电器释放
}
 }
}
