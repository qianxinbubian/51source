#include <reg52.h>
#include <intrins.h>

sbit DQ   = P3^3;       //定义DS18B20端口DQ
sbit BEEP = P3^6;       //定义蜂鸣器控制端口

bit presence;           //检测DS18B20标志位

unsigned char code LEDData[] =
{
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 
  0xF8, 0x80, 0x90, 0xff, 0xc6, 0x9c, 0xbf
};

unsigned char  shift,n = 0;

unsigned char data temp_data[2]; //温度数据存放单元
unsigned char data display[7];   //显存单元


/**********************************************************

us延时子函数  (8*1.085)*num

**********************************************************/
void Delay(unsigned int num) //延时函数
{
   while (--num)
    ;
}

/**********************************************************

ms延时子函数

**********************************************************/
void delayms(unsigned int ms)
{
   unsigned char k;
   while (ms--)
   {
     for (k = 0; k < 114; k++)
      ;
   }
}

/**********************************************************

蜂鸣器驱动子函数

**********************************************************/
void beep()
{
   unsigned char i;
   for (i = 0; i < 180; i++)
   {
     Delay(80);
     BEEP = !BEEP;    //BEEP取反
   }
   BEEP = 1;          //关闭蜂鸣器
   delayms(100);      //延时100ms
}

/**********************************************************

DS18B20初始化子函数

presence=0  OK  presence=1  ERROR

**********************************************************/
unsigned char Init_DS18B20(void)
{
   DQ = 0;            //单片机发出低电平复位信号
   Delay(60);         //延时>480us
   DQ = 1;            //释放数据线
   Delay(8);          //延时>64us,等待应答

   presence = DQ;     //接收应答信号
   Delay(50);         //延时>400us,等待数据线出现高电平
   DQ = 1;            //释放数据线
   
   return (presence); //返回presence信号
}

/**********************************************************

读一个字节数据子函数

**********************************************************/
unsigned char ReadOneChar(void)
{
  unsigned char i;
  unsigned char dat = 0;

  EA = 0;                  //读取温度数据时，要关闭中断。
  DQ = 1;
  for (i = 0; i < 8; i++)
  {
     DQ = 0;                //给低脉冲信号
     dat >>= 1;
     DQ = 1;                //释放总线
     _nop_();
     _nop_();
     if(DQ)               //读总线电平状态
     dat |= 0x80;          //最高位置1
     Delay(6);             //延时>45us
     DQ = 1;               //释放总线,表示此次读操作完成
   }
   EA = 1;                 //开中断
   return (dat);           //返回所读得数据
}

/**********************************************************

写一个字节数据子函数

**********************************************************/
void WriteOneChar(unsigned char dat)
{
   unsigned char i;
  
   for (i = 0; i < 8; i++)  //一个字节8个bit
   {
     DQ = 0;                //给低脉冲信号
     Delay(1);              //延时<15us
     DQ = (bit)(dat&0x01);  //写1bit数据
     dat >>= 1;             //数据右移一位
     Delay(6);              //延时>45us
     DQ = 1;                //释放总线,表示此次写操作完成
   }
}

/**********************************************************

温度数据转换子函数

**********************************************************/
void Temperature_conver()
{
   bit minus = 0;

   display[0]=0x0b;                //显示 C
   display[1]=0x0c;                //显示 ° 

   if (temp_data[1] > 127)         //温度为负值
   {
     temp_data[0] = (~temp_data[0]) + 1; //取反加一，将补码变成原码
     if ((~temp_data[0]) >= 0xff)
      temp_data[1] = (~temp_data[1]) + 1;
     else
      temp_data[1] = ~temp_data[1];
     minus = 1;                         //温度为负值标志
   }

   display[6] = temp_data[0] &0x0f;     //取小数位数据
   display[2] = (display[6] *10) / 16;  //保留一位小数

   display[6] = ((temp_data[0] &0xf0) >> 4)|((temp_data[1] &0x0f) << 4); //取整数
    
   display[5] = display[6] / 100;        //百位
   display[4] = (display[6] % 100) / 10; //十位
   display[3] = display[6] % 10;         //个位

   if (!display[5])
   {
     display[5] = 0x0a;   //高位为0,不显示
     if (!display[4])
     display[4] = 0x0a;   //次高位为0,不显示
   }

   if (minus)
   {
     display[5] = 0x0d;   //显示负号
   }
}

/**********************************************************

主函数

**********************************************************/
void main(void)
{
   P0 = 0xff;
   P2 = 0xff;
  
   T2CON = 0x00;           //设置T2CON寄存器       
   TH2 = 0xfc;             //1ms定时常数
   TL2 = 0x66;
   ET2 = 1;                //允许Timer2中断
   EA = 1;                 //总中断允许
   TR2 = 1;                //启动定时器2
   shift = 0x7f;           //送位码初值

   while (1)
   {
    Init_DS18B20();
    if (presence == 0)
    {
      WriteOneChar(0xCC);  //跳过ROM匹配操作
      WriteOneChar(0x44);  //启动温度转换
      delayms(2);
    }

    Init_DS18B20();
    if (presence == 0)
    {
      WriteOneChar(0xCC);  //跳过ROM匹配操作
      WriteOneChar(0xBE);  //读取温度寄存器

      temp_data[0] = ReadOneChar(); //读温度低8位
      temp_data[1] = ReadOneChar(); //读温度高8位
      
      Temperature_conver();         //温度数据转换
      delayms(200);
    }
    else
    {
      beep(); //蜂鸣器报警    
    }
   }
}

/**********************************************************

  Timer2中断服务子函数 （负责数码管显示）

**********************************************************/
void  timer2(void) interrupt 5 
{ 
   TF2 = 0;                    //手工清中断标志  
   TH2 = 0xfc;                 //1ms定时常数
   TL2 = 0x66;

   if (presence)               //如果DS18B20不存在
   {
     P2 = 0xff;                //关闭显示
     P0 = 0xff;
     return;                   //退出中断 
   }

   if(n >= 6)
   {
     n = 0;                    //中断计数单元清零
     shift = 0x7f;             //赋位码初值
     P2 = 0xff;                //关闭数码管显示 
   }
   else
   {
      if (n == 3)
      P0 = (LEDData[display[n]]) &0x7f; //加小数点显示    
      else
      P0 = LEDData[display[n]]; //送段码

      P2 = shift;               //送位码
      shift = shift>>1|0x80;    //修改位码
      n++;                      //计数值加1
   }
}

/*********************************************************/
