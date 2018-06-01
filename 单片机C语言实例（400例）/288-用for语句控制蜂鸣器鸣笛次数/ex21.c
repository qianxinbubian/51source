//实例21：用for语句控制蜂鸣器鸣笛次数
#include<reg51.h>  //包含单片机寄存器的头文件
sbit sound=P3^7;  //将sound位定义为P3.7
/****************************************
函数功能：延时形成1600Hz音频
****************************************/
void delay1600(void)
{
 unsigned char n;
   for(n=0;n<100;n++)
        ;
}
/****************************************
函数功能：延时形成800Hz音频
****************************************/
void delay800(void)
{
 unsigned char n;
   for(n=0;n<200;n++)
        ;
}

/****************************************
函数功能：主函数
****************************************/
void main(void)
{  
  unsigned int i;
	while(1)
    {
	  for(i=0;i<830;i++)
	   {
        sound=0;  //P3.7输出低电平
        delay1600();  
        sound=1;  //P3.7输出高电平
        delay1600();
		}
	  for(i=0;i<200;i++)
	   {
		  sound=0;  //P3.7输出低电平
        delay800();  
        sound=1;  //P3.7输出高电平
        delay800();
		}
    
	 }
	
}