#include <AT89X51.H>
#include <intrins.h>
#define Key_UP            P1_4 
#define Key_DOWN          P1_5   
#define Key_SET           P1_6   
#define RelayOutPort      P3_7  
#define LEDPort           P0      
#define DELPort           P3_6   
#define LEDTwoC           P2_6    
#define LEDThreeC         P2_7    
#define TMPort			  P3_3
#define INBUF_LEN 5 //数据长度
unsigned char inbuf1[INBUF_LEN]={'0','0','0','0','0'};//发送缓冲区
unsigned char inbuf2[50];//接收缓冲区
unsigned char count3;
void init_serialcomm( void )
{
SCON = 0x50 ; //SCON: serail mode 1, 8-bit UART, enable ucvr
TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
PCON |= 0x80 ; //SMOD=1;
TH1 = 0xFA ; //Baud:4800 fosc=11.0592MHz
IE |= 0x90 ; //Enable Serial Interrupt
TR1 = 1 ; // timer 1 run
}
//向串口发送一个字符
void send_char_com( unsigned char ch)
{
SBUF=ch;
while (TI== 0 );
TI= 0 ;
}
//向串口发送一个字符串，strlen 为该字符串长度
void send_string_com( unsigned char *str, unsigned int strlen)
{
unsigned int k= 0 ;
do
{
send_char_com(*(str + k));
k++;
} while (k < strlen);
}
//串口接收中断函数
void serial () interrupt 4 using 3
{
if (RI) //RI==开始接收
{
unsigned char ch;
RI = 0 ; //软件RI=0
ch=SBUF;
if (ch> 1 )
{
count3= 0 ;
inbuf2[count3]=ch;
}
else
{
count3++;
inbuf2[count3]=ch;
}
}
}	

unsigned char code LEDDis[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF}; //0-9的LED笔划,0xFF为空,0xF7为负号
unsigned char dis_8[12]={'0','1','2','3','4','5','6','7','8','9',' ','-'};
static unsigned char bdata StateREG;	//可位寻址的状态寄存器
sbit DS1820ON = StateREG^0;	          //DS1820是否存在
sbit SetTF = StateREG^1;              //是否是在温度设置状态
sbit KeySETDown = StateREG^2;         //是否已按过SET键标识
sbit PowTF = StateREG^3;              //电源电源标识
sbit KeyTF = StateREG^4;							//键盘是否允许

//sbit KeySETDowning = StateREG^5;			//SET是否正在按下
static unsigned char bdata TLV _at_ 0x0029;         //温度变量高低位
static unsigned char bdata THV _at_ 0x0028;
static signed char TMV;       //转换后的温度值
static unsigned char KeyV,TempKeyV;      //键值
static unsigned char Second;
static unsigned char flag;
static signed char TMRomV _at_ 0x0027;    //高温限制
static signed char TMSetV _at_ 0x0026;    //温度设定值
static unsigned char KSDNum;    //SET键连按时的采集次数
static unsigned char IntNum,IntNum2,IntNum3;    //中断发生次数，IntNum用于SET长按检测，IntNum2用于设定状态时LED闪烁
static signed char LED_One,LED_Two,LED_Three;  //LED的显示位 LED_One为十位，LED_Two为个位
static unsigned char Sign; //负号标识

void main(void)
{

	void InitDS1820(void);   //定义函数
	void ROMDS1820(void);
	void TMVDS1820(void);
	void TMRDS1820(void);
	void TMWDS1820(void);
	void TMREDS1820(void);
	void TMERDS1820(void);
	void ReadDS1820(void);
	void WriteDS1820(void);
	void Delay_510(void);
	void Delay_110(void);
	void Delay_10ms(void);
	void Delay_4s(void);
	void V2ToV(void);
	
	 DELPort=1;
	StateREG = 0;   //初始化变量
	SetTF = 0;	
	PowTF = 0;			//关电源
	THV = 0;
	TLV = 0;
	TMV = 0;
	KeyV = 0;
	TempKeyV = 0;
	KSDNum = 0;
	IntNum = 0;
	IntNum2 = 0;
	IntNum3 = 0;
	LED_One = 0;
	LED_Two	= 0;

	InitDS1820(); //初始化
	ROMDS1820(); 	//跳过ROM
	TMERDS1820();  //E2PRAM中温度上限值调入RAM
	InitDS1820(); //初始化
	ROMDS1820(); 	//跳过ROM
	TMRDS1820(); 	//读出温度指令
	ReadDS1820(); //读出温度值和上限值
	TMSetV = TMRomV;  //拷贝保存在DS18B20ROM里的上限值到TMSetV

	EA = 1;					//允许CPU中断
	ET0 = 1; 				//定时器0中断打开
	TMOD = 0x1;			//设定时器0为模式1，16位模式
	TH0=0xB1;
	TL0=0xDF;				//设定时值为20000us（20ms）
	TR0 = 1;
	
	while(1){				//开始定时

	

	 


if (flag==0){
if (Second==1){
			Delay_4s();
			Delay_4s();
			Delay_4s();
			Delay_4s();
			Delay_4s();
			Delay_4s();
			
		  DELPort=0; 
		  Second=0;
		  }
		   }
		 }
	
	
}

