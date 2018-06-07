#ifndef  _LCD1602_H_
#define  _LCD1602_H_

#define  uint  unsigned int
#define  uchar unsigned char

sbit PinRS   = P2^0;
sbit PinRW   = P2^1;
sbit PinE    = P2^2;

#define  PinData  P0

/*************  ����ʱ  *************/
void ShortDelay(uchar i);

/*************  ����ʱ  *************/
void LongDelay(uint i);

/********** �Ĵ���ѡ���ź� **********/
/******* 1 ���� ���ݣ�0 ���� ָ�� *******/
void  SetRS(bit i);

/********* ��д���������ź� *********/
/********* 1 ���� ����0 ���� д *********/
void SetRW(bit i);

/************* ʹ���ź� *************/
/******* 1 ���� ��Ч��0 ���� ��Ч *******/
void SetE(bit i);

/********  IO�����������  **********/
void InitP0(bit i);
	
/********  �������ܣ�дָ��  ********/
/*        InstrucΪ8λ��ָ��        */
void WriteInstruc(uint Instruc);

/*********  �������ܣ�����  *********/
#define  ClearScreen()			WriteInstruc(0x01)

/*********  �������ܣ���λ  *********/
#define  CursorReturn()			WriteInstruc(0x02)

/*****  �������ܣ����뷽ʽ����  *****/
/*˵�������ù���ƶ������Լ��Ƿ��ƶ�*/
/*          tempȡֵ���£�          */
/*      0x04 : ������ʽ������λ     */
/*      0x05 ��������ʽ����λ       */
/*      0x06 : ������ʽ������λ     */
/*      0x07 ��������ʽ����λ       */
#define  InputMode(temp)		WriteInstruc(temp)

/*****  �������ܣ���ʾ���ؿ���  *****/
/* ˵����������ʾ��/�أ�            */
/*       ���ù�꿪/�أ�            */
/*       ���ù������λ����˸���   */
/*          tempȡֵ���£�          */
/*   0x08 : ��ʾ�أ����أ���˸��  */
/*   0x0c ����ʾ�������أ���˸��  */
/*   0x0d : ��ʾ�������أ���˸��  */
/*   0x0e : ��ʾ������꿪����˸��  */
/*   0x0f : ��ʾ������꿪����˸��  */
#define  DispControl(temp)		WriteInstruc(temp)

/***   �������ܣ���ꡢ������λ  ****/
/* ˵��������Լ�������λ           */
/*         tempȡֵ���£�           */
/*      0x10 ���������             */
/*      0x14 ���������             */
/*      0x18 ����ʾ��������         */
/*      0x1c : ��ʾ��������         */
#define  DispShift(temp)		WriteInstruc(temp)

/*******  �������ܣ���������  *******/
/* ˵�������ýӿ�����λ����         */
/*       ������ʾ������             */
/*       ������ʾ�ַ�����           */
/*         tempȡֵ���£�           */
/*      0x20 �� 4λ��1�У�5��7       */
/*      0x24 �� 4λ��1�У�5��10      */
/*      0x28 �� 4λ��2�У�5��7       */
/*      0x2c �� 4λ��2�У�5��10      */
/*      0x30 �� 8λ��1�У�5��7       */
/*      0x34 �� 8λ��1�У�5��10      */
/*      0x38 �� 8λ��2�У�5��7       */
/*      0x3c �� 8λ��2�У�5��10      */
#define  FunctionSet(temp)	WriteInstruc(temp)	

/****  �������ܣ�CGRAM��ַ����  *****/
/* ˵��������CGRAM��ַ              */
/*       AddressΪCGRAM��ַ         */
#define  SetCGRAM_Add(Address)	WriteInstruc(0x40 | Address)

/****  �������ܣ�DDRAM��ַ����  *****/
/* ˵��������DDRAM��ַ              */
/*       AddressΪDDRAM��ַ         */
#define  SetDDRAM_Add(Address)	WriteInstruc(0x80 | Address)

/*****  �������ܣ���BF�Լ�ACֵ  *****/
/* ˵������ȡBF�Լ�ACֵ��           */
/*       �������λΪBF����7λΪAC  */
uint Read_BF_AC(void);

/******  �������ܣ�д���ݵ�RAM  *****/
void WriteData(uint data1);

/******  �������ܣ���RAM������  *****/
uint ReadData(void);


/** �������ܣ����LCD������״̬���� */
/*  ����һbit����1 ���� æ��0 ���� ��   */
bit StatusCheck();

/**  �������ܣ���ָ��λ����ʾ�ַ�  **/
/* ˵����xΪ�к�(0 ~ 1)             */
/*       yΪ�к�(0 ~ 15)            */
/*       dataΪ��ʾ�ַ�����������   */
void DispCharacter(uint x, uint y, uint data1);

/********  �������ܣ�LCD��λ  *******/
void LCDReset();

#endif   //_LCD1602_H_