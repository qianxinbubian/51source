#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
#define OP_WRITE 0xa0          // 器件地址以及写入操作 
#define OP_READ  0xa1           // 器件地址以及读取操作 
uchar code display[72]={ 
        0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F, 
        0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE,0xFF, 
        0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00, 
        0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF, 
        0xFC,0xF9,0xF3,0xE7,0xCF,0x9F,0x3F, 
        0x9F,0xCF,0xE7,0xF3,0xF9,0xFC,0xFF, 
        0xE7,0xDB,0xBD,0x7E,0xBD,0xDB,0xE7,0xFF, 
        0xE7,0xC3,0x81,0x00,0x81,0xC3,0xE7,0xFF, 
        0xAA,0x55,0x18,0xFF,0xF0,0x0F, 
        0x00,0xFF,0x00,0xFF }; 
sbit SDA = P3^5; 
sbit SCL = P3^4; 
void start(); 
void stop(); 
uchar shin(); 
bit shout(uchar write_data); 
void write_byte( uchar addr, uchar write_data); 
void fill_byte(uchar fill_size,uchar fill_data); 
void delayms(uint ms); 
uchar read_current(); 
uchar read_random(uchar random_addr); 
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();}; 
/**********************************************************/ 
main(void) 
{ 
   uchar i; 
   SDA = 1; 
   SCL = 1; 
   fill_byte(72,0xff);          // 将前72字节填充0xff 
   for(i = 0 ; i < 72; i++)    //写入显示代码到AT24C02 
    { 
      write_byte(i,display[i]); 
    } 
   for(i =0 ;i <72 ; i++)      //从AT24C02移出数据送到P0口显示 
   { 
      P0 = read_random(i); 
	  P1=~P0;
      delayms(350); 
   }  
} 
/**********************************************************/ 
void start() 
 //开始位 
{ 
   SDA = 1; 
   SCL = 1; 
   delayNOP(); 
   SDA = 0; 
   delayNOP(); 
   SCL = 0; 
} 
/**********************************************************/ 
void stop() 
 // 停止位 
{ 
   SDA = 0; 
   delayNOP(); 
   SCL = 1; 
   delayNOP(); 
   SDA = 1; 
} 
/**********************************************************/ 
uchar shin() 
// 从AT24C02移出数据到MCU 
{ 
   uchar i,read_data; 
   for(i = 0; i < 8; i++) 
   { 
    SCL = 1; 
    read_data <<= 1; 
    read_data |= SDA; 
    SCL = 0; 
   } 
    return(read_data); 
} 
/**********************************************************/ 
bit shout(uchar write_data)  
// 从MCU移出数据到AT24C02 
{ 
   uchar i; 
   bit ack_bit; 
   for(i = 0; i < 8; i++)   // 循环移入8个位 
   { 
     SDA = (bit)(write_data & 0x80); 
    _nop_(); 
    SCL = 1; 
    delayNOP(); 
    SCL = 0; 
    write_data <<= 1; 
   } 
   SDA = 1;                     // 读取应答 
   delayNOP(); 
   SCL = 1; 
   delayNOP(); 
   ack_bit = SDA; 
   SCL = 0; 
   return ack_bit;            // 返回AT24C02应答位 
} 
/**********************************************************/ 
void write_byte(uchar addr, uchar write_data) 
// 在指定地址addr处写入数据write_data 
{ 
   start(); 
   shout(OP_WRITE); 
   shout(addr); 
   shout(write_data); 
   stop(); 
   delayms(10);          // 写入周期 
} 
/**********************************************************/ 
void fill_byte(uchar fill_size,uchar fill_data) 
// 填充数据fill_data到EEPROM内fill_size字节 
{ 
   uchar i; 
   for(i = 0; i < fill_size; i++) 
   { 
     write_byte(i, fill_data); 
   } 
} 
/**********************************************************/ 
uchar read_current() 
// 在当前地址读取 
{ 
   uchar read_data; 
   start(); 
   shout(OP_READ); 
   read_data = shin(); 
   stop(); 
   return read_data; 
} 
/**********************************************************/ 
uchar read_random(uchar random_addr) 
// 在指定地址读取 
{ 
   start(); 
   shout(OP_WRITE); 
   shout(random_addr); 
   return(read_current()); 
} 
/**********************************************************/ 
void delayms(uint ms)  
// 延时子程序 
{ 
   uchar k; 
   while(ms--) 
   { 
      for(k = 0; k < 120; k++); 
   } 
}
