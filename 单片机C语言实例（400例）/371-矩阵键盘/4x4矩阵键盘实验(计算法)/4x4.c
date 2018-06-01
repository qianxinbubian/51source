#include <AT89X52.H> 

#include<stdio.h>

unsigned char code table[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};//共阳数码管显示0~F
void delay()                   //延时子程序
{
 unsigned char n, m;
 for (n = 100; n > 0; n--)
  for (m = 300; m > 0; m--);    
}
void display(unsigned char S)      //显示子程序
{
 P0 = table[S];	//数码管是P0口驱动的
 delay();                    
 P2 = 0x7f; //数码管选通开关（P2.7选通 也就是数码管的第八位 (从左到右顺序)）
}
unsigned char Keycan(void) //按键扫描程序 P1.0--P1.3为行线 P1.4--P1.7为列线
{
  unsigned char rcode, ccode;
  P1 = 0xF0;      // 发全0行扫描码，列线输入
  if((P1&0xF0) != 0xF0) // 若有键按下
  {
    delay();// 延时去抖动 
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
   case 0x11: display(0); break;
   case 0x21: display(1); break;
   case 0x41: display(2); break;
   case 0x81: display(3); break;
   case 0x12: display(4); break;
   case 0x22: display(5); break;
   case 0x42: display(6); break;
   case 0x82: display(7); break;
   case 0x14: display(8); break;
   case 0x24: display(9); break;
   case 0x44: display(10); break;
   case 0x84: display(11); break;
   case 0x18: display(12); break;
   case 0x28: display(13); break;
   case 0x48: display(14); break;
   case 0x88: display(15); break;
   default: break;
   }
}
int main(void)
{ 
   unsigned char Key;
   while(1)
   {
     Key = Keycan();   
     KeyDispaly(Key);
   } 
return 0;
}
