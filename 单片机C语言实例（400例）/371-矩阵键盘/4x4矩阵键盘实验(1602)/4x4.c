#include <AT89X52.H> 
#include<lcd.h>
#include<stdio.h>
uchar code DIS[]= {"4X4-JIANPAN-TEST"};//
uchar code DIS1[]={"PRESS_KEY_IS--"};//

void delay1()                   //延时子程序
{
 unsigned char n, m;
 for (n = 100; n > 0; n--)
  for (m = 300; m > 0; m--);    
}

unsigned char Keycan(void) //按键扫描程序 P1.0--P1.3为行线 P1.4--P1.7为列线
{
  unsigned char rcode, ccode;
  P1 = 0xF0;      // 发全0行扫描码，列线输入
  if((P1&0xF0) != 0xF0) // 若有键按下
  {
    delay1();// 延时去抖动 
    if((P1&0xF0) != 0xF0)
    {  rcode = 0xFE;         // 逐行扫描初值
    while((rcode&0x10) != 0)
    {
       P1 = rcode;         // 输出行扫描码
    if((P1&0xF0) != 0xF0) // 本行有键按下
    {
       ccode = (P1&0xF0)|0x0F;
       do{;}while((P1&0xF0) != 0xF0); //等待键释放
       return ((~rcode) + (~ccode)); // 返回键编码
    }
    else
    rcode = (rcode<<1)|0x01; // 行扫描码左移一位
    }
 }
  }  
  return 0; // 无键按下，返回值为0
}
void KeyDispaly(unsigned char key)
{
   switch(key)
   {
   case 0x11: displayonechar(14,1,'0'); break;
   case 0x21: displayonechar(14,1,'1'); break;
   case 0x41: displayonechar(14,1,'2'); break;
   case 0x81: displayonechar(14,1,'3'); break;
   case 0x12: displayonechar(14,1,'4'); break;
   case 0x22: displayonechar(14,1,'5'); break;
   case 0x42: displayonechar(14,1,'6'); break;
   case 0x82: displayonechar(14,1,'7'); break;
   case 0x14: displayonechar(14,1,'8'); break;
   case 0x24: displayonechar(14,1,'9'); break;
   case 0x44: displayonechar(14,1,'A'); break;
   case 0x84: displayonechar(14,1,'B'); break;
   case 0x18: displayonechar(14,1,'C'); break;
   case 0x28: displayonechar(14,1,'D'); break;
   case 0x48: displayonechar(14,1,'E'); break;
   case 0x88: displayonechar(14,1,'F'); break;
   default: break;
   }
}
int main(void)
{    unsigned char Key; 
delay400ms();//延时400MS 
  initialization();//LCD复位;
      displaylistchar(0,0,DIS);
	displaylistchar(0,1,DIS1);
	displayonechar(15,1,'-');
 
   while(1)
   {
 
     Key = Keycan();   
     KeyDispaly(Key);

   } 
return 0;
}
