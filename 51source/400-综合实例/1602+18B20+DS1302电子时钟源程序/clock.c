#include <AT89x51.h>
#include <string.h>
#include<intrins.h>

#define LCM_RS P2_0 //��������
#define LCM_RW P2_1
#define LCM_E P2_2
#define LCM_Data P0
#define Busy 0x80 //���ڼ��LCM״̬���е�Busy��ʶ
#define uchar unsigned char

uchar id,timecount,dipsmodid;
bit lmcinit_or_not; //�Ƿ���Ҫ������־λ��1��Ϊ��Ҫ��0��Ϊ����Ҫ
bit flag,sflag; //flag��ʱ��ð����˸��־��sflag���¶ȸ�����ʾ��־
void Disp_line1(void); //��ʾ��Ļ��һ�� 
void Disp_line2(void); //��ʾ��Ļ�ڶ���
void id_case1_key();
void Disp_mod0(void);//��ʾģʽ0
void Disp_mod1(void);//��ʾģʽ


//*********** DS1302 ʱ����ʾ���岿��
sbit T_CLK=P1^0;
sbit T_IO =P1^1;
sbit T_RST=P1^2;
sbit ACC0=ACC^0;
sbit ACC7=ACC^7;
void Set(uchar,uchar);     //����ѡ�������Ӧ��Ŀ
void   RTInputByte(uchar);       /* ���� 1Byte */
uchar  RTOutputByte(void);       /* ��� 1Byte */
void   W1302(uchar, uchar);   // ��DS1302д��һ���ֽ�
uchar  R1302(uchar);          // ��DS1302����һ���ֽ�
void   Set1302(unsigned char * ); //  ����ʱ�� 
bit sec,min,hour,year,mon,day,weekk; //��˸��־λ
//��ʼ��������Ϊ��04��12��2������4 0��0��0��
unsigned char inittime[7]={0x00,0x00,0x00,0x02,0x12,0x04,0x04};
//                          ��  ���� Сʱ  ��   ��   ��  ����

//***** 18B20�¶���ʾ���岿
sbit DQ=P3^3; //18B20 ��P07��
typedef unsigned char byte;
typedef unsigned int  word;
void Read_Temperature(char,char);
void mychar(void);
byte ow_reset(void);
byte read_byte(void);
void write_byte(char val);
void adjust_res(char res); //res �ֱ���� 0x1f, 0x3f, 0x5f �¶ȶ����ֱ��ʷֱ��Ӧ
                           //              0.5, 0.25, 0.125   

//*******�¶ȿ��Ʋ���*********
bit tl_flash_flag;       //������˸��־
bit th_flash_flag;       //������˸��־
bit tl_flag;               //����Խ�ޱ�־  
bit th_flag;               //����Խ�ޱ�־  
bit tl_sign,th_sign;		//�¶����±�־ 0�����ϣ�1������
char tempid;
void Disp_Temp(void);    //��ʾ�������¶�
void Set_TH_Temp(void);  //�����¶�����
void Set_TL_Temp(void);  //�����¶�����
signed char tl=20,th=45;
void Compare_Temp(signed char l,signed char h);     //�Ƚ��¶�
void Tl_Al(void);         //����Խ�ޱ���
void Th_Al(void);         //����Խ�ޱ���



//******* 1602LCD���� **********************************************************
void WriteDataLCM(unsigned char WDLCM);
void WriteCommandLCM(unsigned char WCLCM,BuysC);
unsigned char ReadStatusLCM(void);
void LCMInit(void);
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData);
void Delay5Ms(void);
void Delay400Ms(void);
unsigned char code week[]={"Week."};


