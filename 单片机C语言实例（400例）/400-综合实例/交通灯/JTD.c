#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

sbit     RED_ZHU    =  P0^0; //主路红灯
sbit     YELLOW_ZHU =  P0^1; //主路黄灯
sbit     GREEN_ZHU  =  P0^2; //主路绿灯

sbit     RED_ZHI    =  P1^0; //支路红灯
sbit     YELLOW_ZHI =  P1^1; //支路黄灯
sbit     GREEN_ZHI  =  P1^2; //支路绿灯

/*********************************************************
   500ms延时函数
   晶振：11.0592MHz
*********************************************************/
void delay(unsigned char j) 
{ 
   unsigned char k;
   unsigned int i;
   for(;j>0;j--)
  {
     for(i=1250;i>0;i--) 
     {
       for(k=180;k>0;k--);
     }
   }
} 

/*********************************************************
   主函数
*********************************************************/
main()
{
   uchar t;
   P0=0xff;
   P3=0xff;
   RED_ZHU=0;                    //第一个状态
  RED_ZHI=0;
   delay(10);
  while(1)
  {    
     RED_ZHU=1;                   //第二个状态
   RED_ZHI=0;
    GREEN_ZHU=0;
    delay(60);

     for (t=6;t>0;t--)           //第三个状态
    {
        GREEN_ZHU=~GREEN_ZHU;
       delay(1);
      }

    YELLOW_ZHU=0;
    GREEN_ZHU=1;
    delay(10);
    YELLOW_ZHU=1;

     RED_ZHU=0;                  //第四个状态
   RED_ZHI=1;
    GREEN_ZHI=0;
    delay(50);

     for (t=6;t>0;t--)           //第五个状态
    {
        GREEN_ZHI=~GREEN_ZHI;
        delay(1);
      }

    YELLOW_ZHI=0;
    GREEN_ZHI=1;
    delay(10);

     YELLOW_ZHI=1;
  }

}