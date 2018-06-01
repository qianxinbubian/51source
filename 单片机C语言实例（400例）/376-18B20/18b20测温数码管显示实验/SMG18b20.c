
//DS18B20�Ķ�д����,���ݽ�P3.3                     //
//�¶ȴ�����18B20������,��������Ĭ�ϵ�12λת��   //
//���ת��ʱ��750΢��,��ʾ�¶�-55��+125��,��ʾ���� //
//Ϊ0.1�ȣ���ʾ����4λLED������ʾ����ֵ            //
//P0��Ϊ��������,P24~P27Ϊλѡ                       //
/***************************************************/

#include "reg51.h"
#include "intrins.h"                            //_nop_();��ʱ������
#define  Disdata P0                             //���������
#define  discan  P2                             //ɨ���
#define  uchar unsigned char
#define  uint  unsigned int
sbit     DQ=P3^3;                               //�¶������
sbit     DIN=P0^7;                              //LEDС�������
uint     h; 
uchar flag;
//**************�¶�С�������ò��***********//
uchar code ditab[16]=
{0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};
//
uchar code dis_7[12]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
//����LED�����        "0"  "1"  "2"  "3"  "4"  "5"  "6"  "7"  "8"  "9" "����" "-"              
uchar code scan_con[4]={0x7f,0xbf,0xdf,0xef};    //��ɨ�������
uchar data temp_data[2]={0x00,0x00};             //�����¶��ݷ�
uchar data display[5]={0x00,0x00,0x00,0x00,0x00};     //��ʾ��Ԫ���ݣ���4�����ݺ�һ����������

//
//
//
/***********11΢����ʱ����**********/
//
void delay(uint t)
{
for(;t>0;t--);
}
//
/***********��ʾɨ�躯��**********/
scan()
{
char k;
    for(k=0;k<4;k++)         //��λLEDɨ�����
     {
	 Disdata=0xff;
      Disdata=dis_7[display[k]];
      if(k==1){DIN=0;}
      discan=scan_con[k];delay(90);
	  discan=0xff;
     }
}
//
//
/***********18B20��λ����**********/
ow_reset(void)
{
char presence=1;
while(presence)
{
while(presence)
{
DQ=1;_nop_();_nop_();
DQ=0;    //
delay(50); // 550us
DQ=1;    // 
delay(6); // 66us
presence=DQ; // presence=0������һ��
   }
delay(45);    //��ʱ500us
presence = ~DQ;
}
DQ=1;
} 
//
//
/**********18B20д�����*********/
//�� 1-WIRE ������дһ���ֽ�
void write_byte(uchar val)
{
uchar i;
for (i=8; i>0; i--) //
{
DQ=1;_nop_();_nop_(); 
DQ = 0;_nop_();_nop_();_nop_();_nop_();_nop_();//5us
DQ = val&0x01;      //���λ�Ƴ�
delay(6);           //66us
val=val/2;          //����һλ
}
DQ = 1;
delay(1); 
}
//
/*********18B20��1���ֽں���********/
//�������϶�ȡһ���ֽ�
uchar read_byte(void)
{
uchar i;
uchar value = 0;
for (i=8;i>0;i--)
{
DQ=1;_nop_();_nop_();
value>>=1;
DQ = 0;             //
_nop_();_nop_();_nop_();_nop_();   //4us
DQ = 1;_nop_();_nop_();_nop_();_nop_();   //4us 
if(DQ)value|=0x80;
delay(6);           //66us
}
DQ=1;
return(value);
}
//
/***********�����¶Ⱥ���**********/
//
read_temp()
{
ow_reset();       //���߸�λ
write_byte(0xCC); // ��Skip ROM����
write_byte(0xBE); // ��������
temp_data[0]=read_byte(); //�¶ȵ�8λ
temp_data[1]=read_byte(); //�¶ȸ�8λ
ow_reset();
write_byte(0xCC); // Skip ROM
write_byte(0x44); // ��ת������
}
//
/***********�¶����ݴ�����**********/
void work_temp()
{
uchar n=0;
uchar doth,dotl;
uchar flag3=1,flag2=1;       //������ʾ�������
if((temp_data[1]&0xf8)!=0x00)
{
   temp_data[1]=~(temp_data[1]);
   temp_data[0]=~(temp_data[0])+1;
   n=1;
   flag=1;
}//���¶�����
if(temp_data[0]>255)
{
temp_data[1]++; 
}
display[4]=temp_data[0]&0x0f;
display[0]=ditab[display[4]];
doth=display[0]/10;
dotl=display[0]%10;
display[4]=((temp_data[0]&0xf0)>>4)|((temp_data[1]&0x07)<<4);
display[3]=display[4]/100;
display[2]=display[4]/10%10;
display[1]=display[4]%10;
if(!display[3])
   {
    display[3]=0x0a;
    flag3=0;
    if(!display[2])
    {
     display[2]=0x0a;
     flag2=0;
    }
   }//���λΪ0ʱ������ʾ
if(n)
{
   display[3]=0x0b;//���¶�ʱ���λ��ʾ"-"
   flag3=0;
}
}

// 
//
/**************������****************/
main()
{
Disdata=0xff;    //��ʼ���˿�
discan=0xff;
for(h=0;h<4;h++){display[h]=8;}//������ʾ8888
ow_reset();       // ������ת��һ��
write_byte(0xCC); // Skip ROM
write_byte(0x44); // ��ת������
for(h=0;h<500;h++)
   {scan();}          //������ʾ"8888"2��
while(1)
{
read_temp();         //����18B20�¶�����
work_temp();         //�����¶�����

   scan();        //��ʾ�¶�ֵ2��
}
}
//
//*********************����**************************//
