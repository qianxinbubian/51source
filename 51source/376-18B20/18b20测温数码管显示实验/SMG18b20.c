
//DS18B20的读写程序,数据脚P3.3                     //
//温度传感器18B20汇编程序,采用器件默认的12位转化   //
//最大转化时间750微秒,显示温度-55到+125度,显示精度 //
//为0.1度，显示采用4位LED共阳显示测温值            //
//P0口为段码输入,P24~P27为位选                       //
/***************************************************/

#include "reg51.h"
#include "intrins.h"                            //_nop_();延时函数用
#define  Disdata P0                             //段码输出口
#define  discan  P2                             //扫描口
#define  uchar unsigned char
#define  uint  unsigned int
sbit     DQ=P3^3;                               //温度输入口
sbit     DIN=P0^7;                              //LED小数点控制
uint     h; 
uchar flag;
//**************温度小数部分用查表法***********//
uchar code ditab[16]=
{0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};
//
uchar code dis_7[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
//共阳LED段码表        "0"  "1"  "2"  "3"  "4"  "5"  "6"  "7"  "8"  "9" "不亮" "-"              
uchar code scan_con[4]={0x7f,0xbf,0xdf,0xef};    //列扫描控制字
uchar data temp_data[2]={0x00,0x00};             //读出温度暂放
uchar data display[5]={0x00,0x00,0x00,0x00,0x00};     //显示单元数据，共4个数据和一个运算暂用

//
//
//
/***********11微秒延时函数**********/
//
void delay(uint t)
{
for(;t>0;t--);
}
//
/***********显示扫描函数**********/
scan()
{
char k;
    for(k=0;k<4;k++)         //四位LED扫描控制
     {
	 Disdata=0xff;
      Disdata=dis_7[display[k]];
      if(k==1){DIN=0;}
      discan=scan_con[k];delay(90);
	  discan=0xff;
     }
}
//
//
/***********18B20复位函数**********/
ow_reset(void)
{
char presence=1;
while(presence)
{
while(presence)
{
DQ=1;_nop_();_nop_();
DQ=0;    //
delay(50); // 550us
DQ=1;    // 
delay(6); // 66us
presence=DQ; // presence=0继续下一步
   }
delay(45);    //延时500us
presence = ~DQ;
}
DQ=1;
} 
//
//
/**********18B20写命令函数*********/
//向 1-WIRE 总线上写一个字节
void write_byte(uchar val)
{
uchar i;
for (i=8; i>0; i--) //
{
DQ=1;_nop_();_nop_(); 
DQ = 0;_nop_();_nop_();_nop_();_nop_();_nop_();//5us
DQ = val&0x01;      //最低位移出
delay(6);           //66us
val=val/2;          //右移一位
}
DQ = 1;
delay(1); 
}
//
/*********18B20读1个字节函数********/
//从总线上读取一个字节
uchar read_byte(void)
{
uchar i;
uchar value = 0;
for (i=8;i>0;i--)
{
DQ=1;_nop_();_nop_();
value>>=1;
DQ = 0;             //
_nop_();_nop_();_nop_();_nop_();   //4us
DQ = 1;_nop_();_nop_();_nop_();_nop_();   //4us 
if(DQ)value|=0x80;
delay(6);           //66us
}
DQ=1;
return(value);
}
//
/***********读出温度函数**********/
//
read_temp()
{
ow_reset();       //总线复位
write_byte(0xCC); // 发Skip ROM命令
write_byte(0xBE); // 发读命令
temp_data[0]=read_byte(); //温度低8位
temp_data[1]=read_byte(); //温度高8位
ow_reset();
write_byte(0xCC); // Skip ROM
write_byte(0x44); // 发转换命令
}
//
/***********温度数据处理函数**********/
void work_temp()
{
uchar n=0;
uchar doth,dotl;
uchar flag3=1,flag2=1;       //数字显示修正标记
if((temp_data[1]&0xf8)!=0x00)
{
   temp_data[1]=~(temp_data[1]);
   temp_data[0]=~(temp_data[0])+1;
   n=1;
   flag=1;
}//负温度求补码
if(temp_data[0]>255)
{
temp_data[1]++; 
}
display[4]=temp_data[0]&0x0f;
display[0]=ditab[display[4]];
doth=display[0]/10;
dotl=display[0]%10;
display[4]=((temp_data[0]&0xf0)>>4)|((temp_data[1]&0x07)<<4);
display[3]=display[4]/100;
display[2]=display[4]/10%10;
display[1]=display[4]%10;
if(!display[3])
   {
    display[3]=0x0a;
    flag3=0;
    if(!display[2])
    {
     display[2]=0x0a;
     flag2=0;
    }
   }//最高位为0时都不显示
if(n)
{
   display[3]=0x0b;//负温度时最高位显示"-"
   flag3=0;
}
}

// 
//
/**************主函数****************/
main()
{
Disdata=0xff;    //初始化端口
discan=0xff;
for(h=0;h<4;h++){display[h]=8;}//开机显示8888
ow_reset();       // 开机先转换一次
write_byte(0xCC); // Skip ROM
write_byte(0x44); // 发转换命令
for(h=0;h<500;h++)
   {scan();}          //开机显示"8888"2秒
while(1)
{
read_temp();         //读出18B20温度数据
work_temp();         //处理温度数据

   scan();        //显示温度值2秒
}
}
//
//*********************结束**************************//
