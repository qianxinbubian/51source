#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
void key_scan();
uchar count0,count1,count3,num,n=0,temp,a,j,count4;
uchar mima[8];							//初始密码存储区
uchar tab_key[50];						//输入密码存储区
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
bit enterflag;							//确认键按下与否的标志
bit mimaflag;							//密码正确与否的标志
bit xiugaiflag;							//修改密码标志	
bit enter1flag;						//修改密码确认键标志	
sbit red=P3^7;
sbit bell=P3^6;
sbit rs=P2^0;
 sbit rw=P2^1;
sbit lcden=P2^2;
sbit scl=P3^4;
sbit sda=P3^5;
uchar code table1[]="input the passco";
uchar code table2[]="de:     --------";
uchar code table3[]="*";
uchar code table4[]="right   (^_^)   ";
uchar code table5[]="first  error!!!!";
uchar code table6[]="second error!!!!";
uchar code table7[]="third error see ";
uchar code table8[]="u tomorrow (^_^)";
uchar code table9[]="define the passc";
uchar code table10[]="ode:    --------";
uchar code table11[]="code   is    new";
//******************************键盘消抖函数*******************************
void delay1()
{ ;; }
void delay2(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
	 for(b=100;b>0;b--);
}
	
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

//****************************e^2room的初始化*******************************
void start()  //开始信号
{	
	sda=1;
	delay1();
	scl=1;
	delay1();
	sda=0;
	delay1();
}

