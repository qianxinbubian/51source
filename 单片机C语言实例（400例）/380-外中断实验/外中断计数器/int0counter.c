#include <REG51.H>		
#define uchar unsigned char
#define uint unsigned int
unsigned char code tab[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF};
 sbit P32=P3^2;
 sbit P25=P2^5;
 sbit P26=P2^6;
 sbit P27=P2^7;
   uint cnt;
void init(void)					
{												
	EX0=1;					
	IT0=1;					
	EA=1;
	ET0 = 1; 				//��ʱ��0�жϴ�
	TMOD = 0x1;			//�趨ʱ��0Ϊģʽ1��16λģʽ
	TH0=0xd8;
	TL0=0xf0;				//�趨ʱֵΪ10000us��10ms��
	TR0 = 1;
	PT0=1;
					
}							
/**************************************************/
void delay(uint k)				
{							
uint data i,j;					
	for(i=0;i<k;i++)			
	{						
	for(j=0;j<121;j++){;}		
	}						
}							
/**************************************************/

void ledshow()
{

 if(cnt>999)cnt=0;			
					
		P0=tab[cnt/100];	
		P25=0;			
		delay(1);
		P25=1;				
		P0=tab[(cnt%100)/10];
		P26=0;			
		delay(1);
		P26=1;				
		P0=tab[cnt%10];	
		P27=0;			
		delay(1);
		P27=1;					
						




}

void display(void) interrupt 1 using 2
{
  TH0=0xd8;
	TL0=0xf0;				//�趨ʱֵΪ10000us��10ms)

   ledshow();



}





void main(void)				
{					
	init();					
	while(1);					
							
}							
/************************************************/
void extern_int0(void) interrupt 0 using 0	
{
EX0=0;
delay(10);
if(P32==0){						
cnt++; }
while(!P32);
EX0=1;	
			
}							
							
