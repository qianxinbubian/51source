#include <reg52.h>
#include "LCD1602.h"

//����ʱ������ShortDelay()
void ShortDelay(uchar i)
{
	for(;i>0;i--) ;
	}
	
//����ʱ������LongDelay()
void LongDelay(uint i)
{
	uint j;
	for(;i>0;i--)
		{ for(j=1000;j>0;j--);}
	}
	
//�Ĵ���ѡ���źţ�SetRS()
void  SetRS(bit i)
{
	if(i==1) PinRS = 1;
	else     PinRS = 0;
	}

//��д���������źţ�SetRW()
void SetRW(bit i)
{
	if(i==1) PinRW = 1;
	else     PinRW = 0;
	}

//ʹ���źţ�SetE()
void SetE(bit i)
{
	if(i==1) PinE = 1;
	else     PinE = 0;
	}	
	
//IO�����������
void InitP0(bit i) 
{
	if(i==1) P0 = 0xff;
	else     P0 = 0x00;
	} 
		
//дָ�����WriteInstruc()
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
	
//��BF�Լ�AC��ֵ������Read_BF_AC()
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
	
//д���ݵ�RAM������WriteData()
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
	
//��RAM�����ݺ�����ReadData()
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

//���LCD������״̬������StatusCheck()
//����һbit����1 ���� æ��0 ���� ��
bit StatusCheck()
{
	return((bit)(Read_BF_AC() & 0x80));
	}

//��ָ��λ����ʾ�ַ�����: DispCharacter()
//  xΪ�кţ�yΪ�кţ�        
//  dataΪ��ʾ�ַ�����������   
void DispCharacter(uint x, uint y, uint data1)
{
	uint temp;       
	while(StatusCheck());  //��LCD������æ����ȴ�
	
	temp = y & 0x0f;
	x &= 0x01;
	if(x) temp |= 0x40;
	SetDDRAM_Add(temp);	  //������ʾλ��
	
	WriteData(data1);	
	}

//LCD��λ������LCDReset()
void LCDReset()
{
	ClearScreen();
	CursorReturn();
	}