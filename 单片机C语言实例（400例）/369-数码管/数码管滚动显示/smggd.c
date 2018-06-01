
#include <reg52.h> //51芯片管脚定义头文件 
#include <intrins.h>//内部包含延时函数 _nop_(); 

#define uchar unsigned char 
#define uint unsigned int 

uchar display[ ]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

 0xBF,0xBF,0x89,0x86,0xC7,0xC7,0xC0,0xBF,0xBF, 

// --H E L L O -


0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; 

uchar shift,j=0; 

/********************************************************

 ms延时函数 

********************************************************/ 

void delayms(uint ms) 

{

 uchar t; 

while(ms--)


 { 
for(t = 0; t < 120; t++); 
} 
} 

/******************************************************* 
显示函数 
*******************************************************/ 

void disp(void) 

{ 
uchar i; 
shift=0x7f; 

for(i=8; i>0; i--)

 { 	P2=0xff ; 
P0=display[i+j]; 
P2=shift; 
shift=(shift>>1)|0x80; 
delayms(1);


 } 

} 

/******************************************************* 
主函数 
*******************************************************/ 

void main() 
{ 
uchar k;

 P0=0xFF; 
P2=0xFF; 


while(1)


 { 
for(j=0;j<17;j++) 
{ 


for(k=0;k<125;k++) 
disp(); 
} 
} 
} 


