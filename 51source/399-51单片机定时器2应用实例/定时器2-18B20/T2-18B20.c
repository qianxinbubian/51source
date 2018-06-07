#include <reg52.h>
#include <intrins.h>

sbit DQ   = P3^3;       //����DS18B20�˿�DQ
sbit BEEP = P3^6;       //������������ƶ˿�

bit presence;           //���DS18B20��־λ

unsigned char code LEDData[] =
{
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 
  0xF8, 0x80, 0x90, 0xff, 0xc6, 0x9c, 0xbf
};

unsigned char  shift,n = 0;

unsigned char data temp_data[2]; //�¶����ݴ�ŵ�Ԫ
unsigned char data display[7];   //�Դ浥Ԫ


/**********************************************************

us��ʱ�Ӻ���  (8*1.085)*num

**********************************************************/
void Delay(unsigned int num) //��ʱ����
{
   while (--num)
    ;
}

/**********************************************************

ms��ʱ�Ӻ���

**********************************************************/
void delayms(unsigned int ms)
{
   unsigned char k;
   while (ms--)
   {
     for (k = 0; k < 114; k++)
      ;
   }
}

/**********************************************************

�����������Ӻ���

**********************************************************/
void beep()
{
   unsigned char i;
   for (i = 0; i < 180; i++)
   {
     Delay(80);
     BEEP = !BEEP;    //BEEPȡ��
   }
   BEEP = 1;          //�رշ�����
   delayms(100);      //��ʱ100ms
}

/**********************************************************

DS18B20��ʼ���Ӻ���

presence=0  OK  presence=1  ERROR

**********************************************************/
unsigned char Init_DS18B20(void)
{
   DQ = 0;            //��Ƭ�������͵�ƽ��λ�ź�
   Delay(60);         //��ʱ>480us
   DQ = 1;            //�ͷ�������
   Delay(8);          //��ʱ>64us,�ȴ�Ӧ��

   presence = DQ;     //����Ӧ���ź�
   Delay(50);         //��ʱ>400us,�ȴ������߳��ָߵ�ƽ
   DQ = 1;            //�ͷ�������
   
   return (presence); //����presence�ź�
}

/**********************************************************

��һ���ֽ������Ӻ���

**********************************************************/
unsigned char ReadOneChar(void)
{
  unsigned char i;
  unsigned char dat = 0;

  EA = 0;                  //��ȡ�¶�����ʱ��Ҫ�ر��жϡ�
  DQ = 1;
  for (i = 0; i < 8; i++)
  {
     DQ = 0;                //���������ź�
     dat >>= 1;
     DQ = 1;                //�ͷ�����
     _nop_();
     _nop_();
     if(DQ)               //�����ߵ�ƽ״̬
     dat |= 0x80;          //���λ��1
     Delay(6);             //��ʱ>45us
     DQ = 1;               //�ͷ�����,��ʾ�˴ζ��������
   }
   EA = 1;                 //���ж�
   return (dat);           //��������������
}

/**********************************************************

дһ���ֽ������Ӻ���

**********************************************************/
void WriteOneChar(unsigned char dat)
{
   unsigned char i;
  
   for (i = 0; i < 8; i++)  //һ���ֽ�8��bit
   {
     DQ = 0;                //���������ź�
     Delay(1);              //��ʱ<15us
     DQ = (bit)(dat&0x01);  //д1bit����
     dat >>= 1;             //��������һλ
     Delay(6);              //��ʱ>45us
     DQ = 1;                //�ͷ�����,��ʾ�˴�д�������
   }
}

/**********************************************************

�¶�����ת���Ӻ���

**********************************************************/
void Temperature_conver()
{
   bit minus = 0;

   display[0]=0x0b;                //��ʾ C
   display[1]=0x0c;                //��ʾ �� 

   if (temp_data[1] > 127)         //�¶�Ϊ��ֵ
   {
     temp_data[0] = (~temp_data[0]) + 1; //ȡ����һ����������ԭ��
     if ((~temp_data[0]) >= 0xff)
      temp_data[1] = (~temp_data[1]) + 1;
     else
      temp_data[1] = ~temp_data[1];
     minus = 1;                         //�¶�Ϊ��ֵ��־
   }

   display[6] = temp_data[0] &0x0f;     //ȡС��λ����
   display[2] = (display[6] *10) / 16;  //����һλС��

   display[6] = ((temp_data[0] &0xf0) >> 4)|((temp_data[1] &0x0f) << 4); //ȡ����
    
   display[5] = display[6] / 100;        //��λ
   display[4] = (display[6] % 100) / 10; //ʮλ
   display[3] = display[6] % 10;         //��λ

   if (!display[5])
   {
     display[5] = 0x0a;   //��λΪ0,����ʾ
     if (!display[4])
     display[4] = 0x0a;   //�θ�λΪ0,����ʾ
   }

   if (minus)
   {
     display[5] = 0x0d;   //��ʾ����
   }
}

/**********************************************************

������

**********************************************************/
void main(void)
{
   P0 = 0xff;
   P2 = 0xff;
  
   T2CON = 0x00;           //����T2CON�Ĵ���       
   TH2 = 0xfc;             //1ms��ʱ����
   TL2 = 0x66;
   ET2 = 1;                //����Timer2�ж�
   EA = 1;                 //���ж�����
   TR2 = 1;                //������ʱ��2
   shift = 0x7f;           //��λ���ֵ

   while (1)
   {
    Init_DS18B20();
    if (presence == 0)
    {
      WriteOneChar(0xCC);  //����ROMƥ�����
      WriteOneChar(0x44);  //�����¶�ת��
      delayms(2);
    }

    Init_DS18B20();
    if (presence == 0)
    {
      WriteOneChar(0xCC);  //����ROMƥ�����
      WriteOneChar(0xBE);  //��ȡ�¶ȼĴ���

      temp_data[0] = ReadOneChar(); //���¶ȵ�8λ
      temp_data[1] = ReadOneChar(); //���¶ȸ�8λ
      
      Temperature_conver();         //�¶�����ת��
      delayms(200);
    }
    else
    {
      beep(); //����������    
    }
   }
}

/**********************************************************

  Timer2�жϷ����Ӻ��� �������������ʾ��

**********************************************************/
void  timer2(void) interrupt 5 
{ 
   TF2 = 0;                    //�ֹ����жϱ�־  
   TH2 = 0xfc;                 //1ms��ʱ����
   TL2 = 0x66;

   if (presence)               //���DS18B20������
   {
     P2 = 0xff;                //�ر���ʾ
     P0 = 0xff;
     return;                   //�˳��ж� 
   }

   if(n >= 6)
   {
     n = 0;                    //�жϼ�����Ԫ����
     shift = 0x7f;             //��λ���ֵ
     P2 = 0xff;                //�ر��������ʾ 
   }
   else
   {
      if (n == 3)
      P0 = (LEDData[display[n]]) &0x7f; //��С������ʾ    
      else
      P0 = LEDData[display[n]]; //�Ͷ���

      P2 = shift;               //��λ��
      shift = shift>>1|0x80;    //�޸�λ��
      n++;                      //����ֵ��1
   }
}

/*********************************************************/
