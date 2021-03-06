/*

*/
/*-----------------------------------------------
  名称：花样灯
  公司：上海浩豚电子科技有限公司
  网站：www.doflye.cn
  编写：师访
  日期：2009.5
  修改：无
  内容：花样灯一般有两种方法实现：
        1、程序循环执行
        2、查表法
        这里我们两种都使用一下
------------------------------------------------*/
#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义


#include <reg52.h>
/******************************************************************/
/*                   花样表格                                     */
/******************************************************************/
unsigned char code seg[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe,0xff,0xff,0x00,0,0x55,0x55,0xaa,0xaa};//rom允许情况可以无限添加
/******************************************************************/
/*                    延时函数                                    */
/******************************************************************/
void delay(unsigned int cnt)//简易延时
{
 while(--cnt);
}
/******************************************************************/
/*                    主函数                                      */
/******************************************************************/
main()
{
 unsigned char i;
 while(1)
      {
	  P1=0xFE;                   //第一个LED亮
      for(i=0;i<8;i++)
	     {
		 delay(10000);
	     P1 <<=1;
		 }

      P1=0x7F;                   //第七个LED亮
      for(i=0;i<8;i++)
	     {
		 delay(10000);
	     P1 >>=1;
		 }

      P1=0xFE;                  //第一个LED亮
      for(i=0;i<8;i++)
	     {
		 delay(10000);
	     P1 <<=1;
		 P1 |=0x01;
		 }

	  P1=0x7F;                  //第七个LED亮
      for(i=0;i<8;i++)
	     {
		 delay(10000);
	     P1 >>=1;
		 P1 |=0x80;
		 }
         /*下面通过查表方法获得花样参数*/
         for(i=0;i<16;i++)//查表可以简单的显示各种花样 实用性更强
	     {
		 delay(20000);
         P1=seg[i];
		 }
      }

}