void stop()   //停止
{
	sda=0;
	delay1();
	scl=1;
	delay1();
	sda=1;
	delay1();
}
//****************************应答信号*************************************
void respond()  
{
	uchar i;
	scl=1;
	delay1();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delay1();
}
//*****************************写字节操作函数**********************************
void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;			//保持最高位，左移到进位CY
		scl=0;
	    delay1();
		sda=CY;
		delay1();
		scl=1;
		delay1();
	}
	scl=0;
	delay1();
	sda=1;//总线释放
	delay1();
}
//*******************************读字节操作函数*****************************
uchar read_byte()
{
	uchar i,k;
	scl=0;
	delay1();
	sda=1;
	delay1();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay1();	
		k=(k<<1)|sda;    //或运算，放到最低位
		scl=0;
		delay1();	
	}
	return k;
}
//**********************************写地址函数******************************
void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);
	respond();
	write_byte(address);
	respond();
	write_byte(date);
	respond();
	stop();
}
//*******************************读地址函数*************************************
uchar read_add(uchar address)
{
	uchar date;
	start();
	write_byte(0xa0);
	respond();
	write_byte(address);
	respond();
	start();
	write_byte(0xa1);
	respond();
	date=read_byte();
	stop();
	return date;
}
//****************************LCD1602的初始化*******************************
void write_com(uchar com)
{
	rs=0;
	lcden=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

void write_date(uchar date)
{
	rs=1;
	lcden=0;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}

//***************************************密码比较函数********************************
bit mimacmp()					
{
	bit flag;
	uchar i;
	for(i=0;i<8;i++)
		{
			if(mima[i]==tab_key[i])
				flag=1;
			else
				{
				flag=0;
				i=8;
				}
		}
	return(flag);							//返回flag
}

////**********************************LCD显示函数开始**************************************
void lcd_display()
{
uchar i=0;
write_com(0x80+0x40+8);
for(i=0;i<n;i++)
{
write_date(table3[0]);
}
}


//****************************************键盘功能分配函数群开始****************************
//** 0 ** 1 **2 ** 3**                     
//** 4** 5** 6 **7 **
//**8** 9** 确认（A）  **无效（B）
//**取消（C）**修改密码键（D）**确认修改键（E）**无效（F）




void key_manage1()
{
tab_key[n]=0;
n++;
if(xiugaiflag==1)
{
mima[count4]=0;
count4++;
}
}

void key_manage2()
{

tab_key[n]=1;
n++;
if(xiugaiflag==1)
{
mima[count4]=1;
count4++;
}
}

void key_manage3()
{

tab_key[n]=2;
n++;
if(xiugaiflag==1)
{
mima[count4]=2;
count4++;
}
}

void key_manage4()
{
tab_key[n]=3;
n++;
if(xiugaiflag==1)
{
mima[count4]=3;
count4++;
}
}

void key_manage5()
{
tab_key[n]=4;
n++;
if(xiugaiflag==1)
{
mima[count4]=4;
count4++;
}
}

void key_manage6()
{
tab_key[n]=5;
n++;
if(xiugaiflag==1)
{
mima[count4]=5;
count4++;
}
}
void key_manage7()
{
tab_key[n]=6;
n++;
if(xiugaiflag==1)
{
mima[count4]=6;
count4++;
}
}
void key_manage8()
{
tab_key[n]=7;
n++;
if(xiugaiflag==1)
{
mima[count4]=7;
count4++;
}
}

void key_manage9()
{
tab_key[n]=8;
n++;
if(xiugaiflag==1)
{
mima[count4]=8;
count4++;
}
}
void key_manage10()
{
tab_key[n]=9;
n++;
if(xiugaiflag==1)
{
mima[count4]=9;
count4++;
}
}
//**********************************确认键**************************************************************  
void key_manage11()
{
	enterflag=1;					//确认键按下
	if(n==8)					   //只有输入8个密码后按确认才做比较
		mimaflag=mimacmp();
	else
		mimaflag=0;
	 if(enterflag==1)
		{
			enterflag=0;
			n=0;
			//用FFFFFFFF清除已经输入的密码
			for(count3=0;count3<8;count3++)
			{	
				delay(5);
				tab_key[count3]=0x0f;		
			}		
					
			TR1=1;									//打开计数器1
			count1=0;								//定时器1由50MS累计到1S所用的计数器
			if(mimaflag==1)
			{	
				a=0;
			
				write_com(0x01);
				write_com(0x80);
				for(count3=0;count3<16;count3++)
				{
					write_date(table4[count3]);	//密码正确，显示RIGHT，绿灯亮
					delay(5);
				}
			}
			        
		    else
			{   
			 	n=0;
			 	red=0;
		    	bell=0;
				a++;
				if(a==1)
				{
					for(count3=0;count3<8;count3++) //ffffffff清除密码
					{	
						delay(5);
						tab_key[count3]=0x0f;		
					}
					write_com(0x01);										
					write_com(0x80);
					for(count3=0;count3<16;count3++)
					{
						write_date(table5[count3]);	//密码错误，显示 first error，红灯亮
						delay(5);
					}
						TR1=1;
				}
				if(a==2)
				{	
					for(count3=0;count3<8;count3++) //ffffffff清除密码
					{	
						delay(5);
						tab_key[count3]=0x0f;		
					}
						write_com(0x01);
						write_com(0x80);
						for(count3=0;count3<16;count3++)
						{
							write_date(table6[count3]);	//密码错误，显示SECOND ERROR，红灯亮
							delay(5);
						}
							TR1=1;
				}
					
				  if(a==3)
					{	
						for(count3=0;count3<8;count3++) //ffffffff清除密码
						{	
							delay(5);
							tab_key[count3]=0x0f;		
						}
						write_com(0x01);
						write_com(0x80);
						for(count3=0;count3<16;count3++)
						{
							write_date(table7[count3]);	//密码错误，显示third error see，红灯亮
							delay(5);
						}
						write_com(0x80+0x40);
						for(count3=0;count3<16;count3++)
						{
							write_date(table8[count3]);//密码错误，显示 U TOMORROW ,红灯亮
							delay(5);
						}
							TR1=0;
								
					}
							
		}
	}

}
void key_manage12()
{
tab_key[n]=11;
n++;						  //密码计数清零

}
//****************************************************取消键********************************************  
void key_manage13()
{

n=0;						  //密码计数清零
write_com(0x80);						//指针所指位置
for(count3=0;count3<16;count3++)
{
	write_date(table1[count3]);		//第一行显示INPUT THE PASSPORD:
	delay(5);
}
write_com(0x80+0x40);
for(count3=0;count3<16;count3++)
{
	write_date(table2[count3]);	//开机显示--------
	delay(5);
	tab_key[count3]=0x0f;			//用FFFFFFFF清楚已经输入的密码
}

}
//*******************************************修改密码键**********************************
void key_manage14()
{
	uchar aa=0;
	n=0;
	xiugaiflag=1;
	write_com(0x01);
	write_com(0x80);
for(count3=0;count3<16;count3++)
{
	write_date(table9[count3]);	//显示define the password
	delay(5);
	tab_key[count3]=0x0f;			//用FFFFFFFF清楚已经输入的密码
}
	write_com(0x80+0x40);
for(count3=0;count3<16;count3++)
{
	write_date(table10[count3]);	//显示--------
	delay(5);
}
TR0=1;

}
//******************************************修改密码键的确认键********************************
void key_manage15()
{
n=0;
enter1flag=1;
if(enter1flag==1)
{
	enter1flag=0;
	count4=0;
	for(count3=0;count3<16;count3++)
	{
	tab_key[count3]=0x0f;			//用FFFFFFFF清楚已经输入的密码
 	}
	write_com(0x01);
	write_com(0x80);
	for(count3=0;count3<16;count3++)
	{
		write_date(table11[count3]);
		delay(100);
	}
	TR1=1;
	count1=0;
}
}
void key_manage16()
{
tab_key[n]=15;
n++;
}

//****************************************定时器1的50MS,共延时1秒*****************************
void time_1() interrupt 3
{	

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	if(count1<20)
	{
		count1++;
	}
	else 							//计时到1S
	{
		TR1=0;
		count1=0;
		mimaflag=0;

		red=1;
		bell=1;
		//显示FFFFFFFF
		write_com(0x01);
		write_com(0x80);
		for(count3=0;count3<16;count3++)
			{
				write_date(table1[count3]);	//显示INPUT THE PASSCODE
				delay(5);
			}
		write_com(0x80+0x40);
		for(count3=0;count3<16;count3++)
			{
				write_date(table2[count3]);	//开机显示FFFFFFFF
				delay(5);
			}
	}

}
//***********************************************定时0**********************************************
void time_0() interrupt 1
{	

	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
if(count4<8)
{
	key_scan();
}
else
{
	TR0=0;
	count4=0;
}
}

//初始化函数
void init()
{	

	uchar i;
	lcden=0;
	write_com(0x38);					//打开显示模式设置
	write_com(0x0c);					//打开显示，光标等等设置未零
	write_com(0x06);					//当读或写一个字符后地址指针加一，且光标加一，当写一个字符后整频显示左移，
	write_com(0x01);					//清零指令
	write_com(0x80);					//指针所指位置

	//定时器初始化
	TMOD=0x11;							//T0,T1工作方式1
	TH0=(65536-2000)/256;
	TL0=(65536-2000)%256;				//T0初始化2MS

	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;				//T1初始化50MS

	TR1=0;
	ET1=1;
	EA=1;
	TR0=0;
	ET0=1;

	count0=0;							//初始没有密码输入，故为零
	enterflag=0;						//没有确认键按下
	mimaflag=0;							//密码正确与否键先置零
					
	red=1;								//红灯不亮
//************密码存入EPROM中**********************************
	sda=1;
	delay(5);
	scl=1;
	delay(5);
	for(i=0;i<8;i++)
	{
		write_add(i,8);
		delay2(100);
	}
	for(i=0;i<8;i++)
	{
		mima[i]=read_add(i);
		delay(5);
	}

}
void main()
{	rw=0;		
	init();
	write_com(0x80);						//指针所指位置
	for(count3=0;count3<16;count3++)
	{
		write_date(table1[count3]);		//第一行显示INPUT THE PASSPORD:
		delay(5);
	}
	write_com(0x80+0x40);
	for(count3=0;count3<16;count3++)
	{
		write_date(table2[count3]);		//开机显示FFFFFFFF
		delay(5);
	}
	while(1)
	{
		key_scan();	   				   	//调用键盘扫描函数
		lcd_display(); 
	}
		 
}
//**************************************************键盘扫描函数开始********************************
void key_scan()
{
//**********扫描第一行*********
P1=0xfe; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xee:
key_manage1();
break;

case 0xde:
key_manage2();
break;

case 0xbe:
key_manage3();
break;

case 0x7e:
key_manage4();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
//**************************************************扫描第二行***********************************
P1=0xfd; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xed:
key_manage5();
break;

case 0xdd:
key_manage6();
break;

case 0xbd:
key_manage7();
break;

case 0x7d:
key_manage8();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
//*********************************************扫描第三行***********************************
P1=0xfb; 
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xeb:
key_manage9();
break;

case 0xdb:
key_manage10();
break;

case 0xbb:
key_manage11();
break;

case 0x7b:
key_manage12();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}

//***************************************************扫描第四行****************************************
P1=0xf7;
temp=P1;
temp=temp&0xf0;
if(temp!=0xf0)
{
delay(100);
if(temp!=0xf0)
{
temp=P1;
switch(temp)
{
case 0xe7:
key_manage13();
break;

case 0xd7:
key_manage14();
break;

case 0xb7:
key_manage15();
break;

case 0x77:
key_manage16();
break;
}
while(temp!=0xf0)
{
temp=P1;
temp=temp&0xf0;
}
}
}
} 

