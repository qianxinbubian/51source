#include <reg52.h>
#include <intrins.h>

unsigned char data dis_digit;
unsigned char key_s, key_v;

unsigned char code dis_code[11]={0xc0,0xf9,0xa4,0xb0, // 0, 1, 2, 3
    0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off 
unsigned char data dis_buf[8];
unsigned char data dis_index;
unsigned char hour,min,sec;
unsigned char sec100; 

sbit K1 = P3^2;
sbit K2 = P3^3;
bit scan_key();
void proc_key();
void inc_sec();
void inc_min();
void inc_hour();
void display();
void delayms(unsigned char ms);

void main(void)
{
 P0 = 0xff;
 P2 = 0xff;
 TMOD = 0x11;  // 定时器0, 1工作模式1, 16位定时方式
 TH1 = 0xdc;
 TL1 = 0;

 TH0 = 0xFC;
 TL0 = 0x17;
 
 hour = 12;
 min = 00;
 sec = 00;

 sec100 = 0;
 
 dis_buf[0] = dis_code[hour / 10];  // 时十位
 dis_buf[1] = dis_code[hour % 10];  // 时个位
 dis_buf[3] = dis_code[min / 10];  // 分十位
 dis_buf[4] = dis_code[min % 10];  // 分个位
 dis_buf[6] = dis_code[sec / 10];  // 秒十位
 dis_buf[7] = dis_code[sec % 10];  // 秒个位
 dis_buf[2] = 0xbf;      // 显示"-"
 dis_buf[5] = 0xbf;      // 显示"-"
   
 dis_digit = 0xfe;
 dis_index = 0;
 
 TCON = 0x01;
 IE = 0x8a;    // 使能timer0,1 中断
 
 TR0 = 1;
 TR1 = 1;

 key_v = 0x03;

 while(1)
 {
  if(scan_key())
  {
   delayms(10);
   if(scan_key())
   {
    key_v = key_s;
    proc_key();
   }
  }
  
 }
}

bit scan_key()
{
 key_s = 0x00;
 key_s |= K2;
 key_s <<= 1;
 key_s |= K1;
 return(key_s ^ key_v); 
}

void proc_key()
{
 EA = 0;
 if((key_v & 0x01) == 0)  // K1
 {
  inc_hour();
 // JJ=1;
 }
 else if((key_v & 0x02) == 0) // K2
 {
  min++;
 // JJ=0;
  if(min > 59)
  {
   min = 0;
  }
  dis_buf[3] = dis_code[min / 10];  // 分十位
  dis_buf[4] = dis_code[min % 10];  // 分个位
 }

 EA = 1;
}

void timer0() interrupt 1
// 定时器0中断服务程序, 用于数码管的动态扫描
// dis_index --- 显示索引, 用于标识当前显示的数码管和缓冲区的偏移量
// dis_digit --- 位选通值, 传送到P2口用于选通当前数码管的数值, 如等于0xfe时,
//    选通P2.0口数码管
// dis_buf   --- 显于缓冲区基地址 
{
 TH0 = 0xFC;
 TL0 = 0x17;
 
 P2 = 0xff;       // 先关闭所有数码管
 P0 = dis_buf[dis_index];   // 显示代码传送到P0口
 P2= dis_digit;      // 

 dis_digit = _crol_(dis_digit,1); // 位选通值左移, 下次中断时选通下一位数码管
 dis_index++;      // 
     
 dis_index &= 0x07;   // 8个数码管全部扫描完一遍之后，再回到第一个开始下一次扫描
} 

void timer1() interrupt 3
{
 TH1 = 0xdc;
 
 sec100++;
 
 if(sec100 >= 100)
 {
  sec100 = 0;
  inc_sec();
 }
}

void inc_sec()
{
 sec++;
 if(sec > 59)
 {
  sec = 0;
  inc_min();
 }
 dis_buf[6] = dis_code[sec / 10];  // 秒十位
 dis_buf[7] = dis_code[sec % 10];  // 秒个位 
}

void inc_min()
{
 min++;
 if(min > 59)
 {
  min = 0;
  inc_hour();
 }
 dis_buf[3] = dis_code[min / 10];  // 分十位
 dis_buf[4] = dis_code[min % 10];  // 分个位
}

void inc_hour()
{
 hour++;
 if(hour > 23)
 {
  hour = 0;
 }
 if(hour > 9)
  dis_buf[0] = dis_code[hour / 10];  // 时十位
 else
  dis_buf[0] = 0xff;     // 当小时的十位为0时不显示
 dis_buf[1] = dis_code[hour % 10];  // 时个位

}


void delayms(unsigned char ms) 
// 延时子程序
{      
 unsigned char i;
 while(ms--)
 {
  for(i = 0; i < 120; i++);
 }
}
