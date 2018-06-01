/*
1������Ŀ�ģ�ʹ�ö�ʱ��ѧϰ����ʱ���ж�0�����߱��ж�1�������� 
2��Ӳ��Ҫ������ܡ�����12M
*/
#include <reg52.h>

code unsigned char tab[]=
			{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			//��������� 0-9 

unsigned char Dis_Shiwei;//����ʮλ
unsigned char Dis_Gewei; //�����λ
unsigned char S_Dis_Shiwei;//������ʮλ
unsigned char S_Dis_Gewei; //�������λ
unsigned char second,msecond;

void CLR(void);
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
EX0=1;//�ⲿ�ж�0����
IT0=1;

EX1=1;//�ⲿ�ж�1����
IT1=1;

TMOD |=0x01;//��ʱ������ 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
ET0=1;  //���ж�
TR0=0;

EA=1;
CLR();
while(1)
  {
  P0=S_Dis_Shiwei;//��ʾ��ʮλ
  P2=1;
  delay(300);//������ʱ
  P0=S_Dis_Gewei; //��ʾ���λ
  P2=2;
  delay(300);
  P0=0x40; //��ʾ���λ
  P2=3;
  delay(300);
  P0=Dis_Shiwei;//��ʾʮλ
  P2=4;
  delay(300);//������ʱ
  P0=Dis_Gewei; //��ʾ��λ
  P2=5;
  delay(300);
  }
}
/********************************/
/*      ��ʱ�ж�                */
/********************************/
void tim(void) interrupt 1 using 1
{

TH0=0xd8;//���¸�ֵ
TL0=0xf0;
msecond++;
if (msecond==100)
    {
    msecond=0;
    second++;//���1
	if(second==100)
	   second=0;
	S_Dis_Shiwei=tab[second/10];//ʮλ��ʾֵ����
    S_Dis_Gewei=tab[second%10]; //��λ��ʾ����

        }
    Dis_Shiwei=tab[msecond/10];//ʮλ��ʾֵ����
    Dis_Gewei=tab[msecond%10]; //��λ��ʾ����

}
void ISR_INT0(void) interrupt 0 using 1
{
 TR0=!TR0; //�����ⲿ�жϴ򿪺͹رն�ʱ��0 ���ڿ�ʼ��ֹͣ��ʱ

}
void ISR_INT1(void) interrupt 2 using 1
{
if(TR0==0)//ֹͣʱ�ſ�������
   CLR();
}

void CLR(void)
{
 second=0; //�����ⲿ�ж�����
 msecond=0;
 Dis_Shiwei=tab[msecond/10];//ʮλ��ʾֵ����
 Dis_Gewei=tab[msecond%10]; //��λ��ʾ����
 S_Dis_Shiwei=tab[second/10];//ʮλ��ʾֵ����
 S_Dis_Gewei=tab[second%10]; //��λ��ʾ����
}
