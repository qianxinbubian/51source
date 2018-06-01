#ifndef _DS1302
#define _DS1302
#define uchar unsigned char
//�Ĵ����궨��
#define WRITE_SECOND              	0x80
#define WRITE_MINUTE        			0x82
#define WRITE_HOUR                  0x84
#define READ_SECOND                	0x81
#define READ_MINUTE                 0x83
#define READ_HOUR                  	0x85
#define WRITE_PROTECT             	0x8E
 
//λѰַ�Ĵ�������
sbit ACC_7 = ACC^7;
sbit SCLK = P1^0;        // DS1302ʱ���ź�         7��
sbit DIO= P1^1;          // DS1302�����ź�         6��
sbit CE = P1^2;          // DS1302Ƭѡ                 5��
 
//��ַ�����ݷ����ӳ���
void Write1302 ( unsigned char addr,dat )    
{
	unsigned char i,temp;
	CE=0;         //CE����Ϊ�ͣ����ݴ�����ֹ
	SCLK=0;       //����ʱ������
	CE = 1;       //CE����Ϊ�ߣ��߼�������Ч
	//���͵�ַ
	for ( i=8; i>0; i-- ) //ѭ��8����λ
	{    
		SCLK = 0;
		temp = addr;
		DIO = (bit)(temp&0x01); //ÿ�δ�����ֽ�
		addr >>= 1;             //����һλ
		SCLK = 1;
	}
	
	//��������
	for ( i=8; i>0; i-- )
	{    
	     SCLK = 0;
	     temp = dat;
	     DIO = (bit)(temp&0x01);         
	     dat >>= 1;                  
	     SCLK = 1;
	}
	SCLK=1;
	CE = 0;        
}
 
//���ݶ�ȡ�ӳ���
unsigned char Read1302 ( unsigned char addr )
{
       unsigned char i,temp,dat1,dat2;
       CE=0;          
       SCLK=0;            
       CE = 1; 
       //���͵�ַ
       for ( i=8; i>0; i-- )                      //ѭ��8����λ
       {    
              SCLK = 0;
              temp = addr;
              DIO = (bit)(temp&0x01);          //ÿ�δ�����ֽ�
              addr >>= 1;                              //����һλ
              SCLK = 1;
       }
       //��ȡ����
       for ( i=8; i>0; i-- )
       {
              ACC_7=DIO;
              SCLK = 1;
             ACC>>=1;
              SCLK = 0;
       }
		SCLK=1;    
       CE=0;
       dat1=ACC;
       dat2=dat1/16;                           //���ݽ���ת��
       dat1=dat1%16;                         //ʮ������תʮ����
       dat1=dat1+dat2*10;
       return (dat1);
}
 
//��ʼ��DS1302(ѡ����ʹ��)
void init1302(void)   
{
	
	Write1302 (WRITE_PROTECT,0X00);  //��ֹд����
	Write1302 (WRITE_SECOND,0x50);   //��λ��ʼ��
	Write1302 (WRITE_MINUTE,0x59);   //���ӳ�ʼ��
	Write1302 (WRITE_HOUR,0x23);     //Сʱ��ʼ��
	//Write1302 (0x80,0x80);      //ʱ����ͣ
	//Write1302 (0x86,0x13);		//�ճ�ʼ��
	//Write1302 (0x88,0x03);		//�³�ʼ��
	//Write1302 (0x8c,0x09);		//���ʼ��
	Write1302 (WRITE_PROTECT,0x80);  //����д����
}
#endif

/*������Read1302��0x81��
0x81,0x83,0x85,0x87,0x89,0x8d
��    ��   ʱ   ��   ��   ��
*/
/*Write1302 (0x80,0x80);      //ʱ����ͣ
Read1302()������������ʮ����������Write1302()д������������16���Ƶ�
*/


