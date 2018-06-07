#include <AT89x51.h>
#include <string.h>
#include<intrins.h>

#define LCM_RS P2_0 //定义引脚
#define LCM_RW P2_1
#define LCM_E P2_2
#define LCM_Data P0
#define Busy 0x80 //用于检测LCM状态字中的Busy标识
#define uchar unsigned char

uchar id,timecount,dipsmodid;
bit lmcinit_or_not; //是否需要清屏标志位“1”为需要“0”为不需要
bit flag,sflag; //flag是时钟冒号闪烁标志，sflag是温度负号显示标志
void Disp_line1(void); //显示屏幕第一行 
void Disp_line2(void); //显示屏幕第二行
void id_case1_key();
void Disp_mod0(void);//显示模式0
void Disp_mod1(void);//显示模式


//*********** DS1302 时间显示定义部分
sbit T_CLK=P1^0;
sbit T_IO =P1^1;
sbit T_RST=P1^2;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;
void Set(uchar,uchar);     //根据选择调整相应项目
void   RTInputByte(uchar);       /* 输入 1Byte */
uchar  RTOutputByte(void);       /* 输出 1Byte */
void   W1302(uchar, uchar);   // 向DS1302写入一个字节
uchar  R1302(uchar);          // 从DS1302读出一个字节
void   Set1302(unsigned char * ); //  设置时间 
bit sec,min,hour,year,mon,day,weekk; //闪烁标志位
//初始化后设置为：04年12月2日星期4 0点0分0秒
unsigned char inittime[7]={0x00,0x00,0x00,0x02,0x12,0x04,0x04};
//                          秒  分钟 小时  日   月   年  星期

//***** 18B20温度显示定义部
sbit DQ=P3^3; //18B20 接P07口
typedef unsigned char byte;
typedef unsigned int  word;
void Read_Temperature(char,char);
void mychar(void);
byte ow_reset(void);
byte read_byte(void);
void write_byte(char val);
void adjust_res(char res); //res 分别等于 0x1f, 0x3f, 0x5f 温度读数分辨率分别对应
                           //              0.5, 0.25, 0.125   

//*******温度控制部分*********
bit tl_flash_flag;       //下限闪烁标志
bit th_flash_flag;       //上限闪烁标志
bit tl_flag;               //下限越限标志  
bit th_flag;               //上限越限标志  
bit tl_sign,th_sign;		//温度零下标志 0：零上，1：零下
char tempid;
void Disp_Temp(void);    //显示上下限温度
void Set_TH_Temp(void);  //设置温度上限
void Set_TL_Temp(void);  //设置温度下限
signed char tl=20,th=45;
void Compare_Temp(signed char l,signed char h);     //比较温度
void Tl_Al(void);         //下限越限报警
void Th_Al(void);         //上限越限报警



//******* 1602LCD驱动 **********************************************************
void WriteDataLCM(unsigned char WDLCM);
void WriteCommandLCM(unsigned char WCLCM,BuysC);
unsigned char ReadStatusLCM(void);
void LCMInit(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);
unsigned char code week[]={"Week."};


