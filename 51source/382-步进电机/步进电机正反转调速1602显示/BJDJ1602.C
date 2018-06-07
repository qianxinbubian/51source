
    
#include <reg51.h>       //51芯片管脚定义头文件
#include <intrins.h>  //内部包含延时函数 _nop_();

#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};


uchar code FFW[8]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9};
uchar code REV[8]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1};

sbit  K1   = P3^2;       //运行与停止
sbit  K2   = P3^3;       //设定圈数
sbit  K3   = P3^4;       //方向转换
sbit  K4   = P3^5;       //速率调整
sbit  BEEP = P3^6;       //蜂鸣器

sbit  LCD_RS = P2^0;             
sbit  LCD_RW = P2^1;
sbit  LCD_EN = P2^2;

bit  on_off=0;            //运行与停止标志
bit  direction=1;         //方向标志
bit  rate_dr=1;           //速率标志
bit  snum_dr=1;           //圈数标志

uchar code  cdis1[ ] = {" STEPPING MOTOR "};
uchar code  cdis2[ ] = {"CONTROL  PROCESS"};
uchar code  cdis3[ ] = {"    STOP        "};
uchar code  cdis4[ ] = {"NUM:    RATE:   "};
uchar code  cdis5[ ] = {"  RUNNING       "};

uchar   m,v=0,q=0;

uint   number=0,number1=0; 
uchar   snum=10,snum1=10;       //预设定圈数
uchar   rate=2;               //预设定速率
uchar   data_temp,data_temP1,data_temp2;   

/********************************************************/
/*                                                  
/* 延时t毫秒 
/* 11.0592MHz时钟，延时约1ms                                     
/*                                                      
/********************************************************/
void delay(uint t)
{                           
   uchar k;
   while(t--)
   {
     for(k=0; k<125; k++)
     { }
   }
}

/********************************************************/
void delayB(uchar x)    //x*0.14MS
 {
   uchar i;
   while(x--)
   {
     for (i=0; i<13; i++)
     { }
   }
 }

/********************************************************/
void beep()
 {
   uchar j;
   for (j=0;j<100;j++)
    { 
     delayB(4);
     BEEP=!BEEP;                 //BEEP取反
    } 
     BEEP=1;                    //关闭蜂鸣器
  delay(170);
 }

/********************************************************/
/*                                                                
/*检查LCD忙状态                                                   
/*lcd_busy为1时，忙，等待。为0时,闲，可写指令与数据。     
/*                                                               
/********************************************************/ 

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

/********************************************************/
/*                                                                 
/*写指令数据到LCD                                                 
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             
/*                                                               
/********************************************************/

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

/********************************************************/
/*                                                                
/*写显示数据到LCD                                                  
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                              
/*                                                               
/********************************************************/

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

/********************************************************/
/*                                                                
/*  LCD初始化设定                                                
/*                                                                
/********************************************************/

void lcd_init()
{ 
    delay(30);                   
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

/********************************************************/
/*                                                                 
/*  设定显示位置                                                  
/*                                                                
/********************************************************/

void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80);  //数据指针=80+地址变量
}

/********************************************************/
/*                                                       
/* LCD1602初始显示子程序                                             
/*                                                      
/********************************************************/
void  LCD_init_DIS()
{            
     delay(10);                 //延时
     lcd_init();                //初始化LCD             
        
     lcd_pos(0);                //设置显示位置为第一行的第1个字符
     m = 0;
     while(cdis1[m] != '\0')
      {                         //显示字符
        lcd_wdat(cdis1[m]);
        m++;
      }

     lcd_pos(0x40);             //设置显示位置为第二行第1个字符
     m = 0;
     while(cdis2[m] != '\0')
      {
        lcd_wdat(cdis2[m]);      //显示字符
        m++;
      }

      delay(3000);               //延时        
        
      lcd_pos(0);                //设置显示位置为第一行的第1个字符
      m = 0;
      while(cdis3[m] != '\0')
        {                        //显示字符
          lcd_wdat(cdis3[m]);
          m++;
        }

      lcd_pos(0x40);             //设置显示位置为第二行第1个字符
      m = 0;
     while(cdis4[m] != '\0')
        {
          lcd_wdat(cdis4[m]);    //显示字符
          m++;
        }      

        for(m=0;m<2;m++)
          { 
       lcd_pos(0x0c+m);    //显示方向符号
             lcd_wdat(0x3e);
    }

}
/********************************************************/
/*
/*数据转换子程序
/*
/********************************************************/
void  data_conv() 
 {
     data_temP1=data_temp/10;       //高位
  if(data_temP1==0)
  {data_temP1=0x20;}             //高位为0不显示
     else 
  {data_temP1=data_temP1+0x30;}

   data_temp2=data_temp%10;       //低位
     data_temp2=data_temp2+0x30;
 }