//定时器0中断外理中键扫描和显示
void KeyAndDis_Time0(void) interrupt 1 using 2
{
  TH0=0xB1;
	TL0=0xDF;				//设定时值为20000us（20ms)

	LEDPort = 0xFF;
	
   
	   if (inbuf2[0]==0x33){
	   send_char_com('O');
	   send_char_com('O');
	   send_char_com('O');
	   SetTF = 1;
//send_string_com(inbuf2,1);
//BEEP=0;
//RELAY=0;
send_char_com('O');
     //inbuf2[0]=0x00;
	 inbuf2[0]=0x00;
} 

if (inbuf2[0]==0x36){

//send_string_com(inbuf2,1);
//BEEP=1;
send_char_com('N');
send_char_com('N');
send_char_com('N');
SetTF = 0;

inbuf2[0]=0x00;

} 

if (inbuf2[0]==0x34){
	   KeyV=2;
//send_string_com(inbuf2,1);
//BEEP=0;
//RELAY=0;
  TMSetV = TMSetV - 1;
    inbuf2[0]=0x00;
} 

if (inbuf2[0]==0x35){

//send_string_com(inbuf2,1);
//BEEP=1;
//RELAY=1;
 TMSetV = TMSetV + 1;    //上调温度
 inbuf2[0]=0x00;
} 

if (inbuf2[0]==0x74){
	 KeyV=2;
//send_string_com(inbuf2,1);
//BEEP=1;
//RELAY=1;
} 

if (inbuf2[0]==0x90){
	 KeyV=1;
//send_string_com(inbuf2,1);
//BEEP=1;
//RELAY=1;
} 


  if (!Key_UP)
    KeyV = 1;
  if (!Key_DOWN)
    KeyV = 2;
  if (!Key_SET)
    KeyV = 3;
	//KeySETDowning = 0;  //清除
  if (KeyV != 0)      //有键按下
    {
      Delay_10ms();   //延时防抖  按下10ms再测
      if (!Key_UP)
        TempKeyV = 1;
      if (!Key_DOWN)
        TempKeyV = 2;
      if (!Key_SET)
        TempKeyV = 3;
      if (KeyV == TempKeyV)   //两次值相等为确定接下了键
        {
          if (KeyV == 3)      //按下SET键，如在SET状态就退出，否则进入
            {
							//KeySETDowning = 1; //表明SET正在按下
							
			
              PowTF = 0;      //电源标识开
							if (!KeyTF)
              	if (SetTF){
				send_char_com('N');
send_char_com('N');
send_char_com('N');
                		SetTF = 0;    //标识位标识退出设定
						
										InitDS1820(); //初始化
  									ROMDS1820(); 	//跳过ROM
  									TMWDS1820(); 	//写温度上限指令
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										WriteDS1820(); 	//写温度上限到DS18B20ROM
										InitDS1820(); //初始化
  									ROMDS1820(); 	//跳过ROM
  									TMREDS1820(); 	//温度上限值COPY回E2PRAM
									}
              	else {
				send_char_com('O');
	   send_char_com('O');
	   send_char_com('O');
                	SetTF = 1;}
					 
				   
              if (!KeySETDown) //没有第一次按下SET时，KeySETDown标识置1
                KeySETDown = 1;
              else
                KSDNum = KSDNum + 1;  //前一秒内有按过SET则开始计数
            }
          if (SetTF)  //在SET状态下
            {
			 
              if ((KeyV == 1) && (!KeyTF))
                TMSetV = TMSetV + 1;    //上调温度
              if ((KeyV == 2) && (!KeyTF))
                TMSetV = TMSetV - 1;    //下调温度
              if (TMSetV <= 20)          //限制温度上下限
                TMSetV = 20;
              if (TMSetV >= 75)
                TMSetV = 75;
            }
		if ((!KeyTF) && (IntNum3 == 0)) KeyTF = 1; //当键盘处于可用时，锁定
        }

     
    }
  KeyV = 0;
  TempKeyV = 0;   //清空变量准备下次键扫描

  if (!PowTF)
    {
      InitDS1820();	//初始化
      ROMDS1820(); 	//跳过ROM
      TMVDS1820();  //温度转换指令

      Delay_510();
      Delay_510();  //延时等待转换完成

      InitDS1820(); //初始化
      ROMDS1820(); 	//跳过ROM
      TMRDS1820(); 	//读出温度指令
      ReadDS1820(); //读出温度值

      V2ToV();        //转换显示值
      if (TMV > TMSetV)   //根据采集到的温度值控制继电器
        {
          RelayOutPort = 0;
		  flag=0;
		  }
      else
		    {
          RelayOutPort = 1;
		  DELPort=1; 
		  Second=1;
		  flag=1;

		
  		  }

      if (SetTF) IntNum2 = IntNum2 + 1; //用于闪烁计数
      if (IntNum2 > 50 ) IntNum2 = 0;
			if (KeyTF) IntNum3 = IntNum3 + 1; //用于防止按键连按
			if (IntNum3 > 25) 
				{
					IntNum3 = 0;
					KeyTF = 0;
				}

      if ((SetTF) && (IntNum2 < 25)) goto InitEnd;  //计数在后半段时显示
         
	  LEDPort = LED_Two;
      LEDTwoC = 0;
      Delay_510();
      LEDTwoC = 1;    //显示十位数
      LEDPort = LED_Three;
      LEDThreeC = 0;
      Delay_510();
      LEDThreeC = 1;    //显示个位数
    }
InitEnd:;
}

