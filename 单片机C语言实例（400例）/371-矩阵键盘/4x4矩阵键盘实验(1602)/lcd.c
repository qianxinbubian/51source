#include<at89x52.h>
#include<lcd.h>

/*---------------------------------------------------------------------
------------------ LCD初始化-----------------------------------------*/

//LCD初始化程序;主要作用初始化LCD,对LCD进行复位以及设置;

void initialization(void)				//定义函数;
{
  delay(50);							//延时5MS;
  write_dictate(0x38,0);				//写指令38H;不检测忙;
  delay(50);							//延时5MS;
  write_dictate(0x38,0);				//写指令38H;不检测忙;
  delay(50);							//延时5MS;
  write_dictate(0x38,0);				//写指令38H;不检测忙;
  delay(50);
  write_dictate(0x38,1);				//显示模式设置;检测忙;
  write_dictate(0x08,1);				//关闭显示;检测忙;
  write_dictate(0x01,1);				//显示清屏;检测忙;
  write_dictate(0x06,1);				//显示光标移动设置;检测忙;
  write_dictate(0x0C,1);				//显示开及光标设置;检测忙;
}

/*---------------------------------------------------------------------
------------------ 写指令--------------------------------------------*/

//写指令子程序;无返回值;输入二个变量I和J.
//I为要写入LCD的指令;J为判断要不要检测忙.
//如果J为0则不判断检测忙;
//如果J为1则判断检测忙;

void write_dictate(uchar i,j)			//定义二个变量;
{
  if(j)     read_estate();              //根据需要检测忙; 
  LCD_DATA=i;                           //把要写入的数据送到LCD数据端;
  LCD_RS=0;			                    //RS置0;
  LCD_RW=0;								//RW置0;
  LCD_E =1;								//E端置0;
  delay(10);							//延时;
  LCD_E =0;								//E端置1;以锁存数据;
}

/*------------------------------------------------------------------
------------------ 写数据------------------------------------------*/

//写数据子程序;无返回值;输入变量I;
//I为要写入LCD中的数据;
//数据类型CHAR形;

void write_data(uchar i)				//定义输入变量值I;
{										
  read_estate();						//检测忙信号;
  LCD_DATA=i;							//把I中数据送到LCD数据端;
  LCD_RS=1;								//RS置1;
  LCD_RW=0;								//RW置0;
  LCD_E=0;								//E置0;
  delay(10);							//短延时;
  LCD_E=1;								//E置1;以锁存数据;
}

/*----------------------------------------------------------------
-------------------读状态----------------------------------------*/

//读状态子程序;有返回值;返回值类型为CHAR型;
//读回的状态通过RETURN返回;

uchar read_estate(void)					//定义有返回值的函数;
{
  LCD_DATA=0Xff;						//把LCD端口全置1方便读取信号;
  LCD_RS=0;								//RS置0;
  LCD_RW=1;								//RW置1;
  LCD_E=0;							    //E端置0;
  delay(10);							//短延时;
  LCD_E=1;								//E端置1;以锁存数据;
  while(LCD_DATA&0x80);                 //检测忙,则一直循环;
  return(LCD_DATA);						//返回读取的信号;
}

/*---------------------------------------------------------------
-------------------读数据--------------------------------------*/

//读数据子程序;有返回值,返回值类型为CHAR型;

/*uchar read_data(void)					//定义有返回值的子函数;
{
  LCD_DATA=0Xff;						//LCD数据端口置1;
  LCD_RS=1;								//RS置1;
  LCD_RW=1;								//RW置1;
  LCD_E=0;								//E置0;
  delay(10);							//短延时;
  LCD_E=1;								//E置1;以锁存数据;
  return(LCD_DATA);						//返回读取的值;
}


/*---------------------------------------------------------------
-------------------在指定位置显示一个字符----------------------*/
void displayonechar(uchar x, y,ddata)
{
y &= 0x01;
x &= 0x0f;                             //限制X不能大于15，Y不能大于1
if (y) x+= 0x40;                      //当要显示第二行时地址码+0x40;
x+= 0x80;                             //算出指令码
write_dictate(x,0);                    //这里不检测忙信号，发送地址码
write_data(ddata);
}

/*---------------------------------------------------------------
-------------------在指定位置显示一串字符----------------------*/
void displaylistchar(uchar x,y,uchar code *ddata)
{
  uchar a=0;
  y&=0x01;
  x&=0xf;
  while(ddata[a]>0x20)
  {
    if(x<=0xff)
	{  
	  displayonechar(x, y,ddata[a]);
	  a++;
	  x++;
	}
   }
}

/*---------------------------------------------------------------
-------------------延时子程序----------------------------------*/
void delay(uint a)
{
  uint b;
  for(b=0;b<a;b++);
}


//400ms延时
void delay400ms(void)
{
  uchar a = 5;
  uint b;
  while(a--)
  {
    b=7269;
    while(b--);
  }
}
