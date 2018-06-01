//用LED数码管显示计数器T0的计数值
#include<reg51.h>   //包含51单片机寄存器定义的头文件
sbit S=P3^2 ;  //将S位定义为P3.2引脚
unsigned char Tab[ ]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};  //段码表
unsigned char x;
/******************************************************************
函数功能： 延时约0.6ms
********************************************************************/
 void delay(void)      
 {
    unsigned char j;
	  for(j=0;j<200;j++)
	        ;
  } 

/******************************************************************
函数功能：显示计数次数的子程序
入口参数：x
********************************************************************/
 void Display(unsigned char x)
{
    P2=0xbf;        //P3.6引脚输出低电平，DS6点亮 
    P0=Tab[x/10];   //显示十位
    delay();
	 delay();
	  delay();
	 delay();
    P2=0x7f;      //P3.7引脚输出低电平，DS7点亮 
    P0=Tab[x%10];  //显示个位
    delay();
	 delay();
	  delay();
	 delay();
	P2=0xff;
	P0=0xff;
	 delay();
	 delay();
	  delay();
	 delay(); 
 } 

/*******************************************
函数功能：主函数
******************************************/	
void main(void)
  {
   EA=1;   //开放总中断
   EX0=1;  //允许使用外中断
   IT0=1;  //选择负跳变来触发外中断
	x=0;
	
	while(1) 
    Display(x);
    
 }
/**************************************************************
函数功能：外中断T0的中断服务程序
**************************************************************/
void int0(void) interrupt 0 using 0 //外中断0的中断编号为0
{
  x++;
  if(x==100)
   x=0;

 }
