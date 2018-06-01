#include <reg52.h>
#include <intrins.h>
#include <string.h>

#define uchar unsigned char
#define uint  unsigned int
#define disdata  P0              //显示数据码输出口
bit flag;
sbit SCL=P3^4;                   // 将p1.0口模拟时钟口
sbit SDA=P3^5;                   // 将p1.3口模拟数据口


sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

sbit  K1   = P1^4;       
sbit  K2   = P1^5;       //选择健
sbit  K3   = P1^6;       //启动健
sbit  K4   = P1^7;       //停止健
sbit  BEEP = P3^6;       //蜂鸣器

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

bit   bdata SystemError;                //从机错误标志位

uchar N=0x00,k_count;
/*********************** PCF8591专用变量定义 ***********************/

#define	PCF8591_WRITE	0x90
#define	PCF8591_READ 	0x91
#define  NUM  4 	            //接收和发送缓存区的深度
uchar idata receivebuf[NUM];    //数据接收缓冲区

uchar  code  sin_tab[] =        //正弦波输出表
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
/*  延时函数                                                       */
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
/*  延时函数                                                       */
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
/*  蜂鸣器函数                                                     */
/*                                                                 */
/*******************************************************************/
void beep()
 {
   uchar j;
   for (j=0;j<180;j++)
    { 
     delayB(5);
     BEEP=!BEEP;                 //BEEP取反
    } 
     BEEP=1;                    //关闭蜂鸣器
	 delay(100);
 }


/*******************************************************************/
/*                                                                 */
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
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
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
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
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
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
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/

void lcd_init()
{
    delay(15);
    lcd_wcmd(0x38);      //16*2显示，5*7点阵，8位数据
    delay(5);
    lcd_wcmd(0x38);
    delay(5);
    lcd_wcmd(0x38);
    delay(5);

    lcd_wcmd(0x0c);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x06);      //移动光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}

/*******************************************************************/
/*                                                                 */
/*  设定显示位置                                                   */
/*                                                                 */
/*******************************************************************/

void lcd_pos(uchar pos)
{
  lcd_wcmd(pos | 0x80);  //数据指针=80+地址变量
}

//-------------------------------------------------------------------
// 函数名称： iic_start()
// 函数功能： 启动I2C总线子程序
//-------------------------------------------------------------------
void iic_start(void)
{ //时钟保持高，数据线从高到低一次跳变，I2C通信开始
	SDA = 1;
	SCL = 1;
	delayNOP();      // 延时5us
   	SDA = 0;
	delayNOP();
    SCL = 0;
}
//-------------------------------------------------------------------
// 函数名称： iic_stop()
// 函数功能： 停止I2C总线数据传送子程序
//-------------------------------------------------------------------
void iic_stop(void)
{
	SDA = 0;   	   //时钟保持高，数据线从低到高一次跳变，I2C通信停止
	SCL = 1;
	delayNOP();
	SDA = 1;
	delayNOP();
    SCL = 0;
}
//------------------------------------------------------------------
// 函数名称： iicInit_()
// 函数功能： 初始化I2C总线子程序
//------------------------------------------------------------------
 void iicInit(void)
   {
   SCL = 0;
   iic_stop();
   }
//-------------------------------------------------------------------
// 函数名称： slave_ACK
// 函数功能： 从机发送应答位子程序
//-------------------------------------------------------------------
void slave_ACK(void)
{
	SDA = 0;
	SCL = 1;
	delayNOP();
	SCL = 0;
}
//-------------------------------------------------------------------
// 函数名称： slave_NOACK
// 函数功能： 从机发送非应答位子程序，迫使数据传输过程结束
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
// 函数名称： check_ACK
// 函数功能： 主机应答位检查子程序，迫使数据传输过程结束
//-------------------------------------------------------------------
void check_ACK(void)
{
    SDA = 1;        // 将p1.1设置成输入，必须先向端口写1
	SCL = 1;
	flag = 0;
	delayNOP();
	if(SDA == 1)    // 若SDA=1表明非应答，置位非应答标志flag
    flag = 1;
   	SCL = 0;
}
//-------------------------------------------------------------------
// 函数名称： IICSendByte
// 入口参数： ch
// 函数功能： 发送一个字节
//-------------------------------------------------------------------
void IICSendByte(uchar ch)

{
  	unsigned char idata n=8;     // 向SDA上发送一位数据字节，共八位

	while(n--)
	{
	if((ch&0x80) == 0x80)    // 若要发送的数据最高位为1则发送位1
	   {
	 		SDA = 1;    // 传送位1
			SCL = 1;
		    delayNOP();
		//	SDA = 0;
			SCL = 0;
	   }
		else
		{
			SDA = 0;    // 否则传送位0
			SCL = 1;
			delayNOP();
		  	SCL = 0;
		}
		ch = ch<<1;    // 数据左移一位
	}
}
//-------------------------------------------------------------------
// 函数名称： IICreceiveByte
// 返回接收的数据
// 函数功能： 接收一字节子程序
//-------------------------------------------------------------------
uchar IICreceiveByte(void)
{
	uchar idata n=8;     // 从SDA线上读取一上数据字节，共八位
	uchar tdata=0;
	while(n--)
	{
	   SDA = 1;
	   SCL = 1;
	   tdata =tdata<<1;		    //左移一位
	   	if(SDA == 1)
		  tdata = tdata|0x01;   // 若接收到的位为1，则数据的最后一位置1
		else
		  tdata = tdata&0xfe;   // 否则数据的最后一位置0
	   SCL = 0;
	 }

	 return(tdata);
}
//-------------------------------------------------------------------
// 函数名称： DAC_PCF8591
// 入口参数： slave_add从机地址,n要发送的数据个数
// 函数功能： 发送n位数据子程序
//-------------------------------------------------------------------
void DAC_PCF8591(uchar controlbyte)
{

	iic_start();                    // 启动I2C
	delayNOP();

	IICSendByte(PCF8591_WRITE);     // 发送地址位
	check_ACK();                    // 检查应答位
    if(flag == 1)
	 {
		SystemError = 1;
		return;                     // 若非应答，置错误标志位
     }
    IICSendByte(controlbyte&0x77);	//Control byte
	check_ACK();                    //检查应答位
    if(flag == 1)
	 {
		SystemError = 1;
		return;                    // 若非应答，置错误标志位
	 }
}