void main(void)
{ 
  Delay400Ms(); //�����ȴ�����LCM���빤��״̬
  LCMInit(); //LCM��ʼ��
  Delay5Ms(); //��ʱƬ��(�ɲ�Ҫ)
  mychar();
  TMOD=0x01;
  TH0=(65535-50000)/256;
  TL0=(65535-50000)%256;
  EA=1;
  TR0=1;
  ET0=1;  
  W1302(0x90,0xa5);//�򿪳�������  һ�������ܴ���һ��2K���� 
  W1302(0x8e,0x80);//д��������ֹд���� 
  adjust_res(0x1f); //����18B20�ķֱ��� 0x1f:0.5; 0x3f:0.25; 0x5f:0.125
  while(1)
    {              
	 if (P1_7==0)
      {dipsmodid++;
	   lmcinit_or_not=1;
       if(dipsmodid>2) dipsmodid=0;
       while(P1_7==0);
      }
     switch(dipsmodid)
      {
       case 0:
        Disp_mod0();
        break;
       case 1:
        Disp_mod1();
        break;
  	  }
    }
}
/*//ѡ����ʾģ?
void Dispmod_id_case_key()
{
 if (P0_3==0)
  {dipsmodid++;
   if(dipsmodid>1) dipsmodid=0;
  }
}*/
//��ʾģʽ0
void Disp_mod0(void)
{
 if(lmcinit_or_not==1)
  {LCMInit();
   lmcinit_or_not=0;
  }
 if ((P1_4|P1_5)==0) //��ʼ��
  {
   Delay5Ms();
   if ((P1_4|P1_5)==0) Set1302(inittime);
  }
 //mychar(); //��ʾ�Զ����ַ�
 if (P1_6==0) // ���ú�ѡ����Ŀ��
      {
       Delay5Ms();
       if(P1_6==0){id++;if(id>7) id=0;}
       while(P1_6==0);    
      }
     switch(id)
      {
       case 0:
         sec=0;
         Disp_line1(); Disp_line2();
         break;
       case 1://��
         year=1;
         Disp_line1(); Disp_line2();                  
         id_case1_key();
         break;
       case 2://��
         year=0;mon=1;
         Disp_line1(); Disp_line2();                 
         id_case1_key();
         break;
       case 3://��
         mon=0;day=1;
         Disp_line1(); Disp_line2();
         id_case1_key();
         break;             
       case 4://����
         day=0;weekk=1;
         Disp_line1(); Disp_line2();         
         id_case1_key();
         break;
       case 5://Сʱ
         weekk=0;hour=1;
         Disp_line1(); Disp_line2(); 
         id_case1_key();      
         break;       
       case 6://����
         hour=0;min=1;
         Disp_line1(); Disp_line2(); 
         id_case1_key();
         break;
       case 7://��
         min=0;sec=1;
         Disp_line1(); Disp_line2();             
         id_case1_key();
         break;              
      }
}
//��ʾģʽ1
void Disp_mod1(void)
{
 if(lmcinit_or_not==1)
  {LCMInit();
   lmcinit_or_not=0;
  }
 Read_Temperature(1,0);
 DisplayOneChar(0,1,'T');
 DisplayOneChar(1,1,'L');
 DisplayOneChar(2,1,':');
 DisplayOneChar(7,1,'T');
 DisplayOneChar(8,1,'H');
 DisplayOneChar(9,1,':');
 if(P1_6==0)
  { 
    if(P1_6==0)
     {
      tempid++;
      if(tempid>2) tempid=0;
      while(P1_6==0);
     }
  }
 switch(tempid)
  {
   case 0:
    tl_flash_flag=0;
    th_flash_flag=0;
    Disp_Temp();
   break;
   case 1:
    tl_flash_flag=1;
    th_flash_flag=0;
    Disp_Temp();
	Set_TL_Temp(); 
   break;
   case 2:
    tl_flash_flag=0;
    th_flash_flag=1;
    Disp_Temp();
    Set_TH_Temp();
   break;
  }
 Compare_Temp(tl,th);
 Tl_Al();
 Th_Al();
}

