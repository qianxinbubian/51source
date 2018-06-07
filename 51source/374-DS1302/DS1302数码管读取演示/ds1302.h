#ifndef _DS1302
#define _DS1302
#define uchar unsigned char
//寄存器宏定义
#define WRITE_SECOND              	0x80
#define WRITE_MINUTE        			0x82
#define WRITE_HOUR                  0x84
#define READ_SECOND                	0x81
#define READ_MINUTE                 0x83
#define READ_HOUR                  	0x85
#define WRITE_PROTECT             	0x8E
 
//位寻址寄存器定义
sbit ACC_7 = ACC^7;
sbit SCLK = P1^0;        // DS1302时钟信号         7脚
sbit DIO= P1^1;          // DS1302数据信号         6脚
sbit CE = P1^2;          // DS1302片选                 5脚
 
//地址、数据发送子程序
void Write1302 ( unsigned char addr,dat )    
{
	unsigned char i,temp;
	CE=0;         //CE引脚为低，数据传送中止
	SCLK=0;       //清零时钟总线
	CE = 1;       //CE引脚为高，逻辑控制有效
	//发送地址
	for ( i=8; i>0; i-- ) //循环8次移位
	{    
		SCLK = 0;
		temp = addr;
		DIO = (bit)(temp&0x01); //每次传输低字节
		addr >>= 1;             //右移一位
		SCLK = 1;
	}
	
	//发送数据
	for ( i=8; i>0; i-- )
	{    
	     SCLK = 0;
	     temp = dat;
	     DIO = (bit)(temp&0x01);         
	     dat >>= 1;                  
	     SCLK = 1;
	}
	SCLK=1;
	CE = 0;        
}
 
//数据读取子程序
unsigned char Read1302 ( unsigned char addr )
{
       unsigned char i,temp,dat1,dat2;
       CE=0;          
       SCLK=0;            
       CE = 1; 
       //发送地址
       for ( i=8; i>0; i-- )                      //循环8次移位
       {    
              SCLK = 0;
              temp = addr;
              DIO = (bit)(temp&0x01);          //每次传输低字节
              addr >>= 1;                              //右移一位
              SCLK = 1;
       }
       //读取数据
       for ( i=8; i>0; i-- )
       {
              ACC_7=DIO;
              SCLK = 1;
             ACC>>=1;
              SCLK = 0;
       }
		SCLK=1;    
       CE=0;
       dat1=ACC;
       dat2=dat1/16;                           //数据进制转换
       dat1=dat1%16;                         //十六进制转十进制
       dat1=dat1+dat2*10;
       return (dat1);
}
 
//初始化DS1302(选择性使用)
void init1302(void)   
{
	
	Write1302 (WRITE_PROTECT,0X00);  //禁止写保护
	Write1302 (WRITE_SECOND,0x50);   //秒位初始化
	Write1302 (WRITE_MINUTE,0x59);   //分钟初始化
	Write1302 (WRITE_HOUR,0x23);     //小时初始化
	//Write1302 (0x80,0x80);      //时钟暂停
	//Write1302 (0x86,0x13);		//日初始化
	//Write1302 (0x88,0x03);		//月初始化
	//Write1302 (0x8c,0x09);		//年初始化
	Write1302 (WRITE_PROTECT,0x80);  //允许写保护
}
#endif

/*读程序Read1302（0x81）
0x81,0x83,0x85,0x87,0x89,0x8d
秒    分   时   日   月   年
*/
/*Write1302 (0x80,0x80);      //时钟暂停
Read1302()读出来的数是十进制数，而Write1302()写进的数必须是16进制的
*/


