#include<reg51.h>
sbit Key_UP=P3^2;
sbit Key_DOWN=P3^3;
unsigned char KeyV,TempKeyV; 							   
sbit P25=P2^5;//����ܵ�5λ
sbit P26=P2^6;//����ܵ�6λ
sbit P27=P2^7;//����ܵ�7λ
sbit JDQ=P3^7;//�̵���

 unsigned int js;
 unsigned int ds;//����ʱ��ʼʱ�� 
 unsigned char jsflag;
static unsigned int i;
unsigned char code LEDDis[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF};

void Delay(unsigned int ii)	//1MS
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
	ds=999;
	KeyV = 0;
	TempKeyV = 0;
	jsflag=0;
while(1);
}

//��ʱ��0�ж������м�ɨ�����ʾ
void Key_Time0(void) interrupt 1 using 2
{
  TH0=0xB8;
	TL0=0x00;				//�趨ʱֵΪ20000us��20ms)
  if (!Key_UP)
    KeyV = 1;
  if (!Key_DOWN)
    KeyV = 2;
   if (KeyV!= 0)      //�м�����
    {
      Delay(10);   //��ʱ����  ����10ms�ٲ�
      if (!Key_UP)
        TempKeyV = 1;
      if (!Key_DOWN)
        TempKeyV = 2;
		 if (KeyV == TempKeyV)   //����ֵ���Ϊȷ�������˼�
	   {
	   	   if (KeyV == 1){
		                   jsflag=0;
		   				   if(ds==0){
						   ds=600;
						   }
						 	js=js+1;
							JDQ=0;   //�̵�������
						
	if(js==50){
	js=0;
	 ds=ds-1;
	 
	}
}
if(KeyV==2){
	  jsflag=1;
	}
}
}
   if(jsflag==1){
   JDQ=1;
   }
   if(ds==0){
   JDQ=1;
   jsflag=1;
   KeyV=0;
   }
}
void disp_Time1(void) interrupt 3 using 3
{	TH1=0xdc;
	TL1=0x00;
  	 P0=LEDDis[ds/100];
P25=0;
  Delay(2);
	  P25=1;
  P0=LEDDis[(ds%100)/10];
P26=0;
  Delay(2);
 P26=1;
 P0=LEDDis[ds%10];
P27=0;
 Delay(2);
  P27=1;
 	
}