//-------------------------------------------------------------------
// 函数名称： ADC_PCF8591
// 入口参数： controlbyte控制字
// 函数功能： 连续读入4路通道的A/D转换结果到receivebuf
//-------------------------------------------------------------------
/*
void ADC_PCF8591(uchar controlbyte)
{
    uchar idata i; //,receive_da;

	iic_start();

	IICSendByte(PCF8591_WRITE);	//控制字
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

	IICSendByte(controlbyte);	//控制字
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

    iic_start();                //重新发送开始命令
   	IICSendByte(PCF8591_READ);	//控制字
	check_ACK();
	if(flag == 1)
	{
		SystemError = 1;
		return;
	}

    IICreceiveByte();   //空读一次，调整读顺序
    slave_ACK();        //收到一个字节后发送一个应答位

//	while(i<4)
    	for(i=0;i<4;i++)
	{
	//  receive_da=IICreceiveByte();
	//  receivebuf[i]=receive_da;
      receivebuf[i] = IICreceiveByte();
	  slave_ACK();       //收到一个字节后发送一个应答位
	}
	slave_NOACK();       //收到最后一个字节后发送一个非应答位
	iic_stop();
}*/

/*********************************************************
  输出三角波
*********************************************************/
void  Triangle()
{
    uchar l;
    for(l=0;l<255;l++)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //检查应答位
       if(flag == 1)
	   {
		SystemError = 1;
    	return;               //置错误标志位SystemError
	   }
	 }

      for(l=255;l>1;l--)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //检查应答位
       if(flag == 1)
	   {
		SystemError = 1;
    	return;               //置错误标志位SystemError
	   }
	 }
}

/*********************************************************
  输出方波函数
*********************************************************/
void  Square()
{
     IICSendByte(N);	     //data byte
	 check_ACK();            //检查应答位
     if(flag == 1)
	 {
	   SystemError = 1;
       return;               //置错误标志位SystemError
	 }
      delay(15);
	  N=~N;
}

/*********************************************************
  输出正弦波函数
*********************************************************/
void  sin()
{
  uint i;
  for(i=0; i<256; i++)
  {
    IICSendByte(sin_tab[i]);	  //data byte
	check_ACK();                  //检查应答位
    if(flag == 1)
	 {
	   SystemError = 1;
       return;                    //置错误标志位SystemError
	 }
  }

}

/*********************************************************
  输出锯齿波函数
*********************************************************/
void  Sawtooth()
{
  uchar l;
    for(l=0;l<255;l++)
	 { 
	   IICSendByte(l);	       //data byte
	   check_ACK();            //检查应答位
       if(flag == 1)
	   {
		SystemError = 1;
    	return;                //置错误标志位SystemError
	   }
	 }
}

/*********************************************************
  功能健函数
*********************************************************/
void  keyscan()
{
   uchar m;
   if((K2==0)&(!START))       //启动输出后，屏蔽K2健
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
   
    if(K3==0)        //启动输出
	{
	   START=1;
	   beep();
	}

	if(K4==0)       //停止输出
	{
	   START=0;
	   beep();
	}
}

/*******************************************************************/
/*                                                                 */
/*  主函数                                                         */
/*                                                                 */
/*******************************************************************/
main()
{
    uchar i;
    delay(10);                 //延时
    lcd_init();                //初始化LCD

    lcd_pos(0);                //设置显示位置为第一行
    for(i=0;i<16;i++) 
    lcd_wdat(dis1[i]);         //显示字符
 

    lcd_pos(0x40);             //设置显示位置为第二行
    for(i=0;i<16;i++)
    lcd_wdat(dis2[i]);         //显示字符

    delay(1500);

    lcd_pos(0);                //设置显示位置为第一行
    for(i=0;i<16;i++) 
    lcd_wdat(dis3[i]);         //显示字符
 
    lcd_pos(0x40);             //设置显示位置为第二行
    for(i=0;i<16;i++)
    lcd_wdat(dis4[i]);         //显示字符

	DAC_PCF8591(0x40);
  
	while(1)
    {
	   keyscan();
	 
	   while(out1&START)
	   {
         sin();              //输出正弦波
         keyscan();
	   }

       while(out2&START)
	   {
         Square();           //输出方波
         keyscan();
	   }

	   while(out3&START) 
	   {
         Triangle();        //输出三角波
         keyscan();
	   }
      
	    while(out4&START)
	   {
         Sawtooth();        //输出锯齿波
         keyscan();
	   }	
    }
}

//*********************************************************
