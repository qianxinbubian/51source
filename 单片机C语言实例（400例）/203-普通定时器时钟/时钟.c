/*
1���������Ե�����ʱʱ��
2���������Ե��ڶ�ʱʱ��
3����ʱʱ�䵽�̵�������

*8λ�������ʾ ʱ���ʽ��ʽ 12-08-00 ��ʾ12��08��00��

S1 ����Сʱ ��1����  
S2 ���ڷ��� ��1����
S2 ���ڷ��Ӽ�1����
*/

#include <reg52.h>


sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;


sbit LED=P1^2;
code unsigned char tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			//��������� 0-9
unsigned char StrTab[8];  //���建����
unsigned char minute=30,hour=12,second;  //���岢�ҳ�ʼ��ֵ 12:30:00

void delay(unsigned int cnt)//��ʱ����
{
 while(--cnt);
}


void Displaypro(void)
{

	        StrTab[0]=tab[hour/10];    //��ʾ����ʱ��
			StrTab[1]=tab[hour%10];
			StrTab[2]=0x40;
			StrTab[3]=tab[minute/10];
			StrTab[4]=tab[minute%10];
			StrTab[5]=0x40;
            StrTab[6]=tab[second/10];
			StrTab[7]=tab[second%10];
	
}

main()//������
{

    
    TMOD |=0x01;//��ʱ��0 10ms in 12M crystal ���ڼ�ʱ
	TH0=0xd8;
	TL0=0xf0;
	ET0=1;
	TR0=1;

    TMOD |=0x10; //��ʱ��1���ڶ�̬ɨ��
	TH1=0xF8;
	TL1=0xf0;
	ET1=1;
	TR1=1;
    EA =1;
    

 Displaypro();

while(1)//��ѭ��
  {
   
/////////////////////////////////////////////////////////////////
  if(!KEY1) //����1ȥ���Լ�����
    {
    delay(10000);
    if(!KEY1)
	   {          
	     hour++;if(hour==24)hour=0;     //����ʱ�� Сʱ ��1
         Displaypro();
	   }
	}
////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
  if(!KEY2) //����ȥ���Լ�����
    {
    delay(10000);
    if(!KEY2)
	   {
	    minute++;if(minute==59)minute=0;//�ּ�1
	    Displaypro();
	  }
	}
/////////////////////////////////////////////////////////////////
  if(!KEY3) //����ȥ���Լ�����
    {
    delay(10000);
    if(!KEY3)
	   {
	    minute--;if(minute==0)minute=59;  //�ּ�1
	    Displaypro();
	  }
	}
  }
}
////////////////////////////////////////////////////////////////
/********************************/
/*      ��ʱ�ж�1                */
/********************************/
void time1_isr(void) interrupt 3 using 0//��ʱ��1������̬ɨ��
{
    static unsigned char num;
    TH1=0xF8;//�����ֵ
	TL1=0xf0;

	switch (num){
     case 0:P2=0;P0=StrTab[num];break; //�ֱ���û�������ֵ����ɨ��
	 case 1:P2=1;P0=StrTab[num];break;
	 case 2:P2=2;P0=StrTab[num];break;
	 case 3:P2=3;P0=StrTab[num];break;
	 case 4:P2=4;P0=StrTab[num];break;
	 case 5:P2=5;P0=StrTab[num];break;
     case 6:P2=6;P0=StrTab[num];break;
	 case 7:P2=7;P0=StrTab[num];break;
	 default:break;
}
num++;     //ɨ��8�Σ�ʹ��8�������
if(num==8)
   num=0;
}
//////////////////////////////////////////////////////////////////
/********************************/
/*      ��ʱ�ж�0                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char count;//�����ڲ���̬����

TH0=0xd8;//���¸�ֵ
TL0=0xf0;
count++;
   switch (count)
          {
           case 0:
           case 20:
           case 40:
           case 60:
           case 80:Displaypro(); //��һ��ʱ�������ʾ����
                   break;
           case 50:LED=!LED; //���� LED��˸
                   break;
           default:break;
          }

if (count==100)
    {
    count=0;
    second++;//���1
	if(second==60)
	   {
	   second=0;
	   minute++; //�ּ�1
	   if(minute==60)
		  {
		   minute=0;
		   hour++;  //ʱ��1
		   if(hour==24)
		      hour=0;
		  }  
      
	   }
    
   } 
 
}