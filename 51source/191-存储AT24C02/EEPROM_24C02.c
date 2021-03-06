/********************************************************************
* 文件名  ： EEPROM_24C02.c
* 描述    :  该文件实现对24C02的操作。
		     确认试验是否成功：电源上电后，数码管的值在递增，观察值。关闭电源，待几秒后上电，
		     数码管显示的值会从断电钱的那个值开始显示。
* 创建人  ： 东流，2009年4月9日
***********************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

sbit scl=P1^5;  //24c08 SCL
sbit sda=P3^6;  //24c08 SDA

uchar code table[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
uchar sec;      //定义计数值，每过1秒，sec加1 
uint write;		//写标志位

/********************************************************************
* 名称 : flash()
* 功能 : 延时,时间为2个NOP，大概为2US
* 输入 : 无
* 输出 : 无
***********************************************************************/
void flash(void) 
{
	_nop_();
	_nop_();
}

/********************************************************************
* 名称 : x24c02_init()
* 功能 : 24c02初始化子程序
* 输入 : 无
* 输出 : 无
***********************************************************************/
void x24c02_init(void) 
{
	scl = 1;
	flash();
	sda = 1;
	flash();
}

/********************************************************************
* 名称 : start(void)
* 功能 : 启动I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void start(void)
{
	scl = 1; 
	flash();
	sda = 1;
	flash(); 
	sda = 0; 
	flash(); 
	scl = 0; 
	flash();
}

/********************************************************************
* 名称 : stop()
* 功能 : 停止I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void stop() 
{
	scl = 0;
	flash();
	sda = 0; 
	flash();
	scl = 1;
	flash();
	sda = 1;
	flash();
}

/********************************************************************
* 名称 : writex()
* 功能 : 写一个字节
* 输入 : j（需要写入的值）
* 输出 : 无
***********************************************************************/
void writex(uchar j)
{  
	uchar i,temp;
	temp = j;
	for(i=0; i<8; i++)
	{
		scl = 0; 
		flash(); 
		sda = (bit)(temp & 0x80); 
		flash();
		scl = 1; 
		flash();
		temp = temp << 1; 
	}
	scl = 0;
	flash(); 
}

/********************************************************************
* 名称 : readx()
* 功能 : 读一个字节
* 输入 : 无
* 输出 : 读出的值
***********************************************************************/
uchar readx(void)
{
	uchar i, j, k = 0;
	for(i=0; i<8; i++)
	{
		scl = 0;
		flash();		
		if(sda == 1)
		{
			j = 1;
		}
		else j = 0;
		k = (k << 1) | j; 
		scl = 1;
		flash();
	} 
	return(k);
}

/********************************************************************
* 名称 : ack()
* 功能 : I2C总线时钟
* 输入 : 无
* 输出 : 无
***********************************************************************/
void ack(void)
{
	uchar i = 0;
	scl = 1;
	flash();
	while((sda == 1) && (i < 255)) 
	{
		i++;
	}
	scl = 0;
	flash();
}

/********************************************************************
* 名称 : x24c02_read()
* 功能 : 从24c02中读出值
* 输入 : address(要在这个地址读取值）
* 输出 : 从24c02中读出的值
***********************************************************************/
uchar x24c02_read(uchar address)
{
	uchar i;
	start();
	writex(0xa0);
	ack();
	writex(address);
	ack();
	start();
	writex(0xa1);
	ack();
	i = readx();
	stop();
	return(i);
}

/********************************************************************
* 名称 : x24c02_write()
* 功能 : 想24c02中写入数据
* 输入 : address(地址） ， info（值）
* 输出 : 无
***********************************************************************/
void x24c02_write(uchar address, uchar info)
{
	start();
	writex(0xa0);
	ack();
	writex(address);
	ack();
	writex(info);
	ack();
	stop();
}

/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时,延时时间为 1ms * i
* 输入 : i(延时1ms的个数）
* 输出 : 无
***********************************************************************/
void Delay_1ms(uint i)
{
	uchar x, j;
	for(j=0; j<i; j++)
	for(x=0; x<=148; x++)
	;	
}

/********************************************************************
* 名称 : LED()
* 功能 : 显示
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LED()                  //LED显示函数
{
	P2 = 6; 
	P0 = table[sec / 10]; 
	Delay_1ms(5);
	P2 = 7;
	P0 = table[sec % 10];
	Delay_1ms(5);
}

/********************************************************************
* 名称 : time0()
* 功能 : 定时中断函数，每秒中sec加一，并且写标识write使能
* 输入 : del
* 输出 : 无
***********************************************************************/
void time0(void) interrupt 1 using 3  //定时中断服务函数
{
	static uchar Count = 0; 
	TH0 = 0x4c; //对TH0 TL0赋值
	TL0 = 0x00; //重装计数初值
	Count++;        
	if(Count == 20)  //计满20次（1秒）时
	{ 
		Count = 0;   //重新再计
		sec++;
		write = 1;   //1秒写一次24C08
		if(sec == 100) //定时100秒，在从零开始计时
		{
			sec = 0;
		} 
	} 
}

/********************************************************************
* 名称 : Time0_Init()
* 功能 : 定时器0的初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Time0_Init(void)
{
	TMOD = 0x01;  			//定时器工作在方式1
	ET0 = 1;
	EA = 1; 
	TH0 = 0x4c; 	//对TH0 TL0赋值
	TL0 = 0x00; 	//使定时器0.05秒中断一次
	TR0 = 1;           				//开始计时
}

/********************************************************************
* 名称 : Main()
* 功能 : 主函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Main(void) 
{
	x24c02_init();        	//初始化24C02
	sec = x24c02_read(2);	//读出保存的数据赋于sec	
	Time0_Init();
	while(1) 
	{
 		LED();
		if(write == 1) 			   //判断计时器是否计时一秒
    	{
			write =0;              //清零 
			x24c02_write(2,sec);   //在24c08的地址2中写入数据sec
		}
	}
}
