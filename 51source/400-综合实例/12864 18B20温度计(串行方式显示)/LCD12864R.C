/**********************************************************
*描述:YM12864R-3带中文字库图形液晶驱动
*版本:V1.0
*控制器:ST7920
*通讯方式:串行方式
*编程:飞扬
**********************************************************/
#include <at89x52.h>
//引脚定义:
sbit  LCD_CS =P2^0;	//LCD片选端
sbit  LCD_SID=P2^1;	//LCD串行数据输入/输出端
sbit  LCD_CLK=P2^2;	//LCD串行同步时钟,上升沿时读取SID数据.
 sbit  LCD_BC=P2^3;
//函数声明:
void LCD_SEND(unsigned char sData);
unsigned char LCD_RECEIVE();
unsigned char LCD_READ_BF();
void LCD_WRITE_IR(unsigned char LCD_COMMAND);
void LCD_WRITE_DR(unsigned char LCD_DATA);
 
/**********************************************************
*LCD初始化函数
**********************************************************/
void LCD_INIT()
{
LCD_BC=0;
 LCD_WRITE_IR(0x30); //功能设置-基本指令集
 LCD_WRITE_IR(0x0c); //显示打开,光标关,反白显示关
 LCD_WRITE_IR(0x01); //清屏
 LCD_WRITE_IR(0x06); //进入点设定,光标向右移动,AC+1
}
/**********************************************************
*写指令到LCD函数
*名称:LCD_WRITE_IR
*入口参数:LCD_COMMAND (字符型--8位数据)
**********************************************************/
void LCD_WRITE_IR(unsigned char LCD_COMMAND)
{
 unsigned char tData; 
 LCD_CS = 0;				//拉低片选终止所有数据传送
 	//判忙
 do{tData = LCD_READ_BF();}
 while(tData&0x80==0x80);
 LCD_CS = 1;				//拉高片选准备传送数据
 LCD_SEND(0xf8); 			//发送写指令
 tData = LCD_COMMAND;
 tData = tData&0xf0;//屏蔽低4位(取出高4位) 
 LCD_SEND(tData); 		//发送高4位
 tData = LCD_COMMAND;
 tData = tData<<4;	//取低4位
 LCD_SEND(tData);		//发送低4位
 LCD_CS = 0;				//拉低片选终止所有数据传送
}
/**********************************************************
*写数据到LCD函数
*名称:LCD_WRITE_DR
*入口参数:LCD_DATA (字符型-8位)
**********************************************************/
void LCD_WRITE_DR(unsigned char LCD_DATA)
{
 unsigned char tData=0;  
 LCD_CS = 0;  			//拉低片选终止所有数据传送
 	//判忙
 do{tData = LCD_READ_BF();}
 while(tData&0x80==0x80);
 LCD_CS = 1;			//拉高片选准备传送数据
 LCD_SEND(0xfa); 		//发送写数据指令
 tData = LCD_DATA;
 tData = tData&0xf0;//屏蔽低4位(取出高4位) 
 LCD_SEND(tData); 	//发送高4位
 tData = LCD_DATA;
 tData = tData<<4;	//取低4位
 LCD_SEND(tData);		//发送低4位
 LCD_CS = 0;			//拉低片选终止所有数据传送
}
/**********************************************************
*读取LCD忙碌标志(BF)和地址计数器(AC)的状态
*名称:LCD_READ_BF
**********************************************************/
unsigned char LCD_READ_BF()
{
 unsigned char i,tData=0; 
 LCD_CS = 1;				//拉高片选准备传送数据
 LCD_SEND(0xfc); 			//发送读数据指令
 i = LCD_RECEIVE();			//读取数据高4位
 tData = LCD_RECEIVE();	   //读取数据低4位
 LCD_CS = 0;				//拉低片选终止所有数据传送
 i = i&0xf0;				//屏蔽低4位
 tData = tData>>4;
 tData = tData&0x0f;//屏蔽高4位
 tData = tData|i; 	//组合成完整数据
 return (tData);
}

/**********************************************************
*LCD发送8BIT的数据(1个字节)
*名称:LCD_SEND
*入口:
**********************************************************/
void LCD_SEND(unsigned char sData)
{
 unsigned char i;
 for (i=8;i>0;i--)
 	{
	 LCD_CLK = 0;
	 LCD_SID = sData&0x80;//发送一位数据
	 LCD_CLK = 1;		//拉高串行时钟端,锁存数据
	 sData <<= 1;
	}
}
/**********************************************************
*LCD接收8BIT数据(1个字节)
*名称:LCD_RECEIVE
*出口:rData
**********************************************************/
unsigned char LCD_RECEIVE()
{
 unsigned char i,rData=0;
 for (i=8;i>0;i--)
 	{
	 LCD_CLK = 0;
	 rData <<=1;
	 LCD_CLK = 1;
	 rData |= LCD_SID;
 	}
	return(rData);
}

