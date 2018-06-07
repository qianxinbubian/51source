//超声波模块显示程序
#include <reg52.h>     //包括一个52标准内核的头文件
#define uchar unsigned char //定义一下方便使用
#define uint  unsigned int
#define ulong unsigned long
sbit Tx  = P3^2; //产生脉冲引脚
sbit Rx  = P3^3; //回波引脚
uchar code SEG7[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//数码管0-9
uint distance[4];  //测距接收缓冲区
uchar ge,shi,bai,temp,flag,outcomeH,outcomeL,i;  //自定义寄存器
bit succeed_flag;  //测量成功标志
//********函数声明
void conversion(uint temp_data);
void delay_20us();
void main(void)   // 主程序
{  uint distance_data,a,b;
   uchar CONT_1;   
   i=0;
   flag=0;
	Tx=0;       //首先拉低脉冲输入引脚
	TMOD=0x11;    //定时器0，定时器1，16位工作方式
	TR0=1;	     //启动定时器0
   IT1=0;        //由高电平变低电平，触发外部中断
	ET0=1;        //打开定时器0中断
	EX1=0;        //关闭外部中断
	EA=1;         //打开总中断0	
  
	
while(1)         //程序循环
	{
  EA=0;
	     Tx=1;
        delay_20us();
        Tx=0;         //产生一个20us的脉冲，在Tx引脚  
        while(Rx==0); //等待Rx回波引脚变高电平
	     succeed_flag=0; //清测量成功标志
	     EX1=1;          //打开外部中断
	 	  TH1=0;          //定时器1清零
        TL1=0;          //定时器1清零
	     TF1=0;          //
        TR1=1;          //启动定时器1
   EA=1;

      while(TH1 < 30);//等待测量的结果，周期65.535毫秒（可用中断实现）  
		  TR1=0;          //关闭定时器1
        EX1=0;          //关闭外部中断

    if(succeed_flag==1)
	     { 	
		   distance_data=outcomeH;                //测量结果的高8位
           distance_data<<=8;                   //放入16位的高8位
		     distance_data=distance_data|outcomeL;//与低8位合并成为16位结果数据
            distance_data*=12;                  //因为定时器默认为12分频
           distance_data/=58;                   //微秒的单位除以58等于厘米
         }                                      //为什么除以58等于厘米，  Y米=（X秒*344）/2
			                                       // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58 
    if(succeed_flag==0)
		   {
            distance_data=0;                    //没有回波则清零

           }

           distance[i]=distance_data; //将测量结果的数据放入缓冲区
            i++;
  	  	 if(i==3)
	  	     {
	  	       distance_data=(distance[0]+distance[1]+distance[2]+distance[3])/4;
            

      
	   a=distance_data;
       if(b==a) CONT_1=0;
       if(b!=a) CONT_1++;
       if(CONT_1>=3)
		   { CONT_1=0;
			  b=a;
			  conversion(b);
			}       
	  		 i=0;
 	  		}	     
	 }
}
//***************************************************************
//外部中断1，用做判断回波电平
INT1_()  interrupt 2   // 外部中断是2号
 {    
     outcomeH =TH1;    //取出定时器的值
     outcomeL =TL1;    //取出定时器的值
     succeed_flag=1;   //至成功测量的标志
     EX1=0;            //关闭外部中断
  }
//****************************************************************
//定时器0中断,用做显示
timer0() interrupt 1  // 定时器0中断是1号
   {
 	 TH0=0xfd; //写入定时器0初始值
	 TL0=0x77;	 	
	 switch(flag)   
      {case 0x00:P0=ge; P2=0x7f;flag++;break;
	    case 0x01:P0=shi;P2=0xbf;flag++;break;
	    case 0x02:P0=bai;P2=0xdf;flag=0;break;
      }
   }

//显示数据转换程序
void conversion(uint temp_data)  
 {  
    uchar ge_data,shi_data,bai_data ;
    bai_data=temp_data/100 ;
    temp_data=temp_data%100;   //取余运算
    shi_data=temp_data/10 ;
    temp_data=temp_data%10;   //取余运算
    ge_data=temp_data;

    bai_data=SEG7[bai_data];
    shi_data=SEG7[shi_data]&0x7f;
    ge_data =SEG7[ge_data];

    EA=0;
    bai = bai_data;
    shi = shi_data;
    ge  = ge_data ; 
	 EA=1;
 }
//******************************************************************


void delay_20us()
 {  uchar bt ;
    for(bt=0;bt<60;bt++);
 }
 
