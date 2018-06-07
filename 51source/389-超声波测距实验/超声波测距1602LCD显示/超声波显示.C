//超声波模块显示程序
#include <reg52.h>     //包括一个52标准内核的头文件
#include<intrins.h>  //包含_nop_()函数定义的头文件
#define uchar unsigned char //定义一下方便使用
#define uint  unsigned int
#define ulong unsigned long
sbit Tx  = P3^2; //产生脉冲引脚
sbit Rx  = P3^3; //回波引脚
sbit RS=P2^0;    //寄存器选择位，将RS位定义为P2.0引脚
sbit RW=P2^1;    //读写选择位，将RW位定义为P2.1引脚
sbit E=P2^2;     //使能信号位，将E位定义为P2.2引脚
sbit BF=P0^7;    //忙碌标志位，，将BF位定义为P0.7引脚
unsigned char code string[ ]= {"CHAO SHENG BO"}; 
//unsigned char code string1[ ]={"QUICK STUDY MCU"};
unsigned char code digit[ ]={"0123456789"}; //定义字符数组显示数字
//uchar code SEG7[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//数码管0-9
uint distance[4];  //测距接收缓冲区
uchar ge,shi,bai,temp,flag,outcomeH,outcomeL,i;  //自定义寄存器
bit succeed_flag;  //测量成功标志
//********函数声明
void conversion(uint temp_data);
void delay_20us();
void pai_xu();


/*****************************************************
函数功能：延时1ms
(3j+2)*i=(3×33+2)×10=1010(微秒)，可以认为是1毫秒
***************************************************/
void delay1ms()
{
   unsigned char i,j;	
	 for(i=0;i<10;i++)
	  for(j=0;j<33;j++)
	   ;		 
 }
/*****************************************************
函数功能：延时若干毫秒
入口参数：n
***************************************************/
 void delay(unsigned char n)
 {
   unsigned char i;
	for(i=0;i<n;i++)
	   delay1ms();
 }
/*****************************************************
函数功能：判断液晶模块的忙碌状态
返回值：result。result=1，忙碌;result=0，不忙
***************************************************/
 unsigned char BusyTest(void)
  {
    bit result;
	RS=0;       //根据规定，RS为低电平，RW为高电平时，可以读状态
    RW=1;
    E=1;        //E=1，才允许读写
    _nop_();   //空操作
    _nop_();
    _nop_(); 
    _nop_();   //空操作四个机器周期，给硬件反应时间	
    result=BF;  //将忙碌标志电平赋给result
   E=0;         //将E恢复低电平
    return result;
  }
