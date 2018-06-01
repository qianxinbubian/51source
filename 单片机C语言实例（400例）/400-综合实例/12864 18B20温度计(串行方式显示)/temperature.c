#include <reg52.h>
#include "temp.h"
/**********************************************************
*����DS1820+YM12864R��ȷ��4λС�����¶ȼ�
*�������ָ��!
*���:����
*�汾:V0.3
*DS1820����Ϊ12λ����
**********************************************************/
//�����ⲿ�¶ȶ�������
extern unsigned char data temperature[2];
//��ŷֽ��7��ASCII���¶�����  
unsigned char temp[7];

unsigned char code string[6]={"�¶�:"};
unsigned char code DispStr[7]={"ϵ�к�"};
unsigned char fCode[8]; //������к�,CRC 
/**********************************************************
*������
**********************************************************/
void main()
{

	unsigned char i;
	delay100us();//�ȴ���Դ�ȶ�,Һ����λ���
	delay100us();
	LCD_INIT();	 //Һ����ʼ��
	DS1820_Reset();//DS1820��λ
	DS1820_WriteData(0x33);//��ROMָ��
	for (i=0;i<8;i++)
		{//��ȡ64λϵ�к�
		fCode[i] = DS1820_ReadData();
		}
	DS1820_Reset();//DS1820��λ
	LCD_WRITE_IR(0x80);//�ڵ�һ����ʾ
	for (i=0;i<5;i++)
		{
		 LCD_WRITE_DR(string[i]);//��ʾ"�¶�:"
		}
	LCD_WRITE_IR(0x88);//�ڵ�3����ʾ
	for (i=0;i<6;i++)
		{
		 LCD_WRITE_DR(DispStr[i]);//��ʾ"ϵ�к�"
		}
	DispCode();//�ڵ�4����ʾ���к�
	do{
		read_temp(); //��ȡ�¶�ֵ
		delay100us();
		display();	 //��ʾ�¶�ֵ
		delay100us();
	 }while (1);	//����ѭ��
}
/**********************************************************
*ϵ�к���ʾ�ӳ���
*��ʾ˳��(������)8λCRC����->48λ���к�->8λ��Ʒϵ�б���
**********************************************************/
void DispCode()
{
 unsigned char i,temp;
 LCD_WRITE_IR(0x98); //�ڵ�4����ʾ
 for (i=8;i>0;i--)
 	{
	//��ʾ��4λ
	temp = fCode[i-1]>>4;
		if (temp<10) //ASCII��ת��
			 LCD_WRITE_DR(temp += 0x30);
		else
			 LCD_WRITE_DR(temp += 0x37);
	//��ʾ��4λ
 	temp = fCode[i-1]&0x0f;
		if (temp<10) //ASCII��ת��
			 LCD_WRITE_DR(temp += 0x30);
		else
			 LCD_WRITE_DR(temp += 0x37);
	 }
}
/**********************************************************
*��ʱ�ӳ���
**********************************************************/
void delay100us()
{
 unsigned char i;
 for (i=49;i>0;i--);
}
/**********************************************************
*ת����ʾ�ӳ���
**********************************************************/
void display()
{
 unsigned char temp_data,temp_data_2;
 unsigned int TempDec;	//�������4λС��
 temp_data = temperature[1];
 temp_data &= 0xf0; 	//ȡ��4λ
 if (temp_data==0xf0)	//�ж������¶Ȼ��Ǹ��¶ȶ���
 	{
	//���¶ȶ�����,ȡ����1,�жϵ�8λ�Ƿ��н�λ
		if (temperature[0]==0)
			{	//�н�λ,��8λȡ����1
			 	temperature[0]=~temperature[0]+1;
				temperature[1]=~temperature[1]+1;
			}
		else
			{	//û��λ,��8λ����1
				temperature[0]=~temperature[0]+1;
				temperature[1]=~temperature[1];
			}
	 }
	temp_data = temperature[1]<<4;		//ȡ���ֽڵ�4λ(�¶ȶ�����4λ)
	temp_data_2 = temperature[0]>>4;	//ȡ���ֽڸ�4λ(�¶ȶ�����4λ)
	temp_data = temp_data|temp_data_2;	//��ϳ���������
	temp[0] = temp_data/100+0x30; 		//ȡ��λת��ΪASCII��
	temp[1] = (temp_data%100)/10+0x30; 	//ȡʮλת��ΪASCII��
	temp[2] = (temp_data%100)%10+0x30; 	//ȡ��λת��ΪASCII��
	temperature[0]&=0x0f; 				//ȡС��λת��ΪASCII��
	TempDec = ((temperature[0]&0x0f)*0.0625)*10000;//С������,����1��
	temp[3]	= TempDec/1000+0x30; 		//ȡС����λת��ΪASCII��
	temp[4]	= (TempDec%1000)/100+0x30; 	//ȡС��ʮλת��ΪASCII��
	temp[5]	= ((TempDec%1000)%100)/10+0x30;//ȡС����λת��ΪASCII��
	temp[6] = ((TempDec%1000)%100)%10+0x30;//ȡС��ǧλת��ΪASCII��
	LCD_WRITE_IR(0x91);//��LCD��2����ʾ
	LCD_WRITE_DR(temp[0]); 	//��ʾ��λ
	LCD_WRITE_DR(temp[1]); 	//��ʾʮλ
	LCD_WRITE_DR(temp[2]); 	//��ʾ��λ
	LCD_WRITE_DR(0x2e);		//��ʾ"."
	LCD_WRITE_DR(temp[3]);	//��ʾС��λ
	LCD_WRITE_DR(temp[4]);	//��ʾС��λ
	LCD_WRITE_DR(temp[5]);	//��ʾС��λ
	LCD_WRITE_DR(temp[6]);	//��ʾС��λ
	LCD_WRITE_DR(0xa1);		//��ʾ"��"
	LCD_WRITE_DR(0xe6);
}
