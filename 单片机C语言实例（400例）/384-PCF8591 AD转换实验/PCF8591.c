
//-----------------------������������������------------------------
#include <reg51.h>
#include <intrins.h>
sbit SCL=P3^4;                   // ��p1.2��ģ��ʱ�ӿ�
sbit SDA=P3^5;                   // ��p1.3��ģ�����ݿ�
bit askflag;
#define uchar unsigned char
#define uint  unsigned int
#define disdata  P0              //��ʾ�����������

sbit LCD_RS = P2^0;             
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

uint data dis[4]={0x00,0x00,0x00,0x00}; 
                          //����3����ʾ���ݵ�Ԫ��1�����ݴ洢��Ԫ

uchar code dis4[] = {"1- .  V  2- .  V"};
uchar code dis5[] = {"3- .  V  4- .  V"};


#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
	                 
bit   bdata SystemError;                //�ӻ������־λ

/*********************** PCF8591ר�ñ������� ***********************/

#define	PCF8591_WRITE	0x90 
#define	PCF8591_READ 	0x91
#define  NUM  4 	            //���պͷ��ͻ����������
uchar idata receivebuf[NUM];    //���ݽ��ջ�����

/*******************************************************************/
/*                                                                 */
/*  ��ʱ����                                                       */
/*                                                                 */
/*******************************************************************/

void delay(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}		

/*******************************************************************/
/*                                                                 */
/*���LCDæ״̬                                                    */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */
/*                                                                 */
/*******************************************************************/ 

bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }

/*******************************************************************/
/*                                                                 */
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣                             */
/*                                                                 */
/*******************************************************************/

void lcd_wcmd(uchar cmd)

{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}

/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/

void lcd_wdat(uchar dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}

/*******************************************************************/
/*                                                                 */
/*  LCD��ʼ���趨                                                  */
/*                                                                 */
/*******************************************************************/

void lcd_init()
{ 
    delay(15);                   
    lcd_wcmd(0x38);      //16*2��ʾ��5*7����8λ����
    delay(5);
    lcd_wcmd(0x38);         
    delay(5);
    lcd_wcmd(0x38);         
    delay(5);

    lcd_wcmd(0x0c);      //��ʾ�����ع��
    delay(5);
    lcd_wcmd(0x06);      //�ƶ����
    delay(5);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(5);
}

/*******************************************************************/
/*                                                                 */
/*  �趨��ʾλ��                                                   */
/*                                                                 */
/*******************************************************************/

void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80);  //����ָ��=80+��ַ����
}

/******************************************************************/
/*                                                                */
/* ���ݴ�������ʾ                                                 */
/* ���ɼ��������ݽ���16����ת��ΪASCLL�롣                        */
/*                                                                */
/******************************************************************/

show_value(uchar ad_data)

{   
    dis[2]=ad_data/51;   //ADֵת��Ϊ3ΪBCD�룬���Ϊ5.00V��
    dis[2]=dis[2]+0x30;	 //ת��ΪACSII��
    dis[3]=ad_data%51;   //�����ݴ�
    dis[3]=dis[3]*10;    //����С����һλ
    dis[1]=dis[3]/51;
    dis[1]=dis[1]+0x30;	 //ת��ΪACSII��
    dis[3]=dis[3]%51;
    dis[3]=dis[3]*10;    //����С���ڶ�λ
    dis[0]=dis[3]/51;                                                                             //
    dis[0]=dis[0]+0x30;  //ת��ΪACSII��           
} 
//-------------------------------------------------------------------
// �������ƣ� iic_start()
// �������ܣ� ����I2C�����ӳ���
//-------------------------------------------------------------------
void iic_start(void)
{ //ʱ�ӱ��ָߣ������ߴӸߵ���һ�����䣬I2Cͨ�ſ�ʼ
	SDA = 1;     
	SCL = 1;
	delayNOP();      // ��ʱ5us 
   	SDA = 0;
	delayNOP();
    SCL = 0;
}
//-------------------------------------------------------------------
// �������ƣ� iic_stop()
// �������ܣ� ֹͣI2C�������ݴ����ӳ���
//-------------------------------------------------------------------
void iic_stop(void)
{  	
	SDA = 0;   	   //ʱ�ӱ��ָߣ������ߴӵ͵���һ�����䣬I2Cͨ��ֹͣ
	SCL = 1;
	delayNOP();
	SDA = 1;
	delayNOP();
    SCL = 0;
}
//------------------------------------------------------------------
// �������ƣ� iicInit_()
// �������ܣ� ��ʼ��I2C�����ӳ���
//------------------------------------------------------------------
 void iicInit(void)
   {
   SCL = 0;
   iic_stop();	
   }  
