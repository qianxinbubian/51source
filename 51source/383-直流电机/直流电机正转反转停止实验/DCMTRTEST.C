#include<reg52.h>
sbit Key_UP=P3^2;   //正转按键 对应实验板上的K1
sbit Key_DOWN=P3^3;//反转按键  对应实验板上的K2
sbit Key_STOP=P3^4;//停止键    对应实验板上的K3
sbit ZZ=P1^0;//控制端
sbit FZ=P1^1;//控制端
sbit BEEP = P3^6; //蜂鸣器控制端口P36 
unsigned char KeyV,TempKeyV; 							   
void Delay(unsigned int ii)
{
unsigned int jj;
for(;ii>0;ii--)
for(jj=0;jj<125;jj++)
{;}
}
/*********************************************************/
void beep()		//蜂鸣器响一声函数
{
  unsigned char i;
  for (i=0;i<100;i++)
   {
   Delay(1);
   BEEP=!BEEP;       //BEEP取反
   } 
   BEEP=1;           //关闭蜂鸣器
   Delay(250);       //延时     
}
void main()
{
  FZ=1;
  ZZ=1;
  while(1)
  {
 	if (!Key_UP)
    KeyV = 1;
    if (!Key_DOWN)
    KeyV = 2;
	if (!Key_STOP)
    KeyV = 3;
    if (KeyV!= 0)     
     {
       Delay(10);   
       if (!Key_UP)
       TempKeyV = 1;	
       if (!Key_DOWN)
       TempKeyV = 2;	
       if (!Key_STOP)
       TempKeyV = 3;
	   if (KeyV == TempKeyV)  
	    {		 
	   	 if (KeyV == 1)
		  {
		    beep();
		    ZZ=1;
		    FZ=0;
          }
         if(KeyV==2)
          {
            beep();
            ZZ=0;
            FZ=1;
          }
         if(KeyV==3)
          {
            beep();
            ZZ=0;
            FZ=0;
          }					
        }  
     } 
     KeyV=0;
     TempKeyV=0; 
   }
}


