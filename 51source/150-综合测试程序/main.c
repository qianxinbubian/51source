/*------------------------------------------------
                ����ͷ�ļ�
------------------------------------------------*/
#include"keyboard.h"
#include"delay.h"
#include"18b20.h"
#include "ds1302.h"
#include "i2c.h"
#include"uart.h"
#include "pcf8591.h"
#include "LED8x8.h"
#include "timer.h"
#include "1602.h"
#include<stdio.h>
extern unsigned char TempData[8]; //�洢��ʾֵ��ȫ�ֱ���
/*------------------------------------------------
                   ����˿�
------------------------------------------------*/
sbit LATCH1=P2^7;//��������ʹ�ܶ˿� ������
sbit LATCH2=P2^3;//                 λ����
/*------------------------------------------------
                   S����ʱ
------------------------------------------------*/
void DelayS(unsigned char u)
{
 while(u--)
      {
	  DelayMs(250);
	  DelayMs(250);
	  DelayMs(250);
	  DelayMs(250);
	  }
}
/*------------------------------------------------
                   ������
------------------------------------------------*/
main()
{
 unsigned char doflye=0x55;//����24c02����
// unsigned char i;
 unsigned char keynum;     //��������ֵ
 int temp;                 //�¶�ֵ



 Ds1302_Init();   //��ʼ��ʱ��оƬ
 InitUART ();     //��ʼ������
 LCD_Init();      //��ʼ��LCD1602
 DelayMs(30);     //�ȶ�
 LCD_Write_String(0,0,"www.doflye.net");//Һ����ʾ����
 LCD_Write_String(0,1,"LCD1602 test ok!");
 EN_CLR;
 Init_Timer0();

  	   P0=0xff;   //ȫ������
       LATCH1=1;  //������
       LATCH1=0;

       P0=0;      //8λȫ��ѡ�� 
       LATCH2=1;  //λ����
       LATCH2=0;
 DelayS(1);

 while(1)
     {
          ISendStr(0xae,400,&doflye,1);//д��24c02
		  DelayMs(60);                 //��ʱΪ����24c02����д��ֵ
		  doflye=0;                    //�����ֵ��Ϊ����֤�洢��Ч��
		  IRcvStr(0xae,400,&doflye,1); //����doflye��ֵ�����������±Ƚ�
		  if(doflye==0x55)             //������д��ֵ��ȣ���ʾ������ȷ����������
		    printf("1��24c02 ��������\n");
		  else
		    printf("1��24c02 ����XXXX������Ű�������\n");
          DelayS(1);
		  if(2==ADCheck())             //2����λ���������м�λ�ã������2·��⵽�����ֵ����ʾADCоƬ����������Ҳ�����������4·
		    printf("2��ADC ��������\n");
		  else
		    printf("2��ADC ����XXXX������Ű�������\n");
          DelayS(1);
		  Ds1302_Write_Time();//д��ʱ��
		  Ds1302_Read_Time(); //����ʱ��
		  if(time_buf1[1]==10)//�ж�д�����������֮һ����ֵ�Ƿ���ȣ���ȱ�ʾ������ȷ
            printf("3��DS1302 ��������\n");
		  else
		    printf("3��DS1302 ���Դ���XXXX,����Ű�������\n");
          DelayS(1);
		  printf("4�����8λ������Ƿ�ȫ��\n");
		  DelayS(1);
		  printf("5��J9ȫ����GND�����8λLED�Ƿ�ȫ��\n");
		  printf("6��J6ȫ����GND����鵥������������Ƿ�ȫ��\n");
		  DelayS(1);

		  printf("7�������18B20����ȴ���\n");
          DelayS(1);
		  temp=ReadTemperature();//��ȡ�¶�
          DelayS(1);
          temp=ReadTemperature();//�ڶ��ζ�ȡ�¶ȣ��Ա�ȡ����ȷֵ
		  printf("   �����¶� %7.3f ��\n",(float)temp*0.0625);
          DelayS(1);
	      Ds1302_Read_Time(); //����ʱ��
		  printf("   ���� %02d-%02d-%02d ����%d\n",(int)time_buf1[1],(int)time_buf1[2],(int)time_buf1[3],(int)time_buf1[7]);//������
          printf("   ʱ�� %02d:%02d:%02d\n",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//ʱ����
		  DelayS(1);
          printf("8������P0�ڵ�J24���밴������󰴼���\n");
		  DelayS(1);
		  printf("9���������󣬼���Ƿ�ȫ��������\n");
		  printf("   �����������ε����󣬷�ֹ���Ĺ���\n");
          
	   while(1)
	        {
          
          keynum=KeyPro(); //��ȡ������̵�ֵ
          if(keynum!=0xff) //�ж���0~15 ��Чֵ
		    {
		    printf("������̼�ֵ�ǣ�%02d\n",(int)keynum);//2λ������ʾ��Чֵ
			Ds1302_Read_Time(); //����ʱ��
            printf("ʱ�� %02d:%02d:%02d\n",(int)time_buf1[4],(int)time_buf1[5],(int)time_buf1[6]);//ʱ����
            DelayMs(50);
			}
			}
	 }
}
/*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 TH0=(65536-2000)/256;		  //���¸�ֵ 2ms
 TL0=(65536-2000)%256;
 
 LED8x8Display();

}
