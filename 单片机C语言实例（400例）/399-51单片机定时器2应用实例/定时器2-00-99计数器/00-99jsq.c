#include <reg52.h>

typedef   unsigned char  uchar; 
typedef   unsigned int   uint;

uchar  Count,Sec,shift,n;

uchar code  LEDData[ ] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,
                          0x82,0xF8,0x80,0x90,0xff};
uchar data  display[2];
/********************************************************

主函数

*********************************************************/
main()
{
   P0 = 0xff;            //端口初始化
   P2 = 0xff;
   Count = 0x00;
   Sec = 0x00;
   n = 0x00;
   shift = 0xfe;         //送位码初值

   TMOD = 0x01; 
   TH0 = 0x4c;           //50ms定时 
   TL0 = 0x00;
   ET0 = 1; 
   
   T2CON = 0x00;         //设置T2CON寄存器
   TH2 = 0xf8;           //2ms定时
   TL2 = 0xcc;
   ET2 = 1;              //启用Timer2中断
   EA = 1;               //总中断允许
   TR2 = 1;              //启动定时器2
   TR0 = 1;              //启动定时器0

   while(1) ; 

}

/*********************************************************

  Timer0中断函数 （负责定时）

**********************************************************/
void Timer0() interrupt 1 
{
   TH0 = 0x4c;               //50ms定时
   TL0 = 0x00;
   ++Count;                  //中断计数单元加1
   if(Count == 20)           //50ms×20＝1s
   {
     Count = 0;              //清中断计数单元
     ++Sec;                  //秒单元加1
     if(Sec == 100)
     Sec = 0;                //清秒单元
   }
}

/*********************************************************

  Timer2中断函数 （负责数码管显示）

**********************************************************/
void  timer2() interrupt 5 
{ 
   TR2 = 0;
   TF2 = 0;                      //手工清中断标志  
   TH2 = 0xf8;                   //2ms定时常数
   TL2 = 0xcc;

   if(n >= 2)                    //两位数码管显示
   {
     n = 0;
     shift = 0x7f;               //送位码初值
     P2 = 0xff;                  //关闭显示
   }
   else
   {
     display[0] = Sec%10;          //个位
     display[1] = Sec/10;          //十位

     P0 = LEDData[display[n++]];   //送段码
     P2 = shift;                   //送位码
     shift = (shift>>1)|0x80;      //调整位码
   }
   TR2 = 1;
}

/**********************************************************/