void V2ToV(void)	//数值转换
{
  TLV = TLV >> 4;			
  THV = THV << 4;			//读出的高低位数值移位
  TMV = TLV | THV;		//合并高低位放入TM为实际温度值
	Sign = 0;
  if (SetTF || !Key_SET)
    Sign = TMSetV >> 7;          //取符号
  else
     Sign = TMV >> 7;

	if (Sign)
		{
  		if (SetTF || !Key_SET)
				{
    			LED_One = (~(TMSetV-1)) / 100;          //SET状态下显示设定值
    			LED_Two = ((~(TMSetV-1)) - LED_One * 100)/10;
			    LED_Three = (~(TMSetV-1)) - LED_One * 100 - LED_Two * 10;
				inbuf1[0]=dis_8[LED_Two];
  inbuf1[1]=dis_8[LED_Three];
  //inbuf1[2]=dis_8[ LED_Three];
  inbuf1[2]=0x0d;
  inbuf1[3]=0x0a;
				}
  		else
				{
    			LED_One = (~(TMV-1)) / 100;							//转换百位值		
    			LED_Two = ((~(TMV-1)) - LED_One * 100)/10;
			    LED_Three = (~(TMV-1)) - LED_One * 100 - LED_Two * 10;
					inbuf1[0]=dis_8[LED_Two];
  inbuf1[1]=dis_8[LED_Three];
 inbuf1[2]=dis_8[0];
  inbuf1[3]=0x0d;
  inbuf1[4]=0x0a;
				}
		}
	else
		{ 
  		if (SetTF || !Key_SET)
				{
    			LED_One = (TMSetV) / 100;          //SET状态下显示设定值
    			LED_Two = (TMSetV - LED_One * 100)/10;
			    LED_Three = TMSetV - LED_One * 100 - LED_Two * 10;
					inbuf1[0]=dis_8[LED_Two];
  inbuf1[1]=dis_8[LED_Three];
 inbuf1[2]=dis_8[0];
  inbuf1[3]=0x0d;
  inbuf1[4]=0x0a;
				}
  		else
				{
    			LED_One = (TMV) / 100;							//转换百位值		
    			LED_Two = (TMV - LED_One * 100)/10;
			    LED_Three = TMV - LED_One * 100 - LED_Two * 10;
					inbuf1[0]=dis_8[LED_Two];
  inbuf1[1]=dis_8[LED_Three];
 inbuf1[2]=dis_8[0];
  inbuf1[3]=0x0d;
  inbuf1[4]=0x0a;
				}
		}

   init_serialcomm(); //初始化串口

//while ( 1 )
//{
send_string_com(inbuf1,INBUF_LEN);

 //转LED字段	
	if (LED_One) //超过百时十位的处理
		LED_Two = LEDDis[LED_Two];		
	else
		{
			if (LED_Two == 0)
				LED_Two = LEDDis[10];
			else
				LED_Two = LEDDis[LED_Two];
		}
	if (Sign)
		LED_One = LEDDis[11];
	else
		{
			if (LED_One == 0)
				LED_One = LEDDis[10];
			else
				LED_One = LEDDis[LED_One];
		}
	LED_Three = LEDDis[LED_Three];
}