void main(void)
{ 
  Delay400Ms(); //启动等待，等LCM讲入工作状态
  LCMInit(); //LCM初始化
  Delay5Ms(); //延时片刻(可不要)
  mychar();
  TMOD=0x01;
  TH0=(65535-50000)/256;
  TL0=(65535-50000)%256;
  EA=1;
  TR0=1;
  ET0=1;  
  W1302(0x90,0xa5);//打开充电二级管  一个二级管串联一个2K电阻 
  W1302(0x8e,0x80);//写保护，禁止写操作 
  adjust_res(0x1f); //调整18B20的分辨率 0x1f:0.5; 0x3f:0.25; 0x5f:0.125
  while(1)
    {              
	 if (P1_7==0)
      {dipsmodid++;
	   lmcinit_or_not=1;
       if(dipsmodid>2) dipsmodid=0;
       while(P1_7==0);
      }
     switch(dipsmodid)
      {
       case 0:
        Disp_mod0();
        break;
       case 1:
        Disp_mod1();
        break;
  	  }
    }
}
/*//选择显示模?
void Dispmod_id_case_key()
{
 if (P0_3==0)
  {dipsmodid++;
   if(dipsmodid>1) dipsmodid=0;
  }
}*/
//显示模式0
void Disp_mod0(void)
{
 if(lmcinit_or_not==1)
  {LCMInit();
   lmcinit_or_not=0;
  }
 if ((P1_4|P1_5)==0) //初始化
  {
   Delay5Ms();
   if ((P1_4|P1_5)==0) Set1302(inittime);
  }
 //mychar(); //显示自定义字符
 if (P1_6==0) // 设置和选择项目键
      {
       Delay5Ms();
       if(P1_6==0){id++;if(id>7) id=0;}
       while(P1_6==0);    
      }
     switch(id)
      {
       case 0:
         sec=0;
         Disp_line1(); Disp_line2();
         break;
       case 1://年
         year=1;
         Disp_line1(); Disp_line2();                  
         id_case1_key();
         break;
       case 2://月
         year=0;mon=1;
         Disp_line1(); Disp_line2();                 
         id_case1_key();
         break;
       case 3://日
         mon=0;day=1;
         Disp_line1(); Disp_line2();
         id_case1_key();
         break;             
       case 4://星期
         day=0;weekk=1;
         Disp_line1(); Disp_line2();         
         id_case1_key();
         break;
       case 5://小时
         weekk=0;hour=1;
         Disp_line1(); Disp_line2(); 
         id_case1_key();      
         break;       
       case 6://分钟
         hour=0;min=1;
         Disp_line1(); Disp_line2(); 
         id_case1_key();
         break;
       case 7://秒
         min=0;sec=1;
         Disp_line1(); Disp_line2();             
         id_case1_key();
         break;              
      }
}
//显示模式1
void Disp_mod1(void)
{
 if(lmcinit_or_not==1)
  {LCMInit();
   lmcinit_or_not=0;
  }
 Read_Temperature(1,0);
 DisplayOneChar(0,1,'T');
 DisplayOneChar(1,1,'L');
 DisplayOneChar(2,1,':');
 DisplayOneChar(7,1,'T');
 DisplayOneChar(8,1,'H');
 DisplayOneChar(9,1,':');
 if(P1_6==0)
  { 
    if(P1_6==0)
     {
      tempid++;
      if(tempid>2) tempid=0;
      while(P1_6==0);
     }
  }
 switch(tempid)
  {
   case 0:
    tl_flash_flag=0;
    th_flash_flag=0;
    Disp_Temp();
   break;
   case 1:
    tl_flash_flag=1;
    th_flash_flag=0;
    Disp_Temp();
	Set_TL_Temp(); 
   break;
   case 2:
    tl_flash_flag=0;
    th_flash_flag=1;
    Disp_Temp();
    Set_TH_Temp();
   break;
  }
 Compare_Temp(tl,th);
 Tl_Al();
 Th_Al();
}

//显示上下限温度
void Disp_Temp(void)
{
 
 unsigned char tl_gw,tl_sw,tl_bw;		//下限温度的个位，十位，百位
 unsigned char th_gw,th_sw,th_bw;		//上限温度的个位，十位，百位

 tl_sign=tl>>7;
 if(tl_sign)
  {
   tl_bw=(~(tl-1))/100;		//计算百位
   tl_sw=((~(tl-1))-tl_bw*100)/10;		//计算十位
   tl_gw=(~(tl-1))-tl_bw*100-tl_sw*10;		//计算个位
  }
 else
  {
   tl_bw=(tl)/100;          
   tl_sw=(tl-tl_bw*100)/10;
   tl_gw=tl-tl_bw*100-tl_sw*10;
  }

 if(tl_flash_flag==1)
  {
   if(flag==1)
    {if(tl_sign)
	  {
	   DisplayOneChar(3,1,0x2d);
       DisplayOneChar(4,1,tl_sw+0x30); 
       DisplayOneChar(5,1,tl_gw+0x30);
	  }
	 else
	  {
	   DisplayOneChar(3,1,tl_bw+0x30);
       DisplayOneChar(4,1,tl_sw+0x30); //显示下限温度
       DisplayOneChar(5,1,tl_gw+0x30);
      } 
    }
   else
    {DisplayOneChar(3,1,0x20);
     DisplayOneChar(4,1,0x20); //显示下限温度
     DisplayOneChar(5,1,0x20);    
    }
  }
 else
  {
   DisplayOneChar(3,1,tl_bw+0x30);
   DisplayOneChar(4,1,tl_sw+0x30); //显示下限温度
   DisplayOneChar(5,1,tl_gw+0x30);
  }

  
  th_sign=th>>7;
  if(th_sign)
   {
    th_bw=(~(th-1))/100;		//计算百位
    th_sw=((~(th-1))-th_bw*100)/10;		//计算十位
    th_gw=(~(th-1))-th_bw*100-th_sw*10;		//计算个位
   }
  else
   {
    th_bw=(th)/100;          
    th_sw=(th-th_bw*100)/10;
    th_gw=th-th_bw*100-th_sw*10;
   }


 if(th_flash_flag==1)
  {
   if(flag==1)
    {if(th_sign)
	  {
       DisplayOneChar(10,1,0x2d);
       DisplayOneChar(11,1,th_sw+0x30); //显示下限温度
       DisplayOneChar(12,1,th_gw+0x30);  
	  }
	 else
	  {
       DisplayOneChar(10,1,th_bw+0x30);
       DisplayOneChar(11,1,th_sw+0x30); //显示下限温度
       DisplayOneChar(12,1,th_gw+0x30);  
      }
    }
   else
    {
	 DisplayOneChar(10,1,0x20);
     DisplayOneChar(11,1,0x20); //显示下限温度
     DisplayOneChar(12,1,0x20);    
    }
  }
 else
  {
   DisplayOneChar(10,1,th_bw+0x30);
   DisplayOneChar(11,1,th_sw+0x30); //显示下限温度
   DisplayOneChar(12,1,th_gw+0x30);      
  }
}

