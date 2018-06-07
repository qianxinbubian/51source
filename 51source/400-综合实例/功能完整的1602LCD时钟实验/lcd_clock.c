//按K1按键进入设定状态
//按K2,依次进入闹钟功能是否启用，闹钟时,分秒,年,月,日及时间时,分,秒的设置,直到退出设置状态
//按K3,调整是否起用闹钟和调节闹钟时,分,秒,年,月,日,时间的时,分,秒的数字
//LCD第二排中间显示小喇叭，表示启用闹钟功能，无则禁止闹钟功能（可在调整状态进行设置）
//正常状态,LCD上排最前面显示自定义字符,LCD下排最前面闪动"RICHMCU"
//设置状态,LCD上排最前面显示"P",下排最前面在设置闹钟时间时显示"alarm:",其它状态显示"time"
//年代变化2000--2099,星期自动转换
//程序中有自定义字符写入
#include <reg51.h>
#include <intrins.h>
unsigned char code dis_week[]={"SUN,MON,TUE,WED,THU,FRI,SAT"};
unsigned char code para_month[13]={0,0,3,3,6,1,4,6,2,5,0,3,5};	//星期月参变数
unsigned char data dis_buf1[16];		//lcd上排显示缓冲区
unsigned char data dis_buf2[16];		//lcd下排显示缓冲区		
unsigned char data year,month,date,week;//年、月、日、星期
unsigned char data armhour,armmin,armsec;//闹钟时、分、秒
unsigned char data hour,min,sec,sec100;	//时、分、秒、百分之一秒
unsigned char data flag,vkey,skey;//设置状态计数标志、按键先前值、按键当前值	
bit	alarm;	//标识是否启用闹钟，1--启用，0--关闭
sbit	 rs = P2^0;				//LCD数据/命令选择端(H/L)
sbit	 rw = P2^1;				//LCD读/写选择端(H/L)
sbit	 ep = P2^2;				//LCD使能控制
sbit	PRE = P3^3;				//调整键(AN3)
sbit	SET = P3^4;				//调整键(AN4)
sbit	SPK = P3^6;				
void delayms(unsigned char ms);	//延时程序
bit  lcd_busy();				//测试LCD忙碌状态程序
void lcd_wcmd(char cmd);		//写入指令到LCD程序
void lcd_wdat(char dat);		//写入数据到LCD程序
void lcd_pos(char pos);			//LCD数据指针位置程序
void lcd_init();				//LCD初始化设定程序
void pro_timedate();			//时间日期处理程序
void pro_display();				//显示处理程序
void pro_key();					//按键处理程序
void time_alarm();				//定时报警功能(闹钟)
unsigned char scan_key();		//按键扫描程序
unsigned char week_proc();		//星期自动计算与显示函数
bit leap_year();				//判断是否为闰年
void lcd_sef_chr();				//LCD自定义字符程序
void update_disbuf(unsigned char t1,unsigned char t2[],unsigned char dis_h,unsigned char dis_m,unsigned char dis_s);			
								//更新显示缓冲区函数
 

// 延时程序
void delay(unsigned char ms)
{	while(ms--)
	{	unsigned char i;
		for(i = 0; i< 250; i++)     
		{
			_nop_();			   //执行一条_nop_()指令为一个机器周期
			_nop_();
			_nop_();
			_nop_();
		}
	}
}		
	

//测试LCD忙碌状态
bit lcd_busy()
{	
	bit result;
	rs = 0;
	rw = 1;
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result =(bit)(P0&0x80);	//LCD的D0--D7中,D7=1为忙碌,D7=0为空闲
	ep = 0;
	return result;	
}

//写入指令到LCD
void lcd_wcmd(char cmd)
{							
	while(lcd_busy());	//当lcd_busy为1时,再次检测LCD忙碌状态,lcd-busy为0时,开始写指令
	rs = 0;
	rw = 0;
	ep = 0;
	_nop_();
	_nop_();	
	P0 = cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;		
}

//写入数据到LCD
void lcd_wdat(char dat)	
{							
	while(lcd_busy());	//当lcd_busy为1时,再次检测LCD忙碌状态,lcd-busy为0时,开始写数据
	rs = 1;
	rw = 0;
	ep = 0;
	P0 = dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;	
}

//LCD数据指针位置程序
void lcd_pos(char pos)
{						
	lcd_wcmd(pos|0x80);	//数据指针=80+地址码(00H~27H,40H~67H)
}


