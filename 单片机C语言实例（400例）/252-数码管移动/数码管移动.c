/*-----------------------------------------------
  名称：数码管移动
  公司：上海浩豚电子科技有限公司
  网站：www.doflye.cn
  编写：师访
  日期：2009.5
  修改：无
  内容：从右往左移动，利用缓冲区即可
------------------------------------------------*/
#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

unsigned  char const  dofly[]={0,0,0,0,0,0,0,0,          //不显示
                               0x6e,0x38,0x71,0x5c,0x5e, //DOFLY 共阴值，显示时候取反
							   0,0,0,0,0,0,0,0,};//dofly 码值
code unsigned char table[]={0,1,2,3,4,5,6,7};    //8位动态扫描值
                                                 //显示位码表
/******************************************************************/
/*                    延时函数                                    */
/******************************************************************/
void delay(unsigned int cnt)//简单延时
{
 while(--cnt);
}
/******************************************************************/
/*                    主函数                                      */
/******************************************************************/
main()
{
 unsigned char i,j ;          //定义内部变量
 unsigned char a;
 a=13;                        //移动13次 数码管个数8 + 显示字符个数 5 = 13
 while(1)
      {
	  for(j=0;j<8;j++)        //移动字符时间
	     {
       for(i=0;i<8;i++)       //动态扫描8为数码管
		  {
            P0=dofly[i+a];    //取显示的码值
			P2=table[7-i];    //取显示的位置
			delay(300);       //扫描延时 大会闪烁 小会降低亮度并重影
		   }
		   P2=0xff;           //有效的消除重影
        }
        a--;
		if(a==0)a=13;         //重新循环
     }
 }
