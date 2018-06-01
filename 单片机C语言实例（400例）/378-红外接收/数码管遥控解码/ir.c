#include<reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long


#define		Nop()		{_nop_(); _nop_(); _nop_(); _nop_(); _nop_();}



volatile ulong IRcode=0x00000000;  //32位的键代码
volatile ulong Irdcode=0x00000000;
volatile uint customcode=0x0000;      //16位用户码
volatile uint time_us=0x0000;      //两个下降沿之间的时间

volatile uchar timeH,timeL;  //保存TH1 TL的值

uchar Lcustomcode;      //低8用户码
uchar Hcustomcode;     //高8
uchar datacode;    //8位键数据码
uchar mycode;
uchar Rdatacode;      //8位键数据反码
uchar uc1ms;
uchar uc10ms;
uchar uc3ms;


uchar  ucDispTime;
uchar  ucDispOrder;
uchar  ucDispCon;
uchar ucSpeakerTime;
unsigned char code LedData[16] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code LedCon[2] = {0x8f,0x4f};
unsigned char ucDispData[2];	// 存放显示数据
sbit led1 = P2^7;
sbit led2 = P2^6;
sbit power=P1^4;  //电源开关
sbit BEEP= P3^6;
bit NewIRcode=0;  //指示当处理完了32位码后，就有了新的遥控码
bit DataRight=0;   //为1时读取数据正确
bit bSampleOk;
bit bSampling;
bit b10msInt;
bit  b1msInt;
bit bKeySound;
bit b1msMain;
bit IR_E;           //表示有新的遥控键控下就更新扫描数据
bit b3msint;

void SendDataToDispDevice();
void Ir_process();
void display();
void beeping();

/****** 晶振为12 MHZ*******/
/**定时器1,12 MHZ最大定时为65.53ms***/
//------------------------------------------------------------------------------
void init()
{ 
  IP=0x09;   //定时器1，外部中断0优先级高
  TMOD=0x11; //定时器0，工作方式1 ;  定时器1，工作方式1
  TCON=0x01; //外中断0下降沿触发，（包括TR1=0，TR0=0）
  TH0=0xff;  //初始化定时器0,定时0.2ms 
  TL0=0x47;
  TH1=0x00;  //初始化定时器1
  TL1=0x00;

  EA=1;    //开全中断
  ET0=1;   //开放T0中断
  ET1=1;      //开放T1中断
  EX0=1;   //开放INT0
  TR1=0;
  TR0=1;

}
//--------------------------------------
void TimeProg(void)
{       
	b1msMain = b1msInt;
   	b1msMain=0;	
	b10msInt = 0;


	if(b1msInt == 1)
	{  
         b1msMain=1;
	   if(++uc10ms == 10)	
		{
			uc10ms = 0;
			b10msInt = 1;
			if(bKeySound==1)
		  {	beeping();
			bKeySound=0;
			}
		}	
	
	}
}//void TimeProg(void)
//-------------------------------------interrupt0-------------------------------------
void IR_ISR() interrupt 0 using 1 //遥控器中断处理函数
{
  static uchar cn;

  TR1=0;
  timeH=TH1;
  timeL=TL1;
  TH1=0;
  TL1=0;
  TR1=1;                     //开定时器中断1

  time_us=(unsigned int)timeH;
  time_us=time_us<<8;
  time_us=time_us|timeL;
 
  if(time_us>12200&&time_us<13000) {cn=1;IRcode=0;}  //遇到引导码,就把cn清0，IRcode清0
        //引导码的时间长度为9ms+4.5ms
  if(cn<34)
      {
          if(time_us>950&&time_us<1120)                   //0
           {
             IRcode=IRcode|0x00000000;
             if(cn<33) IRcode=IRcode>>1;
           }

      else if(time_us>1920&&time_us<2120)        //1t > 1950 && t < 2150
          { 
             IRcode=IRcode|0x80000000;
             if(cn<33) IRcode=IRcode>>1;
           }
  

  //else if(time_us>10000&&time_us<11000) {Irdcode=IRcode;cn=34; }   //遇到重复码
                                               //cn用于记录接收到的数据位
    }
	cn++;
  if(cn==34) 
   { NewIRcode=1;
     TR1=0;  
	
     Irdcode= IRcode;  cn=0;  
	}  //读完32位码，则有新码产生

}

