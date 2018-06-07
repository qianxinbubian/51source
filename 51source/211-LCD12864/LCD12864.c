/*****************************************************************************************************************

���ߣ��ŵ���

���ڣ�2007��7��26��

��Ƭ���ͺţ�ATMEGA16��L��

Ƶ�ʣ�8M HZ

���ܣ�LCD12864 �ӳ������

******************************************************************************************************************
PA0  ��  RS

PA1  ��  RW

PA2  ��  E

PA3  ��  CSA

PA4  ��  CSB

RST  ֱ�ӽӵ�VCC��������

PC0-7 ��DB0-7  

******************************************************************************************************************/
#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>

#define RS1    PORTA|=_BV(0);
#define RS0    PORTA&=~_BV(0);

#define RW1    PORTA|=_BV(1);
#define RW0    PORTA&=~_BV(1);

#define EN1    PORTA|=_BV(2);
#define EN0    PORTA&=~_BV(2);

#define CSB1   PORTA=_BV(3);
#define CSB0   PORTA&=~_BV(3);

#define CSA1   PORTA|=_BV(4);
#define CSA0   PORTA&=~_BV(4);

#define LCD_ON           0x3f  //��ʾ��
#define LCD_OFF          0x3e  //��ʾ��
#define BEGIN_LINE_SET   0xc0  //��ʾ��ʼ������Ϊ��0��
#define PAGE_SET         0xb8  //ҳ��ַ����
#define Y_ADDRESS_SET    0x40  //�е�ַ����



unsigned char x;




/******************************************************************************************************************/
const unsigned char char_table[]PROGMEM={

0x80,0x40,0xF0,0x2C,0x43,0x20,0x98,0x0F,0x0A,0xE8,0x08,0x88,0x28,0x1C,0x08,0x00,
0x00,0x00,0x7F,0x00,0x10,0x0C,0x03,0x21,0x40,0x3F,0x00,0x00,0x03,0x1C,0x08,0x00,//��0
0x10,0x10,0xF0,0x1F,0x10,0xF0,0x80,0x82,0x82,0x82,0xF2,0x8A,0x86,0x82,0x80,0x00,
0x80,0x43,0x22,0x14,0x0C,0x73,0x20,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,//��1
0x00,0xFC,0x04,0x04,0xFC,0x00,0x82,0xFA,0x82,0x82,0x82,0x82,0xFE,0x80,0x00,0x00,
0x00,0x07,0x02,0x02,0x07,0x00,0x04,0x05,0x04,0x04,0x04,0x44,0x80,0x7F,0x00,0x00,//��2
0x00,0x60,0x10,0x08,0x08,0x90,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//��3











/*��ʱ*****************************************************************************************************************/
void delay(unsigned int t)
{while(t--){_delay_ms(1);}}


/*дָ��*****************************************************************************************************************/
void Command_Input(unsigned char CSA_B, unsigned char Command)// ( Ƭѡ��ָ����� )
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 ѡ��A��
if(CSA_B==1){CSA1;CSB0;}//CSB=0 ѡ��B��
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 ͬʱѡ��AB��

DDRC=0x00;PORTC=0XFF;       //�˿�����Ϊ����

RS0 RW1 EN1      			// RW=E=1 RS=0  ��״̬
while(PINC&0x80); 			// æ�ȴ�

RS0 RW0 EN1          		// RS=RW=0 E=1  дָ��      
PORTC=Command; DDRC=0XFF;	// ��ָ������͵�����
EN0       					// E=0     E�½��ӽ�ָ�����д��LCD
}
/******************************************************************************************************************/

/*д����*****************************************************************************************************************/
void Data_Input(unsigned char CSA_B, unsigned char Data)  //( Ƭѡ������ )
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 ѡ��A��
if(CSA_B==1){CSA1;CSB0;}//CSB=0 ѡ��B��
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 ͬʱѡ��AB��

DDRC=0x00;PORTC=0XFF; 	//�˿�����Ϊ����

RS0 RW1 EN1      		// RW=E=1  RS=0  ��״̬
while(PINC&0x80); 		// æ�ȴ�

RS1 RW0 EN1            	// RS=E=1  RW=0  д����      
PORTC=Data;DDRC=0XFF;  	// �����ݴ����͵�����
EN0     				// E=0     E�½��ӽ����ݴ���д��LCD
}
/******************************************************************************************************************/

