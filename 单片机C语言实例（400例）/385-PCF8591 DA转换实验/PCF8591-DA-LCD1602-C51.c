#include <reg52.h>
#include <intrins.h>
#include <string.h>

#define uchar unsigned char
#define uint  unsigned int
#define disdata  P0              //��ʾ�����������
bit flag;
sbit SCL=P3^4;                   // ��p1.0��ģ��ʱ�ӿ�
sbit SDA=P3^5;                   // ��p1.3��ģ�����ݿ�


sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

sbit  K1   = P1^4;       
sbit  K2   = P1^5;       //ѡ��
sbit  K3   = P1^6;       //������
sbit  K4   = P1^7;       //ֹͣ��
sbit  BEEP = P3^6;       //������

uchar code dis1[] = {"    PCF-8591    "};
uchar code dis2[] = {"  DA CONVERTER  "};

uchar code dis3[] = {" K2: D/A CHOOSE "};
uchar code dis4[] = {"K3:ENTER K4:EXIT"};

uchar code dis5[] = {"   SINE  WAVE   "};
uchar code dis6[] = {"  SQUARE  WAVE  "};
uchar code dis7[] = {" TRIANGLE  WAVE "};
uchar code dis8[] = {" SAWTOOTH  WAVE "};

bit  out1=0,out2=0,out3=0,out4=0,START=0;

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

bit   bdata SystemError;                //�ӻ������־λ

uchar N=0x00,k_count;
/*********************** PCF8591ר�ñ������� ***********************/

#define	PCF8591_WRITE	0x90
#define	PCF8591_READ 	0x91
#define  NUM  4 	            //���պͷ��ͻ����������
uchar idata receivebuf[NUM];    //���ݽ��ջ�����

uchar  code  sin_tab[] =        //���Ҳ������
{
     0x80,0x83,0x86,0x89,0x8D,0x90,0x93,0x96,0x99,0x9C,0x9F,0xA2,0xA5,0xA8,0xAB,0xAE,
     0xB1,0xB4,0xB7,0xBA,0xBC,0xBF,0xC2,0xC5,0xC7,0xCA,0xCC,0xCF,0xD1,0xD4,0xD6,0xD8,
     0xDA,0xDD,0xDF,0xE1,0xE3,0xE5,0xE7,0xE9,0xEA,0xEC,0xEE,0xEF,0xF1,0xF2,0xF4,0xF5,
     0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFD,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
     0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFD,0xFD,0xFC,0xFB,0xFA,0xF9,0xF8,0xF7,0xF6,
     0xF5,0xF4,0xF2,0xF1,0xEF,0xEE,0xEC,0xEA,0xE9,0xE7,0xE5,0xE3,0xE1,0xDF,0xDD,0xDA,
     0xD8,0xD6,0xD4,0xD1,0xCF,0xCC,0xCA,0xC7,0xC5,0xC2,0xBF,0xBC,0xBA,0xB7,0xB4,0xB1,
     0xAE,0xAB,0xA8,0xA5,0xA2,0x9F,0x9C,0x99,0x96,0x93,0x90,0x8D,0x89,0x86,0x83,0x80,
     0x80,0x7C,0x79,0x76,0x72,0x6F,0x6C,0x69,0x66,0x63,0x60,0x5D,0x5A,0x57,0x55,0x51,
     0x4E,0x4C,0x48,0x45,0x43,0x40,0x3D,0x3A,0x38,0x35,0x33,0x30,0x2E,0x2B,0x29,0x27,
     0x25,0x22,0x20,0x1E,0x1C,0x1A,0x18,0x16,0x15,0x13,0x11,0x10,0x0E,0x0D,0x0B,0x0A,
     0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
     0x0A,0x0B,0x0D,0x0E,0x10,0x11,0x13,0x15,0x16,0x18,0x1A,0x1C,0x1E,0x20,0x22,0x25,
     0x27,0x29,0x2B,0x2E,0x30,0x33,0x35,0x38,0x3A,0x3D,0x40,0x43,0x45,0x48,0x4C,0x4E,
     0x51,0x55,0x57,0x5A,0x5D,0x60,0x63,0x66,0x69,0x6C,0x6F,0x72,0x76,0x79,0x7C,0x7E
};

