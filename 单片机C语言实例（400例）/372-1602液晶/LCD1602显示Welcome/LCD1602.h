#ifndef  _LCD1602_H_
#define  _LCD1602_H_

#define  uint  unsigned int
#define  uchar unsigned char

sbit PinRS   = P2^0;
sbit PinRW   = P2^1;
sbit PinE    = P2^2;

#define  PinData  P0

/*************  短延时  *************/
void ShortDelay(uchar i);

/*************  长延时  *************/
void LongDelay(uint i);

/********** 寄存器选择信号 **********/
/******* 1 —— 数据；0 —— 指令 *******/
void  SetRS(bit i);

/********* 读写操作控制信号 *********/
/********* 1 —— 读；0 —— 写 *********/
void SetRW(bit i);

/************* 使能信号 *************/
/******* 1 —— 有效；0 —— 无效 *******/
void SetE(bit i);

/********  IO输入输出控制  **********/
void InitP0(bit i);
	
/********  函数功能：写指令  ********/
/*        Instruc为8位的指令        */
void WriteInstruc(uint Instruc);

/*********  函数功能：清屏  *********/
#define  ClearScreen()			WriteInstruc(0x01)

/*********  函数功能：归位  *********/
#define  CursorReturn()			WriteInstruc(0x02)

/*****  函数功能：输入方式设置  *****/
/*说明：设置光标移动方向以及是否移动*/
/*          temp取值如下：          */
/*      0x04 : 减量方式，不移位     */
/*      0x05 ：减量方式，移位       */
/*      0x06 : 增量方式，不移位     */
/*      0x07 ：增量方式，移位       */
#define  InputMode(temp)		WriteInstruc(temp)

/*****  函数功能：显示开关控制  *****/
/* 说明：设置显示开/关，            */
/*       设置光标开/关，            */
/*       设置光标所在位置闪烁与否   */
/*          temp取值如下：          */
/*   0x08 : 显示关，光标关，闪烁关  */
/*   0x0c ：显示开，光标关，闪烁关  */
/*   0x0d : 显示开，光标关，闪烁开  */
/*   0x0e : 显示开，光标开，闪烁关  */
/*   0x0f : 显示开，光标开，闪烁开  */
#define  DispControl(temp)		WriteInstruc(temp)

/***   函数功能：光标、画面移位  ****/
/* 说明：光标以及画面移位           */
/*         temp取值如下：           */
/*      0x10 ：光标左移             */
/*      0x14 ：光标右移             */
/*      0x18 ：显示整体左移         */
/*      0x1c : 显示整体右移         */
#define  DispShift(temp)		WriteInstruc(temp)

/*******  函数功能：功能设置  *******/
/* 说明：设置接口数据位数，         */
/*       设置显示行数，             */
/*       设置显示字符字体           */
/*         temp取值如下：           */
/*      0x20 ： 4位，1行，5×7       */
/*      0x24 ： 4位，1行，5×10      */
/*      0x28 ： 4位，2行，5×7       */
/*      0x2c ： 4位，2行，5×10      */
/*      0x30 ： 8位，1行，5×7       */
/*      0x34 ： 8位，1行，5×10      */
/*      0x38 ： 8位，2行，5×7       */
/*      0x3c ： 8位，2行，5×10      */
#define  FunctionSet(temp)	WriteInstruc(temp)	

/****  函数功能：CGRAM地址设置  *****/
/* 说明：设置CGRAM地址              */
/*       Address为CGRAM地址         */
#define  SetCGRAM_Add(Address)	WriteInstruc(0x40 | Address)

/****  函数功能：DDRAM地址设置  *****/
/* 说明：设置DDRAM地址              */
/*       Address为DDRAM地址         */
#define  SetDDRAM_Add(Address)	WriteInstruc(0x80 | Address)

/*****  函数功能：读BF以及AC值  *****/
/* 说明：读取BF以及AC值，           */
/*       其中最高位为BF，低7位为AC  */
uint Read_BF_AC(void);

/******  函数功能：写数据到RAM  *****/
void WriteData(uint data1);

/******  函数功能：从RAM读数据  *****/
uint ReadData(void);


/** 函数功能：检测LCD控制器状态函数 */
/*  返回一bit数：1 —— 忙；0 —— 闲   */
bit StatusCheck();

/**  函数功能：在指定位置显示字符  **/
/* 说明：x为行号(0 ~ 1)             */
/*       y为列号(0 ~ 15)            */
/*       data为显示字符的码字数据   */
void DispCharacter(uint x, uint y, uint data1);

/********  函数功能：LCD复位  *******/
void LCDReset();

#endif   //_LCD1602_H_