//--------------------------------------timer_ISR------------------------------
void Timer0_ISR() interrupt 1 using 2  //定时器0中断函数
{
  TR0=0;
  TH0=0xff;  //初始化定时器0,定时0.2ms  晶振为11.0592 MHZ
  TL0=0x47;
  TR0=1;
  if(++uc1ms == 5)
	{
		uc1ms = 0;
        b1msInt=1;
		 if(++uc3ms==8)
		  {
		   uc3ms=0;
		   b3msint=1;
		   SendDataToDispDevice();     //n* ms送一次显示
		   }
				
		
	}
}//void Timer0IntProg() interrupt 1 using 1

void Timer1_ISR() interrupt 3 using 3  //定时器1中断函数
{ 
  TR1=0;
  TH1=0x00;  //初始化定时器1
  TL1=0x00;
  TR1=1;

}
//--------------------SendDataToDispDevice----------
void SendDataToDispDevice()
{
 unsigned char n;
    //watchdog();
	if(++ucDispOrder >= 2)  ucDispOrder = 0; // 下一显示巡回
	
	// 下面为发送控制数据	  位控
    
         if(ucDispOrder==0) 
	      {led1=0;
           led2=1;
	       Nop();
            Nop();
		  
		   }
	 if(b3msint==1)
       {if(ucDispOrder==1) 
	       {led2=0;
            led1=1;
		    Nop();
            Nop();
		    }
		}
	// 下面为发送显示数据
      n = LedData[ucDispData[ucDispOrder]];
      P0=n;

  
	
} //void SendDataToDispDevice()

//------------------------------------main()----------------------------------------------
void main()
{

    init();
  beeping();
 while(1)
  {
     TimeProg();
     Ir_process();
     display(); 
    
  }

}

void Ir_process()

 {
 
  if(NewIRcode==1)    //如果有新的遥控码就读
  {
    NewIRcode=0; //读完之后清零，表示新码已读
    customcode=(Irdcode>>16);    //取红外码中的按码键
                    //取低8位用户码
      Lcustomcode=customcode>>8;//取低8位按码键
    datacode=(unsigned char)(customcode&0x00ff);  //取高8位按码键
     
   
       Rdatacode=Lcustomcode;   //取低8位按码键的反码

      if(~Rdatacode!=datacode) 
	  {  DataRight=0;
	     Irdcode=0;
	     datacode=Rdatacode=0;
	  } //校验用户码，反码

     else 
	 { DataRight=1;
	   IR_E=1;
	   mycode=datacode;}
     if(DataRight==1)  { bKeySound = 1;DataRight=0;  }
     
     }

   
 
}

void display()
  {
  /*  unsigned char a[2];
	a[0] = mycode & 0x0f;
	mycode = mycode >> 4;
	a[1] = mycode & 0x0f;
	ET0 = 0;
	ucDispData[0] = a[0];
	ucDispData[1] = a[1];
	ET0 = 1;*/
	if(IR_E==1)
	{
	ET0 = 0;
	ucDispData[0] = mycode & 0x0f;
	mycode = mycode >> 4;
	ucDispData[1] = mycode & 0x0f;
    IR_E=0;
	ET0 = 1;
	}
}

/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char a;
  while(x--)
 {
  for (a = 0; a<13; a++) {;}
 }
}


/**********************************************************/
void beeping()
{
  unsigned char i;
  
  for (i=0;i<100;i++)
   {
     delay(4);
     BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;
                    //关闭蜂鸣器
}

