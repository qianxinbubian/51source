#include <reg52.h>
#include "temp.h"
/**********************************************************
*采用DS1820+YM12864R精确到4位小数的温度计
*还请高手指点!
*编程:飞扬
*版本:V0.3
*DS1820设置为12位精度
**********************************************************/
//声明外部温度读数数组
extern unsigned char data temperature[2];
//存放分解的7个ASCII码温度数据  
unsigned char temp[7];

unsigned char code string[6]={"温度:"};
unsigned char code DispStr[7]={"系列号"};
unsigned char fCode[8]; //存放序列号,CRC 
/**********************************************************
*主程序
**********************************************************/
void main()
{

	unsigned char i;
	delay100us();//等待电源稳定,液晶复位完成
	delay100us();
	LCD_INIT();	 //液晶初始化
	DS1820_Reset();//DS1820复位
	DS1820_WriteData(0x33);//读ROM指令
	for (i=0;i<8;i++)
		{//读取64位系列号
		fCode[i] = DS1820_ReadData();
		}
	DS1820_Reset();//DS1820复位
	LCD_WRITE_IR(0x80);//在第一行显示
	for (i=0;i<5;i++)
		{
		 LCD_WRITE_DR(string[i]);//显示"温度:"
		}
	LCD_WRITE_IR(0x88);//在第3行显示
	for (i=0;i<6;i++)
		{
		 LCD_WRITE_DR(DispStr[i]);//显示"系列号"
		}
	DispCode();//在第4行显示序列号
	do{
		read_temp(); //读取温度值
		delay100us();
		display();	 //显示温度值
		delay100us();
	 }while (1);	//无限循环
}
/**********************************************************
*系列号显示子程序
*显示顺序(从左到右)8位CRC编码->48位序列号->8位产品系列编码
**********************************************************/
void DispCode()
{
 unsigned char i,temp;
 LCD_WRITE_IR(0x98); //在第4行显示
 for (i=8;i>0;i--)
 	{
	//显示高4位
	temp = fCode[i-1]>>4;
		if (temp<10) //ASCII码转换
			 LCD_WRITE_DR(temp += 0x30);
		else
			 LCD_WRITE_DR(temp += 0x37);
	//显示低4位
 	temp = fCode[i-1]&0x0f;
		if (temp<10) //ASCII码转换
			 LCD_WRITE_DR(temp += 0x30);
		else
			 LCD_WRITE_DR(temp += 0x37);
	 }
}
/**********************************************************
*延时子程序
**********************************************************/
void delay100us()
{
 unsigned char i;
 for (i=49;i>0;i--);
}
/**********************************************************
*转换显示子程序
**********************************************************/
void display()
{
 unsigned char temp_data,temp_data_2;
 unsigned int TempDec;	//用来存放4位小数
 temp_data = temperature[1];
 temp_data &= 0xf0; 	//取高4位
 if (temp_data==0xf0)	//判断是正温度还是负温度读数
 	{
	//负温度读数求补,取反加1,判断低8位是否有进位
		if (temperature[0]==0)
			{	//有进位,高8位取反加1
			 	temperature[0]=~temperature[0]+1;
				temperature[1]=~temperature[1]+1;
			}
		else
			{	//没进位,高8位不加1
				temperature[0]=~temperature[0]+1;
				temperature[1]=~temperature[1];
			}
	 }
	temp_data = temperature[1]<<4;		//取高字节低4位(温度读数高4位)
	temp_data_2 = temperature[0]>>4;	//取低字节高4位(温度读数低4位)
	temp_data = temp_data|temp_data_2;	//组合成完整数据
	temp[0] = temp_data/100+0x30; 		//取百位转换为ASCII码
	temp[1] = (temp_data%100)/10+0x30; 	//取十位转换为ASCII码
	temp[2] = (temp_data%100)%10+0x30; 	//取个位转换为ASCII码
	temperature[0]&=0x0f; 				//取小数位转换为ASCII码
	TempDec = ((temperature[0]&0x0f)*0.0625)*10000;//小数部分,扩大1万倍
	temp[3]	= TempDec/1000+0x30; 		//取小数个位转换为ASCII码
	temp[4]	= (TempDec%1000)/100+0x30; 	//取小数十位转换为ASCII码
	temp[5]	= ((TempDec%1000)%100)/10+0x30;//取小数百位转换为ASCII码
	temp[6] = ((TempDec%1000)%100)%10+0x30;//取小数千位转换为ASCII码
	LCD_WRITE_IR(0x91);//在LCD第2行显示
	LCD_WRITE_DR(temp[0]); 	//显示百位
	LCD_WRITE_DR(temp[1]); 	//显示十位
	LCD_WRITE_DR(temp[2]); 	//显示个位
	LCD_WRITE_DR(0x2e);		//显示"."
	LCD_WRITE_DR(temp[3]);	//显示小数位
	LCD_WRITE_DR(temp[4]);	//显示小数位
	LCD_WRITE_DR(temp[5]);	//显示小数位
	LCD_WRITE_DR(temp[6]);	//显示小数位
	LCD_WRITE_DR(0xa1);		//显示"℃"
	LCD_WRITE_DR(0xe6);
}
