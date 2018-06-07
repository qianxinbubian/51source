
//按K1 继电器吸合
//按K2 继电器断开
/*********************************************************/

#include <reg52.h>
sbit  K1 =P3^2 ;                    //K1
sbit  K2 =P3^3 ;                    //K2
sbit JDQ=P3^7;					   //继电器

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
 JDQ=0;//吸合
 P0=0x00;//P0口LED亮
 }
}

if (K2==0){
Delay(10);
if(K2==0){
	 JDQ=1;	//释放
	 P0=0xff; //P0口LED灯灭
} 
} 
   }
}
