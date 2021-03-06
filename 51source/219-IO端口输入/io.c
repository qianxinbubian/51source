/*-----------------------------------------------
  名称：IO口信号输入
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：通过读入P3端口的电平控制P1口，用LED显示
        该程序是学习如何读入IO电平信息
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{

P1 = 0xFF;        //P1口全部为高电平，对应的LED灯全灭掉，ff换算成二进制是 1111 1111

while (1)         //主循环
  {
  P1=P3;          //读入P3的值然后赋值给P1，如果P3.0=0，其他为1，则执行过该语句后P1.0=0，其他值为1，对应P1.0口的灯亮
                  //程序可以通过独立按键测试，在开发板上需要切换成独立按键模式
  }
}