/*****************************************************
函数功能：将模式设置指令或显示地址写入液晶模块
入口参数：dictate
***************************************************/
void WriteInstruction (unsigned char dictate)
{   
    while(BusyTest()==1); //如果忙就等待
	 RS=0;                  //根据规定，RS和R/W同时为低电平时，可以写入指令
	 RW=0;   
	 E=0;                   //E置低电平(根据表8-6，写指令时，E为高脉冲，
                             // 就是让E从0到1发生正跳变，所以应先置"0"
	 _nop_();
	 _nop_();             //空操作两个机器周期，给硬件反应时间
	 P0=dictate;            //将数据送入P0口，即写入指令或地址
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	 E=1;                   //E置高电平
	 _nop_();
	 _nop_();
	 _nop_();
	 _nop_();               //空操作四个机器周期，给硬件反应时间
	  E=0;                  //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }
/*****************************************************
函数功能：指定字符显示的实际地址
入口参数：x
***************************************************/
 void WriteAddress(unsigned char x)
 {
     WriteInstruction(x|0x80); //显示位置的确定方法规定为"80H+地址码x"
 }
/*****************************************************
函数功能：将数据(字符的标准ASCII码)写入液晶模块
入口参数：y(为字符常量)
***************************************************/
 void WriteData(unsigned char y)
 {
    while(BusyTest()==1);  
	  RS=1;           //RS为高电平，RW为低电平时，可以写入数据
	  RW=0;
	  E=0;            //E置低电平(根据表8-6，写指令时，E为高脉冲，
                       // 就是让E从0到1发生正跳变，所以应先置"0"
	  P0=y;           //将数据送入P0口，即将数据写入液晶模块
	  _nop_();
	  _nop_();
 	  _nop_();
     _nop_();       //空操作四个机器周期，给硬件反应时间
	  E=1;          //E置高电平
	  _nop_();
	  _nop_();
	  _nop_();
	 _nop_();        //空操作四个机器周期，给硬件反应时间
	 E=0;            //当E由高电平跳变成低电平时，液晶模块开始执行命令
 }
/*****************************************************
函数功能：对LCD的显示模式进行初始化设置
***************************************************/
void LcdInitiate(void)
{
   delay(15);             //延时15ms，首次写指令时应给LCD一段较长的反应时间
   WriteInstruction(0x38);  //显示模式设置：16×2显示，5×7点阵，8位数据接口
	delay(5);               //延时5ms　，给硬件一点反应时间
   WriteInstruction(0x38);
	delay(5);
	WriteInstruction(0x38); //连续三次，确保初始化成功
	delay(5);
	WriteInstruction(0x0c);  //显示模式设置：显示开，无光标，光标不闪烁
	delay(5);
        WriteInstruction(0x06);  //显示模式设置：光标右移，字符不移
	delay(5);
	WriteInstruction(0x01);  //清屏幕指令，将以前的显示内容清除
	delay(5);
 }
 








void main(void)   // 主程序
{  uint distance_data,a,b;
   uchar CONT_1;  
  uchar k;   //定义变量i指向字符串数组元素
    LcdInitiate();         //调用LCD初始化函数  
   delay(10);             //延时10ms，给硬件一点反应时间
   WriteAddress(0x01);   // 从第1行第3列开始显示
   k = 0;               //指向字符数组的第1个元素
 	while(string[k] != '\0')
		{					
			WriteData(string[k]);
			k++;             //指向下字符数组一个元素				
		}	 
   i=0;

   flag=0;
	Tx=0;       //首先拉低脉冲输入引脚
	TMOD=0x10;    //定时器0，定时器1，16位工作方式
//	TR0=1;	     //启动定时器0
   IT1=0;        //由高电平变低电平，触发外部中断
	//ET0=1;        //打开定时器0中断
	EX1=0;        //关闭外部中断
	EA=1;         //打开总中断0	
  
	
while(1)         //程序循环
	{
	 WriteAddress(0x41);    // 从第2行第6列开始显示
	        WriteData('J');  //将万位数字的字符常量写入LCD
			 WriteData('U');  //将万位数字的字符常量写入LCD
			  WriteData('L');  //将万位数字的字符常量写入LCD
			   WriteData('I');  //将万位数字的字符常量写入LCD
			    WriteData(':');  //将万位数字的字符常量写入LCD
		  WriteData(digit[bai]);  //将万位数字的字符常量写入LCD
		  WriteData(digit[shi]);  //将千位数字的字符常量写入LCD
		  WriteData('.');  //将万位数字的字符常量写入LCD
		  WriteData(digit[ge]);  //将百位数字的字符常量写入LCD
		  WriteData(' ');  //将百位数字的字符常量写入LCD
		  WriteData('C');  //将万位数字的字符常量写入LCD
		  WriteData('M');  //将万位数字的字符常量写入LCD
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
              
            pai_xu();
             distance_data=distance[1];

      
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
//外部中断0，用做判断回波电平
INTO_()  interrupt 2   // 外部中断是2号
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
 	// TH0=0xfd; //写入定时器0初始值
	// TL0=0x77;	 	
	
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

    //bai_data=SEG7[bai_data];
    //shi_data=SEG7[shi_data]&0x7f;
    //ge_data =SEG7[ge_data];

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
   void pai_xu()
  {  uint t;
  if (distance[0]>distance[1])
    {t=distance[0];distance[0]=distance[1];distance[1]=t;} 
  if(distance[0]>distance[2])
    {t=distance[2];distance[2]=distance[0];distance[0]=t;} 
  if(distance[1]>distance[2])
    {t=distance[1];distance[1]=distance[2];distance[2]=t;}  
    }


