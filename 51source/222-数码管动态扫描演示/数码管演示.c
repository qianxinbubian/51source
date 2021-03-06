/*-----------------------------------------------
  名称：数码管动态扫描
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：数码管使用动态扫描显示，所以可以同时看到数码管同时显示数字或者字符，扫描原理可以参考说明书
        或者视频教程。
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义


unsigned char const dofly[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值01234567
unsigned char code  seg[]={0,1,2,3,4,5,6,7};//分别对应相应的数码管点亮,即位码
/*------------------------------------------------
                    延时子程序
------------------------------------------------*/
void delay(unsigned int cnt)
{
 while(--cnt);
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{
 unsigned char i;

 while(1)
      {
 
       P0=dofly[i];//取显示数据，段码
	   P2=seg[i];  //取位码
	   delay(20000); //扫描间隙延时，时间太长会闪烁，太短会造成重影,这里使用长时间方便了解显示原理
	   i++;
	   if(8==i)    //检测8位扫描完全？
	     i=0;
      }
}