//��ʾ�������¶�
void Disp_Temp(void)
{
 
 unsigned char tl_gw,tl_sw,tl_bw;		//�����¶ȵĸ�λ��ʮλ����λ
 unsigned char th_gw,th_sw,th_bw;		//�����¶ȵĸ�λ��ʮλ����λ

 tl_sign=tl>>7;
 if(tl_sign)
  {
   tl_bw=(~(tl-1))/100;		//�����λ
   tl_sw=((~(tl-1))-tl_bw*100)/10;		//����ʮλ
   tl_gw=(~(tl-1))-tl_bw*100-tl_sw*10;		//�����λ
  }
 else
  {
   tl_bw=(tl)/100;          
   tl_sw=(tl-tl_bw*100)/10;
   tl_gw=tl-tl_bw*100-tl_sw*10;
  }

 if(tl_flash_flag==1)
  {
   if(flag==1)
    {if(tl_sign)
	  {
	   DisplayOneChar(3,1,0x2d);
       DisplayOneChar(4,1,tl_sw+0x30); 
       DisplayOneChar(5,1,tl_gw+0x30);
	  }
	 else
	  {
	   DisplayOneChar(3,1,tl_bw+0x30);
       DisplayOneChar(4,1,tl_sw+0x30); //��ʾ�����¶�
       DisplayOneChar(5,1,tl_gw+0x30);
      } 
    }
   else
    {DisplayOneChar(3,1,0x20);
     DisplayOneChar(4,1,0x20); //��ʾ�����¶�
     DisplayOneChar(5,1,0x20);    
    }
  }
 else
  {
   DisplayOneChar(3,1,tl_bw+0x30);
   DisplayOneChar(4,1,tl_sw+0x30); //��ʾ�����¶�
   DisplayOneChar(5,1,tl_gw+0x30);
  }

  
  th_sign=th>>7;
  if(th_sign)
   {
    th_bw=(~(th-1))/100;		//�����λ
    th_sw=((~(th-1))-th_bw*100)/10;		//����ʮλ
    th_gw=(~(th-1))-th_bw*100-th_sw*10;		//�����λ
   }
  else
   {
    th_bw=(th)/100;          
    th_sw=(th-th_bw*100)/10;
    th_gw=th-th_bw*100-th_sw*10;
   }


 if(th_flash_flag==1)
  {
   if(flag==1)
    {if(th_sign)
	  {
       DisplayOneChar(10,1,0x2d);
       DisplayOneChar(11,1,th_sw+0x30); //��ʾ�����¶�
       DisplayOneChar(12,1,th_gw+0x30);  
	  }
	 else
	  {
       DisplayOneChar(10,1,th_bw+0x30);
       DisplayOneChar(11,1,th_sw+0x30); //��ʾ�����¶�
       DisplayOneChar(12,1,th_gw+0x30);  
      }
    }
   else
    {
	 DisplayOneChar(10,1,0x20);
     DisplayOneChar(11,1,0x20); //��ʾ�����¶�
     DisplayOneChar(12,1,0x20);    
    }
  }
 else
  {
   DisplayOneChar(10,1,th_bw+0x30);
   DisplayOneChar(11,1,th_sw+0x30); //��ʾ�����¶�
   DisplayOneChar(12,1,th_gw+0x30);      
  }
}

//�����¶�����
void Set_TL_Temp(void)
{
 if(P1_5==0)
  {
   Delay5Ms();
   if(P1_5==0)
    {
	 tl++;
	 if(tl>125) tl=-55;
	 //if(tl<-55) tl=155;
	}
   while(P1_5==0);
  }
 if(P1_4==0)
  {
   Delay5Ms();
   if(P1_4==0)
    {
	 tl--;
	 if(tl<-55) tl=125;
	}
  }
}

//�����¶�����
void Set_TH_Temp(void)
{
 if(P1_5==0)
  {
   Delay5Ms();
   if(P1_5==0)
    {
	 th++;
	 if(th>125) th=-55;
	 //if(tl<-55) tl=155;
	}
   while(P1_5==0);
  }
 if(P1_4==0)
  {
   Delay5Ms();
   if(P1_4==0)
    {
	 th--;
	 if(th<-55) th=125;
	}
  }
}

