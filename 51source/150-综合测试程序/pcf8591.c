/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591 AD/DAת��
  ��̳��www.doflye.net
  ��д��shifang
  �޸ģ���
  ���ݣ�ʹ��4·AD�е�4·����ⲿģ��������
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "pcf8591.h"

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

extern bit ack;


/*------------------------------------------------
                   AD������
------------------------------------------------*/
unsigned char  ADCheck(void)
{
 unsigned char num=0,i;
 unsigned char Val=0;
 for(i=0;i<5;i++)
     WriteDAC(0);//д��̶�ֵ��DA��������ڼ��
 for(i=0;i<5;i++)//������5�Σ�����ͨ���������Σ�ȥ���һ��ֵ���Ա�����ȶ�ֵ
     num=ReadADC(0);
 if((num>100)&&(num<150)) // 2.5V
    Val++;

 for(i=0;i<5;i++)
   num=ReadADC(1);
if((num>100)&&(num<150)) // 2.5V
    Val++;  

 for(i=0;i<5;i++)
    num=ReadADC(2);
if((num>100)&&(num<150)) // 2.5V
    Val++;

 for(i=0;i<5;i++)
     num=ReadADC(3);
if((num>100)&&(num<150)) // 2.5V
    Val++;
return Val;
}
/*------------------------------------------------
             ��ADתֵ����
������� Chl ��ʾ��Ҫת����ͨ������Χ��0-3
����ֵ��Χ0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
 {
  unsigned char Val;
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40|Chl);            //���������ӵ�ַ
     if(ack==0)return(0);
   Start_I2c();
   SendByte(AddRd);
      if(ack==0)return(0);
   Val=RcvByte();
   NoAck_I2c();                 //���ͷ�Ӧλ
   Stop_I2c();                  //��������
  return(Val);
 }
/*------------------------------------------------
               д��DAת����ֵ
���������dat ��ʾ��Ҫת����DA��ֵ����Χ��0-255
------------------------------------------------*/
bit WriteDAC(unsigned char dat)
{
   Start_I2c();               //��������
   SendByte(AddWr);             //����������ַ
     if(ack==0)return(0);
   SendByte(0x40);            //���������ӵ�ַ
     if(ack==0)return(0);
   SendByte(dat);             //��������
     if(ack==0)return(0);
   Stop_I2c();  
}


