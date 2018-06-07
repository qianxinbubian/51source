#include <REG52.H>	
#define uint unsigned int	
#define uchar unsigned char
uchar code DIS_SEG7[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
uchar code DIS_BIT[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
/****************************/
void delay(uint k)			
{					
uint data i,j;			
for(i=0;i<k;i++)		
{for(j=0;j<121;j++)		
{;}}					
}					
/***********************************/
void main(void)		
{	
	uchar cnt;				
	while(1)				
	{					
	  for(cnt=0;cnt<8;cnt++)	   
	  {P0=DIS_SEG7[cnt+1];
	   P2=DIS_BIT[cnt];
	   delay(1);
P2=0xff;
       P0=0xff;
}
       
	 }				
}					

















