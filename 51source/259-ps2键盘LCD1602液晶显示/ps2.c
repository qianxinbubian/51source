/*-----------------------------------------------
˵�����˳���ʹ�ñ�׼PS2�������룬LCD1602Һ���������ʾ����������������ʹ��
����ʹ��12M����11.0592M,������ʹ�ò�����ĸ�����ֲ��ԣ�������������ʹ�ã���
������������չ�����ڿ�����ͳ���ĸ��ֲ�����������û��ʹ����żУ�飬����֤û��
����,У�������������ӡ�
-------------------------------------------------*/
/*-----------------------------------------------
  ���ƣ�99���ʱ��
  ��˾���Ϻ�������ӿƼ����޹�˾
  ��վ��www.doflye.cn
  ��д��ʦ��
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�ͨ����ʱ����ʱ
------------------------------------------------*/
#include<reg52.h>        //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include "scancodes.h"
#include "LCD1602.h"

sbit Key_Data = P3^0 ;   //����Keyboard����
sbit Key_CLK =P3^2;      //ʹ���ж�

bit BF;          
bit Shift;               //�����ϵ�����־
bit Key_UP;              //����ͨ������־
unsigned char KeyV;
unsigned char IntNum;
unsigned char DisNum;
/******************************************************************/
/*                    ��������                                    */
/******************************************************************/
void Decode(unsigned char ScanCode);  //�����ӳ���
/******************************************************************/
/*                    ������                                      */
/******************************************************************/
void main()
{

IT1 = 0;                              //���ⲿ�ж�1Ϊ�͵�ƽ����
EA = 1;                               //�ⲿ�жϿ�
EX0 = 1;                              //���ж�

InitLcd();                            //��ʼ��1602Һ����

do
 {
 if (BF)
   Decode(KeyV);
 else
   EA = 1; //���ж�
 }
while(1);
}
/******************************************************************/
/*                    �ⲿ�ж϶�����Ϣ                            */
/******************************************************************/
void Keyboard_out(void) interrupt 0
{
if ((IntNum > 0) && (IntNum < 9))
   { 
	KeyV = KeyV >> 1;                //����������ǵ�>>�ߣ������һ����������һλ
	if (Key_Data)
    	KeyV = KeyV | 0x80;          //������������Ϊ1ʱ�����λ
	}
	IntNum++;
	while (!Key_CLK);                //�ȴ�PS/2CLK����

	if (IntNum > 10)
	   { 
 		IntNum = 0;                  //���ж�11�κ��ʾһ֡�������꣬�����׼����һ�ν���
 		BF = 1;                      //��ʶ���ַ���������
		EA = 0;                      //���жϵ���ʾ����ٿ��ж� 
		}
}

/******************************************************************/
/*                    ������Ϣ                                    */
/******************************************************************/
void Decode(unsigned char ScanCode) //ע��:��SHIFT+GΪ12H 34H F0H 34H F0H 12H��Ҳ����˵shift��ͨ��+G��ͨ��+shift�Ķ���+G�Ķ���
{
unsigned char TempCyc; 

if (!Key_UP)                        //�����̰���ʱ
	{
	switch (ScanCode)
			{
			case 0xF0 :             // ���յ�0xF0��Key_UP��1��ʾ���뿪ʼ
				Key_UP = 1;
				break;

			case 0x12 :             // �� SHIFT
				Shift = 1;
				break;

			case 0x59 :             // �� SHIFT
				Shift = 1;
				break;

			default: 
				if(!Shift)          //���SHIFTû����
					{
					for (TempCyc = 0;(UnShifted[TempCyc][0]!=ScanCode)&&(TempCyc<59); TempCyc++); //�����ʾ
					if (UnShifted[TempCyc][0] == ScanCode)
						{
   						ShowChar(DisNum,UnShifted[TempCyc][1]);
   						DisNum++;
   						if(DisNum==32)
     					  {
	  					  WriteCommand(0x01);//����
	  					  DisNum=0;//��ͷд����
	 				      }
 					     }
                     } 
				else //����SHIFT
                    {
					for(TempCyc = 0; (Shifted[TempCyc][0]!=ScanCode)&&(TempCyc<59); TempCyc++); //�����ʾ
					if (Shifted[TempCyc][0] == ScanCode)
                       {
  						ShowChar(DisNum,Shifted[TempCyc][1]);
   					    DisNum++;
                        if(DisNum==32)
    					  {
	  				       WriteCommand(0x01);//����
	                       DisNum=0;//��ͷд����
	 					  }
  						}
                      }

					break;
 			}
	}
else
	{ 
	Key_UP = 0;
	switch (ScanCode) //�����ɿ�ʱ���������룬��G 34H F0H 34H ��ô�ڶ���34H���ᱻ����
			{
			case 0x12 : // �� SHIFT
				Shift = 0;
				break;

			case 0x59 : // �� SHIFT
				Shift = 0;
				break;
			}
	}
BF = 0; //��ʶ�ַ���������
} 