//设置温度下限
void Set_TL_Temp(void)
{
 if(P1_5==0)
  {
   Delay5Ms();
   if(P1_5==0)
    {
	 tl++;
	 if(tl>125) tl=-55;
	 //if(tl<-55) tl=155;
	}
   while(P1_5==0);
  }
 if(P1_4==0)
  {
   Delay5Ms();
   if(P1_4==0)
    {
	 tl--;
	 if(tl<-55) tl=125;
	}
  }
}

//设置温度上限
void Set_TH_Temp(void)
{
 if(P1_5==0)
  {
   Delay5Ms();
   if(P1_5==0)
    {
	 th++;
	 if(th>125) th=-55;
	 //if(tl<-55) tl=155;
	}
   while(P1_5==0);
  }
 if(P1_4==0)
  {
   Delay5Ms();
   if(P1_4==0)
    {
	 th--;
	 if(th<-55) th=125;
	}
  }
}

void Compare_Temp(signed char l,signed char h)     //比较温度
{
  unsigned char tl_temp;
  unsigned char y,s;
  //unsigned int  x;
  //unsigned char ct[8];
  union{byte c[2]; int x;}temp;
  
  ow_reset();
  write_byte(0xCC); // Skip ROM
  write_byte(0x44); // 转换温度
  ow_reset();
  write_byte(0xCC); //Skip ROM
  write_byte(0xbe); // 读取寄存器
  temp.c[1]=read_byte();//读出温度低8位
  temp.c[0]=read_byte();//读出温度高8位
 
  // 零下温度判断
  sflag=0; //温度零下标志 0：零上，1：零下
  if((temp.c[0]&0xf8)!=0x00)
   {
    sflag=1; //零下标志位置1
    temp.c[1]=~temp.c[1]; //低8位取反
    temp.c[0]=~temp.c[0]; //高8位取反
    tl_temp=temp.c[1]+1;  //低8位加1 
    temp.c[1]=tl_temp;    //计算后重新存入数组
    if(tl_temp>255) temp.c[0]++;  //如果低8位大于255，向高8位进1
   }
  y=0;
  s=0;
  y=temp.c[0]<<4;  
  s=temp.c[1]>>4;
  y=s|y;
  //x=((temp.c[0]&0x07)*256+temp.c[1])*.625;
  if(y<l)
   
    tl_flag=1;
   
  else
   
    tl_flag=0;
   
  if(y>h)
   
    th_flag=1;
   
  else
   
    th_flag=0;
   
}

void Tl_Al(void)         //下限越限报?
{
 if(tl_flag==1)
  {
   if(flag==1)
    {    
     DisplayOneChar(9,0,'A'); 
     DisplayOneChar(10,0,'L');        
    }
   else
    {
     DisplayOneChar(9,0,0x20); 
     DisplayOneChar(10,0,0x20);    
    }
  } 
 else
  {
   DisplayOneChar(9,0,0x20); 
   DisplayOneChar(10,0,0x20);         
  }
}