//-------------------------------------------------------------------
// �������ƣ� slave_ACK
// �������ܣ� �ӻ�����Ӧ��λ�ӳ���
//-------------------------------------------------------------------
void slave_ACK(void)
{
	SDA = 0;  
	SCL = 1;
	delayNOP();	
	SCL = 0;
}
//-------------------------------------------------------------------
// �������ƣ� slave_NOACK
// �������ܣ� �ӻ����ͷ�Ӧ��λ�ӳ�����ʹ���ݴ�����̽���
//-------------------------------------------------------------------
void slave_NOACK(void)
{ 
	SDA = 1;
	SCL = 1;
	delayNOP();
	SDA = 0;
    SCL = 0;  
}
//-------------------------------------------------------------------
// �������ƣ� check_ACK
// �������ܣ� ����Ӧ��λ����ӳ�����ʹ���ݴ�����̽���
//-------------------------------------------------------------------
void check_ACK(void)
{ 	    
    SDA = 1;        // ��p1.1���ó����룬��������˿�д1
	SCL = 1;
	askflag = 0;
	delayNOP();   
	if(SDA == 1)    // ��SDA=1������Ӧ����λ��Ӧ���־askflag
    askflag = 1;
   	SCL = 0;
}
//-------------------------------------------------------------------
// �������ƣ� IICSendByte
// ��ڲ����� ch
// �������ܣ� ����һ���ֽ�
//-------------------------------------------------------------------
void IICSendByte(uchar ch)
 
