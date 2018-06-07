#include <reg52.h>
#include "LCD1602.h"

//短延时函数：ShortDelay()
void ShortDelay(uchar i)
{
	for(;i>0;i--) ;
	}
	
//长延时函数：LongDelay()
void LongDelay(uint i)
{
	uint j;
	for(;i>0;i--)
		{ for(j=1000;j>0;j--);}
	}
	
//寄存器选择信号：SetRS()
void  SetRS(bit i)
{
	if(i==1) PinRS = 1;
	else     PinRS = 0;
	}

//读写操作控制信号：SetRW()
void SetRW(bit i)
{
	if(i==1) PinRW = 1;
	else     PinRW = 0;
	}

//使能信号：SetE()
void SetE(bit i)
{
	if(i==1) PinE = 1;
	else     PinE = 0;
	}	
	
//IO输入输出控制
void InitP0(bit i) 
{
	if(i==1) P0 = 0xff;
	else     P0 = 0x00;
	} 
		
//写指令函数：WriteInstruc()
void WriteInstruc(uint Instruc)
{
	while(StatusCheck());
	InitP0(0);
	SetRS(0);
	SetRW(0);
	//ShortDelay(1);
	SetE(0);
	PinData = Instruc;
	//ShortDelay(1);
	SetE(1);
	ShortDelay(10);
	SetE(0);
	//ShortDelay(1);
	SetRW(1);
	SetRS(1);
	}
	
//读BF以及AC的值函数：Read_BF_AC()
uint Read_BF_AC()
{
	uint temp;
	InitP0(1);
	SetRS(0);
	SetRW(1);
	//ShortDelay(1);
	SetE(0);
	//ShortDelay(1);
	SetE(1);
	ShortDelay(1);
	temp = PinData;
	ShortDelay(10);
	SetE(0);
	//ShortDelay(1);
	SetRW(0);
	SetRS(1);
	return(temp);
	}
	
//写数据到RAM函数：WriteData()
void WriteData(uint data1)
{
	InitP0(0);
	SetRS(1);
	SetRW(0);
	//ShortDelay(1);
	SetE(0);
	PinData = data1;
	//ShortDelay(1);
	SetE(1);
	ShortDelay(10);
	SetE(0);
	//ShortDelay(1);
	SetRW(1);
	SetRS(0);
	}
	
//从RAM读数据函数：ReadData()
uint ReadData(void)
{
	uint temp;
	InitP0(1);
	SetRS(1);
	SetRW(1);
	//ShortDelay(1);
	SetE(0);
	//ShortDelay(1);
	SetE(1);
	ShortDelay(1);
	temp = PinData;
	ShortDelay(10);
	SetE(0);
	//ShortDelay(1);
	SetRW(0);
	SetRS(0);
	return(temp);
	}

//检测LCD控制器状态函数：StatusCheck()
//返回一bit数：1 —— 忙；0 —— 闲
bit StatusCheck()
{
	return((bit)(Read_BF_AC() & 0x80));
	}

//在指定位置显示字符函数: DispCharacter()
//  x为行号，y为列号，        
//  data为显示字符的码字数据   
void DispCharacter(uint x, uint y, uint data1)
{
	uint temp;       
	while(StatusCheck());  //若LCD控制器忙，则等待
	
	temp = y & 0x0f;
	x &= 0x01;
	if(x) temp |= 0x40;
	SetDDRAM_Add(temp);	  //设置显示位置
	
	WriteData(data1);	
	}

//LCD复位函数：LCDReset()
void LCDReset()
{
	ClearScreen();
	CursorReturn();
	}