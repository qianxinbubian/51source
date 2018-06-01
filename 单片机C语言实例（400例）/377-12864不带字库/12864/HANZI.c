 #include<reg51.h>
#include<intrins.h>
sbit CS1=P2^3;
sbit CS2=P2^4;
sbit RST=P2^5;
sbit DI=P2^0;
sbit RW=P2^1;
sbit ALE=P2^2;
unsigned char  COM;//指令数据寄存器
unsigned char  DAT;//显示数据寄存器
#ifndef __WRITE_12864_H__

//软件复位程序
void    IntRst(void)
        {
            unsigned char  i;
            RST=0;
            i=0;
            i=1;
            RST=1;        //软件复位

        }

//12864发送命令子程序
void	ComSend(void)
	{	unsigned char	i;
        unsigned char  temp;
		DI=0;
		RW=1;        
        do {	P0=0xff;
				ALE=1;
                i=0;
				temp=P0;
                ALE=0;
			} while((temp&0x80)==1);	//读BUSY信号
		RW=0;
		P0=COM;
		ALE=1;
		i=0;		//NOP		
		ALE=0;
	}

//12864发送数据子程序 
void	DaSend(void)
	{   unsigned char	i;
        unsigned char  temp;
		DI=0;
		RW=1;
		do	{P0=0xff;
			ALE=1;
			i=0;
			temp=P0;
            ALE=0;
			}while((temp&0x80)==1);		//读BUSY信号
		DI=1;
		RW=0;
		P0=DAT;
		ALE=1;
		i=0;		//NOP
		ALE=0;
	}

//左半屏写命令子程序 
void	Left_ComSend(void)
	{
		CS2=0;CS1=1;
		ComSend();
		CS1=0;
	}

//左半屏写数据子程序 
void	Left_DaSend(void)
	{
		CS2=0;CS1=1;
		DaSend();
		CS1=0;
	}

//右半屏写命令子程序 
void	Right_ComSend(void)
	{
		CS1=0;CS2=1;
		ComSend();
		CS2=0;
	}


//右半屏写数据子程序 
void	Right_DaSend(void)
	{
		CS1=0;CS2=1;
		DaSend();
		CS2=0;
	}

//左半屏写一个字节数据程序,
//入口参数为X,Y的起始地址
//以及Y的变址范围(写入的字节数)和写入数据的指针
void	Left_Bytes_DaSend(unsigned char x,unsigned char y,unsigned char *loc,unsigned char bytes)
		{	unsigned char	i;
			COM=(x|0xB8);		//填入特征位0xB8 
			Left_ComSend();		//定位X
			COM=(y|0x40);		//填入特征位0x40 
			Left_ComSend();		//定位Y
			for(i=0;i<bytes;i++)
				{
				DAT=*(loc+i);
				Left_DaSend();
				}

		}

//右半屏写一个字节数据程序,
//入口参数为X,Y的起始地址
//以及Y的变址范围(写入的字节数)和写入数据的指针
void	Right_Bytes_DaSend(unsigned char x,unsigned char y,unsigned char *loc,unsigned char bytes)
		{	unsigned char	i;
			COM=(x|0xB8);			//填入特征位0xB8 
			Right_ComSend();		//定位X
			COM=(y|0x40);			//填入特征位0x40 
			Right_ComSend();		//定位Y
			for(i=0;i<bytes;i++)
				{
				DAT=*(loc+i);
				Right_DaSend();
				}
		}
//开LCD
void    Lcd_On(void)
        {
             COM=0x3F;
             Left_ComSend();
             Right_ComSend();
        }
/*
//关LCD
void	Lcd_Off(void)
		{
			 COM=0x3e;
             Left_ComSend();
             Right_ComSend();	
		}
*/
//设置Z地址 
void	Lcd_Z(unsigned char z)
		{
			 COM=(z|0xc0);
             Left_ComSend();
             Right_ComSend();	
		}

  unsigned char code  tabe[]=
{	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//清屏
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x10,0x21,0x86,0x70,0x00,0x7E,0x4A,0x4A, //温
0x4A,0x4A,0x4A,0x7E,0x00,0x00,0x00,0x00,
0x02,0xFE,0x01,0x40,0x7F,0x41,0x41,0x7F,
0x41,0x41,0x7F,0x41,0x41,0x7F,0x40,0x00,
	
0x80,0x40,0xF0,0x2C,0x43,0x20,0x98,0x0F,//你
0x0A,0xE8,0x08,0x88,0x28,0x1C,0x08,0x00,
0x00,0x00,0x7F,0x00,0x10,0x0C,0x03,0x21,
0x40,0x3F,0x00,0x00,0x03,0x1C,0x08,0x00,

0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x00,
0x07,0x07,0x07,0x07,0x07,0x07,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00



	};


   void	Clean_Lcd(void)
{
		unsigned char	i,*p;
		p=tabe;
		for(i=0;i<8;i++)
		{
		Left_Bytes_DaSend( i, 0, p,32);
		Left_Bytes_DaSend( i, 32, p,32);
		Right_Bytes_DaSend( i, 0, p,32);
		Right_Bytes_DaSend( i, 32, p,32);
		}
}
void write_Code(unsigned char *pon,unsigned char hang,unsigned char lei)	  //pon字表的位置列数1=<lei<=8,行数1=<hang<=4，
{ 
   unsigned char *str; str=pon+32;	
   switch(hang){case 1: hang=0;break;
			    case 2: hang=2;break;
				case 3: hang=4;break;
			    case 4: hang=6;break;
				}
   switch(lei){	case 1: lei=0;break;
			    case 2: lei=16;break;
				case 3: lei=32;break;
			    case 4: lei=48;break;
				case 5: lei=64;break;
			    case 6: lei=80;break;
				case 7: lei=96;break;
			    case 8: lei=112;break;
			

              }
   if(lei<64){      Left_Bytes_DaSend( hang, lei,str,16);
                    str=str+16;hang++;
					Left_Bytes_DaSend( hang, lei,str,16);
			 }
   else{lei=lei-64;	        Right_Bytes_DaSend( hang, lei,str,16);
                    str=str+16;hang++;
					Right_Bytes_DaSend( hang, lei,str,16);

       }


}

void Main()
	{ //unsigned char *p;			
		IntRst();               //软件复位
        Lcd_On();                //开显示                   
		Clean_Lcd();				//清屏		
		 write_Code(tabe+32,1,4); 
		 while(1);




	/*	 p=tabe;
		p=p+64;	Left_Bytes_DaSend( 0, 0, p,16);
		p=p+16;	Left_Bytes_DaSend( 1, 0, p,16);
		p=tabe;p=p+64;
		Right_Bytes_DaSend( 0, 0, p,16);
		p=p+16;	
		Right_Bytes_DaSend( 1, 0, p,16); 
	*/	
	

	}