{
  	unsigned char idata n=8;     // ��SDA�Ϸ���һλ�����ֽڣ�����λ

	while(n--)
	{ 
	if((ch&0x80) == 0x80)    // ��Ҫ���͵��������λΪ1����λ1
	   {
	 		SDA = 1;    // ����λ1
			SCL = 1;
		    delayNOP();
		//	SDA = 0;	
			SCL = 0; 
	   }
		else
		{  
			SDA = 0;    // ������λ0
			SCL = 1;
			delayNOP();
		  	SCL = 0;
		}
		ch = ch<<1;    // ��������һλ
	}
}
//-------------------------------------------------------------------
// �������ƣ� IICreceiveByte
// ���ؽ��յ�����
// �������ܣ� ����һ�ֽ��ӳ���
//-------------------------------------------------------------------
uchar IICreceiveByte(void)
{
	uchar idata n=8;     // ��SDA���϶�ȡһ�������ֽڣ�����λ
	uchar tdata=0;
	while(n--)
	{
	   SDA = 1;
	   SCL = 1;
	   tdata =tdata<<1;		    //����һλ
	   	if(SDA == 1)
		  tdata = tdata|0x01;   // �����յ���λΪ1�������ݵ����һλ��1
		else 
		  tdata = tdata&0xfe;   // �������ݵ����һλ��0
	   SCL = 0;
	 }

	 return(tdata);
}
//-------------------------------------------------------------------
// �������ƣ� DAC_PCF8591
// ��ڲ����� slave_add�ӻ���ַ,nҪ���͵����ݸ���
// �������ܣ� ����nλ�����ӳ���
//-------------------------------------------------------------------
void DAC_PCF8591(uchar controlbyte,uchar w_data)
{    
	
	iic_start();                    // ����I2C
	delayNOP();

	IICSendByte(PCF8591_WRITE);     // ���͵�ַλ
	check_ACK();                    // ���Ӧ��λ
    if(askflag == 1)
	 { 
		SystemError = 1;
		return;                     // ����Ӧ���ô����־λ
     }
    IICSendByte(controlbyte&0x77);	//Control byte 
	check_ACK();                    //���Ӧ��λ
    if(askflag == 1)
	 { 
		SystemError = 1;
		return;                    // ����Ӧ���ô����־λ
	 }
    IICSendByte(w_data);	       //data byte
	check_ACK();                  // ���Ӧ��λ
    if(askflag == 1)
	 { 
		SystemError = 1;
    	return;   // ����Ӧ���������������ѻ����ô����־λSystemError
	 }
	iic_stop();         // ȫ��������ֹͣ
	delayNOP();
	delayNOP();
	delayNOP();
	delayNOP();	
}
//-------------------------------------------------------------------
// �������ƣ� ADC_PCF8591
// ��ڲ����� controlbyte������
// �������ܣ� ��������4·ͨ����A/Dת�������receivebuf
//-------------------------------------------------------------------
void ADC_PCF8591(uchar controlbyte)
{ 
    uchar idata receive_da,i=0;

	iic_start();

	IICSendByte(PCF8591_WRITE);	//������
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}

	IICSendByte(controlbyte);	//������
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}

    iic_start();                //���·��Ϳ�ʼ����
   	IICSendByte(PCF8591_READ);	//������
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}
	 
    IICreceiveByte();   //�ն�һ�Σ�������˳��
    slave_ACK();        //�յ�һ���ֽں���һ��Ӧ��λ

	while(i<4)
	{  
	  receive_da=IICreceiveByte();
	  receivebuf[i++]=receive_da;
	  slave_ACK();       //�յ�һ���ֽں���һ��Ӧ��λ
	}
	slave_NOACK();       //�յ����һ���ֽں���һ����Ӧ��λ
	iic_stop();
}
//-------------------------------------------------------------------
// �������ƣ� main
// �������ܣ� ������
//-------------------------------------------------------------------
main()
{
    uchar i,l;
    delay(10);                 //��ʱ
    lcd_init();                //��ʼ��LCD             
        
    lcd_pos(0);                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
     i = 0;
    while(dis4[i] != '\0')
     {                         //��ʾ�ַ�
       lcd_wdat(dis4[i]);
       i++;
     }

    lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ��е�1���ַ�
     i = 0;
    while(dis5[i] != '\0')
     {
       lcd_wdat(dis5[i]);      //��ʾ�ַ�
       i++;
     }
	
  while(1)
  {
	iicInit();	              //I2C���߳�ʼ��
    ADC_PCF8591(0x04);

	if(SystemError == 1)	  //�д���������
	  {
	  	iicInit();				  //I2C���߳�ʼ��
	    ADC_PCF8591(0x04);
	   }   
	
	for(l=0;l<4;l++)	
	 {
	  show_value(receivebuf[0]); //��ʾͨ��0       
	    lcd_pos(0x02);             
        lcd_wdat(dis[2]);        //����λ��ʾ
        lcd_pos(0x04);             
        lcd_wdat(dis[1]);        //��һλС����ʾ 
        lcd_pos(0x05);             
        lcd_wdat(dis[0]);        //�ڶ�λС����ʾ

      show_value(receivebuf[1]); //��ʾͨ��1	 
	    lcd_pos(0x0b);             
        lcd_wdat(dis[2]);        //����λ��ʾ
        lcd_pos(0x0d);             
        lcd_wdat(dis[1]);        //��һλС����ʾ 
        lcd_pos(0x0e);             
        lcd_wdat(dis[0]);        //�ڶ�λС����ʾ

	  show_value(receivebuf[2]); //��ʾͨ��2         	 
	    lcd_pos(0x42);             
        lcd_wdat(dis[2]);        //����λ��ʾ
        lcd_pos(0x44);             
        lcd_wdat(dis[1]);        //��һλС����ʾ 
        lcd_pos(0x45);             
        lcd_wdat(dis[0]);        //�ڶ�λС����ʾ

      show_value(receivebuf[3]); //��ʾͨ��3	 
	    lcd_pos(0x4b);             
        lcd_wdat(dis[2]);        //����λ��ʾ
        lcd_pos(0x4d);              
        lcd_wdat(dis[1]);        //��һλС����ʾ 
        lcd_pos(0x4e);             
        lcd_wdat(dis[0]);        //�ڶ�λС����ʾ

	  iicInit();	       	      //I2C���߳�ʼ��  
      DAC_PCF8591(0x40,receivebuf[0]); //D/A���

	   	if(SystemError == 1)	  //�д���������
	    {
	  	 iicInit();				  //I2C���߳�ʼ��
		 DAC_PCF8591(0x40,receivebuf[0]); //D/A���
	    }	        

      //delay(50);                       //��ʱ
	 }

   }
}
