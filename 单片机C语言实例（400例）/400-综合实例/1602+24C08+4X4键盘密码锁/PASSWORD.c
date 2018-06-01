#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
void key_scan();
uchar count0,count1,count3,num,n=0,temp,a,j,count4;
uchar mima[8];							//��ʼ����洢��
uchar tab_key[50];						//��������洢��
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
bit enterflag;							//ȷ�ϼ��������ı�־
bit mimaflag;							//������ȷ���ı�־
bit xiugaiflag;							//�޸������־	
bit enter1flag;						//�޸�����ȷ�ϼ���־	
sbit red=P3^7;
sbit bell=P3^6;
sbit rs=P2^0;
 sbit rw=P2^1;
sbit lcden=P2^2;
sbit scl=P3^4;
sbit sda=P3^5;
uchar code table1[]="input the passco";
uchar code table2[]="de:     --------";
uchar code table3[]="*";
uchar code table4[]="right   (^_^)   ";
uchar code table5[]="first  error!!!!";
uchar code table6[]="second error!!!!";
uchar code table7[]="third error see ";
uchar code table8[]="u tomorrow (^_^)";
uchar code table9[]="define the passc";
uchar code table10[]="ode:    --------";
uchar code table11[]="code   is    new";
//******************************������������*******************************
void delay1()
{ ;; }
void delay2(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
	 for(b=100;b>0;b--);
}
	
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

//****************************e^2room�ĳ�ʼ��*******************************
void start()  //��ʼ�ź�
{	
	sda=1;
	delay1();
	scl=1;
	delay1();
	sda=0;
	delay1();
}

