#include<stdio.h>
//�����������Ӷ˿�
//1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK

void ShowString (unsigned char line,char *ptr);

void ShowChar(unsigned char pos,unsigned char c);

void DelayMs(unsigned char ms);// Ms  ��ʱ

void DelayUs(unsigned char us);// us  ��ʱ

void InitLcd();//��ʼ��

void WriteCommand(unsigned char c);//д����

