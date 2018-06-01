#include"REGx52.h"
#include"RICHMCU.H"
sbit a0 = ACC^0;
sbit a1 = ACC^1;
sbit a2 = ACC^2;
sbit a3 = ACC^3;
sbit a4 = ACC^4;
sbit a5 = ACC^5;
sbit a6 = ACC^6;
sbit a7 = ACC^7;
sbit DQ = P3^3 ; 		//18B20�ӿ�
sbit speaker = P3^6 ;	//������
sbit MODE = P1^4 ;	//ģʽ��
sbit INC = P1^5 ;	//���Ӽ�
sbit DEC = P1^6 ;	//���ټ�
sbit OK = P1^7 ;	//OK��
#define LCD1602_DATA P0	//8λ���ݲ��п�
#define LCD1602_RS P2_0	//ָ��OR���ݼĴ���ѡ��
#define LCD1602_RW P2_1	//��д����
#define LCD1602_EN P2_2	//ʹ�ܿ���
#define LCD1602_CLR 0x01	//����
#define LCD1602_MODE 0x38	//8λ���ݣ�˫����ʾ��5*7����
#define LCD1602_ON 0x0c	//����ʾ���ع�꣬��겻��˸
#define LCD1602_ADDR_MODE 0x06		//��ַ����
#define DS1302_SCLK P1_0	//1302ʱ����	
#define DS1302_IO P1_1		//1302������
#define DS1302_RST P1_2		//1302��λ��

volatile uint8 data timedata[7] ;	//ʱ��ֵ
volatile uint8 data display_buffer1[16];	//��ʾ������1 
volatile uint8 data display_buffer2[16];	//��ʾ������2
volatile uint8 data temdata[5] ;	//����¶�ֵ
volatile uint16 data tdat ;	//�¶�ֵ����
volatile uint8 data tflag ;	//�¶�����ֵ��־
volatile int8 self_pos = 0 ;//�Զ��������α�־��ʾλ�ñ���
volatile uint8 OK_VALUE = 0 ; //OK����ֵ���� 
volatile bit bdata DIS_ON = 1 ; //��ʾ����λ 1 == on 0 == off
volatile bit bdata SCAN_ON = 0 ; //����ɨ�迪��λ���ɶ�ʱ��ÿ20MSˢ��
volatile data MODE_ON = 0 ; //ģʽ�����º���ʾ�˵��Ŀ���λ
volatile data INC_VALUE = 0 ;
volatile data DEC_VALUE = 0 ;
volatile bit bdata ALARM_VALUE = 0 ;//���ӱ�־λ 
volatile bit bdata ALARM_ON = 1 ; //������ʱ�رձ�־λ
volatile uint8 data read_1 = 1,read_2 = 1,read_3 = 1,read_4 = 1;
volatile uint8 data stopwatch_count = 0;
volatile uint8 data stopwatch_second = 0;
volatile uint8 data stopwatch_minute  = 0;
volatile bit bdata STOPWATCH_START = 0;
uint8 code self_table1[]={
						0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,//	��
						0x1f,0x11,0x1f,0x11,0x1f,0x11,0x15,0x17,//��
						0x00,0x1f,0x11,0x1f,0x11,0x11,0x1f,0x00,//	��
						0x01,0x0c,0x17,0x14,0x17,0x0c,0x01,0x00,//���ӱ�־
						0x10,0x18,0x1c,0x1e,0x1f,0x1c,0x18,0x10	};//�����η���								


/**********************************************************
������
************************************************************/
void delay_ms(uint16 count)        // ��ʱʱ��count*1ms
{	uint16 i;
	for(;count>0;count--)
	{
		for(i=0;i<110;i++)
		{
			nop;
		}
	}
}
void speakers(uint8 speak_count)
{
	for(;speak_count>0;speak_count--)
		{
			speaker = 0 ;
			delay_ms(1) ;
			speaker = 1 ;
			delay_ms(3) ;
		}
}
/***********************************************************
æ���
************************************************************/
 void LCD1602_check_busy(void)
{
	LCD1602_DATA = 0xff;
	LCD1602_RS = 0 ;	
	LCD1602_RW = 1 ;	
	LCD1602_EN = 1 ;	
	while(LCD1602_DATA & 0x80) ;	
	LCD1602_EN = 0 ;			
}

/**********************************************************
дָ��
************************************************************/

 void LCD1602_write_cmd(uint8 cmd)
{
	LCD1602_check_busy();	
	LCD1602_RS = 0 ;	
	LCD1602_RW = 0 ;	
	LCD1602_DATA = cmd ;	
	LCD1602_EN = 1 ;	
	LCD1602_EN = 0 ;
}

/***********************************************************
д����
*************************************************************/
 void LCD1602_write_data(uint8 dat)
{
	LCD1602_check_busy();	
	LCD1602_RS = 1 ;	
	LCD1602_RW = 0 ;	
	LCD1602_DATA = dat ;	
	LCD1602_EN = 1 ;	
	LCD1602_EN = 0 ;
}


/***********************************************************
1602��ʼ��
************************************************************/
 void LCD1602_init(void)
{
	uint8 i  ;
	LCD1602_write_cmd(0x40);//CGRAM��ʼ��ַ
	for(i=0;i<40;i++)
		LCD1602_write_data(self_table1[i]);//д���Զ����ַ�
	LCD1602_write_cmd(LCD1602_MODE) ;
	LCD1602_write_cmd(LCD1602_ON) ;
	LCD1602_write_cmd(LCD1602_ADDR_MODE) ;
	LCD1602_write_cmd(LCD1602_CLR) ;
}

/************************************************************
������ʾ����
************************************************************/
 void LCD1602_set_postion(uint8 x , uint8 y)
{
	if(y<2)
		{
			y &= 0x01 ;	//yֵ�޶���0��1֮��
			x &= 0x0f ;	//xֵ�޶���0��15֮��
			if(y == 0)
				x |= 0x40 ;	//�����ʾ���ڵڶ��У���x��ֵ��0x40
			x |= 0x80 ;		//���x��ֵ
			LCD1602_write_cmd(x) ;//д������ֵ��LCD
		}
}

/************************************************************
ָ��λ��д�ַ�
*************************************************************/
 void LCD1602_write_char(uint8 x , uint8 y , uint8 chardata)
{
	LCD1602_set_postion(x,y) ;
	LCD1602_write_data(chardata) ;
}

/************************************************************
ָ��λ��д�ַ���
*************************************************************/
 void LCD1602_write_string(uint8 x , uint8 y , uint8 *string)
{
	LCD1602_set_postion(x,y) ;
	while((*string) != '\0')
	{	
		LCD1602_write_data(*string) ;
		string++ ;
	}
}
/***************************************************************
ָ��λ����ʾ�Զ����ַ�
**************************************************************
uint8 LCD1602_display_self(uint8 count)
{
	switch(count)
		{
			case 0 : sele_count = 0 ; break ;
			case 1 : sele_count = 1 ; break ;
			case 2 : sele_count = 2 ; break ;
			case 3 : sele_count = 3 ; break ;
			default : break ;
		}
	return(self_count) ;
}*/


/****************************************************************
�ײ���������(����)
*****************************************************************/
 void DS1302_input(uint8 inputdata)
{
	ACC = inputdata ;
	DS1302_IO = a0 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a1 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a2 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a3 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a4 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a5 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a6 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
	DS1302_IO = a7 ; DS1302_SCLK = 1 ; DS1302_SCLK = 0 ;
}

/****************************************************************
�ײ���������(���)����λ�ĵ���λ����Ϊ1
*****************************************************************/
 uint8 DS1302_output(void)
{
	DS1302_IO = 1 ;
	a0 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a1 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a2 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a3 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a4 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a5 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a6 = DS1302_IO ;
	DS1302_SCLK = 1 ; DS1302_SCLK =0 ; a7 = DS1302_IO ;
	return(ACC);
}

/****************************************************************
�ײ���������(ָ����ַдһ���ֽڵ�����)
*****************************************************************/
 void DS1302_write_byte(uint8 cmd , uint8 dat)
{
	DS1302_SCLK = 0 ;
	DS1302_RST = 0 ;
	DS1302_RST = 1 ;
	DS1302_input(cmd) ;
	DS1302_input(dat) ;
	DS1302_RST = 0 ;
	DS1302_SCLK = 1 ;
}

/****************************************************************
�ײ���������(ָ����ַ��ȡһ�ֽڵ�����)
*****************************************************************/
 uint8 DS1302_read_byte(uint8 cmd)
{
	uint8 receivedata = 0 ;
	DS1302_SCLK = 0 ;
	DS1302_RST = 0 ;
	DS1302_RST = 1 ;
	DS1302_input(cmd) ;
	receivedata = DS1302_output() ;
	DS1302_RST = 0 ;
	DS1302_SCLK = 1 ;
	return(receivedata) ;
}

/********************************************************
����1302�ĳ�ʼʱ��
*********************************************************/
 void DS1302_init()
{	
		if(DS1302_read_byte(0xc9) != 0xf0) 
		{
			DS1302_write_byte(0x8e,0x00) ;//����д����
			DS1302_write_byte(0xc8,0xf0) ; //д���ʼ����־	��ϵͳ�ϵ����˱�־�������Ӻ���ֻ���ڵ�һ�γ�ʼ��һ�Ρ�
			DS1302_write_byte(0x8c,0x08) ;//��
			DS1302_write_byte(0x8a,0x05) ;//����
			DS1302_write_byte(0x88,0x09) ;//��
			DS1302_write_byte(0x86,0x19) ;//��
			DS1302_write_byte(0x84,0x12) ;//ʱ
			DS1302_write_byte(0x82,0x00) ;//��
			DS1302_write_byte(0x80,0x00) ;//��
			DS1302_write_byte(0x90,0xa5) ;//���
			
			
	
			DS1302_write_byte(0x8e,0x80) ;//��ֹд����	 
		}
}

/********************************************************
��ȡʱ�����ݲ�����timedata[]��
*********************************************************/
 void DS1302_read_time()
{
	timedata[0] = DS1302_read_byte(0x8d) ;	//��
	timedata[1] = DS1302_read_byte(0x89) ;	//��
	timedata[2] = DS1302_read_byte(0x87) ;	//��
	timedata[3] = DS1302_read_byte(0x85) ;	//ʱ
	timedata[4] = DS1302_read_byte(0x83) ;	//��
	timedata[5] = DS1302_read_byte(0x81) ;	//��
	timedata[6] = DS1302_read_byte(0x8b) ;	//����
}


/***********************************************************
DS18B20��غ���
*************************************************************/
void delay_18b20(uint16 sum) //������ʱ
{
	while(sum--);
}

void rst_18b20()                 //18B20��λ
{	//uchar flag;
	DQ=1;
	delay_18b20(8);
	DQ=0;
	delay_18b20(80);
	DQ=1;
	delay_18b20(13);
	//flag=DQ;
	//return(flag);
//	delay_18b20(20);
}	


void wr_18b20(uint8 dat)	//дһ���ֽڵ�����
{
	uint8 i=8;
	for(;i>0;i--)
	{
		DQ=0;
		DQ=dat&0x01;
		delay_18b20(5);
		DQ=1;
		dat>>=1;
	}	
}

uint8 rd_18b20()		//��һ���ֽڵ�����
{
	uint8 dat=0,i=8;
	for(;i>0;i--)
	{
		DQ=0;
		dat>>=1;
		DQ=1;
		if(DQ)
		dat|=0x80;
		delay_18b20(4);
	}
	return(dat);
}
uint16 rd_temperature()		//��ȡ�¶�ֵ
{	
	uint8 a=0,b=0;
	uint16 t=0;
	float tt=0;
	rst_18b20();		//��λ
	wr_18b20(0xcc);		//����ROM
	wr_18b20(0x44);		//�����¶�ת��
	rst_18b20();		//�ٴζ�18B20����ʱ,��Ҫ���¸�λһ��
	wr_18b20(0xcc);		//����ROM
	wr_18b20(0xbe);		//��ȡ�ڲ�RAM������
	a=rd_18b20();		//���¶�ֵ��λ(�ڲ�RAM�ĵ�0���ֽ�)
	b=rd_18b20();		//���¶�ֵ��λ(�ڲ�RAM�ĵ�1���ֽ�)
	t=b;
	t<<=8;				//�¶�ֵ�ĸ�8λ����t�ĸ�8λ
	t=t|a;				//�¶�ֵ�ĵ�8λ����t�ĵ�8λ
	if(t<0x0fff)		//�ж��¶�ֵ������ֵ
   		tflag=0;		//tflag=0�¶�ֵΪ��
    else
   {
		t=~t+1;			//�¶�ֵΪ��,��ֵ�Բ�����ʽ���,����Ҫ���仹ԭ��ԭ��
		tflag=1;		//tflag=1�¶�ֵΪ��
   }
	tt=t*0.0625;
	t = tt*100 ;			
	return(t);
	
}

//***********************************************//



void tem_conv()			//�¶�ת��
			{		
				uint8 flagdat ;//�����¶�ֵ����
				if(tflag==0)
				temdata[4]=flagdat=0x20;//�¶�Ϊ������ʾ����
				   else
				 temdata[4]=flagdat=0x2d;//���¶���ʾ����:-
				 temdata[0]=tdat/1000+0x30;//�¶�ʮλ
			     temdata[1]=tdat%1000/100+0x30;//�¶ȸ�λ
			     temdata[2]=tdat%100/10+0x30;//С��ʮλ
			     temdata[3]=tdat%10+0x30;//С����λ
				/*if(temdata[0]==0x30)
				    {temdata[0]=0x20;//?????0,???
				   		if(temdata[1]==0x30)
					    {
							temdata[1]=0x20;//?????0,???0????
					    }
				   	}*/
				
			  }
			
/**************************************************************
������ʾ������
***************************************************************/
 void updata_buffer(void)
{
	display_buffer1[0] = '2' ;
	display_buffer1[1] = '0' ;
	display_buffer1[2] = timedata[0]/16+0x30 ;
	display_buffer1[3] = timedata[0]%16+0x30 ;
	display_buffer1[4] = 0 ;
	display_buffer1[5] = timedata[1]/16+0x30 ;
	display_buffer1[6] = timedata[1]%16+0x30 ;
	display_buffer1[7] = 1 ;
	display_buffer1[8] = timedata[2]/16+0x30 ;
	display_buffer1[9] = timedata[2]%16+0x30 ;
	display_buffer1[10] = 2 ;
	
	display_buffer1[11] = ' ' ;
	switch(timedata[6]%16)
		{
			case 1 : {display_buffer1[12] = 'M' ; 
					display_buffer1[13] = 'o' ;
					display_buffer1[14] = 'n' ;	}break ;
			case 2 : {display_buffer1[12] = 'T' ; 
					display_buffer1[13] = 'u' ;
					display_buffer1[14] = 'e' ;	}break ;
			case 3 : {display_buffer1[12] = 'W' ; 
					display_buffer1[13] = 'e' ;
					display_buffer1[14] = 'd' ;	}break ;
			case 4 : {display_buffer1[12] = 'T' ; 
					display_buffer1[13] = 'h' ;
					display_buffer1[14] = 'u' ;	}break ;
			case 5 : {display_buffer1[12] = 'F' ; 
					display_buffer1[13] = 'r' ;
					display_buffer1[14] = 'i' ;	}break ;
			case 6 : {display_buffer1[12] = 'S' ; 
					display_buffer1[13] = 'a' ;
					display_buffer1[14] = 't' ;	}break ;
			case 7 : {display_buffer1[12] = 'S' ; 
					display_buffer1[13] = 'u' ;
					display_buffer1[14] = 'n' ;	}break ;
			default : break;
		}
	if(ALARM_VALUE == 1)
		display_buffer1[15] = 3 ;
	else
		display_buffer1[15] = ' ' ;
	display_buffer2[0] = timedata[3]/16+0x30 ;
	display_buffer2[1] = timedata[3]%16+0x30 ;
	display_buffer2[2] = ':' ;
	display_buffer2[3] = timedata[4]/16+0x30 ;
	display_buffer2[4] = timedata[4]%16+0x30 ;
	display_buffer2[5] = ':' ;
	display_buffer2[6] = timedata[5]/16+0x30 ;
	display_buffer2[7] = timedata[5]%16+0x30 ;
	display_buffer2[8] = temdata[4] ;
	display_buffer2[9] = temdata[0] ;
	display_buffer2[10] = temdata[1] ;
	display_buffer2[11] = '.' ;
	display_buffer2[12] = temdata[2] ;
	display_buffer2[13] = temdata[3] ;
	display_buffer2[14] = 0xdf ;
	display_buffer2[15] = 'C' ;
}

/******************************************************************
��ʾ����������
*******************************************************************/
 void display_buffer(void)
{
	uint8 i ;
	LCD1602_write_cmd(0x80) ;
	for(i=0;i<16;i++)
	LCD1602_write_data(display_buffer1[i]);
	LCD1602_write_cmd(0xc0) ;
	for(i=0;i<16;i++)
	LCD1602_write_data(display_buffer2[i]);
}
/*****************************************************************
����ɨ�躯��
******************************************************************/

void key_scan(void)
{
	if(MODE == 0)
		{
			delay_ms(20) ;
			while(!MODE) ;
			speakers(5) ;
			DIS_ON = 0 ;
			MODE_ON ++ ;
			if(MODE_ON == 1 && OK_VALUE == 0)
			LCD1602_write_cmd(0x01) ;
			if(MODE_ON == 2 && OK_VALUE == 0)
				{
					MODE_ON = 0 ;
					DIS_ON = 1 ;
					self_pos = 0 ;
				}
		}
	
	if(INC == 0 && MODE_ON == 1 && OK_VALUE == 0)
		{
			delay_ms(20) ; 
			while(!INC) ;
			speakers(5) ;
			LCD1602_write_cmd(0x01) ;
			self_pos++ ;
			if(self_pos > 2)
			self_pos = 0 ;
		}
	if(DEC == 0 && MODE_ON == 1 && OK_VALUE == 0)
		{
			delay_ms(20) ;
			while(!DEC) ;
			speakers(5) ;
			LCD1602_write_cmd(0x01) ;
			self_pos-- ;
			if(self_pos < -3)
			self_pos = -1 ;
		}
	if(DEC == 0 )
		{
			delay_ms(20) ;
			while(!DEC) ;
			speakers(5) ;
			DEC_VALUE++;
		}
	if(INC == 0)
		{
			delay_ms(20) ;
			while(!INC) ;
			speakers(5) ;
			INC_VALUE++;
		}
	if(OK == 0 && MODE_ON != 0)
		{
			delay_ms(20) ;
			while(!OK) ;
			speakers(5) ;
			LCD1602_write_cmd(0x01) ;
			OK_VALUE++;
			INC_VALUE = 0 ;
			DEC_VALUE = 0 ;
		}
	if(OK == 0 && MODE_ON == 0)
		{
			delay_ms(20) ;
			while(!OK) ;
			speakers(5) ;
			ALARM_ON = 1 ;
			if(ALARM_VALUE == 1)
				ALARM_VALUE = 0 ;
			else
				ALARM_VALUE = 1 ;
		}	
}
/*******************************************************************
�����˵��˻���������(��һ��)
********************************************************************/
void menu_display_1()
{
	if(MODE_ON == 1 && OK_VALUE == 0)
		{
			switch(self_pos)
				{	
					case -3 : {
								LCD1602_write_char(0,1,4) ;
								LCD1602_write_string(1,1,"1 Time setting ") ;
								LCD1602_write_string(1,0,"2 Alarm setting") ;
							  }break;
					case -2 : {
								LCD1602_write_char(0,0,4) ;
								LCD1602_write_string(1,1,"1 Time setting ") ;
								LCD1602_write_string(1,0,"2 Alarm setting") ;
							  }break;
					case -1 : {
								LCD1602_write_char(0,0,4) ;
								LCD1602_write_string(1,1,"2 Alarm setting ") ;
								LCD1602_write_string(1,0,"3 Stopwatch") ;
							  }break;
					case 0 :  {
								LCD1602_write_char(0,1,4) ;
								LCD1602_write_string(1,1,"1 Time setting ") ;
								LCD1602_write_string(1,0,"2 Alarm setting") ;
							  }break ;
					case 1 :  {
								LCD1602_write_char(0,0,4) ;
								LCD1602_write_string(1,1,"1 Time setting ") ;
							 	LCD1602_write_string(1,0,"2 Alarm setting") ;
							  }break ;
					case 2 :  {
								LCD1602_write_char(0,0,4) ;
								LCD1602_write_string(1,1,"2 Alarm setting ") ;
								LCD1602_write_string(1,0,"3 Stopwatch") ;
							  }break ;
					default :  break ;
				}	
		}
}

/*******************************************************************
�����˵��˻���������(�ڶ���)
********************************************************************/
void menu_display_2(void)
{
	uint8 i ,j;
	if(MODE_ON != 0 && OK_VALUE != 0)
		{
			LCD1602_write_cmd(0x0f) ;
			if(self_pos == 0 || self_pos == -3)
				{
					
					LCD1602_write_cmd(0x81) ;
					for(i=0;i<15;i++)
						LCD1602_write_data(display_buffer1[i]) ;
					LCD1602_write_cmd(0xc4) ;
					for(j=0;j<8;j++)
						LCD1602_write_data(display_buffer2[j]) ;
					if(MODE_ON == 1)
						{
							uint8 item ;
							LCD1602_write_cmd(0x84) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x8d) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==100)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x8c,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x8d) ;
									item=(item/16)*10+item%16;
									item--;
									if(item < 0)item=99;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x8c,item);
									DEC_VALUE = 0 ;
								}
						}
					else if(MODE_ON == 2)
							{
								uint8 item ;
								LCD1602_write_cmd(0x87) ;
								if(INC_VALUE !=0 )
									{
										item = DS1302_read_byte(0x89) ;
										item=(item/16)*10+item%16;
										item++;
										if(item==13)item=1;
										item=(item/10)*16+item%10;
										DS1302_write_byte(0x8e,0x00);//����д����
										DS1302_write_byte(0x88,item);
										INC_VALUE = 0 ;
									}
								if(DEC_VALUE != 0)
									{
										item = DS1302_read_byte(0x89) ;
										item=(item/16)*10+item%16;
										item--;
										if(item == 0)item=12;
										item=(item/10)*16+item%10;
										DS1302_write_byte(0x8e,0x00);//����д����
										DS1302_write_byte(0x88,item);
										DEC_VALUE = 0 ;
									}
								}
					
					else if(MODE_ON == 3)
						{
							uint8 item ;
							LCD1602_write_cmd(0x8a) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x87) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==32)item=1;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x86,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x87) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == 0)item=31;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x86,item);
									DEC_VALUE = 0 ;
								}
						}
					
					else if(MODE_ON == 4)
						{
							uint8 item ;
							LCD1602_write_cmd(0x8f) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x8b) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==8)item=1;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x8a,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x8b) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == 0)item=7;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x8a,item);
									DEC_VALUE = 0 ;
								}	
						}	
					else if(MODE_ON == 5)
						{
							uint8 item ;
							LCD1602_write_cmd(0xc5) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x85) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==24)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x84,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x85) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=23;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x84,item);
									DEC_VALUE = 0 ;
								}
						}
						
					else if(MODE_ON == 6)
						{
							uint8 item ;
							LCD1602_write_cmd(0xc8) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x83) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==60)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x82,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x83) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=59;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x82,item);
									DEC_VALUE = 0 ;
								}
						}
						
					else if(MODE_ON == 7)
						{
							uint8 item ;
							LCD1602_write_cmd(0xcb) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0x81) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==60)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x80,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0x81) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=59;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0x80,item);
									DEC_VALUE = 0 ;
								}
						}
					else
						{
							LCD1602_write_cmd(0x86);
							MODE_ON = 1 ;
						}
								
					if(OK_VALUE == 2 )
						{
							MODE_ON = 1 ;
							OK_VALUE = 0 ;
							LCD1602_write_cmd(0x01) ;
							LCD1602_write_cmd(0x0c) ;
						}
				}
			if(self_pos == 1 || self_pos == -2)
				{	//uint8 alarm_temp ;
					LCD1602_write_cmd(0x80);
					LCD1602_write_string(0,1,"Alarm time ") ;
					LCD1602_write_data(DS1302_read_byte(0xc1)/16+0x30);
					LCD1602_write_data(DS1302_read_byte(0xc1)%16+0x30);
					LCD1602_write_data(':');
					
					LCD1602_write_data(DS1302_read_byte(0xc3)/16+0x30);
					LCD1602_write_data(DS1302_read_byte(0xc3)%16+0x30);
					if(ALARM_VALUE == 1)
						LCD1602_write_string(2,0," ON") ; 
					else 
						LCD1602_write_string(2,0,"OFF") ;
					LCD1602_write_cmd(0xcb)	;
					LCD1602_write_data(DS1302_read_byte(0xc5)/16+0x30);
					LCD1602_write_data(DS1302_read_byte(0xc5)%16+0x30);
					LCD1602_write_data(':');
					LCD1602_write_data(DS1302_read_byte(0xc7)/16+0x30);
					LCD1602_write_data(DS1302_read_byte(0xc7)%16+0x30);
					LCD1602_write_cmd(0x8c) ;
					if(MODE_ON == 1)
						{
							uint8 item ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0xc1) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==24)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc0,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0xc1) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=23;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc0,item);
									DEC_VALUE = 0 ;
								}
						}
					else if(MODE_ON == 2)
						{
							uint8 item ;
							LCD1602_write_cmd(0x8f) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0xc3) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==60)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc2,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0xc3) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=59;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc2,item);
									DEC_VALUE = 0 ;
								}
						}
					else if(MODE_ON == 3)
						{
							LCD1602_write_cmd(0xc4);
							if(INC_VALUE != 0 || DEC_VALUE !=0 )
								{
									INC_VALUE = 0 ;
									DEC_VALUE = 0 ;
									ALARM_VALUE^=1 ; //ȡ��
								}
						}
					else if(MODE_ON == 4)
						{
							uint8 item ;
							LCD1602_write_cmd(0xcc) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0xc5) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==24)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc4,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0xc5) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=23;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc4,item);
									DEC_VALUE = 0 ;
								}
						}
					else if(MODE_ON == 5)
						{
							uint8 item ;
							LCD1602_write_cmd(0xcf) ;
							if(INC_VALUE !=0 )
								{
									item = DS1302_read_byte(0xc7) ;
									item=(item/16)*10+item%16;
									item++;
									if(item==60)item=0;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc6,item);
									INC_VALUE = 0 ;
								}
							if(DEC_VALUE != 0)
								{
									item = DS1302_read_byte(0xc7) ;
									item=(item/16)*10+item%16;
									item--;
									if(item == -1)item=59;
									item=(item/10)*16+item%10;
									DS1302_write_byte(0x8e,0x00);//����д����
									DS1302_write_byte(0xc6,item);
									DEC_VALUE = 0 ;
								}
						}
					else
						{
							LCD1602_write_cmd(0x8c);
							MODE_ON = 1 ;
						}
					if(OK_VALUE == 2 )
						{
							MODE_ON = 1 ;
							OK_VALUE = 0 ;
							LCD1602_write_cmd(0x01) ;
							LCD1602_write_cmd(0x0c) ;
						}
				}
			if(self_pos == 2 || self_pos == -1)
				{
					
					if(DEC_VALUE != 0)
						{
							TR0 = 0;
							
							DEC_VALUE = 0 ;
						}
					
					if(INC_VALUE != 0 )
						{	
							TR0= 1 ;
							INC_VALUE = 0 ;
						}
					LCD1602_write_cmd(0x0c) ;
					LCD1602_write_string(4,1,"stopwatch") ;
         			LCD1602_write_char(5,0,stopwatch_minute/10+0x30) ;
					LCD1602_write_char(6,0,stopwatch_minute%10+0x30) ;
					LCD1602_write_char(7,0,':') ;
					LCD1602_write_char(8,0,stopwatch_second/10+0x30) ;
					LCD1602_write_char(9,0,stopwatch_second%10+0x30) ;
					LCD1602_write_char(10,0,':') ;
					LCD1602_write_char(11,0,stopwatch_count/10+0x30) ;
					LCD1602_write_char(12,0,stopwatch_count%10+0x30) ;
				
					
					if(OK_VALUE == 2 )
						{
							TR0 = 0 ;
							stopwatch_minute = 0;
							stopwatch_second = 0;
							stopwatch_count = 0;
						}
					if(OK_VALUE == 3)
						{
							TR0 = 0 ;
							MODE_ON = 1 ;
							OK_VALUE = 0 ;
							stopwatch_minute = 0;
							stopwatch_second = 0;
							stopwatch_count = 0;
							LCD1602_write_cmd(0x01) ;
						}
				}
		}
	
}
/***********************************************************
���ӳ�ʼ��
************************************************************/
void alarm_init(void)
{
	DS1302_write_byte(0x8e,0x00) ;//����д����

	DS1302_write_byte(0xc0,0x12) ;//��һ�����ӵ�ʱ
	DS1302_write_byte(0xc2,0x00) ;//��һ�����ӵķ�
	DS1302_write_byte(0xc4,0x12) ;//�ڶ������ӵ�ʱ
	DS1302_write_byte(0xc6,0x00) ;//�ڶ������ӵķ�
		
	DS1302_write_byte(0x8e,0x80) ;//��ֹд����		
}
/***********************************************************
��������Ƿ��˶�ʱʱ��
************************************************************/
void alarm_check(void)
{
	
	if(ALARM_VALUE ==1 && ALARM_ON == 1)
		{
			read_1 = DS1302_read_byte(0x85) ;
			read_1 ^= DS1302_read_byte(0xc1);
			read_2 = DS1302_read_byte(0x83) ;
			read_2 ^= DS1302_read_byte(0xc3);
			read_3 = DS1302_read_byte(0x85) ;
			read_3 ^= DS1302_read_byte(0xc5);
			read_4 = DS1302_read_byte(0x83) ;
			read_4 ^= DS1302_read_byte(0xc7);
		}
	if((read_1 == 0) && (read_2 == 0) || (read_3 == 0) && (read_4 == 0))
		{
			if(ALARM_VALUE == 1)
				{
					speakers(100) ;
					delay_ms(10);
					speakers(10);
					delay_ms(5);
					ALARM_ON = 0 ;
				}
		}
}
/***********************************************************
������
************************************************************/
void main(void)
{
	TMOD = 0x01 ;
	TH0 = 0xd8 ;
	TL0 = 0xf0 ;
	EA = 1 ;
	ET0 = 1 ;
	TR0 = 0 ;
	LCD1602_init() ;
	alarm_init() ;
	DS1302_init() ;
	speakers(20)  ;
	while(1)
		{	
			
			key_scan() ;
			menu_display_1() ;
			menu_display_2() ;
			if(STOPWATCH_START == 0)
				{
					alarm_check();
					DS1302_read_time();
					tdat=rd_temperature();	//�¶ȶ�ȡ
					tem_conv();				//�¶�ת��
					updata_buffer();
					if(DIS_ON == 1)
						{
								
							display_buffer();
						}	
				}
		}
}
void t0_server(void) interrupt 1 using 1
{
	TH0 = 0xd8 ;
	TL0 = 0xf0 ;
	stopwatch_count++ ;
	if(stopwatch_count == 100)
		{
			stopwatch_second++ ;
			stopwatch_count = 0 ;
		}
	if(stopwatch_second == 60)
		{
			stopwatch_second = 0 ;
			stopwatch_minute++ ;
		}
	if(stopwatch_minute == 60)
		{stopwatch_minute = 0 ;}
}
