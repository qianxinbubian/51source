/*-----------------------------------------------
  ���ƣ�18B20�¶ȴ�����
  ��˾���Ϻ�������ӿƼ����޹�˾
  ��վ��www.doflye.cn
  ��д��ʦ��
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�18B20�����¶ȼ���Ӧ����������,�뽫18b20�����
        Ȼ��������ܿ�����ʾXX.XC��C��ʾ���϶ȣ�����ʾ25.3C��ʾ��ǰ�¶�25.3��
        ������ʹ��11.0592M����
        ���ڲ������ ѡ���Ӧ��com�� 
------------------------------------------------*/
#include<reg52.h>     //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<math.h>
#include <stdio.h>
#include<INTRINS.H>

#define uchar unsigned char
#define uint   unsigned int;
/******************************************************************/
/*                    ����˿�                                    */
/******************************************************************/
sbit seg1=P2^0;
sbit seg2=P2^1;
sbit seg3=P2^2;
sbit DQ=P1^3;//ds18b20 �˿�
sfr dataled=0x80;//��ʾ���ݶ˿�
/******************************************************************/
/*                    ȫ�ֱ���                                    */
/******************************************************************/
uint temp;
uchar flag_get,count,num,minute,second;
uchar code tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
                                               //7������ܶ������
uchar  str[6];
/******************************************************************/
/*                   ��������                                     */
/******************************************************************/
void delay1(uchar MS);
unsigned int ReadTemperature(void);
void Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
void delay(unsigned int i);

void UARTinit(void)
{
 
    SCON  = 0x50;		        /* SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���         */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 0xFD;               /* TH1:  reload value for 9600 baud @ 11.0592MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    //EA    = 1;                  /*�����ж�*/
    //ES    = 1;       
	TI    = 1; 
}
/******************************************************************/
/*                    ������                                      */
/******************************************************************/
main()
{
unsigned int TempH,TempL;

TMOD|=0x01;//��ʱ������
TH0=0xef;
TL0=0xf0;
IE=0x82;
TR0=1;
UARTinit();
P2=0x00;  
count=0;
while(1)
{
   str[5]=0x39;         //��ʾC����
   str[1]=tab[TempH/100]; //ʮλ�¶�
   str[2]=tab[(TempH%100)/10]; //ʮλ�¶�
   str[3]=tab[(TempH%100)%10]|0x80; //��λ�¶�,��С����
   str[4]=tab[TempL];
  if(flag_get==1)       //��ʱ��ȡ��ǰ�¶�
    {
  temp=ReadTemperature();
  if(temp&0x8000)
     {
     str[0]=0x40;//���ű�־
     temp=~temp;  // ȡ����1
	 temp +=1;
	 }
  else
     str[0]=0;
  TempH=temp>>4;
  TempL=temp&0x0F;
  TempL=TempL*6/10;//С�����ƴ���
  printf("%d.%d\n",TempH,TempL);
  flag_get=0;
    }
  }
}


/******************************************************************/
/*                  ��ʱ���ж�                                    */
/******************************************************************/
void tim(void) interrupt 1 using 1//�жϣ����������ɨ����¶ȼ����
{
TH0=0xef;//��ʱ����װֵ
TL0=0xf0;
num++;
if (num==200)
    {num=0;
	  flag_get=1;//��־λ��Ч
      second++;
       if(second>=60)
         {second=0;
           minute++;  
          }
      }
count++;
if(count==1)
   {P2=0;
    dataled=str[0];}//�����ɨ��
if(count==2)
   {P2=1;
    dataled=str[1];}
if(count==3)
   { P2=2;
     dataled=str[2];
     }
if(count==4)
   { P2=3;
     dataled=str[3];
     }
if(count==5)
   { P2=4;
     dataled=str[4];
     }
if(count==6)
   { P2=5;
     dataled=str[5];
     count=0;}
}
/******************************************************************/
/*                    ��ʱ����                                    */
/******************************************************************/
void delay(unsigned int i)//��ʱ����
{
 while(i--);
}
/******************************************************************/
/*                    ��ʼ��                                      */
/******************************************************************/
void Init_DS18B20(void)
{
 unsigned char x=0;
 DQ = 1;    //DQ��λ
 delay(8);  //������ʱ
 DQ = 0;    //��Ƭ����DQ����
 delay(80); //��ȷ��ʱ ���� 480us
 DQ = 1;    //��������
 delay(10);
 x=DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
 delay(5);
}

/******************************************************************/
/*                    ��һ���ֽ�                                  */
/******************************************************************/
unsigned char ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
for (i=8;i>0;i--)
 {
  DQ = 0; // �������ź�
  dat>>=1;
  DQ = 1; // �������ź�
  if(DQ)
   dat|=0x80;
  delay(5);
 }
 return(dat);
}

/******************************************************************/
/*                 дһ���ֽ�                                     */
/******************************************************************/
void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  DQ = 0;
  DQ = dat&0x01;
  delay(5);
  DQ = 1;
  dat>>=1;
 }
delay(5);
}

/******************************************************************/
/*                   ��ȡ�¶�                                     */
/******************************************************************/
unsigned int ReadTemperature(void)
{
unsigned char a=0;
unsigned int b=0;
unsigned int t=0;
Init_DS18B20();
WriteOneChar(0xCC); // ����������кŵĲ���
WriteOneChar(0x44); // �����¶�ת��
delay(200);
Init_DS18B20();
WriteOneChar(0xCC); //����������кŵĲ��� 
WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
a=ReadOneChar();   //��λ
b=ReadOneChar();   //��λ

b<<=8;
t=a+b;

return(t);
}