void Compare_Temp(signed char l,signed char h)     //�Ƚ��¶�
{
  unsigned char tl_temp;
  unsigned char y,s;
  //unsigned int  x;
  //unsigned char ct[8];
  union{byte c[2]; int x;}temp;
  
  ow_reset();
  write_byte(0xCC); // Skip ROM
  write_byte(0x44); // ת���¶�
  ow_reset();
  write_byte(0xCC); //Skip ROM
  write_byte(0xbe); // ��ȡ�Ĵ���
  temp.c[1]=read_byte();//�����¶ȵ�8λ
  temp.c[0]=read_byte();//�����¶ȸ�8λ
 
  // �����¶��ж�
  sflag=0; //�¶����±�־ 0�����ϣ�1������
  if((temp.c[0]&0xf8)!=0x00)
   {
    sflag=1; //���±�־λ��1
    temp.c[1]=~temp.c[1]; //��8λȡ��
    temp.c[0]=~temp.c[0]; //��8λȡ��
    tl_temp=temp.c[1]+1;  //��8λ��1 
    temp.c[1]=tl_temp;    //��������´�������
    if(tl_temp>255) temp.c[0]++;  //�����8λ����255�����8λ��1
   }
  y=0;
  s=0;
  y=temp.c[0]<<4;  
  s=temp.c[1]>>4;
  y=s|y;
  //x=((temp.c[0]&0x07)*256+temp.c[1])*.625;
  if(y<l)
   
    tl_flag=1;
   
  else
   
    tl_flag=0;
   
  if(y>h)
   
    th_flag=1;
   
  else
   
    th_flag=0;
   
}

void Tl_Al(void)         //����Խ�ޱ�?
{
 if(tl_flag==1)
  {
   if(flag==1)
    {    
     DisplayOneChar(9,0,'A'); 
     DisplayOneChar(10,0,'L');        
    }
   else
    {
     DisplayOneChar(9,0,0x20); 
     DisplayOneChar(10,0,0x20);    
    }
  } 
 else
  {
   DisplayOneChar(9,0,0x20); 
   DisplayOneChar(10,0,0x20);         
  }
}

void Th_Al(void)         //����Խ�ޱ���
{
 if(th_flag==1)
  {
   if(flag==1)
    {    
     DisplayOneChar(13,0,'A'); 
     DisplayOneChar(14,0,'H');        
    }
   else
    {
     DisplayOneChar(13,0,0x20); 
     DisplayOneChar(14,0,0x20);    
    }
  } 
 else
  {
   DisplayOneChar(13,0,0x20); 
   DisplayOneChar(14,0,0x20);         
  }
}



//�ж���ڣ�ð����˸
void t0(void) interrupt 1 using 0
{
  TH0=(65535-50000)/256; //50ms��ʱ
  TL0=(65535-50000)%256;  
  timecount++;
  if(timecount>9)
   {
    timecount=0;    
    flag=~flag;             
   }  
}
//idΪ1ʱ����һ��ѡ��
void id_case1_key()
{
  if (P1_5==0) //����
      {
       Delay5Ms();
       if(P1_5==0) Set(id,0);  
       if(id!=7)  while(P1_5==0);    
      }
   if (P1_4==0)  //����
      {
       Delay5Ms();
       if(P1_4==0) Set(id,1);          
       if(id!=7) while(P1_4==0);    
      }
}

//����ѡ�������Ӧ��Ŀ��д��DS1302
void Set(uchar sel,uchar sel_1) 
{
  signed char address,item;
  signed char max,mini;
  if(sel==7)  {address=0x80; max=0;mini=0;}     //��
  if(sel==6)  {address=0x82; max=59;mini=0;}    //����
  if(sel==5)  {address=0x84; max=23;mini=0;}    //Сʱ
  if(sel==3)  {address=0x86; max=31;mini=1;}    //��
  if(sel==2)  {address=0x88; max=12;mini=1;}    //��
  if(sel==1)  {address=0x8c; max=99;mini=0;}    //��
  if(sel==4)  {address=0x8a; max=7; mini=1;}    //����
  
  item=R1302(address+1)/16*10+R1302(address+1)%16;
  if (sel_1==0) item++;  else item--;
  if(item>max) item=mini;   
  if(item<mini) item=max;
           
  W1302(0x8e,0x00);//����д����
  W1302(address,item/10*16+item%10);
  W1302(0x8e,0x80);//д��������ֹд����  
}