/*******************************************************************/
/*                                                                 */
/*  ��ʱ����                                                       */
/*                                                                 */
/*******************************************************************/
void delayB(uchar x)    //x*0.14MS
 {
   uchar i;
   while(x--)
   {
     for (i=0; i<13; i++)
     { }
   }
 }

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
/*  ����������                                                     */
/*                                                                 */
/*******************************************************************/
void beep()
 {
   uchar j;
   for (j=0;j<180;j++)
    { 
     delayB(5);
     BEEP=!BEEP;                 //BEEPȡ��
    } 
     BEEP=1;                    //�رշ�����
	 delay(100);
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
	flag = 0;
	delayNOP();
	if(SDA == 1)    // ��SDA=1������Ӧ����λ��Ӧ���־flag
    flag = 1;
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
void DAC_PCF8591(uchar controlbyte)
{

	iic_start();                    // ����I2C
	delayNOP();

	IICSendByte(PCF8591_WRITE);     // ���͵�ַλ
	check_ACK();                    // ���Ӧ��λ
    if(flag == 1)
	 {
		SystemError = 1;
		return;                     // ����Ӧ���ô����־λ
     }
    IICSendByte(controlbyte&0x77);	//Control byte
	check_ACK();                    //���Ӧ��λ
    if(flag == 1)
	 {
		SystemError = 1;
		return;                    // ����Ӧ���ô����־λ
	 }
}

//-------------------------------------------------------------------
// �������ƣ� ADC_PCF8591
// ��ڲ����� controlbyte������
// �������ܣ� ��������4·ͨ����A/Dת�������receivebuf
//-------------------------------------------------------------------
/*
void ADC_PCF8591(uchar controlbyte)
{
    uchar idata i; //,receive_da;

	iic_start();

	IICSendByte(PCF8591_WRITE);	//������
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

	IICSendByte(controlbyte);	//������
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

    iic_start();                //���·��Ϳ�ʼ����
   	IICSendByte(PCF8591_READ);	//������
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

    IICreceiveByte();   //�ն�һ�Σ�������˳��
    slave_ACK();        //�յ�һ���ֽں���һ��Ӧ��λ

//	while(i<4)
    	for(i=0;i<4;i++)
	{
	//  receive_da=IICreceiveByte();
	//  receivebuf[i]=receive_da;
      receivebuf[i] = IICreceiveByte();
	  slave_ACK();       //�յ�һ���ֽں���һ��Ӧ��λ
	}
	slave_NOACK();       //�յ����һ���ֽں���һ����Ӧ��λ
	iic_stop();
}*/

/*********************************************************
  ������ǲ�
*********************************************************/
void  Triangle()
{
    uchar l;
    for(l=0;l<255;l++)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //���Ӧ��λ
       if(flag == 1)
	   {
		SystemError = 1;
    	return;               //�ô����־λSystemError
	   }
	 }

      for(l=255;l>1;l--)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //���Ӧ��λ
       if(flag == 1)
	   {
		SystemError = 1;
    	return;               //�ô����־λSystemError
	   }
	 }
}

/*********************************************************
  �����������
*********************************************************/
void  Square()
{
     IICSendByte(N);	     //data byte
	 check_ACK();            //���Ӧ��λ
     if(flag == 1)
	 {
	   SystemError = 1;
       return;               //�ô����־λSystemError
	 }
      delay(15);
	  N=~N;
}

/*********************************************************
  ������Ҳ�����
*********************************************************/
void  sin()
{
  uint i;
  for(i=0; i<256; i++)
  {
    IICSendByte(sin_tab[i]);	  //data byte
	check_ACK();                  //���Ӧ��λ
    if(flag == 1)
	 {
	   SystemError = 1;
       return;                    //�ô����־λSystemError
	 }
  }

}

/*********************************************************
  �����ݲ�����
*********************************************************/
void  Sawtooth()
{
  uchar l;
    for(l=0;l<255;l++)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //���Ӧ��λ
       if(flag == 1)
	   {
		SystemError = 1;
    	return;                //�ô����־λSystemError
	   }
	 }
}

/*********************************************************
  ���ܽ�����
*********************************************************/
void  keyscan()
{
   uchar m;
   if((K2==0)&(!START))       //�������������K2��
   {
     k_count++;
     beep();   
     switch (k_count)
     {
        case 1: 
        lcd_pos(0x00);
	    for(m=0;m<16;m++)
        lcd_wdat(dis2[m]);
     
        lcd_pos(0x40);
	    for(m=0;m<16;m++)
        lcd_wdat(dis5[m]);
        out1=1;
		out4=0;
        break;
  
        case 2: 
        lcd_pos(0x40);
	    for(m=0;m<16;m++)
        lcd_wdat(dis6[m]); 
        out1=0;
		out2=1;
        break;
      
		case 3: 
        lcd_pos(0x40);
	    for(m=0;m<16;m++)
        lcd_wdat(dis7[m]);
		out2=0;
		out3=1;
     
        break;

        case 4: 
        lcd_pos(0x40);
	    for(m=0;m<16;m++)
        lcd_wdat(dis8[m]);
        k_count=0;
        out3=0;
		out4=1;
        break;
     
        default:  
        out4=0; out3=0; out2=0; out1=0;
        break;
     }   
   }
   
    if(K3==0)        //�������
	{
	   START=1;
	   beep();
	}

	if(K4==0)       //ֹͣ���
	{
	   START=0;
	   beep();
	}
}

/*******************************************************************/
/*                                                                 */
/*  ������                                                         */
/*                                                                 */
/*******************************************************************/
main()
{
    uchar i;
    delay(10);                 //��ʱ
    lcd_init();                //��ʼ��LCD

    lcd_pos(0);                //������ʾλ��Ϊ��һ��
    for(i=0;i<16;i++) 
    lcd_wdat(dis1[i]);         //��ʾ�ַ�
 

    lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ���
    for(i=0;i<16;i++)
    lcd_wdat(dis2[i]);         //��ʾ�ַ�

    delay(1500);

    lcd_pos(0);                //������ʾλ��Ϊ��һ��
    for(i=0;i<16;i++) 
    lcd_wdat(dis3[i]);         //��ʾ�ַ�
 
    lcd_pos(0x40);             //������ʾλ��Ϊ�ڶ���
    for(i=0;i<16;i++)
    lcd_wdat(dis4[i]);         //��ʾ�ַ�

	DAC_PCF8591(0x40);
  
	while(1)
    {
	   keyscan();
	 
	   while(out1&START)
	   {
         sin();              //������Ҳ�
         keyscan();
	   }

       while(out2&START)
	   {
         Square();           //�������
         keyscan();
	   }

	   while(out3&START) 
	   {
         Triangle();        //������ǲ�
         keyscan();
	   }
      
	    while(out4&START)
	   {
         Sawtooth();        //�����ݲ�
         keyscan();
	   }	
    }
}

//*********************************************************
