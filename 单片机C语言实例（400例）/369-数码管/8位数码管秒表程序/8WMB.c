

#include <reg51.h>
#include <intrins.h>

unsigned char data dis_digit;
unsigned char key_s, key_v;

unsigned char code dis_code[11]={0xc0,0xf9,0xa4,0xb0,	// 0, 1, 2, 3
				0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off 
unsigned char dis_buf[8];		// 显示缓冲区
unsigned char sec_bcd[8]; 		// 秒计数值, BCD码
unsigned char dis_index;		// 
unsigned char key_times;		// K1 按下次数				//

void clr_time();	
void update_disbuf();
bit	scan_key();
void proc_key();
void delayms(unsigned char ms);

sbit	K1 = P3^2;


void main(void)
{
	P0 = 0xff;
	P2 = 0xff;
	TMOD = 0x11;		// 定时器0, 1工作模式1, 16位定时方式
	TH1 = 0xdc;
	TL1 = 0;

	TH0 = 0xFC;
	TL0 = 0x17;
	
	clr_time();			// 
			                   
	dis_digit = 0x7f;		// 初始显示P20口数码管
	dis_index = 0;			// 
	
	key_times = 0;
	key_v = 0x01;
	
	IE = 0x8a;				// 使能timer0, timer1中断
	
	TR0 = 1;
	TR1 = 0;
	while(1)
	{
		if(scan_key())
		{
			delayms(10);
			if(scan_key())
			{
				key_v = key_s;
				proc_key();
			}
		}
		
	}
}

void clr_time()
{
	sec_bcd[0] = 0x0;
	sec_bcd[1] = 0x0;
	sec_bcd[2] = 0x0;
	sec_bcd[3] = 0x0;
	sec_bcd[4] = 0x0;
	sec_bcd[5] = 0x0;
	sec_bcd[6] = 0x0;
	sec_bcd[7] = 0x0;   
	
	update_disbuf();
	
}

bit scan_key()
{
	key_s = 0x00;
	key_s |= K1;
	return(key_s ^ key_v);	
}

void proc_key()
{
	if((key_v & 0x01) == 0)
	{
		key_times++;
		if(key_times == 1)
		{
			TR1 = 1;
		}
		else if(key_times == 2)
		{	
			TR1 = 0;
		}
		else
		{
			clr_time();
			key_times = 0;
		}
		
	}
}

void timer0() interrupt 1
// 定时器0中断服务程序, 用于数码管的动态扫描
// dis_index --- 显示索引, 用于标识当前显示的数码管和缓冲区的偏移量
// dis_digit --- 位选通值, 传送到P2口用于选通当前数码管的数值, 如等于0xfe时,
//				选通P2.0口数码管
// dis_buf   --- 显于缓冲区基地址	
{
	TH0 = 0xFC;
	TL0 = 0x17;
	
	P2 = 0xff;							// 先关闭所有数码管
	P0 = dis_buf[dis_index];			// 显示代码传送到P0口
	P2 = dis_digit;						// 

	dis_digit = _cror_(dis_digit,1);	// 位选通值右移(P30<-P37), 下次中断时选通下一位数码管
	dis_index++;						// 
					
	dis_index &= 0x07;			// 8个数码管全部扫描完一遍之后，再回到第一个开始下一次扫描
}

void timer1() interrupt 3
//
{	
	unsigned char i;
	TH1 |= 0xdc;
	for(i = 0; i < 8; i++)
	{
		sec_bcd[i]++;			// 低位加1
		if(sec_bcd[i] < 10)		// 如果低位满10则向高位进1
			break;			// 低位未满10
		sec_bcd[i] = 0;			// 低位满10清0
	}
	update_disbuf();			// 更新显示缓冲区
}

void update_disbuf()
// 更新显示缓冲区
{
	dis_buf[0] = dis_code[sec_bcd[0]];
	dis_buf[1] = dis_code[sec_bcd[1]];
	dis_buf[2] = dis_code[sec_bcd[2]] & 0x7f;	// 加上小数点
	dis_buf[3] = dis_code[sec_bcd[3]];
	dis_buf[4] = dis_code[sec_bcd[4]];
	dis_buf[5] = dis_code[sec_bcd[5]];
	dis_buf[6] = dis_code[sec_bcd[6]];
	dis_buf[7] = dis_code[sec_bcd[7]];
}

void delayms(unsigned char ms)	
// 延时子程序
{						
	unsigned char i;
	while(ms--)
	{
		for(i = 0; i < 120; i++);
	}
}