/********************************************************/
/*
/*数据显示子程序
/*
/********************************************************/
void  data_dis()
{
    data_temp = snum;        //显示圈数
    data_conv();
       lcd_pos(0x44); 
       lcd_wdat(data_temP1);
       lcd_pos(0x45); 
       lcd_wdat(data_temp2);
    
    data_temp = rate;         //显示速率
    data_conv();
       lcd_pos(0x4d); 
       lcd_wdat(data_temP1);
       lcd_pos(0x4e); 
       lcd_wdat(data_temp2);
}
/********************************************************
/*
/* 显示运行方向符号
/*
/********************************************************/
void  motor_DR()
  {
       if(direction==1)           //正转方向标志
        { for(m=0;m<2;m++)
          { 
      lcd_pos(0x0c+m);      //显示方向符号
            lcd_wdat(0x3e);
     }
   }
        else
         { for(m=0;m<2;m++)       //反转方向标志
           { 
          lcd_pos(0x0c+m);     //显示方向符号 
             lcd_wdat(0x3c);
     }
   }
  }

/********************************************************
/*
/* 显示运行状态
/*
/********************************************************/
void  motor_RUN()
 {
      if(on_off==1)
    { TR0=1; 
      lcd_pos(0);     //设置显示位置为第一行的第1个字符
         m = 0;
         while(cdis5[m] != '\0')
          { lcd_wdat(cdis5[m]);      //RUNNING
            m++;   }
            motor_DR();              //
    }  
      else  
    { TR0=0; P1 =0x0f; 
   lcd_pos(0);     //设置显示位置为第一行的第1个字符
         m = 0;
         while(cdis3[m] != '\0')
          { lcd_wdat(cdis3[m]);      //STOP
            m++;   }
            motor_DR();              //
   snum=snum1;             //
   number1=0;              //清圈数计数器
         }
  }

/********************************************************
*                                                       
*  主程序                                               
*                                                      
*********************************************************/

main()
 { 
         LCD_init_DIS();
  
   TMOD = 0x01;       //T0定时方式1
   TL0  = 0x33;
   TH0  = 0xf5;
   EA   = 1;
   ET0  = 1; 
    P1   = 0x0f;
  
   while(1)
    {  
       if(K1==0)
    {
         beep();
      while(K1==0);       //等待键释放
      on_off=~on_off;       
         motor_RUN();  
        }   //K1 end
/********************************************************/
      if(K2==0)    
       {
      beep();
   if(snum_dr==1)
    { snum++;
        snum1=snum;
      if(snum==0x14)
            { snum_dr=~snum_dr;}
    }
   else  
     {snum--;
   snum1=snum;
      if(snum==0x01)
   { snum_dr=~snum_dr; }
     }
   
    } //K2  end
/********************************************************/
        if(K3==0)    
      {
      beep();
   direction=~direction; 
         motor_DR();
      }//K3 end

/********************************************************/
        if(K4==0)    
       {
      beep();
   if(rate_dr==1)
    { rate++;
      if(rate==0x10)
            { rate_dr=~rate_dr;}
    }
    else  
     { 
      rate--;
      if(rate==0x01)
   { rate_dr=~rate_dr; }
     }
  } //K4 end
    
/********************************************************/
      if(number1==snum1)   //与设定圈数是否相等  
     { number1=0; 
         on_off=0;
         TR0=0;
        snum=snum1;
   P1=0x0f;
    motor_RUN();
    }         
         data_dis();
 }  // while(1) end
 }  //main end

/********************************************************/
/*
/*  定时器 0 中断 
/*
/********************************************************/

void  motor_onoff()  interrupt  1  
 {     
       TL0  = 0x33;
       TH0  = 0xf5; 
       q++;
    if(q < rate)
        { return; }
    else 
       {  q=0;    
       number++;                  //脉冲计数
        
      if(number==4096)              //64个脉冲电机转一圈
       { snum--;
      number=0;
            number1++; }          //电机转动圈数
    

         if(direction==1)            //方向标志
       { if(v<8)  
         {P1 = FFW[v];v++;}       //取数据，正转
        if(v==8) 
        { v=0; } 
          }
   
      else
       { if(v<8)  
         {P1 = REV[v];v++;}       //取数据，反转
         if(v==8) 
         { v=0; } 
          }
    }
 } 
