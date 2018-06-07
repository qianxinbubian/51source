#include<reg52.h>							   
sbit P24=P2^4;
sbit P25=P2^5;
sbit P26=P2^6;
sbit P27=P2^7;
unsigned int js;
unsigned int count;
unsigned char code LEDDis[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF};
void Delay(unsigned int ii)
{
unsigned int jj;
for(;ii>0;ii--)
for(jj=0;jj<125;jj++)
{;}

}
void main()
{
 
    EA = 1;					//����CPU�ж�
	ET0 = 1; 				//��ʱ��0�жϴ�
	ET1=1;					//��ʱ��1�жϴ�
	TMOD = 0x11;			//�趨ʱ��0 1Ϊģʽ1��16λģʽ
	TH0=0xB8;
	TL0=0x00;				//�趨ʱֵΪ��20ms��
	TH1=0xdc;					//�趨ʱֵΪ��10ms��
	TL1=0x00;
	TR0 = 1;
	TR1=1;

while(1);
}

//��ʱ��0�ж������м�ɨ�����ʾ
void Key_Time0(void) interrupt 1 using 2
{
  TH0=0xb8;
	TL0=0x00;//�趨ʱֵΪ20000us��20ms)
  count++;
  if(count==50){
  	 count=0;
  js++;
  }
  if(js==9999){
  
  js=0;
  }
}

void Dis_Time1(void) interrupt 3 using 3
{
  TH1=0xdc;
	TL1=0x00;//�趨ʱֵΪ20000us��20ms)

		P0=LEDDis[js/1000];
P24=0;
  Delay(2);
	  P24=1;

	P0=LEDDis[(js%1000)/100];
P25=0;
  Delay(2);
	  P25=1;
  P0=LEDDis[(js%100)/10];
P26=0;
  Delay(2);
 P26=1;
 P0=LEDDis[js%10];
P27=0;
 Delay(2);
  P27=1;
}



 