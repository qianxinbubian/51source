/*-----------------------------------------------
  ���ƣ�IICЭ�� PCF8591ADDAת��
  ��˾���Ϻ�������ӿƼ����޹�˾
  ��д��ʦ��
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ��˳���ͨ��IICЭ���DAADоƬ��������ȡ��λ���ĵ�ѹ�������ģ��������LED���Ƚ���ָʾ
------------------------------------------------*/
 #include<reg52.h>    //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
 #include <intrins.h> //����NOP��ָ���_nop_();

 #define AddWr 0x90   //д���ݵ�ַ 
 #define AddRd 0x91   //�����ݵ�ַ

 
 sbit Sda=P1^2;       //�����������Ӷ˿�
 sbit Scl=P1^1;

 bit ADFlag;          //����AD������־λ
unsigned char code Datatab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//7����������ܶ����
data unsigned char  Display[8];//������ʱ����������ֵ
/*------------------------------------------------
                    ��ʱ����
------------------------------------------------*/
 void mDelay(unsigned char j)
 {
  unsigned int i;
  for(;j>0;j--)
     {
	  for(i=0;i<125;i++)
	     {;}
	  }
  }
/*------------------------------------------------
                    ��ʼ����ʱ��1
------------------------------------------------*/
void Init_Timer1(void)
{
 TMOD |= 0x10;			     
 TH1=0xff;	              	/* Init value */
 TL1=0x00;
 //PT1=1;                   /* ���ȼ�    */
 EA=1;                      /* interupt enable */
 ET1=1;                     /* enable timer1 interrupt */
 TR1=1;  
}
/*------------------------------------------------
                    ����IIC����
------------------------------------------------*/
 - void Start(void)
  {
   Sda=1;
   _nop_();
   Scl=1;
   _nop_();
   Sda=0;
   _nop_();
   Scl=0;
  }


/*------------------------------------------------
                    ֹͣIIC����
------------------------------------------------*/
  void Stop(void)
  {
   Sda=0;
   _nop_();
   Scl=1;
   _nop_();
   Sda=1;
   _nop_();
   Scl=0;
   }


/*------------------------------------------------
                   Ӧ��IIC����
------------------------------------------------*/
   void Ack(void)
   {
    Sda=0;
	_nop_();
	Scl=1;
	_nop_();
	Scl=0;
	_nop_();
	}


/*------------------------------------------------
                 ��Ӧ��IIC����
------------------------------------------------*/
	void NoAck(void)
	{
	 Sda=1;
	 _nop_();
	 Scl=1;
	 _nop_();
	 Scl=0;
	 _nop_();
	 }


/*------------------------------------------------
              ����һ���ֽ�
------------------------------------------------*/
	 void Send(unsigned char Data)
	 { 
	  unsigned char BitCounter=8;
	  unsigned char temp;

	  do
	    {
		 temp=Data;
		 Scl=0;
		 _nop_();
		 if((temp&0x80)==0x80)
		    Sda=1;
		 else
		    Sda=0;

			Scl=1;
			temp=Data<<1;
			Data=temp;
			BitCounter--;
		  }
	  while(BitCounter);
	      Scl=0;
	  }


/*------------------------------------------------
               ����һ���ֽڲ�����
------------------------------------------------*/
	  unsigned char Read(void)
	  {
	   unsigned char temp=0;
	   unsigned char temp1=0;
	   unsigned char BitCounter=8;

	   Sda=1;
	   do
	     {
		  Scl=0;
          _nop_();
		  Scl=1;
		  _nop_();
		  if(Sda)
		     temp=temp|0x01;
		  else
		     temp=temp&0xfe;

		  if(BitCounter-1)
		     {
			  temp1=temp<<1;
			  temp=temp1;
			  }
			  BitCounter--;
			 }
		while(BitCounter);
		return(temp);
	  }
 
/*------------------------------------------------
                    д��DA��ģת��ֵ
------------------------------------------------*/
	  void DAC(unsigned char Data)
	  {
		   Start();
		   Send(AddWr); //д��оƬ��ַ
		   Ack();
		   Send(0x40);  //д�����λ��ʹ��DAC���
		   Ack();
		   Send(Data);  //д����
		   Ack();
		   Stop();
		 
	   }

/*------------------------------------------------
                 ��ȡADģ��ת����ֵ���з���ֵ
------------------------------------------------*/
	  unsigned char ReadADC(unsigned char Chl)
	  {
       unsigned char Data;
		   Start();        //д��оƬ��ַ
		   Send(AddWr);
		   Ack();
 		   Send(0x40|Chl);//д��ѡ���ͨ����������ֻ�õ������룬��ֲ�����Ҫ�������
		                  //Chl��ֵ�ֱ�Ϊ0��1��2��3���ֱ����1-4ͨ��
		   Ack();
		   Start();
		   Send(AddRd);    //�����ַ
		   Ack();
		   Data=Read();    //������
		   Scl=0;
		   NoAck();
		   Stop();
		   return Data;   //����ֵ
	  }

/*------------------------------------------------
                   ������
------------------------------------------------*/
	void main()
	{
	 unsigned char num;                   //DA��ģ�������
    unsigned char ADtemp;                //�����м����
     Init_Timer1();

	 while(1)
	      { 
           DAC(num);       //DA�����������LEDģ���ѹ�仯
		   num++;          //�ۼӣ���256�������Ϊ0������ѭ������ʾ��LED�������𽥱仯
		   mDelay(20);     //��ʱ�������������仯
		   if(ADFlag)      //��ʱ�ɼ�����ģ����
           {
   		ADFlag=0;

   		ADtemp=ReadADC(0);
		Display[0]=Datatab[(ReadADC(0))/50]|0x80;//����0ͨ����ѹ��ʾ
  		Display[1]=Datatab[((ReadADC(0))%50)/10];
  	
        ADtemp=ReadADC(1);
		Display[2]=Datatab[((ReadADC(1))/50)]|0x80;//����1ͨ����ѹ��ʾ ��ͨ����ʱ���Σ������������
  		Display[3]=Datatab[((ReadADC(1))%50)/10];
  		
        ADtemp=ReadADC(2);
		Display[4]=Datatab[((ReadADC(2))/50)]|0x80;//����1ͨ����ѹ��ʾ ��ͨ����ʱ���Σ������������
  		Display[5]=Datatab[((ReadADC(2))%50)/10];
  		
        ADtemp=ReadADC(3);
		Display[6]=Datatab[((ReadADC(3))/50)]|0x80;//����1ͨ����ѹ��ʾ ��ͨ����ʱ���Σ������������
  		Display[7]=Datatab[((ReadADC(3))%50)/10];
  		

		   }
	   }
	}

/*------------------------------------------------
                   ��ʱ���жϳ���
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 using 1//��ʱ��1ִ������ܶ�̬ɨ��
{

 static unsigned int count,j;
 TH1=0xfb;		//���¸�ֵ
 TL1=0x00;
 j++;
 if(j==300) 
    {j=0;ADFlag=1;} //��ʱ��λAD������־λ
	 
    P0=Display[count];//���ڶ�̬ɨ�������
    P2=count;
    count++;
   if(count==8)     //��ʾɨ��8�������
      count=0;

 
}