//��Ļ��ʾ��һ�� ʱ����¶�
void Disp_line1(void) 
{  
  Read_Temperature(10,0); //�¶���ʾ
  //ð����˸     
  if(flag==0)
    {DisplayOneChar(3,0,0x3a); DisplayOneChar(6,0,0x3a);}
  else
    {DisplayOneChar(3,0,0x20); DisplayOneChar(6,0,0x20);}

  if(sec==1)  //����˸��־λ
    {
     if(flag==1)
       {    
        DisplayOneChar(7,0,R1302(0x81)/16+0x30); //��ʾ��
        DisplayOneChar(8,0,R1302(0x81)%16+0x30);        
       }
     else
       {
        DisplayOneChar(7,0,0x20); //��ʾ��
        DisplayOneChar(8,0,0x20);    
       }
    } 
  else
    {
     DisplayOneChar(7,0,R1302(0x81)/16+0x30); //��ʾ��
     DisplayOneChar(8,0,R1302(0x81)%16+0x30);         
    }    
   
  if(min==1)  //������˸��־λ
    {
     if(flag==1)
       {    
        DisplayOneChar(4,0,R1302(0x83)/16+0x30); //��ʾ����
        DisplayOneChar(5,0,R1302(0x83)%16+0x30);        
       }
     else
       {
        DisplayOneChar(4,0,0x20); //��ʾ����
        DisplayOneChar(5,0,0x20);    
       }
    } 
  else
    {
     DisplayOneChar(4,0,R1302(0x83)/16+0x30); //��ʾ����
     DisplayOneChar(5,0,R1302(0x83)%16+0x30);         
    } 

  if(hour==1)  //Сʱ��˸��־λ
    {
     if(flag==1)
       {    
        DisplayOneChar(1,0,R1302(0x85)/16+0x30);//��ʾСʱ
        DisplayOneChar(2,0,R1302(0x85)%16+0x30); 
       }
     else
       {
        DisplayOneChar(1,0,0x20); //��ʾСʱ
        DisplayOneChar(2,0,0x20);    
       }
    } 
  else
   { 
    DisplayOneChar(1,0,R1302(0x85)/16+0x30);//��ʾСʱ
    DisplayOneChar(2,0,R1302(0x85)%16+0x30);
   }
}

