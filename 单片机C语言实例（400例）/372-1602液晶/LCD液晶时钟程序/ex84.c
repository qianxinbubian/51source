//液晶时钟设计
#include<reg52.h>    //包含单片机寄存器的头文件
#include<stdlib.h>   //包含随机函数rand()的定义文件
#include<intrins.h>  //包含_nop_()函数定义的头文件
sbit RS=P2^0;    //寄存器选择位，将RS位定义为P2.0引脚
sbit RW=P2^1;    //读写选择位，将RW位定义为P2.1引脚
sbit E=P2^2;     //使能信号位，将E位定义为P2.2引脚
sbit BF=P0^7;    //忙碌标志位，，将BF位定义为P0.7引脚
unsigned char code digit[ ]={"0123456789"}; //定义字符数组显示数字
unsigned char code string[ ]={"Beijing Time"}; //定义字符数组显示提示信息
unsigned char count;   //定义变量统计中断累计次数
unsigned char s,m,h;   //定义变量储存秒、分钟和小时
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
 /******************************************************************************
 函数功能：显示小时                          
******************************************************************************/
void DisplayHour()
{
   unsigned char i,j;
	i=h/10;                //取整运算，求得十位数字
	j=h%10;                //取余运算，求得各位数字
   WriteAddress(0x44);   //写显示地址，将十位数字显示在第2行第5列
	WriteData(digit[i]);  //将十位数字的字符常量写入LCD
	WriteData(digit[j]);  //将个位数字的字符常量写入LCD
     
 }
/******************************************************************************
 函数功能：显示分钟                          
 ******************************************************************************/
void DisplayMinute()
{
  unsigned char i,j;
   i=m/10;                //取整运算，求得十位数字
	j=m%10;                //取余运算，求得各位数字
   WriteAddress(0x47);   //写显示地址，将十位数字显示在第2行第8列
	WriteData(digit[i]);  //将十位数字的字符常量写入LCD
	WriteData(digit[j]);  //将个位数字的字符常量写入LCD
     
 }
 /******************************************************************************
 函数功能：显示秒                        
  ******************************************************************************/
void DisplaySecond()
{
   unsigned char i,j;
  	i=s/10;                //取整运算，求得十位数字
	j=s%10;                //取余运算，求得各位数字
   WriteAddress(0x4a);   //写显示地址，将十位数字显示在第2行第11列
	WriteData(digit[i]);  //将十位数字的字符常量写入LCD
	WriteData(digit[j]);  //将个位数字的字符常量写入LCD
 }

/*********************************************************************
               main function
***********************************************************************/
 void main(void)
 {
  unsigned char i;
    LcdInitiate();  //调用LCD初始化函数

   TMOD=0x01;                //使用定时器T0的模式1
	TH0=(65536-46083)/256;    //定时器T0的高8位设置初值
	TL0=(65536-46083)%256;    //定时器T0的低8位设置初值
	EA=1;                     //开总中断
	ET0=1;                    //定时器T0中断允许
	TR0=1;                    //启动定时器T0

	count=0;              //中断次数初始化为0
	s=0;                  //秒初始化为0
	m=0;                  //分钟初始化为0
	h=0;                  //小时初始化为0

   WriteAddress(0x03); 	 //写地址，从第1行第4列开始显示
	 i=0;              //从字符数组的第1个元素开始显示
	 while(string[i]!='\0')   //只要没有显示到字符串的结束标志'\0'，就继续
	 {
		 WriteData(string[i]); //将第i个字符数组元素写入LCD
		 i++;                 //指向下一个数组元素
	 }
 	WriteAddress(0x46);     //写地址，将第二个分号显示在第2行第7列
	WriteData(':');         //将分号的字符常量写入LCD
	WriteAddress(0x49);     //写地址，将第二个分号显示在第2行第10列
	WriteData(':');         //将分号的字符常量写入LCD  
    while(1)            //无限循环
      {
        	DisplayHour();	    //显示小时
			 delay(5);         //给硬件一点反应时间
			DisplayMinute();   //显示分钟
			 delay(5);         //给硬件一点反应时间
			DisplaySecond();   //显示秒	
            delay(5);	    //给硬件一点反应时间
	   }
}
/*******************************************************
函数功能：定时器T0的中断服务函数
********************************************************/
 void Time0(void ) interrupt 1 using 1 //定时器T0的中断编号为1，使用第1组工作寄存器
  {
     count++;  //每产生1次中断，中断累计次数加1
		 if(count==20)  //如果中断次数计满20次
		    {
              count=0;  //中断累计次数清0
		        s++;      //秒加1				 
		 	}
		  if(s==60)   //如果计满60秒
			 {
			  s=0;     //秒清0
			  m++;     //分钟加1
		    }
	  	if(m==60)    //如果计满60分
		   {
			   m=0;   //分钟清0 
			   h++;    //小时加1
		    }
	     if(h==24)  //如果计满24小时
		   {
			   h=0;   //小时清0
		   }
	   TH0=(65536-46083)/256;   //定时器T0高8位重新赋初值
		TL0=(65536-46083)%256;   //定时器T0低8位重新赋初值		
  }  


	     
