#include <at89x52.h>
#include <DS1302.h>
//定义共阳极字型码0123456789-
unsigned char code dispcode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
unsigned char time[]={0,0,0,0,0,0,0,0};//用来储存时间
unsigned char date[]={0,0,0,0,0,0,0,0};//用来储存日期
SYSTEMTIME  time1;   //在ds1302.h文件中已经定义了一个名字为SYSTEMTIME的结构体,在这里我们定义一个变量名
                     //为time的SYSTEMTIME结构体        
void delay(unsigned char i)     //延时子程序  
{
  unsigned char j;
  while((i--)!=0)
  {
    for(j=123;j>0;j--);
  }
}
unsigned char button_time(n,x,y)        //时钟调整子程序
unsigned char n,x,y;
{
    if(P1_7==0)
    {
        delay(50);
        if(P1_7==0)
    	{
        	n++;
            if(n==x)
        	n=0;
            while(P1_7==0);
    	}
    }

    if(P1_5==0)
    {
        delay(50);
        if(P1_5==0)
    	{
            if(n==0)
        	n=y;
        	else
            n--;						
            while(P1_5==0);
       	}
    }
    return n;
}

unsigned char button_date(n,x,y)        //日期调整子程序
unsigned char n,x,y;
{
    if(P1_7==0)
    {
        delay(50);
        if(P1_7==0)
    	{
        	n++;
            if(n==x)
        	n=1;
            while(P1_7==0);
    	}
    }

    if(P1_5==0)
    {
        delay(50);
        if(P1_5==0)
    	{
            if(n==1)
        	n=y;
        	else
            n--;						
            while(P1_5==0);
       	}
    }
    return n;
}

void display(second10,second1,minute10,minute1,hour10,hour1)	//显示子程序
unsigned char second10,second1,minute10,minute1,hour10,hour1;  
{
  P2=0xfe;
  P0=dispcode[second10];//显示秒的十位
  delay(1);

  P2=0xfd;
  P0=dispcode[second1];	//显示秒的个位
  delay(1);

  P2=0xfb;
  P0=dispcode[10];		//显示"-"
  delay(1);

  P2=0xf7;
  P0=dispcode[minute10];//显示分的十位
  delay(1);

  P2=0xef;
  P0=dispcode[minute1];	//显示分的个位
  delay(1);

  P2=0xdf;
  P0=dispcode[10];		//显示"-"
  delay(1);

  P2=0xbf;
  P0=dispcode[hour10];	//显示时的十位
  delay(1);

  P2=0x7f;
  P0=dispcode[hour1];	//显示时的个位
  delay(1);
}	

void main()
{  
   unsigned char flag;
   Initial_DS1302();				    //初始化DS1302这个时钟芯片,   
   while(1)							    
   {
       DS1302_GetTime(&time1);  		//读取时间参数            
       time[5]=(time1.Second)%10;		//把秒的个位数据存入time[5]
       time[4]=(time1.Second)/10;		//把秒的十位数据存入time[4]
       time[3]=(time1.Minute)%10;		//把分的个位数据存入time[3]
       time[2]=(time1.Minute)/10;		//把分的十位数据存入time[2]
       time[1]=(time1.Hour)%10;  		//把时的个位数据存入time[1]
       time[0]=(time1.Hour)/10;		    //把时的十位数据存入time[0]
	   date[5]=(time1.Day)%10;
	   date[4]=(time1.Day)/10;
	   date[3]=(time1.Month)%10;
	   date[2]=(time1.Month)/10;
	   date[1]=(time1.Year)%10;
	   date[0]=(time1.Year)/10;		

	   if(P1_4==0)		//如果按下Time Start键一下,时钟开始正常显示时间,再按一下,显示日期
	   {
			delay(50);
			if(P1_4==0)
			{
				flag++;
				if(flag>1)
				{
					flag=0;
				}
			}
			while(P1_4==0);
		}
	   if(P1_6==0)		//如果按下Time Set键一下,开始显示日期,再按一下进入日期跟时钟的调节模式
	   {
	   		delay(50);
			if(P1_6==0)
			{
				flag++;
				if(flag>7)
				{
					flag=0;
				}
			}
			while(P1_6==0);
		}

		switch(flag)
		{
			case 0:display(time[0],time[1],time[2],time[3],time[4],time[5]);		//调用子函数display,把存入数组time的数据给显示出来
				   break;

			case 1:display(date[0],date[1],date[2],date[3],date[4],date[5]);		//调用子函数display,把存入数组date的数据给显示出来
				   break;

			case 2:time1.Year=button_date(time1.Year,100,99);						//调整年
				   DS1302_SetTime(0x8c,time1.Year);
				   display(date[0],date[1],10,10,10,10);
				   break;

			case 3:time1.Month=button_date(time1.Month,13,12);						//调整月
				   DS1302_SetTime(0x88,time1.Month);
				   display(10,10,date[2],date[3],10,10);
				   break;

			case 4:time1.Day=button_date(time1.Day,32,31);							//调整日
				   DS1302_SetTime(0x86,time1.Day);
				   display(10,10,10,10,date[4],date[5]);
				   break;

			case 5:time1.Hour=button_time(time1.Hour,24,23);					    //调整时
				   DS1302_SetTime(0x84,time1.Hour);
				   display(time[0],time[1],10,10,10,10);
				   break;

			case 6:time1.Minute=button_time(time1.Minute,60,59);				    //调整分
				   DS1302_SetTime(0x82,time1.Minute);
				   display(10,10,time[2],time[3],10,10);
				   break;

			case 7:time1.Second=button_time(time1.Second,60,59);				    //调整秒
			       DS1302_SetTime(0x80,time1.Second);
				   display(10,10,10,10,time[4],time[5]);
				   break;
		}		 
	}
}