// ��Ļ��ʾ�ڶ���  ���ں�����   
void Disp_line2(void)
{
  DisplayOneChar(3,1,0x2f); //��ʾ�̶��ַ�
  DisplayOneChar(6,1,0x2f);
  DisplayListChar(10,1,week);
    
  if(year==1)  //����˸��־λ
    {
     if(flag==1)
       {    
        DisplayOneChar(1,1,R1302(0x8d)/16+0x30);//��ʾ��
        DisplayOneChar(2,1,R1302(0x8d)%16+0x30);
       }
     else
       {
        DisplayOneChar(1,1,0x20); //��ʾ��
        DisplayOneChar(2,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(1,1,R1302(0x8d)/16+0x30);//��ʾ��
      DisplayOneChar(2,1,R1302(0x8d)%16+0x30);
    }
     
  if(mon==1)  //����˸��־λ
    {
     if(flag==1)
       {    
        DisplayOneChar(4,1,R1302(0x89)/16+0x30);//��ʾ�� 
        DisplayOneChar(5,1,R1302(0x89)%16+0x30);
       }
     else
       {
        DisplayOneChar(4,1,0x20); //��ʾ��
        DisplayOneChar(5,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(4,1,R1302(0x89)/16+0x30);//��ʾ�� 
      DisplayOneChar(5,1,R1302(0x89)%16+0x30);
    }   

  if(day==1)  //����˸��־λ
    {
     if(flag==1)
       {    
         DisplayOneChar(7,1,R1302(0x87)/16+0x30);//��ʾ��
         DisplayOneChar(8,1,R1302(0x87)%16+0x30);
       }
     else
       {
        DisplayOneChar(7,1,0x20); //��ʾ��
        DisplayOneChar(8,1,0x20);    
       }
    } 
  else
    { 
      DisplayOneChar(7,1,R1302(0x87)/16+0x30);//��ʾ��
      DisplayOneChar(8,1,R1302(0x87)%16+0x30);
    }
    
   if(weekk==1)  //������˸��־λ
    {
     if(flag==1)
       {    
         DisplayOneChar(15,1,R1302(0x8b)%16+0x30);//��ʾ����
       }
     else
       {
        DisplayOneChar(15,1,0x20); //��ʾ����          
       }
    } 
  else
    { 
      DisplayOneChar(15,1,R1302(0x8b)%16+0x30);//��ʾ����
    }     
}

//********* LCM1602�������� ***************
//д����
void WriteDataLCM(unsigned char WDLCM)
{
  ReadStatusLCM(); //���æ
  LCM_Data = WDLCM;
  LCM_RS = 1;
  LCM_RW = 0;
  LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
  LCM_E = 0; //��ʱ
  LCM_E = 1;
}
//дָ��
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
  if (BuysC) ReadStatusLCM(); //������Ҫ���æ
  LCM_Data = WCLCM;
  LCM_RS = 0;
  LCM_RW = 0; 
  LCM_E = 0;
  LCM_E = 0;
  LCM_E = 1; 
}
//��״̬
unsigned char ReadStatusLCM(void)
{
  LCM_Data = 0xFF; 
  LCM_RS = 0;
  LCM_RW = 1;
  LCM_E = 0;
  LCM_E = 0;
  LCM_E = 1;
  while (LCM_Data & Busy); //���æ�ź�
  return(LCM_Data);
}
//LCM��ʼ��
void LCMInit(void) 
{
  LCM_Data = 0;
  WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
  Delay5Ms(); 
  WriteCommandLCM(0x38,0);
  Delay5Ms(); 
  WriteCommandLCM(0x38,0);
  Delay5Ms(); 
  WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
  WriteCommandLCM(0x08,1); //�ر���ʾ
  WriteCommandLCM(0x01,1); //��ʾ����
  WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
  WriteCommandLCM(0x0C,1); // ��ʾ�����������
}
//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
  Y &= 0x1;
  X &= 0xF; //����X���ܴ���15��Y���ܴ���1
  if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
  X |= 0x80; //���ָ����
  WriteCommandLCM(X, 0); //���ﲻ���æ�źţ����͵�ַ��
  WriteDataLCM(DData);
}
//��ָ��λ����ʾһ���ַ�  ***ԭ���������ո�0x20�Ͳ���ʾ***
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
  unsigned char ListLength,j;
  ListLength = strlen(DData);
  Y &= 0x1;
  X &= 0xF; //����X���ܴ���15��Y���ܴ���1
      if (X <= 0xF) //X����ӦС��0xF
        { 
		    for(j=0;j<ListLength;j++)
		      {
               DisplayOneChar(X, Y, DData[j]); //��ʾ�����ַ�
               X++;
              }
        }
}
//5ms��ʱ
void Delay5Ms(void)
{
  unsigned int TempCyc = 5552;
  while(TempCyc--);
}
//400ms��ʱ
void Delay400Ms(void)
{
  unsigned char TempCycA = 5;
  unsigned int TempCycB;
  while(TempCycA--)
   {
    TempCycB=7269;
    while(TempCycB--);
   };
}

//********DS1302��д����***************
/******************************************************************** 
 �� �� ����RTInputByte()
 ��    �ܣ�ʵʱʱ��д��һ�ֽ�
 ˵    ������DS1302д��1Byte���� (�ڲ�����)
 ��ڲ�����d д������� 
 �� �� ֵ����  
 ***********************************************************************/
void RTInputByte(uchar d) 
{ 
    uchar i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        T_IO = ACC0;           /*�൱�ڻ���е� RRC */
        T_CLK = 1;
        T_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
/******************************************************************** 
 �� �� ����RTOutputByte()
 ��    �ܣ�ʵʱʱ�Ӷ�ȡһ�ֽ�
 ˵    ������DS1302��ȡ1Byte���� (�ڲ�����)
 ��ڲ�������  
 �� �� ֵ��ACC
 ��    �ƣ�zhaojunjie           ��    �ڣ�2002-03-19
 ��    �ģ�                     ��    �ڣ� 
***********************************************************************/
uchar RTOutputByte(void) 
{ 
    uchar i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         /*�൱�ڻ���е� RRC */
        ACC7 = T_IO;
        T_CLK = 1;
        T_CLK = 0;
    } 
    return(ACC); 
}
/******************************************************************** 
 �� �� ����W1302()
 ��    �ܣ���DS1302д������
 ˵    ������д��ַ����д����/���� (�ڲ�����)
 ��    �ã�RTInputByte() , RTOutputByte()
 ��ڲ�����ucAddr: DS1302��ַ, ucData: Ҫд������
 �� �� ֵ����
 ***********************************************************************/
void W1302(uchar ucAddr, uchar ucDa)
{
    T_RST = 0;
    T_CLK = 0;
    T_RST = 1;
    RTInputByte(ucAddr);       /* ��ַ������ */
    RTInputByte(ucDa);       /* д1Byte����*/
    T_CLK = 1;
    T_RST = 0;
}
/******************************************************************** 
 �� �� ����R1302()
 ��    �ܣ���ȡDS1302ĳ��ַ������
 ˵    ������д��ַ���������/���� (�ڲ�����)
 ��    �ã�RTInputByte() , RTOutputByte()
 ��ڲ�����ucAddr: DS1302��ַ
 �� �� ֵ��ucData :��ȡ������
***********************************************************************/
uchar R1302(uchar ucAddr)
{
    uchar ucData;
    T_RST = 0;
    T_CLK = 0;
    T_RST = 1;
    RTInputByte(ucAddr);             /* ��ַ������ */
    ucData = RTOutputByte();         /* ��1Byte���� */
    T_CLK = 1;
    T_RST = 0;
    return(ucData);
}

/******************************************************************** 
 �� �� ����Set1302()
 ��    �ܣ����ó�ʼʱ��
 ˵    ������д��ַ���������/����(�Ĵ������ֽڷ�ʽ)
 ��    �ã�W1302()
 ��ڲ�����pClock: ����ʱ�����ݵ�ַ ��ʽΪ: �� �� ʱ �� �� ���� ��
                               7Byte (BCD��)1B 1B 1B 1B 1B  1B  1B
 �� �� ֵ����
***********************************************************************/
void Set1302(uchar *pClock) 
{
    uchar i;
    uchar ucAddr = 0x80; 
    W1302(0x8e,0x00);           /* ��������,WP=0,д����?*/
    for(i =7; i>0; i--)
    { 
        W1302(ucAddr,*pClock);  /* �� �� ʱ �� �� ���� �� */ 
        pClock++;
        ucAddr +=2;
    }
    W1302(0x8e,0x80);           /* ��������,WP=1,д����?*/
}

//*********** 18B20���� **************************
//��ʱ
void delay(unsigned int num)
{
  while( --num ) ;
}

//��λ
byte ow_reset(void)
{
  byte presence;
  DQ = 0;        //��������
  delay(29);    // ���� 480us
  DQ = 1;       // �ͷ�����
  delay(3);     // �ȴ��ظ�
  presence = DQ; // ��ȡ�ź�
  delay(25);    // �ȴ������ź�
  return(presence); // ����   0������ 1��������
}     

//�� 1-wire �����϶�ȡһ���ֽ�
byte read_byte(void)
{
  byte i;
  byte value = 0;
  for (i=8;i>0;i--)
  {
    value>>=1;
    DQ = 0; 
    DQ = 1;
    delay(1);  
    if(DQ)value|=0x80;
    delay(6); 
  }
  return(value);
}

//�� 1-WIRE ������дһ���ֽ�
void write_byte(char val)
{
  byte i;
  for (i=8; i>0; i--) // һ��дһλ
  {
    DQ = 0; // 
    DQ = val&0x01;
    delay(5); // 
    DQ = 1;
    val=val/2;
  }
  delay(5);
}

//��ȡ����ʾ�¶�
void  Read_Temperature(char xx,char yy)
{
  unsigned char i,tl_temp;
  unsigned int  x;
  unsigned char ct[8];
  union{byte c[2]; int x;}temp;
  
  ow_reset();
  write_byte(0xCC); // Skip ROM
  write_byte(0x44); // ת���¶�
  ow_reset();
  write_byte(0xCC); //Skip ROM
  write_byte(0xbe); // ��ȡ�Ĵ���
  temp.c[1]=read_byte();//�����¶ȵ�8λ
  temp.c[0]=read_byte();//�����¶ȸ�8λ
 
  // �����¶��ж�
  sflag=0; //�¶����±�־ 0�����ϣ�1������
  if((temp.c[0]&0xf8)!=0x00)
   {
    sflag=1; //���±�־λ��1
    temp.c[1]=~temp.c[1]; //��8λȡ��
    temp.c[0]=~temp.c[0]; //��8λȡ��
    tl_temp=temp.c[1]+1;  //��8λ��1 
    temp.c[1]=tl_temp;    //��������´�������
    if(tl_temp>255) temp.c[0]++;  //�����8λ����255�����8λ��1
  }  

  x=((temp.c[0]&0x07)*256+temp.c[1])*.625;
  for(i=0;i<8;i++)
       {
         ct[i]=0;
       }
  i=0;
  while(x/10)
   {
    ct[i]=x%10;
    x=x/10;
    i++;
   }
  ct[i]=x;

  if(sflag==1) DisplayOneChar(xx-1,yy,0x2d);
  else         DisplayOneChar(xx-1,yy,0x20);
  DisplayOneChar(xx,  yy,ct[2]+0x30);//��ʾ�¶�ʮλ��
  DisplayOneChar(xx+1,yy,ct[1]+0x30);//��ʾ�¶ȸ�λ��
  DisplayOneChar(xx+2,yy,0x2e);//��ʾС����
  DisplayOneChar(xx+3,yy,ct[0]+0x30);//��ʾ�¶�С��λ
  DisplayOneChar(xx+4,yy,0x01);//��ʾ�Զ����ַ�
  DisplayOneChar(xx+5,yy,0x43);//��ʾ�ַ���C
}

//�¶ȷֱ��ʵ���
void adjust_res(char res) ///res �ֱ���� 0x1f, 0x3f, 0x5f �¶ȶ����ֱ��ʷֱ��Ӧ
                           //              0.5, 0.25, 0.125   
{
  ow_reset();        //��λ
  write_byte(0xcc);  //����Rom
  write_byte(0x4e);  //д�ݴ���
  write_byte(0x02);  //дTH 
  write_byte(0x01);  //дTL
  //write_byte(0x5f);  //д�ṹ�Ĵ���
  write_byte(res);
  ow_reset();       //��λ
  write_byte(0xcc); //����Rom 
  write_byte(0x48); //���ݴ�������д��EPRam��
} 

void mychar()//�Զ����ַ�
{
///////////////////////�Զ����ַ�
 WriteCommandLCM(0x48, 0); //��һ��
 WriteDataLCM(0x02);
 WriteCommandLCM(0x49, 0); //��2��
 WriteDataLCM(0x05);
 WriteCommandLCM(0x4a, 0); //��3
 WriteDataLCM(0x05);
 WriteCommandLCM(0x4b, 0); //��4
 WriteDataLCM(0x02);
 WriteCommandLCM(0x4c, 0); //��5
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4d, 0); //��6
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4e, 0); //��7
 WriteDataLCM(0x00);
 WriteCommandLCM(0x4f, 0); //��8
 WriteDataLCM(0x00);
}