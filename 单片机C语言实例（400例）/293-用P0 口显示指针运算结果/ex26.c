//实例26：用P0 口显示指针运算结果
#include<reg51.h>
void main(void)
{  
  unsigned char *p1,*p2;  //定义无符号字符型指针变量p1,p2
  unsigned char i,j;     //定义无符号字符型数据
  i=25;     //给i赋初值25
  j=15;
  p1=&i;    //使指针变量指向i ，对指针初始化
  p2=&j;    //使指针变量指向j ，对指针初始化
  P0=*p1+*p2;   //*p1+*p2相当于i+j,所以P0=25+15=40=0x28
        //则P0=0010 1000B，结果P0.3、P0.5引脚LED熄灭，其余点亮
  while(1)
    ;           //无限循环，防止程序“跑飞”

}