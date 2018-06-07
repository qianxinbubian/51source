
//按K1 继电器吸合
//按K2 继电器断开
/*********************************************************/

#include <reg52.h>
sbit  K1 =P3^2 ;                    //K1
sbit  K2 =P3^3 ;                    //K2
sbit JDQ=P3^7;					   //继电器
unsigned char inbuf2[5];//接收缓冲区
unsigned char count3;
void init_serialcomm( void )
{
SCON = 0x50 ; //SCON: serail mode 1, 8-bit UART, enable ucvr
TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
PCON |= 0x80 ; //SMOD=1;
TH1 = 0xFA ; //Baud:4800 fosc=11.0592MHz
IE |= 0x90 ; //Enable Serial Interrupt
TR1=1;
}
//串口接收中断函数
void serial () interrupt 4 using 3
{
if (RI) //RI==开始接收
{
unsigned char ch;
RI = 0 ; //软件RI=0
ch=SBUF;
if (ch>1)
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


/*********************************************************/
void Delay(unsigned int ii)	//1MS
{
unsigned int jj;
for(;ii>0;ii--)
for(jj=0;jj<125;jj++)
{;}
}
void main()
{  init_serialcomm();
   EA = 1;					//允许CPU中断


   while(1){
  
   	 if (inbuf2[0]==0x31){ 
	 JDQ=0;//吸合
 P0=0x00;//P0口LED亮

	 }

	 if (inbuf2[0]==0x32){
	 JDQ=1;//释放
 P0=0xff;//P0口LED灭
	 }

   if (K1==0) 
{
Delay(10);
if(K1==0){
 inbuf2[0]=0x00; 
 JDQ=0;//吸合
 P0=0x00;//P0口LED亮
 }
}

if (K2==0){
Delay(10);
if(K2==0){
 inbuf2[0]=0x00; 
	 JDQ=1;	//释放
	 P0=0xff; //P0口LED灯灭
} 
} 
   }
}
