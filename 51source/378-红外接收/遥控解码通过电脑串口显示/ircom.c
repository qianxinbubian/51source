

/*    晶振：11.0569MHz              */
#include <REGX52.h>
#define uchar unsigned char
uchar data    IRcode[4];   //定义一个4字节的数组用来存储代码
uchar     CodeTemp;   //编码字节缓存变量
uchar     i,j,k;    //延时用的循环变量
sbit     IRsignal=P3^2;   //HS0038接收头OUT端直接连P3.2(INT0)
/**************************延时0.9ms子程序**********************/
void Delay0_9ms(void)
{uchar j,k;
     for(j=18;j>0;j--)
       for(k=20;k>0;k--) ;
}
/***************************延时1ms子程序**********************/
void Delay1ms(void)
{uchar i,j;
   for(i=2;i>0;i--)
      for(j=230;j>0;j--) ;
}
/***************************延时4.5ms子程序**********************/
void Delay4_5ms(void)
{uchar i,j;
     for(i=10;i>0;i--)
      for(j=225;j>0;j--) ;
}
/**************************** 延时子程序 ************************/
void Delay(void)
{uchar i,j,k;
for(i=200;i>0;i--)  
   for(j=200;j>0;j--)
      for(k=3;k>0;k--) ;
}
/************************中断0解码服务子程序**********************/
void int0(void) interrupt 0 using 2       
{
EA=0;
for(k=0;k<10;k++)
   {Delay0_9ms();
    if (IRsignal==1)         //如果0.9ms后IRsignal=1，说明不是引导码
   {k=10;break;}
    else if(k==9)           //如果持续了10×0.9ms=9ms的低电平，说明是引导码
      {while(IRsignal==0);
         Delay4_5ms();        //跳过持续4.5ms的高电平

      for(i=0;i<4;i++)       //分别读取4个字节
         {for(j=1;j<=8;j++)      //每个字节8个bit的判断
                { while(IRsignal==0);       //等待上升沿
                  Delay0_9ms();      //从上升沿那一时刻开始延时0.9ms，再判断IRsignal
                  if(IRsignal==1)     //如果IRsignal是"1",则向右移入一位"1"
                    {Delay1ms();
                   CodeTemp=CodeTemp|0x80;
                   if(j<8) CodeTemp=CodeTemp>>1;
                  }
                  else
      if(j<8)CodeTemp=CodeTemp>>1;//如果IRsignal是"0",则向右移一位，自动补"0"
                 }
              IRcode[i]=CodeTemp;
              CodeTemp=0;
         }
    for(i=0;i<4;i++)        //通过串口将代码发出
             { 
     SBUF=IRcode[i];
     while(!TI);       //等待一个字节发送完毕
     TI=0;
             }
   Delay();
       }
   }
EA=1;
}
/***********************串口初始化程序*********************/
void initUart(void)
{  
TMOD|=0x20;
SCON=0x50;
PCON|=0x80;
TH1=0xff;     //57600bps @ 11.0592MHz
TL1=0xff;
TR1=1;
}
/**************************主程序*************************/
void main()
{ initUart();
IT0=1;    //INT0为负边沿触发, (1：负边沿触发，0：低电平触发)
EX0=1;    //外部中断INT0开, (1：开，      0：关    )
EA=1;    //开所有中断
CodeTemp=0;    //初始化红外编码字节缓存变量
Delay();
while(1);
}
 