void Th_Al(void)         //上限越限报警
{
 if(th_flag==1)
  {
   if(flag==1)
    {    
     DisplayOneChar(13,0,'A'); 
     DisplayOneChar(14,0,'H');        
    }
   else
    {
     DisplayOneChar(13,0,0x20); 
     DisplayOneChar(14,0,0x20);    
    }
  } 
 else
  {
   DisplayOneChar(13,0,0x20); 
   DisplayOneChar(14,0,0x20);         
  }
}



//中断入口，冒号闪烁
void t0(void) interrupt 1 using 0
{
  TH0=(65535-50000)/256; //50ms定时
  TL0=(65535-50000)%256;  
  timecount++;
  if(timecount>9)
   {
    timecount=0;    
    flag=~flag;             
   }  
}
//id为1时的下一级选择
void id_case1_key()
{
  if (P1_5==0) //增加
      {
       Delay5Ms();
       if(P1_5==0) Set(id,0);  
       if(id!=7)  while(P1_5==0);    
      }
   if (P1_4==0)  //减少
      {
       Delay5Ms();
       if(P1_4==0) Set(id,1);          
       if(id!=7) while(P1_4==0);    
      }
}

//根据选择调整相应项目并写入DS1302
void Set(uchar sel,uchar sel_1) 
{
  signed char address,item;
  signed char max,mini;
  if(sel==7)  {address=0x80; max=0;mini=0;}     //秒
  if(sel==6)  {address=0x82; max=59;mini=0;}    //分钟
  if(sel==5)  {address=0x84; max=23;mini=0;}    //小时
  if(sel==3)  {address=0x86; max=31;mini=1;}    //日
  if(sel==2)  {address=0x88; max=12;mini=1;}    //月
  if(sel==1)  {address=0x8c; max=99;mini=0;}    //年
  if(sel==4)  {address=0x8a; max=7; mini=1;}    //星期
  
  item=R1302(address+1)/16*10+R1302(address+1)%16;
  if (sel_1==0) item++;  else item--;
  if(item>max) item=mini;   
  if(item<mini) item=max;
           
  W1302(0x8e,0x00);//允许写操作
  W1302(address,item/10*16+item%10);
  W1302(0x8e,0x80);//写保护，禁止写操作  
}

//屏幕显示第一行 时间和温度
void Disp_line1(void) 
{  
  Read_Temperature(10,0); //温度显示
  //冒号闪烁     
  if(flag==0)
    {DisplayOneChar(3,0,0x3a); DisplayOneChar(6,0,0x3a);}
  else
    {DisplayOneChar(3,0,0x20); DisplayOneChar(6,0,0x20);}

  if(sec==1)  //秒闪烁标志位
    {
     if(flag==1)
       {    
        DisplayOneChar(7,0,R1302(0x81)/16+0x30); //显示秒
        DisplayOneChar(8,0,R1302(0x81)%16+0x30);        
       }
     else
       {
        DisplayOneChar(7,0,0x20); //显示秒
        DisplayOneChar(8,0,0x20);    
       }
    } 
  else
    {
     DisplayOneChar(7,0,R1302(0x81)/16+0x30); //显示秒
     DisplayOneChar(8,0,R1302(0x81)%16+0x30);         
    }    
   
  if(min==1)  //分钟闪烁标志位
    {
     if(flag==1)
       {    
        DisplayOneChar(4,0,R1302(0x83)/16+0x30); //显示分钟
        DisplayOneChar(5,0,R1302(0x83)%16+0x30);        
       }
     else
       {
        DisplayOneChar(4,0,0x20); //显示分钟
        DisplayOneChar(5,0,0x20);    
       }
    } 
  else
    {
     DisplayOneChar(4,0,R1302(0x83)/16+0x30); //显示分钟
     DisplayOneChar(5,0,R1302(0x83)%16+0x30);         
    } 

  if(hour==1)  //小时闪烁标志位
    {
     if(flag==1)
       {    
        DisplayOneChar(1,0,R1302(0x85)/16+0x30);//显示小时
        DisplayOneChar(2,0,R1302(0x85)%16+0x30); 
       }
     else
       {
        DisplayOneChar(1,0,0x20); //显示小时
        DisplayOneChar(2,0,0x20);    
       }
    } 
  else
   { 
    DisplayOneChar(1,0,R1302(0x85)/16+0x30);//显示小时
    DisplayOneChar(2,0,R1302(0x85)%16+0x30);
   }
}

