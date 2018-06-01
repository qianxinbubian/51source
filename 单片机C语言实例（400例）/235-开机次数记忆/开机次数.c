   
/*-----------------------------------------------
  ���ƣ�IICЭ�� EEPROM24c02
  ��˾���Ϻ�������ӿƼ����޹�˾
  ��д��ʦ��
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ�  ÿ�ο�������������1 ���洢��EEPROM����������ֱ�۵Ŀ���������ʹ�ô���
          ��P1�� LED��Ϊ��ʾ����������256�ǽ����������λģ�⿪�� ����ֱ��ͨ�����ؿ���
------------------------------------------------*/
 #include<reg52.h>    //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
 #include <intrins.h> //����NOP��ָ���_nop_();

 #define AddWr 0xae   //д���ݵ�ַ����Ҫ�ο�24c02оƬ�ĵ�
 #define AddRd 0xaf   //�����ݵ�ַ

 
 sbit Sda=P1^2;       //�����������Ӷ˿�
 sbit Scl=P1^1;
 sbit WP=P1^0;        //д���������ﲻʹ��

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
                    ����IIC����
------------------------------------------------*/
  void Start(void)
  {
   Sda=1;
   _nop_();_nop_();
   Scl=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Sda=0;
   _nop_();_nop_();_nop_();_nop_();_nop_();
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
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Sda=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Scl=0;
   }


/*------------------------------------------------
                   Ӧ��IIC����
------------------------------------------------*/
   void Ack(void)
   {
    Sda=0;
	_nop_();_nop_();_nop_();
	Scl=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	Scl=0;
	_nop_();_nop_();
	}


/*------------------------------------------------
                 ��Ӧ��IIC����
------------------------------------------------*/
	void NoAck(void)
	{
	 Sda=1;
	 _nop_();_nop_();_nop_();
	 Scl=1;
	 _nop_();_nop_();_nop_();_nop_();_nop_();
	 Scl=0;
	 _nop_();_nop_();
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
		 _nop_();_nop_();_nop_();_nop_();_nop_();
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
          _nop_();_nop_();_nop_();_nop_();_nop_();
		  Scl=1;
		  _nop_();_nop_();_nop_();_nop_();_nop_();
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
                    д������
------------------------------------------------*/
	  void WrToROM(unsigned char Data[],unsigned char Address,unsigned char Num)
	  {
	   unsigned char i;
	   unsigned char *PData;
	   PData=Data;
	   for(i=0;i<Num;i++)
	      {
		   Start();
		   Send(AddWr);    //д��оƬ��ַ
		   Ack();
		   Send(Address+i);//д��洢��ַ
		   Ack();
		   Send(*(PData+i));//д����
		   Ack();
		   Stop();
		   mDelay(20);
		   }
	   }

/*------------------------------------------------
                 ��������
------------------------------------------------*/
	  void RdFromROM(unsigned char Data[],unsigned char Address,unsigned char Num)
	  {
	   unsigned char i;
	   unsigned char *PData;
	   PData=Data;
       for(i=0;i<Num;i++)
	      {
		   Start();        //д��оƬ��ַ
		   Send(AddWr);
		   Ack();
 		   Send(Address+i);//д��洢��ַ
		   Ack();
		   Start();
		   Send(AddRd);    //�����ַ
		   Ack();
		   *(PData+i)=Read();//������
		   Scl=0;
		   NoAck();
		   Stop();
		   }
	  }
//���ϳ�����EEPROM24C02���򲿷֣�IICЭ��Scl=0;
/*------------------------------------------------
                   ������
------------------------------------------------*/

	void main()
	{
	 unsigned char Number[1];
	 unsigned char i;
	 WP=0;//д�����ص�
	 
	 RdFromROM(Number,0,1);//���ô洢���� �ӵ�ַ0��ʼдһ������
	 Number[0]++;
	 WrToROM(Number,0,1);//д��24c02
     P1 = ~ Number[0];//��P1������ʾ ������ʾ LED������1
	 
	 
	 while(1)
	      { 
		  }

}
//�û������Լ��ѳ�����ֲ���������ʾ����Һ����ʾ ������ֱ�۵Ŀ������� ��Ҳ���Զ���int���� ���ڼ���������� 

