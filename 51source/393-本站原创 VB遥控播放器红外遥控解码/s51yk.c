#include <regX52.h>

#define c(x) (x*110592/120000)
  sbit Ir_Pin=P3^2;
	  sbit beep=P3^6;
#define INBUF_LEN 4 //数据长度
unsigned char inbuf1[INBUF_LEN]={'0','0','0','0'};//发送缓冲区
unsigned char inbuf2[50];//接收缓冲区
unsigned char count3;
void init_serialcomm( void )
{
SCON = 0x50 ; //SCON: serail mode 1, 8-bit UART, enable ucvr
T2CON=0x30;
TH2=0x00;
TL2=0x00;
RCAP2H=0xFF;
RCAP2L=0xDC;
TR2=1;
}
//向串口发送一个字符
void send_char_com( unsigned char ch)
{
SBUF=ch;
while (TI== 0 );
TI= 0 ;
}
//向串口发送一个字符串，strlen 为该字符串长度
void send_string_com( unsigned char *str, unsigned int strlen)
{
unsigned int k= 0 ;
do
{
send_char_com(*(str + k));
k++;
} while (k < strlen);
}
//串口接收中断函数
void serial () interrupt 4 using 3
{
if (RI) //RI==开始接收
{
unsigned char ch;
RI = 0 ; //软件RI=0
ch=SBUF;
if (ch> 1 )
{
count3= 0 ;
inbuf2[count3]=ch;
}
else
{
count3++;
inbuf2[count3]=ch;
}
}
}	 

    
      unsigned char code Led_Tab[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,
      0xf8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};                        //共阳极数码显示码0-F.
      unsigned char code Led_Sel[]={0xef,0xdf,0xbf,0x7f};
	  unsigned char dis_8[17]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','-'};

      unsigned char Led_Buf[4]; //显示缓冲区
      char Led_Index;                   //位选         

      unsigned char Ir_Buf[4]; //用于保存解码结果
		 void delay_50ms(unsigned int t)
{
 unsigned int j;   
 for(;t>0;t--) 
  for(j=6245;j>0;j--) 
{;}
}

      //==============================================================
      //数码管扫描
      timer0() interrupt 1 using 1
      {
      TL0=65536-1000;
      TH0=(65536-1000)/256; //定时器0设定约1000us中断一次，用于数码管扫描
      //P0=0xff;
      P2=Led_Sel[Led_Index];                            //位选
      P0=Led_Tab[Led_Buf[Led_Index]];                    //段选

      if(++Led_Index>3) Led_Index=0;                    //四个扫描完了,到第一个数码管
      }
      //==============================================================
      unsigned int Ir_Get_Low()
      {
      TL1=0;
      TH1=0;
      TR1=1;
      while(!Ir_Pin && (TH1&0x80)==0);                
      TR1=0;           
      return TH1*256+TL1;
      }
      //=============================================================
      unsigned int Ir_Get_High()
      {
      TL1=0;
      TH1=0;
      TR1=1;
      while(Ir_Pin && (TH1&0x80)==0);
      TR1=0;
      return TH1*256+TL1;
      }
      //==============================================================
      main()
      {
      unsigned int temp;
      char i,j;
      Led_Index=1;
	   init_serialcomm(); //初始化串口
      TMOD=0x11;
      TL0=65536-1000;
      TH0=(65536-1000)/256; //定时器0设定约1000us中断一次，用于数码管扫描
      EA=1;
      ET0=1;
      TR0=1;

      Led_Buf[0]=0;
      Led_Buf[1]=0;
      Led_Buf[2]=0;
      Led_Buf[3]=0; //显示区设成0
      do{
      restart:
      while(Ir_Pin);
      temp=Ir_Get_Low();
      if(temp<c(8500) || temp>c(9500)) continue;//引导脉冲低电平9000
      temp=Ir_Get_High();
      if(temp<c(4000) || temp>c(5000)) continue;//引导脉冲高电平4500
      for(i=0;i<4;i++) //4个字节
      for(j=0;j<8;j++) //每个字节8位
      {
      temp=Ir_Get_Low();
      if(temp<c(200) || temp>c(800)) goto restart;
      temp=Ir_Get_High();
      if(temp<c(200) || temp>c(2000)) goto restart;
      Ir_Buf[i]>>=1;
      if(temp>c(1120)) Ir_Buf[i]|=0x80;
      }
	  Led_Buf[0]=(Ir_Buf[2]/16)&0xf;
      Led_Buf[1]=Ir_Buf[2]&0xf;
      Led_Buf[2]=(Ir_Buf[3]/16)&0xf; //显示结果
      Led_Buf[3]=Ir_Buf[3]&0xf;
	  inbuf1[0]=dis_8[Led_Buf[0]];
  inbuf1[1]=dis_8[Led_Buf[1]];
  inbuf1[2]=0x0d;
  inbuf1[3]=0x0a;
  send_string_com(inbuf1,INBUF_LEN);
	 beep=0;
	 delay_50ms(2);
	 beep=1;
      }
	  while(1);	
      }
	 