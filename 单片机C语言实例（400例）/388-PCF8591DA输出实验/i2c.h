
#ifndef __I2C_H__
#define __I2C_H__  
              
#include <reg52.h>          //头文件的包含
#include <intrins.h>

#define  _Nop()  _nop_()        //定义空指令

/*------------------------------------------------
                    启动总线
------------------------------------------------*/
void Start_I2c();
/*------------------------------------------------
                    结束总线
------------------------------------------------*/
void Stop_I2c();
/*----------------------------------------------------------------
                 字节数据传送函数               
函数原型: void  SendByte(unsigned char c);
功能:  将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
     此状态位进行操作.(不应答或非应答都使ack=0 假)     
     发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
------------------------------------------------------------------*/
void  SendByte(unsigned char c);
/*----------------------------------------------------------------
                 字节数据传送函数               
函数原型: unsigned char  RcvByte();
功能:  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
     发完后请用应答函数。  
------------------------------------------------------------------*/	
unsigned char  RcvByte();
/*----------------------------------------------------------------
                     应答子函数
原型:  void Ack_I2c(void);
----------------------------------------------------------------*/
void Ack_I2c(void);
/*----------------------------------------------------------------
                     非应答子函数
原型:  void NoAck_I2c(void);
----------------------------------------------------------------*/
void NoAck_I2c(void);
/*----------------------------------------------------------------
                    向无子地址器件发送字节数据函数               
函数原型: bit  ISendByte(unsigned char sla,ucahr c);  
功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit ISendByte(unsigned char sla,unsigned char c);

/*----------------------------------------------------------------
                    向有子地址器件发送多字节数据函数               
函数原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

/*----------------------------------------------------------------
                    向无子地址器件读字节数据函数               
函数原型: bit  IRcvByte(unsigned char sla,ucahr *c);  
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit IRcvByte(unsigned char sla,unsigned char *c);
/*----------------------------------------------------------------
                    向有子地址器件读取多字节数据函数               
函数原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);


#endif