/*������*****************************************************************************************************************/
unsigned char Data_Output(unsigned char CSA_B)//( Ƭѡ ) ��������
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 ѡ��A��
if(CSA_B==1){CSA1;CSB0;}//CSB=0 ѡ��B��
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 ͬʱѡ��AB��

DDRC=0x00;PORTC=0XFF;	//�˿�����Ϊ����

RS0 RW1 EN1      		// RS=0  RW=E=1  ��״̬
while(PINC&0x80); 		// æ�ȴ�

RS1 RW1 EN1            	// RS=E=1  RW=0  д����  
    
EN1 EN1 EN1 EN1  EN0 EN0 EN0 EN0 EN0

return  PINC;     		//���ض���������
}
/******************************************************************************************************************/

/*����*****************************************************************************************************************/
void Clear_Screen(void)
{
unsigned char page,n;

for(page=7;page<8;page--)
	{	
	Command_Input(0,PAGE_SET|page);//����ҵ��ַ7-0
	Command_Input(0,Y_ADDRESS_SET);//�����е�ַΪ0
    for(n=63;n<64;n--)Data_Input(0,0x00);//д����0x00

	Command_Input(1,PAGE_SET|page);//����ҵ��ַ7-0
	Command_Input(1,Y_ADDRESS_SET);//�����е�ַΪ0
	for(n=63;n<64;n--)Data_Input(1,0x00);//д����0x00
	}
}
/******************************************************************************************************************/

/*��ʾһ������  ����1*****************************************************************************************************************/
void Show_One_Word(unsigned char X_Address,unsigned char Y_Address,unsigned char Char_Address)//(0-111, 0-6 , 0-255 )ԭ�������Ͻ�
{
unsigned int  char_table_address;	//�������ֱ��е�λ��
unsigned char a_x_address;			//��a���Ŀ�ʼ�е�ַ
unsigned char b_x_address;			//��b���Ŀ�ʼ�е�ַ
unsigned char a_n;					//��a�������ݸ���
unsigned char b_n;					//��b�������ݸ���
unsigned char n;  					//ѭ������


char_table_address = Char_Address*32;//ȷ���������ֱ��е��׵�ַ��һ������32�ֽ�

if(X_Address<=48)//������a��
	{
	 a_x_address=X_Address; 
	 a_n=16;b_n=0;
	 }

if(X_Address>=64)//������b��
    {
	 b_x_address=X_Address-64; 
	 a_n=0;b_n=16;
	 }

if((48<X_Address)&&(X_Address<64))//������a����b��֮��
	{
	 a_x_address=X_Address; 
	 b_x_address=0; 
	 a_n=64-X_Address;
	 b_n=X_Address-48;
     }

Command_Input(0,PAGE_SET|Y_Address);		//����A��ҵ��ַΪY_Address 
Command_Input(0,Y_ADDRESS_SET|a_x_address);	//����A���е�ַΪa_x_address

for(n=a_n ; n>0 ; n--)
	{
	Data_Input(0,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(1,PAGE_SET|Y_Address);		//����B��ҵ��ַΪY_Address
Command_Input(1,Y_ADDRESS_SET|b_x_address); //����B���е�ַΪb_x_address

for(n=b_n ; n>0 ; n--)
	{
	Data_Input(1,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(0,PAGE_SET|(Y_Address+1));	//����A��ҵ��ַΪY_Address+1
Command_Input(0,Y_ADDRESS_SET|a_x_address);	//����A���е�ַΪa_x_address

for(n=a_n ; n>0 ; n--)
	{
	Data_Input(0,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(1,PAGE_SET|(Y_Address+1));	//����B��ҵ��ַΪY_Address+1 
Command_Input(1,Y_ADDRESS_SET|b_x_address); //����B���е�ַΪb_x_address

for(n=b_n ; n>0 ; n--)
	{
	Data_Input(1,pgm_read_byte(char_table+char_table_address++));
	}

}
/******************************************************************************************************************/


///////////////////////////////////////////////////////////////
int main(void)
{
DDRA=0xff;

Command_Input(0,LCD_ON);         //a ������ʾ
Command_Input(1,LCD_ON);         //b ������ʾ

Command_Input(0,BEGIN_LINE_SET); //a ����ʾ��ʼ��Ϊ0
Command_Input(1,BEGIN_LINE_SET); //b ����ʾ��ʼ��Ϊ0

Clear_Screen();                  //���� 


Show_One_Word( 0,4,0);
Show_One_Word(16,4,1);
Show_One_Word(32,4,2);
Show_One_Word(48,4,3);


while(1);


}
