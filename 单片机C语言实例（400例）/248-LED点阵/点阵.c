//��ˮ�Ƶķ�ʽ���Ե���
#include <reg52.h>
////////////////////////////////////////////////////////
void delay(unsigned int cnt)
{
 while(--cnt);
}
///////////////////////////////////////////////////////
main()
{
 P2=0x00;
 P0=0x55;
 P2=0xfe;//����ʼ��ֵ
 while(1)
      {
	   delay(30000);//delay at crystal frequency in 12MHz
	   P2<<=1;//����һλ
	   P2|=0x01;//���һλ��1
	   
	   if(P2==0x7f)//����Ƿ��Ƶ�����ˣ�
          { 
		     delay(30000);//delay
	         P2=0xfe;
			 P0=~P0;
          }
      }
}