// 屏幕显示第二行  日期和星期   
void Disp_line2(void)
{
  DisplayOneChar(3,1,0x2f); //显示固定字符
  DisplayOneChar(6,1,0x2f);
  DisplayListChar(10,1,week);
    
  if(year==1)  //年闪烁标志位
    {
     if(flag==1)
       {    
        DisplayOneChar(1,1,R1302(0x8d)/16+0x30);//显示年
        DisplayOneChar(2,1,R1302(0x8d)%16+0x30);
       }
     else
       {
        DisplayOneChar(1,1,0x20); //显示年
        DisplayOneChar(2,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(1,1,R1302(0x8d)/16+0x30);//显示年
      DisplayOneChar(2,1,R1302(0x8d)%16+0x30);
    }
     
  if(mon==1)  //月闪烁标志位
    {
     if(flag==1)
       {    
        DisplayOneChar(4,1,R1302(0x89)/16+0x30);//显示月 
        DisplayOneChar(5,1,R1302(0x89)%16+0x30);
       }
     else
       {
        DisplayOneChar(4,1,0x20); //显示月
        DisplayOneChar(5,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(4,1,R1302(0x89)/16+0x30);//显示月 
      DisplayOneChar(5,1,R1302(0x89)%16+0x30);
    }   

  if(day==1)  //日闪烁标志位
    {
     if(flag==1)
       {    
         DisplayOneChar(7,1,R1302(0x87)/16+0x30);//显示日
         DisplayOneChar(8,1,R1302(0x87)%16+0x30);
       }
     else
       {
        DisplayOneChar(7,1,0x20); //显示日
        DisplayOneChar(8,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(7,1,R1302(0x87)/16+0x30);//显示日
      DisplayOneChar(8,1,R1302(0x87)%16+0x30);
    }
    
   if(weekk==1)  //星期闪烁标志位
    {
     if(flag==1)
       {    
         DisplayOneChar(15,1,R1302(0x8b)%16+0x30);//显示星期
       }
     else
       {
        DisplayOneChar(15,1,0x20); //显示星期          
       }
    } 
  else
    { 
      DisplayOneChar(15,1,R1302(0x8b)%16+0x30);//显示星期
    }     
}

//********* LCM1602驱动程序 ***************
//写数据
void WriteDataLCM(unsigned char WDLCM)
{
  ReadStatusLCM(); //检测忙
  LCM_Data = WDLCM;
  LCM_RS = 1;
  LCM_RW = 0;
  LCM_E = 0; //若晶振速度太高可以在这后加小的延时
  LCM_E = 0; //延时
  LCM_E = 1;
}
//写指令
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysC为0时忽略忙检测
{
  if (BuysC) ReadStatusLCM(); //根据需要检测忙
  LCM_Data = WCLCM;
  LCM_RS = 0;
  LCM_RW = 0; 
  LCM_E = 0;
  LCM_E = 0;
  LCM_E = 1; 
}
//读状态
unsigned char ReadStatusLCM(void)
{
  LCM_Data = 0xFF; 
  LCM_RS = 0;
  LCM_RW = 1;
  LCM_E = 0;
  LCM_E = 0;
  LCM_E = 1;
  while (LCM_Data & Busy); //检测忙信号
  return(LCM_Data);
}
//LCM初始化
void LCMInit(void) 
{
  LCM_Data = 0;
  WriteCommandLCM(0x38,0); //三次显示模式设置，不检测忙信号
  Delay5Ms(); 
  WriteCommandLCM(0x38,0);
  Delay5Ms(); 
  WriteCommandLCM(0x38,0);
  Delay5Ms(); 
  WriteCommandLCM(0x38,1); //显示模式设置,开始要求每次检测忙信号
  WriteCommandLCM(0x08,1); //关闭显示
  WriteCommandLCM(0x01,1); //显示清屏
  WriteCommandLCM(0x06,1); // 显示光标移动设置
  WriteCommandLCM(0x0C,1); // 显示开及光标设置
}
//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
  Y &= 0x1;
  X &= 0xF; //限制X不能大于15，Y不能大于1
  if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
  X |= 0x80; //算出指令码
  WriteCommandLCM(X, 0); //这里不检测忙信号，发送地址码
  WriteDataLCM(DData);
}
//按指定位置显示一串字符  ***原来的遇到空格0x20就不显示***
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
  unsigned char ListLength,j;
  ListLength = strlen(DData);
  Y &= 0x1;
  X &= 0xF; //限制X不能大于15，Y不能大于1
      if (X <= 0xF) //X坐标应小于0xF
        { 
		    for(j=0;j<ListLength;j++)
		      {
               DisplayOneChar(X, Y, DData[j]); //显示单个字符
               X++;
              }
        }
}
//5ms延时
void Delay5Ms(void)
{
  unsigned int TempCyc = 5552;
  while(TempCyc--);
}
//400ms延时
void Delay400Ms(void)
{
  unsigned char TempCycA = 5;
  unsigned int TempCycB;
  while(TempCycA--)
   {
    TempCycB=7269;
    while(TempCycB--);
   };
}

//********DS1302读写程序***************
/******************************************************************** 
 函 数 名：RTInputByte()
 功    能：实时时钟写入一字节
 说    明：往DS1302写入1Byte数据 (内部函数)
 入口参数：d 写入的数据 
 返 回 值：无  
 ***********************************************************************/
void RTInputByte(uchar d) 
{ 
    uchar i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        T_IO = ACC0;           /*相当于汇编中的 RRC */
        T_CLK = 1;
        T_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
/******************************************************************** 
 函 数 名：RTOutputByte()
 功    能：实时时钟读取一字节
 说    明：从DS1302读取1Byte数据 (内部函数)
 入口参数：无  
 返 回 值：ACC
 设    计：zhaojunjie           日    期：2002-03-19
 修    改：                     日    期： 
***********************************************************************/
uchar RTOutputByte(void) 
{ 
    uchar i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         /*相当于汇编中的 RRC */
        ACC7 = T_IO;
        T_CLK = 1;
        T_CLK = 0;
    } 
    return(ACC); 
}
/******************************************************************** 
 函 数 名：W1302()
 功    能：往DS1302写入数据
 说    明：先写地址，后写命令/数据 (内部函数)
 调    用：RTInputByte() , RTOutputByte()
 入口参数：ucAddr: DS1302地址, ucData: 要写的数据
 返 回 值：无
 ***********************************************************************/
void W1302(uchar ucAddr, uchar ucDa)
{
    T_RST = 0;
    T_CLK = 0;
    T_RST = 1;
    RTInputByte(ucAddr);       /* 地址，命令 */
    RTInputByte(ucDa);       /* 写1Byte数据*/
    T_CLK = 1;
    T_RST = 0;
}
/******************************************************************** 
 函 数 名：R1302()
 功    能：读取DS1302某地址的数据
 说    明：先写地址，后读命令/数据 (内部函数)
 调    用：RTInputByte() , RTOutputByte()
 入口参数：ucAddr: DS1302地址
 返 回 值：ucData :读取的数据
***********************************************************************/
uchar R1302(uchar ucAddr)
{
    uchar ucData;
    T_RST = 0;
    T_CLK = 0;
    T_RST = 1;
    RTInputByte(ucAddr);             /* 地址，命令 */
    ucData = RTOutputByte();         /* 读1Byte数据 */
    T_CLK = 1;
    T_RST = 0;
    return(ucData);
}

/******************************************************************** 
 函 数 名：Set1302()
 功    能：设置初始时间
 说    明：先写地址，后读命令/数据(寄存器多字节方式)
 调    用：W1302()
 入口参数：pClock: 设置时钟数据地址 格式为: 秒 分 时 日 月 星期 年
                               7Byte (BCD码)1B 1B 1B 1B 1B  1B  1B
 返 回 值：无
***********************************************************************/
void Set1302(uchar *pClock) 
{
    uchar i;
    uchar ucAddr = 0x80; 
    W1302(0x8e,0x00);           /* 控制命令,WP=0,写操作?*/
    for(i =7; i>0; i--)
    { 
        W1302(ucAddr,*pClock);  /* 秒 分 时 日 月 星期 年 */ 
        pClock++;
        ucAddr +=2;
    }
    W1302(0x8e,0x80);           /* 控制命令,WP=1,写保护?*/
}

//*********** 18B20驱动 **************************
//延时
void delay(unsigned int num)
{
  while( --num ) ;
}

//复位
byte ow_reset(void)
{
  byte presence;
  DQ = 0;        //拉低总线
  delay(29);    // 保持 480us
  DQ = 1;       // 释放总线
  delay(3);     // 等待回复
  presence = DQ; // 读取信号
  delay(25);    // 等待结束信号
  return(presence); // 返回   0：正常 1：不存在
}     

//从 1-wire 总线上读取一个字节
byte read_byte(void)
{
  byte i;
  byte value = 0;
  for (i=8;i>0;i--)
  {
    value>>=1;
    DQ = 0; 
    DQ = 1;
    delay(1);  
    if(DQ)value|=0x80;
    delay(6); 
  }
  return(value);
}

//向 1-WIRE 总线上写一个字节
void write_byte(char val)
{
  byte i;
  for (i=8; i>0; i--) // 一次写一位
  {
    DQ = 0; // 
    DQ = val&0x01;
    delay(5); // 
    DQ = 1;
    val=val/2;
  }
  delay(5);
}

//读取和显示温度
void  Read_Temperature(char xx,char yy)
{
  unsigned char i,tl_temp;
  unsigned int  x;
  unsigned char ct[8];
  union{byte c[2]; int x;}temp;
  
  ow_reset();
  write_byte(0xCC); // Skip ROM
  write_byte(0x44); // 转换温度
  ow_reset();
  write_byte(0xCC); //Skip ROM
  write_byte(0xbe); // 读取寄存器
  temp.c[1]=read_byte();//读出温度低8位
  temp.c[0]=read_byte();//读出温度高8位
 
  // 零下温度判断
  sflag=0; //温度零下标志 0：零上，1：零下
  if((temp.c[0]&0xf8)!=0x00)
   {
    sflag=1; //零下标志位置1
    temp.c[1]=~temp.c[1]; //低8位取反
    temp.c[0]=~temp.c[0]; //高8位取反
    tl_temp=temp.c[1]+1;  //低8位加1 
    temp.c[1]=tl_temp;    //计算后重新存入数组
    if(tl_temp>255) temp.c[0]++;  //如果低8位大于255，向高8位进1
  }  

  x=((temp.c[0]&0x07)*256+temp.c[1])*.625;
  for(i=0;i<8;i++)
       {
         ct[i]=0;
       }
  i=0;
  while(x/10)
   {
    ct[i]=x%10;
    x=x/10;
    i++;
   }
  ct[i]=x;

  if(sflag==1) DisplayOneChar(xx-1,yy,0x2d);
  else         DisplayOneChar(xx-1,yy,0x20);
  DisplayOneChar(xx,  yy,ct[2]+0x30);//显示温度十位数
  DisplayOneChar(xx+1,yy,ct[1]+0x30);//显示温度个位数
  DisplayOneChar(xx+2,yy,0x2e);//显示小数点
  DisplayOneChar(xx+3,yy,ct[0]+0x30);//显示温度小数位
  DisplayOneChar(xx+4,yy,0x01);//显示自定义字符
  DisplayOneChar(xx+5,yy,0x43);//显示字符“C
}

//温度分辨率调整
void adjust_res(char res) ///res 分别等于 0x1f, 0x3f, 0x5f 温度读数分辨率分别对应
                           //              0.5, 0.25, 0.125   
{
  ow_reset();        //复位
  write_byte(0xcc);  //跳过Rom
  write_byte(0x4e);  //写暂存器
  write_byte(0x02);  //写TH 
  write_byte(0x01);  //写TL
  //write_byte(0x5f);  //写结构寄存器
  write_byte(res);
  ow_reset();       //复位
  write_byte(0xcc); //跳过Rom 
  write_byte(0x48); //把暂存器内容写到EPRam中
} 

void mychar()//自定义字符
{
///////////////////////自定义字符
 WriteCommandLCM(0x48, 0); //第一行
 WriteDataLCM(0x02);
 WriteCommandLCM(0x49, 0); //第2行
 WriteDataLCM(0x05);
 WriteCommandLCM(0x4a, 0); //第3
 WriteDataLCM(0x05);
 WriteCommandLCM(0x4b, 0); //第4
 WriteDataLCM(0x02);
 WriteCommandLCM(0x4c, 0); //第5
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4d, 0); //第6
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4e, 0); //第7
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4f, 0); //第8
 WriteDataLCM(0x00);
}