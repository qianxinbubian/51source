#include<reg52.h>
#include "ds1302.h"
#define uchar unsigned char
#define uint unsigned int

uchar code dis[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF};
//共阴数码管"0-9","灭","-"编码

uchar code wei[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
//位选编码

uchar data dis1[]={0x00,0x00,0x0b,0x00,0x00,0x0b,0x00,0x00};
//时分秒显示缓冲区

uchar data chuzhi[]={0x23,0x59,0x50};//送十六进制数
//时分秒初值

uchar data time1302[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//延时1ms子程序
void delay(uint z)
{
	uint x,y;
	for(x=z;z>0;z--)
		for(y=110;y>0;y--)
			;
}

//显示子函数
void display()
{
	uchar k;
	dis1[7]=chuzhi[2]%10;
	dis1[6]=chuzhi[2]/10;
	dis1[4]=chuzhi[1]%10;
	dis1[3]=chuzhi[1]/10;
	dis1[1]=chuzhi[0]%10;
	dis1[0]=chuzhi[0]/10;
	for(k=0;k<8;k++)
	{		
		P2=wei[k];		
		P0=dis[dis1[k]];
		delay(1);//让数码管正常显示出来
	}
}

void main()
{	
	init1302();	
	while(1)
	{
					
		chuzhi[2]=Read1302(0x81);
		chuzhi[1]=Read1302(0x83);
		chuzhi[0]=Read1302(0x85);
		display();	
	}
}
