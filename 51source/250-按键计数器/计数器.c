/*
1������Ŀ�ģ�ʹ�ö�ʱ��ѧϰ��ʱ
2��Ӳ��Ҫ������ܡ�����12M
*/
#include <reg52.h>

code unsigned char tab[]=
			{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; 
			//��������� 0-9 

unsigned char Dis_Shiwei;//����ʮλ
unsigned char Dis_Gewei; //�����λ
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{

TMOD |=0x01;//��ʱ������ 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
IE= 0x82;  //���ж�
TR0=1;

while(1)
  {
  P0=Dis_Shiwei;//��ʾʮλ
  P2=0xFD;
  delay(300);//������ʱ
  P0=Dis_Gewei; //��ʾ��λ
  P2=0xFE;
  delay(300);
  }

}
/********************************/
/*      ��ʱ�ж�                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char second,count;
TH0=0xd8;//���¸�ֵ
TL0=0xf0;
count++;
if (count==100)
    {
    count=0;
    second++;//���1
	if(second==100)
	   second=0;
	Dis_Shiwei=tab[second/10];//ʮλ��ʾֵ����
    Dis_Gewei=tab[second%10]; //��λ��ʾ����
    
        }


}