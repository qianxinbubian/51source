#include <AT89X51.h>
#define uchar unsigned  char
#define uint unsigned int
uchar  idata trdata[]={'8','9','S','5','1',0x0d,0x0a,0x00};
uchar  idata trdata1[]={'R','I','C','H','M','C','U',0x0d,0x0a,0x00};
main()
{
uchar i;
  uint j;
SCON= 0x40;                //串口方式1
PCON=0;                     //SMOD=0
REN=1;                     //允许接收
TMOD= 0x20;                //定时器1定时方式2
TH1= 0xfd;                //11.0592M 9600波特率
TL1= 0xfd;
TR1= 1;                  //启动定时器
  while(1)
{
    i=0;
    while(trdata[i]!=0x00)
{
SBUF=trdata[i];
      while(TI==0);
      TI=0;
      i++;
    }
    for (j=0;j<50000;j++); 

	i=0;
	while(trdata1[i]!=0x00)
{
SBUF=trdata1[i];
      while(TI==0);
      TI=0;
      i++;
    }
	 for (j=0;j<50000;j++); 
  }
}