void InitDS1820(void)		//初始化DS1820
{
	TMPort = 1;		//拉高TMPort
	_nop_();      //保持一个周期
	TMPort = 0;		//拉低TMPort
  Delay_510();  //延时  DS1820复位时间要500us的低电平
  TMPort = 1;   //拉高TMPort
	_nop_();      //保持  
	_nop_();
	_nop_();

  Delay_110();  //延时110us 等待DS1820回应
  if (!TMPort)  //回应信号为低电平
    DS1820ON = 1;
  else
    DS1820ON = 0;
  Delay_110();  //延时
  Delay_110();
  TMPort = 1;   //拉高TMPort
}

void ROMDS1820(void)  //跳过ROM匹配
{
#pragma asm
  MOV A,#0CCH
  MOV R2,#8
  CLR C
  WR1:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR1
  SETB P3_3
#pragma endasm
}

void TMVDS1820(void)  //温度转换指令
{
#pragma asm
  MOV A,#44H
  MOV R2,#8
  CLR C
  WR2:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR2
  SETB P3_3
#pragma endasm
}

void TMRDS1820(void)  //读出温度指令
{
#pragma asm
  MOV A,#0BEH
  MOV R2,#8
  CLR C
  WR3:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR3
  SETB P3_3
#pragma endasm
}

void TMWDS1820(void)  //写入温度限制指令
{
#pragma asm
  MOV A,#04EH
  MOV R2,#8
  CLR C
  WR13:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR13
  SETB P3_3
#pragma endasm
}

void TMREDS1820(void)		//COPY RAM to E2PRAM
{
#pragma asm
  MOV A,#48H
  MOV R2,#8
  CLR C
  WR33:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR33
  SETB P3_3
#pragma endasm
}

void TMERDS1820(void)		//COPY E2PRAM to RAM
{
#pragma asm
  MOV A,#0B8H
  MOV R2,#8
  CLR C
  WR43:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR43
  SETB P3_3
#pragma endasm
}

void WriteDS1820(void)	//写入温度限制值
{
#pragma asm
	MOV A,26H					//发出4EH写ROM指令后连发两个字节分别为上下限
  MOV R2,#8
  CLR C
  WR23:
  CLR P3_3
  MOV R3,#6
  DJNZ R3,$
  RRC A
  MOV P3_3,C
  MOV R3,#23
  DJNZ R3,$
  SETB P3_3
  NOP
  DJNZ R2,WR23
  SETB P3_3       
#pragma endasm
}

void ReadDS1820(void) //读出温度值
{
#pragma asm
  MOV R4,#3 ; 将温度高位和低位，高温限制位从DS18B20中读出
  MOV R1,#29H ; 低位存入29H(TEMPER_L),高位存入28H(TEMPER_H)，高温限制位存入27H(TMRomV)
  RE00:
  MOV R2,#8
  RE01:
  CLR C
  SETB P3_3
  NOP
  NOP
  CLR P3_3
  NOP
  NOP
  NOP
  SETB P3_3
  MOV R3,#09
  RE10: 
  DJNZ R3,RE10
  MOV C,P3_3
  MOV R3,#23
  RE20: 
  DJNZ R3,RE20
  RRC A
  DJNZ R2,RE01
  MOV @R1,A
  DEC R1
  DJNZ R4,RE00
#pragma endasm
}

void Delay_510(void)	//延时510微秒
{
#pragma asm
	MOV R0,#7DH
	MOV R1,#02H
	TSR1:
	DJNZ R0,TSR1
	MOV R0,#7DH
	DJNZ R1,TSR1
#pragma endasm
}

void Delay_110(void)	//延时110微秒
{
#pragma asm
	MOV R0,#19H
	MOV R1,#02H
	TSR2:
	DJNZ R0,TSR2
	MOV R0,#19H
	DJNZ R1,TSR2
#pragma endasm
}

void Delay_10ms(void)	//延时10ms
{
#pragma asm
	MOV R0,#19H
	MOV R1,#0C8H
	TSR3:
	DJNZ R0,TSR3
	MOV R0,#19H
	DJNZ R1,TSR3
#pragma endasm
}

void Delay_4s(void)	//延时4s
{
#pragma asm
  MOV R2,#28H
  TSR5:
	MOV R0,#0FAH
	MOV R1,#0C8H
	TSR4:
	DJNZ R0,TSR4
	MOV R0,#0FAH
	DJNZ R1,TSR4
  DJNZ R2,TSR5
#pragma endasm
}
