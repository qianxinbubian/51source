/********************************************************************
* �ļ���  �� ������ʾ.c
* ����    :  ��Ҫ������J16�������ñ����������ڵ�������ʾ��Ӧ��ͼ�Ρ�
* ������  �� ������2009��4��8��
* �汾��  �� 2.0
***********************************************************************/
#include <reg52.H>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
//������ʾ
//unsigned char code tabP0[]={0x30,0x48,0x44,0x22,0x44,0x48,0x30,0x00};
//unsigned char code tabP2[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};

//Բ����ʾ
unsigned char code tabP0[]={0x00,0x3c,0x42,0x42,0x42,0x42,0x3c,0x00};
unsigned char code tabP2[]={0x7F,0xBF,0x0DF,0x0EF,0x0F7,0x0FB,0x0FD,0x0FE};

//������ʾ
//unsigned char code tabP0[]={0x00,0x08,0x14,0x22,0x41,0x22,0x14,0x08};
//unsigned char code tabP2[]={0x7F,0xBF,0x0DF,0x0EF,0x0F7,0x0FB,0x0FD,0x0FE};

//������ʾ
//unsigned char code tabP0[]={0x00,0x41,0x22,0x14,0x08,0x14,0x22,0x41};
//unsigned char code tabP2[]={0x7F,0xBF,0x0DF,0x0EF,0x0F7,0x0FB,0x0FD,0x0FE};

//������ʾ
//unsigned char code tabP0[]={0x00,0x1e,0x12,0x12,0x7f,0x12,0x12,0x1e};
//unsigned char code tabP2[]={0x7F,0xBF,0x0DF,0x0EF,0x0F7,0x0FB,0x0FD,0x0FE};

/********************************************************************
* ���� : Delay_1ms()
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms * x
* ���� : x (��ʱһ����ĸ���)
* ��� : ��
***********************************************************************/
void Delay_1ms(uchar i)//1ms��ʱ
{
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}
/********************************************************************
* ���� : Main()
* ���� : ������
* ���� : ��
* ��� : ��
***********************************************************************/
void main()
{
	int i;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			P0=0;
			P2=tabP2[i];
			P0=tabP0[i];
			Delay_1ms(2);
		}		
	}
}