//设定二个自定义字符,(注意：LCD1602中自定义字符的地址为0x00--0x07,即可定义8个字符)
//这里我们设定把一个自定义字符放在0x00位置（000）,另一个放在0x01位子（001）
void lcd_sef_chr()
{	//第一个自定义字符
	lcd_wcmd(0x40);	//"01 000 000"  第1行地址 (D7D6为地址设定命令形式D5D4D3为字符存放位置(0--7)，D2D1D0为字符行地址(0--7)）
  	lcd_wdat(0x1f);	//"XXX 11111"	第1行数据（D7D6D5为XXX，表示为任意数(一般用000），D4D3D2D1D0为字符行数据(1-点亮，0-熄灭）
	lcd_wcmd(0x41);	//"01 000 001" 	第2行地址
  	lcd_wdat(0x11);	//"XXX 10001"	第2行数据
	lcd_wcmd(0x42);	//"01 000 010" 	第3行地址
  	lcd_wdat(0x15);	//"XXX 10101"	第3行数据
	lcd_wcmd(0x43);	//"01 000 011" 	第4行地址
  	lcd_wdat(0x11);	//"XXX 10001"	第4行数据
	lcd_wcmd(0x44);	//"01 000 100" 	第5行地址
  	lcd_wdat(0x1f);	//"XXX 11111"	第5行数据
	lcd_wcmd(0x45);	//"01 000 101" 	第6行地址
  	lcd_wdat(0x0a);	//"XXX 01010"	第6行数据
	lcd_wcmd(0x46);	//"01 000 110" 	第7行地址
  	lcd_wdat(0x1f);	//"XXX 11111"	第7行数据
	lcd_wcmd(0x47);	//"01 000 111" 	第8行地址
  	lcd_wdat(0x00);	//"XXX 00000"	第8行数据 
	//第二个自定义字符
	lcd_wcmd(0x48);	//"01 001 000"  第1行地址  
  	lcd_wdat(0x01);	//"XXX 00001"	第1行数据 
	lcd_wcmd(0x49);	//"01 001 001" 	第2行地址
  	lcd_wdat(0x1b);	//"XXX 11011"	第2行数据
	lcd_wcmd(0x4a);	//"01 001 010" 	第3行地址
  	lcd_wdat(0x1d);	//"XXX 11101"	第3行数据
	lcd_wcmd(0x4b);	//"01 001 011" 	第4行地址
  	lcd_wdat(0x19);	//"XXX 11001"	第4行数据
	lcd_wcmd(0x4c);	//"01 001 100" 	第5行地址
  	lcd_wdat(0x1d);	//"XXX 11101"	第5行数据
	lcd_wcmd(0x4d);	//"01 001 101" 	第6行地址
  	lcd_wdat(0x1b);	//"XXX 11011"	第6行数据
	lcd_wcmd(0x4e);	//"01 001 110" 	第7行地址
  	lcd_wdat(0x01);	//"XXX 00001"	第7行数据
	lcd_wcmd(0x4f);	//"01 001 111" 	第8行地址
  	lcd_wdat(0x00);	//"XXX 00000"	第8行数据 



 
}

//LCD初始化设定
void lcd_init()
{						
	lcd_wcmd(0x38);		//设置LCD为16X2显示,5X7点阵,八位数据借口
	delay(1);
	lcd_wcmd(0x0c);		//LCD开显示及光标设置(光标不闪烁,不显示"-")
	delay(1);
	lcd_wcmd(0x06);		//LCD显示光标移动设置(光标地址指针加1,整屏显示不移动)
	delay(1);
	lcd_wcmd(0x01);		//清除LCD的显示内容
	delay(1);
}

//闰年的计算
bit leap_year()
{
	bit leap;
	if((year%4==0&&year%100!=0)||year%400==0)//闰年的条件
		leap=1;
	else
		leap=0;
	return leap;
}

//星期的自动运算和处理
unsigned char week_proc()
{	unsigned char num_leap;	
	unsigned char c;
	num_leap=year/4-year/100+year/400;//自00年起到year所经历的闰年数
	if( leap_year()&& month<=2 )	  //既是闰年且是1月和2月	
		c=5;
	else 
		c=6;
	week=(year+para_month[month]+date+num_leap+c)%7;//计算对应的星期
	return week;
}	

