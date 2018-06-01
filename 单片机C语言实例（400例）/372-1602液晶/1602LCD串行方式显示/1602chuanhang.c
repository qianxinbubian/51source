/*******************************************************************
    
* 描述：                                                           
*    LCD1602可以分为8位和4位控制方式，8位控制方式是用D0-D7数据线   
* 来传送控制命令及数据。4位控制方式是用D4-D7数据线来传送控制命令   
* 及数据。使用4位数据线做控制时，需要分两次来传送，先送出高4位数   
* 据，再送出低4位数据。可以节省单片机的4根端口线。                 
*                                                                  
*******************************************************************/
#include < reg52.h >
#include < intrins.h >
#define uchar unsigned char
#define uint  unsigned int
sbit  LCD_RS = P2^0;             
sbit  LCD_RW = P2^1;
sbit  LCD_EN = P2^2;
uchar code  cdis1[ ] = {"   WELCOME TO   "}; 
uchar code  cdis2[ ] = {" WWW.RICHMCU.COM "};
/**********************************************************

* 5us 延时子程序

**********************************************************/
void  delayNOP()
{
   _nop_();
   _nop_();
   _nop_();
   _nop_();
   _nop_();
}

/**********************************************************

* 延时子程序

**********************************************************/
void delay(uint ms) 

{
   uchar t;
   while(ms--)
   { 
     for(t = 0; t < 120; t++);
   }
}


/**********************************************************

* 检查LCD忙状态                                           
* lcd_busy为1时，忙，等待。                               
* lcd-busy为0时,闲，可写指令与数据                        

**********************************************************/ 
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

/********************************************************* 

* 写指令或数据

* start=0, 写入命令;  start=1, 写入数据

*********************************************************/

void lcd_write(bit start, uchar in_data) 
{
    uchar Hdata,Ldata;

    while(lcd_busy());

    Hdata=in_data&0xf0;             //取高四位
    Ldata=(in_data<<4)&0xf0;    //取低四位

    if(start==0) 
    LCD_RS = 0;               //写入命令
    else
    LCD_RS = 1;                 //写入数据

    LCD_RW = 0;
    LCD_EN = 0;
    delayNOP();

    P0 = Hdata;                 //发送高四位   
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;
    delayNOP();

    P0 = Ldata;                 //发送低四位     
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;
    delayNOP();
}

/*************************************************************

*  LCD初始化设定                                             

*************************************************************/
void lcd_init()
{
    delay(15);
    lcd_write(0,0x28);                //16*2显示，5*7点阵，4位数据
    delay(5);
    lcd_write(0,0x28);
    delay(5);
    lcd_write(0,0x28);

    delay(5);
    lcd_write(0,0x0c);               //显示开，关光标
    delay(5);
    lcd_write(0,0x06);              //移动光标
    delay(5);
    lcd_write(0,0x01);              //清除LCD的显示内容
    delay(25);                          //延时
}

/**********************************************************

*  设定显示位置                                             
                                                        
**********************************************************/
void lcd_pos(uchar pos)
{                          
   lcd_write(0,pos|0x80);     //数据指针=80+地址变量
}

/**********************************************************

* 主函数

**********************************************************/
void  main()
{
    uchar  m;

    lcd_init();                          //LCD1602初始化

    lcd_pos(0x00);                 //设置显示位置为第一行
    for(m=0;m<16;m++)
    lcd_write(1,cdis1[m]);


     lcd_pos(0x40);                //设置显示位置为第二行     
     for(m=0;m<16;m++) 
     lcd_write(1,cdis2[m]);

      while(1);
}

/*********************************************************/