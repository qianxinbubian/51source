
//-----------------------函数声明，变量定义------------------------
#include <reg51.h>
#include <intrins.h>
sbit SCL=P3^4;                   // 将p1.2口模拟时钟口
sbit SDA=P3^5;                   // 将p1.3口模拟数据口
bit askflag;
#define uchar unsigned char
#define uint  unsigned int
#define disdata  P0              //显示数据码输出口

sbit LCD_RS = P2^0;             
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

uint data dis[4]={0x00,0x00,0x00,0x00}; 
                          //定义3个显示数据单元和1个数据存储单元

uchar code dis4[] = {"1- .  V  2- .  V"};
uchar code dis5[] = {"3- .  V  4- .  V"};


#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
	                 
bit   bdata SystemError;                //从机错误标志位

/*********************** PCF8591专用变量定义 ***********************/

#define	PCF8591_WRITE	0x90 
#define	PCF8591_READ 	0x91
#define  NUM  4 	            //接收和发送缓存区的深度
uchar idata receivebuf[NUM];    //数据接收缓冲区

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

/******************************************************************/
/*                                                                */
/* 数据处理与显示                                                 */
/* 将采集到的数据进行16进制转换为ASCLL码。                        */
/*                                                                */
/******************************************************************/

show_value(uchar ad_data)

{   
    dis[2]=ad_data/51;   //AD值转换为3为BCD码，最大为5.00V。
    dis[2]=dis[2]+0x30;	 //转换为ACSII码
    dis[3]=ad_data%51;   //余数暂存
    dis[3]=dis[3]*10;    //计算小数第一位
    dis[1]=dis[3]/51;
    dis[1]=dis[1]+0x30;	 //转换为ACSII码
    dis[3]=dis[3]%51;
    dis[3]=dis[3]*10;    //计算小数第二位
    dis[0]=dis[3]/51;                                                                             //
    dis[0]=dis[0]+0x30;  //转换为ACSII码           
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
	askflag = 0;
	delayNOP();   
	if(SDA == 1)    // 若SDA=1表明非应答，置位非应答标志askflag
    askflag = 1;
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
void DAC_PCF8591(uchar controlbyte,uchar w_data)
{    
	
	iic_start();                    // 启动I2C
	delayNOP();

	IICSendByte(PCF8591_WRITE);     // 发送地址位
	check_ACK();                    // 检查应答位
    if(askflag == 1)
	 { 
		SystemError = 1;
		return;                     // 若非应答，置错误标志位
     }
    IICSendByte(controlbyte&0x77);	//Control byte 
	check_ACK();                    //检查应答位
    if(askflag == 1)
	 { 
		SystemError = 1;
		return;                    // 若非应答，置错误标志位
	 }
    IICSendByte(w_data);	       //data byte
	check_ACK();                  // 检查应答位
    if(askflag == 1)
	 { 
		SystemError = 1;
    	return;   // 若非应答表明器件错误或已坏，置错误标志位SystemError
	 }
	iic_stop();         // 全部发完则停止
	delayNOP();
	delayNOP();
	delayNOP();
	delayNOP();	
}
//-------------------------------------------------------------------
// 函数名称： ADC_PCF8591
// 入口参数： controlbyte控制字
// 函数功能： 连续读入4路通道的A/D转换结果到receivebuf
//-------------------------------------------------------------------
void ADC_PCF8591(uchar controlbyte)
{ 
    uchar idata receive_da,i=0;

	iic_start();

	IICSendByte(PCF8591_WRITE);	//控制字
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}

	IICSendByte(controlbyte);	//控制字
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}

    iic_start();                //重新发送开始命令
   	IICSendByte(PCF8591_READ);	//控制字
	check_ACK();
	if(askflag == 1)
	{
		SystemError = 1;
		return;
	}
	 
    IICreceiveByte();   //空读一次，调整读顺序
    slave_ACK();        //收到一个字节后发送一个应答位

	while(i<4)
	{  
	  receive_da=IICreceiveByte();
	  receivebuf[i++]=receive_da;
	  slave_ACK();       //收到一个字节后发送一个应答位
	}
	slave_NOACK();       //收到最后一个字节后发送一个非应答位
	iic_stop();
}
//-------------------------------------------------------------------
// 函数名称： main
// 函数功能： 主程序
//-------------------------------------------------------------------
main()
{
    uchar i,l;
    delay(10);                 //延时
    lcd_init();                //初始化LCD             
        
    lcd_pos(0);                //设置显示位置为第一行的第1个字符
     i = 0;
    while(dis4[i] != '\0')
     {                         //显示字符
       lcd_wdat(dis4[i]);
       i++;
     }

    lcd_pos(0x40);             //设置显示位置为第二行第1个字符
     i = 0;
    while(dis5[i] != '\0')
     {
       lcd_wdat(dis5[i]);      //显示字符
       i++;
     }
	
  while(1)
  {
	iicInit();	              //I2C总线初始化
    ADC_PCF8591(0x04);

	if(SystemError == 1)	  //有错误，重新来
	  {
	  	iicInit();				  //I2C总线初始化
	    ADC_PCF8591(0x04);
	   }   
	
	for(l=0;l<4;l++)	
	 {
	  show_value(receivebuf[0]); //显示通道0       
	    lcd_pos(0x02);             
        lcd_wdat(dis[2]);        //整数位显示
        lcd_pos(0x04);             
        lcd_wdat(dis[1]);        //第一位小数显示 
        lcd_pos(0x05);             
        lcd_wdat(dis[0]);        //第二位小数显示

      show_value(receivebuf[1]); //显示通道1	 
	    lcd_pos(0x0b);             
        lcd_wdat(dis[2]);        //整数位显示
        lcd_pos(0x0d);             
        lcd_wdat(dis[1]);        //第一位小数显示 
        lcd_pos(0x0e);             
        lcd_wdat(dis[0]);        //第二位小数显示

	  show_value(receivebuf[2]); //显示通道2         	 
	    lcd_pos(0x42);             
        lcd_wdat(dis[2]);        //整数位显示
        lcd_pos(0x44);             
        lcd_wdat(dis[1]);        //第一位小数显示 
        lcd_pos(0x45);             
        lcd_wdat(dis[0]);        //第二位小数显示

      show_value(receivebuf[3]); //显示通道3	 
	    lcd_pos(0x4b);             
        lcd_wdat(dis[2]);        //整数位显示
        lcd_pos(0x4d);              
        lcd_wdat(dis[1]);        //第一位小数显示 
        lcd_pos(0x4e);             
        lcd_wdat(dis[0]);        //第二位小数显示

	  iicInit();	       	      //I2C总线初始化  
      DAC_PCF8591(0x40,receivebuf[0]); //D/A输出

	   	if(SystemError == 1)	  //有错误，重新来
	    {
	  	 iicInit();				  //I2C总线初始化
		 DAC_PCF8591(0x40,receivebuf[0]); //D/A输出
	    }	        

      //delay(50);                       //延时
	 }

   }
}