//更新显示缓冲区
void update_disbuf(unsigned char t1,unsigned char t2[],unsigned char dis_h,unsigned char dis_m,unsigned char dis_s)	
{	dis_buf1[0]=t1; 			//
	dis_buf1[1]=0x20; 			//空格
	dis_buf1[2]=50; 			//'2' 
	dis_buf1[3]=48;             //'0'
	dis_buf1[4]=year/10+48; 
	dis_buf1[5]=year%10+48; 
	dis_buf1[6]=0x2d;
	dis_buf1[7]=month/10+48;
	dis_buf1[8]=month%10+48; 
	dis_buf1[9]=0x2d; 			//'-'
	dis_buf1[10]=date/10+48;
	dis_buf1[11]=date%10+48;
	dis_buf1[12]=0x20;
	dis_buf1[13]=dis_week[4*week];
	dis_buf1[14]=dis_week[4*week+1];
	dis_buf1[15]=dis_week[4*week+2];

	dis_buf2[0]=t2[0]; 
	dis_buf2[1]=t2[1]; 
	dis_buf2[2]=t2[2]; 
	dis_buf2[3]=t2[3];
	dis_buf2[4]=t2[4]; 
	dis_buf2[5]=t2[5];
	dis_buf2[6]=t2[6];			//空格
 	if (alarm)
		dis_buf2[7]=0x01;		//alarm=1，显示闹钟启用标致（第二个自定义字符）
	else
		dis_buf2[7]=0x20;		//alarm=0，不显示闹钟启用标致
	dis_buf2[8]=dis_h/10+48; 
	dis_buf2[9]=dis_h%10+48; 
	dis_buf2[10]=0x3a;			//':'
	dis_buf2[11]=dis_m/10+48;
	dis_buf2[12]=dis_m%10+48;
	dis_buf2[13]=0x3a;
	dis_buf2[14]=dis_s/10+48;
	dis_buf2[15]=dis_s%10+48;

}

//时间和日期处理程序
void pro_timedate()
{	
	sec++;
	if(sec > 59)
		{sec = 0;
		 min++;
		 if(min>59)
			{min=0;
			 hour++;
			 if(hour>23)
				{hour=0;
				 date++;
				 if (month==1||month==3||month==5||month==7||month==8||month==10||month==12)
					 if (date>31) {date=1;month++;}					//大月31天
				 if (month==4||month==6||month==9||month==11)		
				 	 if (date>30) {date=1;month++;}					//小月30天
				 if (month==2)			
					 {if( leap_year())								//闰年的条件
						{if (date>29) {date=1;month++;}}			//闰年2月为29天
					 else
						{if (date>28) {date=1;month++;}}			//平年2月为28天	
					 }				
				 if (month>12) {month=1;year++;}
				 if (year>99) year=0;
				}
			}
		}
	week_proc();
	if (sec==armsec && min==armmin && hour==armhour)				
		{if (alarm)
			TR1=1;					//闹钟启用时，报警时间到,启动Timer1
		}

}

//显示处理程序
void pro_display()
{	unsigned char i;
	lcd_pos(0x00);
	for (i=0;i<=15;i++)
		{lcd_wdat(dis_buf1[i]);}

	lcd_pos(0x40);
	for (i=0;i<=15;i++)
		{lcd_wdat(dis_buf2[i]);}
}

//Timer0中断处理程序,秒的产生
void timer0() interrupt 1 
{
	TH0=0xD8;			
	TL0=0xF0;		
	sec100++;
	if(sec100 >= 100)	//1秒时间 (100*10ms=1000ms=1s)
		{sec100 = 0;
		 pro_timedate();//调用时间和日期处理程序
		}
	if (sec&0x01)										//"RICHMCU"闪一秒，停一秒
		update_disbuf(0x00,"       ",hour,min,sec);	   //0x00表示显示00位置的自定义字符		
	else 
  update_disbuf(0x00,"RICHMCU",hour,min,sec); 
 pro_display();  //调用显示处理函数



}	
	
//按键扫描程序
unsigned char  scan_key()
{	
	skey=0x00;									//给变量vkey置初值
	skey|=PRE;									//读取PRE键的状态
	skey=skey<<1;								//将PRE键的状态存于skey的B1位
	skey|=SET;									//读取SET键的状态,并存于skey的B0位
	return skey;								//返回skey的键值(即PRE,SET的状态)

}

//外部中断INT0中断处理程序
void int0() interrupt 0 
{		
		TR0=0;									//禁止Timer0
		IE=0;									//禁止中断
		lcd_wcmd(0x0e);							//显示光标"_",整个光标不闪烁
		alarm=1;
		update_disbuf(0x50,"alarm:",armhour,armmin,armsec);	//更新显示数据，0x50表示要显示"P"	
		pro_display();							//调用显示处理程序
		lcd_pos(0x47);			    			//使光标位于第一个调整项下						
		flag=0;									
		vkey=0x03;
		while(flag^0x0a)
			{skey = scan_key();					//扫描按键状态
			if (skey^vkey)						//若skey与vkey相同,跳出循环,相异执行循环体
				{	delay(10);					//去按键抖动	
					skey = scan_key();			//转回扫描按键状态			
					if (skey^vkey)				//若skey与vkey相同,跳出循环,相异执行循环体		
			 			{	vkey=skey;			//将skey的值付给vkey	
							if (skey==0x01)		//PRE键按下
								{	 flag++;	//调整标志位加1 
										switch (flag)	//将光标置于相应调整位置
										{	
										  		
											case 1: lcd_pos(0x49);break;		//光标置小时报警设置位置						 
											case 2:	lcd_pos(0x4c);break;		//光标置分钟报警设置位置	   	
											case 3:	lcd_pos(0x4f);break;		//光标置秒时报警设置位置
											
											case 4:	update_disbuf(0x50,"time: ",hour,min,sec);
													pro_display();
													lcd_pos(0x05);break;		//光标置年调整位置										  	
											case 5:	lcd_pos(0x08);break;		//光标置月调整位置
											case 6:	lcd_pos(0x0b);break;		//光标置日调整位置
											
											case 7: lcd_pos(0x49);break;		//光标置时调整位置
											case 8:	lcd_pos(0x4c);break;		//光标置分调整位置
											case 9:	lcd_pos(0x4f);break;		//光标置秒调整位置
												
											default:break;
										}
								}
							if (skey==0x02)		    //SET键按下
								{	pro_key();		//转设置按键处理程序
								}
						}
				}								 
			}								 
		lcd_wcmd(0x0c);							//设置LCD开显示及光标不闪烁,不显示"-"
		lcd_wcmd(0x01);						    //清除LCD的显示内容
		IE=0x8f;      							//CPU开中断,INT0,INT1,开中断
		TR0=1;									//Timer0启动
}


