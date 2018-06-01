#include <AT89X52.H> 
#include<lcd.h>
#include<stdio.h>
uchar code DIS[]= {"4X4-JIANPAN-TEST"};//
uchar code DIS1[]={"PRESS_KEY_IS--"};//

void delay1()                   //��ʱ�ӳ���
{
 unsigned char n, m;
 for (n = 100; n > 0; n--)
  for (m = 300; m > 0; m--);    
}

unsigned char Keycan(void) //����ɨ����� P1.0--P1.3Ϊ���� P1.4--P1.7Ϊ����
{
  unsigned char rcode, ccode;
  P1 = 0xF0;      // ��ȫ0��ɨ���룬��������
  if((P1&0xF0) != 0xF0) // ���м�����
  {
    delay1();// ��ʱȥ���� 
    if((P1&0xF0) != 0xF0)
    {  rcode = 0xFE;         // ����ɨ���ֵ
    while((rcode&0x10) != 0)
    {
       P1 = rcode;         // �����ɨ����
    if((P1&0xF0) != 0xF0) // �����м�����
    {
       ccode = (P1&0xF0)|0x0F;
       do{;}while((P1&0xF0) != 0xF0); //�ȴ����ͷ�
       return ((~rcode) + (~ccode)); // ���ؼ�����
    }
    else
    rcode = (rcode<<1)|0x01; // ��ɨ��������һλ
    }
 }
  }  
  return 0; // �޼����£�����ֵΪ0
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
delay400ms();//��ʱ400MS 
  initialization();//LCD��λ;
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
