
//��K1 �̵�������
//��K2 �̵����Ͽ�
/*********************************************************/

#include <reg52.h>
sbit  K1 =P3^2 ;                    //K1
sbit  K2 =P3^3 ;                    //K2
sbit JDQ=P3^7;					   //�̵���

/*********************************************************/
void Delay(unsigned int ii)	//1MS
{
unsigned int jj;
for(;ii>0;ii--)
for(jj=0;jj<125;jj++)
{;}
}
void main()
{  

   while(1){
   
   if (K1==0) 
{
Delay(10);
if(K1==0){
 JDQ=0;//����
 P0=0x00;//P0��LED��
 }
}

if (K2==0){
Delay(10);
if(K2==0){
	 JDQ=1;	//�ͷ�
	 P0=0xff; //P0��LED����
} 
} 
   }
}