void stop()   //ֹͣ
{
	sda=0;
	delay1();
	scl=1;
	delay1();
	sda=1;
	delay1();
}
//****************************Ӧ���ź�*************************************
void respond()  
{
	uchar i;
	scl=1;
	delay1();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delay1();
}
//*****************************д�ֽڲ�������**********************************
void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;			//�������λ�����Ƶ���λCY
		scl=0;
	    delay1();
		sda=CY;
		delay1();
		scl=1;
		delay1();
	}
	scl=0;
	delay1();
	sda=1;//�����ͷ�
	delay1();
}
//*******************************���ֽڲ�������*****************************
uchar read_byte()
{
	uchar i,k;
	scl=0;
	delay1();
	sda=1;
	delay1();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay1();	
		k=(k<<1)|sda;    //�����㣬�ŵ����λ
		scl=0;
		delay1();	
	}
	return k;
}
//**********************************д��ַ����******************************
void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);
	respond();
	write_byte(address);
	respond();
	write_byte(date);
	respond();
	stop();
}
//*******************************����ַ����*************************************
uchar read_add(uchar address)
{
	uchar date;
	start();
	write_byte(0xa0);
	respond();
	write_byte(address);
	respond();
	start();
	write_byte(0xa1);
	respond();
	date=read_byte();
	stop();
	return date;
}
//****************************LCD1602�ĳ�ʼ��*******************************
void write_com(uchar com)
{
	rs=0;
	lcden=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void write_date(uchar date)
{
	rs=1;
	lcden=0;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

//***************************************����ȽϺ���********************************
bit mimacmp()					
{
	bit flag;
	uchar i;
	for(i=0;i<8;i++)
		{
			if(mima[i]==tab_key[i])
				flag=1;
			else
				{
				flag=0;
				i=8;
				}
		}
	return(flag);							//����flag
}

////**********************************LCD��ʾ������ʼ**************************************
void lcd_display()
{
uchar i=0;
write_com(0x80+0x40+8);
for(i=0;i<n;i++)
{
write_date(table3[0]);
}
}


//****************************************���̹��ܷ��亯��Ⱥ��ʼ****************************
//** 0 ** 1 **2 ** 3**                     
//** 4** 5** 6 **7 **
//**8** 9** ȷ�ϣ�A��  **��Ч��B��
//**ȡ����C��**�޸��������D��**ȷ���޸ļ���E��**��Ч��F��




void key_manage1()
{
tab_key[n]=0;
n++;
if(xiugaiflag==1)
{
mima[count4]=0;
count4++;
}
}

void key_manage2()
{

tab_key[n]=1;
n++;
if(xiugaiflag==1)
{
mima[count4]=1;
count4++;
}
}

void key_manage3()
{

tab_key[n]=2;
n++;
if(xiugaiflag==1)
{
mima[count4]=2;
count4++;
}
}

void key_manage4()
{
tab_key[n]=3;
n++;
if(xiugaiflag==1)
{
mima[count4]=3;
count4++;
}
}

void key_manage5()
{
tab_key[n]=4;
n++;
if(xiugaiflag==1)
{
mima[count4]=4;
count4++;
}
}

void key_manage6()
{
tab_key[n]=5;
n++;
if(xiugaiflag==1)
{
mima[count4]=5;
count4++;
}
}
void key_manage7()
{
tab_key[n]=6;
n++;
if(xiugaiflag==1)
{
mima[count4]=6;
count4++;
}
}
void key_manage8()
{
tab_key[n]=7;
n++;
if(xiugaiflag==1)
{
mima[count4]=7;
count4++;
}
}

void key_manage9()
{
tab_key[n]=8;
n++;
if(xiugaiflag==1)
{
mima[count4]=8;
count4++;
}
}
void key_manage10()
{
tab_key[n]=9;
n++;
if(xiugaiflag==1)
{
mima[count4]=9;
count4++;
}
}
//**********************************ȷ�ϼ�**************************************************************  
void key_manage11()
{
	enterflag=1;					//ȷ�ϼ�����
	if(n==8)					   //ֻ������8�������ȷ�ϲ����Ƚ�
		mimaflag=mimacmp();
	else
		mimaflag=0;
	 if(enterflag==1)
		{
			enterflag=0;
			n=0;
			//��FFFFFFFF����Ѿ����������
			for(count3=0;count3<8;count3++)
			{	
				delay(5);
				tab_key[count3]=0x0f;		
			}		
					
			TR1=1;									//�򿪼�����1
			count1=0;								//��ʱ��1��50MS�ۼƵ�1S���õļ�����
			if(mimaflag==1)
			{	
				a=0;
			
				write_com(0x01);
				write_com(0x80);
				for(count3=0;count3<16;count3++)
				{
					write_date(table4[count3]);	//������ȷ����ʾRIGHT���̵���
					delay(5);
				}
			}
			        
		    else
			{   
			 	n=0;
			 	red=0;
		    	bell=0;
				a++;
				if(a==1)
				{
					for(count3=0;count3<8;count3++) //ffffffff�������
					{	
						delay(5);
						tab_key[count3]=0x0f;		
					}
					write_com(0x01);										
					write_com(0x80);
					for(count3=0;count3<16;count3++)
					{
						write_date(table5[count3]);	//���������ʾ first error�������
						delay(5);
					}
						TR1=1;
				}
				if(a==2)
				{	
					for(count3=0;count3<8;count3++) //ffffffff�������
					{	
						delay(5);
						tab_key[count3]=0x0f;		
					}
						write_com(0x01);
						write_com(0x80);
						for(count3=0;count3<16;count3++)
						{
							write_date(table6[count3]);	//���������ʾSECOND ERROR�������
							delay(5);
						}
							TR1=1;
				}
					
				  if(a==3)
					{	
						for(count3=0;count3<8;count3++) //ffffffff�������
						{	
							delay(5);
							tab_key[count3]=0x0f;		
						}
						write_com(0x01);
						write_com(0x80);
						for(count3=0;count3<16;count3++)
						{
							write_date(table7[count3]);	//���������ʾthird error see�������
							delay(5);
						}
						write_com(0x80+0x40);
						for(count3=0;count3<16;count3++)
						{
							write_date(table8[count3]);//���������ʾ U TOMORROW ,�����
							delay(5);
						}
							TR1=0;
								
					}
							
		}
	}

}
void key_manage12()
{
tab_key[n]=11;
n++;						  //�����������

}
//****************************************************ȡ����********************************************  
void key_manage13()
{

n=0;						  //�����������
write_com(0x80);						//ָ����ָλ��
for(count3=0;count3<16;count3++)
{
	write_date(table1[count3]);		//��һ����ʾINPUT THE PASSPORD:
	delay(5);
}
write_com(0x80+0x40);
for(count3=0;count3<16;count3++)
{
	write_date(table2[count3]);	//������ʾ--------
	delay(5);
	tab_key[count3]=0x0f;			//��FFFFFFFF����Ѿ����������
}

}
//*******************************************�޸������**********************************
void key_manage14()
{
	uchar aa=0;
	n=0;
	xiugaiflag=1;
	write_com(0x01);
	write_com(0x80);
for(count3=0;count3<16;count3++)
{
	write_date(table9[count3]);	//��ʾdefine the password
	delay(5);
	tab_key[count3]=0x0f;			//��FFFFFFFF����Ѿ����������
}
	write_com(0x80+0x40);
for(count3=0;count3<16;count3++)
{
	write_date(table10[count3]);	//��ʾ--------
	delay(5);
}
TR0=1;

}
//******************************************�޸��������ȷ�ϼ�********************************
void key_manage15()
{
n=0;
enter1flag=1;
if(enter1flag==1)
{
	enter1flag=0;
	count4=0;
	for(count3=0;count3<16;count3++)
	{
	tab_key[count3]=0x0f;			//��FFFFFFFF����Ѿ����������
 	}
	write_com(0x01);
	write_com(0x80);
	for(count3=0;count3<16;count3++)
	{
		write_date(table11[count3]);
		delay(100);
	}
	TR1=1;
	count1=0;
}
}
void key_manage16()
{
tab_key[n]=15;
n++;
}

//****************************************��ʱ��1��50MS,����ʱ1��*****************************
void time_1() interrupt 3
{	

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	if(count1<20)
	{
		count1++;
	}
	else 							//��ʱ��1S
	{
		TR1=0;
		count1=0;
		mimaflag=0;

		red=1;
		bell=1;
		//��ʾFFFFFFFF
		write_com(0x01);
		write_com(0x80);
		for(count3=0;count3<16;count3++)
			{
				write_date(table1[count3]);	//��ʾINPUT THE PASSCODE
				delay(5);
			}
		write_com(0x80+0x40);
		for(count3=0;count3<16;count3++)
			{
				write_date(table2[count3]);	//������ʾFFFFFFFF
				delay(5);
			}
	}

}
//***********************************************��ʱ0**********************************************
void time_0() interrupt 1
{	

	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
if(count4<8)
{
	key_scan();
}
else
{
	TR0=0;
	count4=0;
}
}

//��ʼ������
void init()
{	

	uchar i;
	lcden=0;
	write_com(0x38);					//����ʾģʽ����
	write_com(0x0c);					//����ʾ�����ȵ�����δ��
	write_com(0x06);					//������дһ���ַ����ַָ���һ���ҹ���һ����дһ���ַ�����Ƶ��ʾ���ƣ�
	write_com(0x01);					//����ָ��
	write_com(0x80);					//ָ����ָλ��

	//��ʱ����ʼ��
	TMOD=0x11;							//T0,T1������ʽ1
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;				//T0��ʼ��2MS

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;				//T1��ʼ��50MS

	TR1=0;
	ET1=1;
	EA=1;
	TR0=0;
	ET0=1;

	count0=0;							//��ʼû���������룬��Ϊ��
	enterflag=0;						//û��ȷ�ϼ�����
	mimaflag=0;							//������ȷ����������
					
	red=1;								//��Ʋ���
//************�������EPROM��**********************************
	sda=1;
	delay(5);
	scl=1;
	delay(5);
	for(i=0;i<8;i++)
	{
		write_add(i,8);
		delay2(100);
	}
	for(i=0;i<8;i++)
	{
		mima[i]=read_add(i);
		delay(5);
	}

}
void main()
{	rw=0;		
	init();
	write_com(0x80);						//ָ����ָλ��
	for(count3=0;count3<16;count3++)
	{
		write_date(table1[count3]);		//��һ����ʾINPUT THE PASSPORD:
		delay(5);
	}
	write_com(0x80+0x40);
	for(count3=0;count3<16;count3++)
	{
		write_date(table2[count3]);		//������ʾFFFFFFFF
		delay(5);
	}
	while(1)
	{
		key_scan();	   				   	//���ü���ɨ�躯��
		lcd_display(); 
	}
		 
}
//**************************************************����ɨ�躯����ʼ********************************
void key_scan()
{
//**********ɨ���һ��*********
P1=0xfe; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xee:
key_manage1();
break;

case 0xde:
key_manage2();
break;

case 0xbe:
key_manage3();
break;

case 0x7e:
key_manage4();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
//**************************************************ɨ��ڶ���***********************************
P1=0xfd; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xed:
key_manage5();
break;

case 0xdd:
key_manage6();
break;

case 0xbd:
key_manage7();
break;

case 0x7d:
key_manage8();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
//*********************************************ɨ�������***********************************
P1=0xfb; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xeb:
key_manage9();
break;

case 0xdb:
key_manage10();
break;

case 0xbb:
key_manage11();
break;

case 0x7b:
key_manage12();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}

//***************************************************ɨ�������****************************************
P1=0xf7;
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xe7:
key_manage13();
break;

case 0xd7:
key_manage14();
break;

case 0xb7:
key_manage15();
break;

case 0x77:
key_manage16();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
} 