//主程序,初始化及初值设定
void main()
{	
	lcd_init();		                   	//初始化LCD
	lcd_sef_chr();						//写入自定义字符号
	hour=0;min=0;sec=0;				    //开机时的时,分,秒显示
	armhour=0;armmin=0;armsec=0;		//开机时的时,分,秒报警初值
	year= 5; month=1;date=1;     		//开机时的年,月,日,星期显示
	week_proc();
	alarm=1;							//初始开机，启用闹钟
	IE = 0x8f;							//CPU开中断,INT0,INT1,Timer0,Timer1开中断
	IP = 0x04;							//设置INT0为中断最高优先级
	IT0=0;IT1=0;						//外部INT0,INT1设置为电平触发方式（注意，触发不要选边沿方式，易误动）
	TMOD = 0x11;						//Timer0,Timer1工作于模式1, 16位定时方式
	TH0 = 0xdc;TL0 = 0x00;				//Timer0置10ms定时初值 	
	TH1 = 0xff;TL1 = 0x00;				//Timer1置初值 	
	TR0 = 1;							//Timer0启动
	TR1 = 0;
	while(1);
}

//设置按键处理程序
void pro_key()
{
	switch (flag)
		{ 
		case 0:alarm=!alarm;			//启用或关闭闹钟（alarm=1:启用,alarm=0:关闭)
				update_disbuf(0x50,"alarm:",armhour,armmin,armsec); //更新显示数据
				pro_display();										//调用显示处理
				lcd_pos(0x47);break;								//光标回到原调整位置 
		case 1:armhour++;
				if (armhour>23) armhour=0;
				update_disbuf(0x50,"alarm:",armhour,armmin,armsec); //更新显示数据
				pro_display();										//调用显示处理
				lcd_pos(0x49);break;								//光标回到原调整位置
		case 2:armmin++;
				if (armmin>59) armmin=0;
				update_disbuf(0x50,"alarm:",armhour,armmin,armsec);
				pro_display();
				lcd_pos(0x4c);break;
		case 3:armsec++;
				if (armsec>59) armsec=0;
				update_disbuf(0x50,"alarm:",armhour,armmin,armsec);
				pro_display();
				lcd_pos(0x4f);break;

		case 4:year++;
				if	(year> 99) year= 0;
				week_proc();                        //星期自动运算
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x05);break;
	 	case 5:month++;
				if (month>12) month=1;
				week_proc();						//星期自动运算
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x08);break;
		case 6:date++;
				if (month==1||month==3||month==5||month==7||month==8||month==10||month==12)
					if (date>31) date=1;			//大月31天
				if (month==4||month==6||month==9||month==11)		
				 	if (date>30) date=1;			//小月30天
				if (month==2)			
					{if(leap_year())				//闰年的条件
						{if (date>29) date=1;}		//闰年2月为29天
					 else
						{if (date>28) date=1;}}		//平年2月为28天	
				week_proc();					    //星期自动运算
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x0b);break;
		
		case 7:hour++;
				if (hour>23) hour=0;
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x49);break;	
		case 8:min++;
				if (min>59) min=0;
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x4c);break;
		case 9:sec++;
				if (sec>59) sec=0;
				update_disbuf(0x50,"time: ",hour,min,sec);
				pro_display();
				lcd_pos(0x4f);break;
		default: break ;												 
		}
}	

//Timer1中断处理程序,产生报警的声音
void timer1() interrupt 3 
{
	TH1=0xff;				
	TL1=0x00;
	SPK=~SPK;
	 
				
}

//外部中断INT1中断处理程序,停止报警声音
void int1() interrupt 2
{
	if(TR1)
		TR1=0;				

} 
