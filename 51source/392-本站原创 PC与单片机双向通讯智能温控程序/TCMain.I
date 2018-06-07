
#line 1 "TCMain.c" /0








 
  
#line 1 "C:\KEIL\C51\INC\ATMEL\AT89X051.H" /0





 
 
 
 
 


 
 sfr SP      = 0x81;
 sfr DPL     = 0x82;
 sfr DPH     = 0x83;
 sfr PCON    = 0x87;
 sfr TCON    = 0x88;
 sfr TMOD    = 0x89;
 sfr TL0     = 0x8A;
 sfr TL1     = 0x8B;
 sfr TH0     = 0x8C;
 sfr TH1     = 0x8D;
 sfr P1      = 0x90;
 sfr SCON    = 0x98;
 sfr SBUF    = 0x99;
 sfr IE      = 0xA8;
 sfr P3      = 0xB0;
 sfr IP      = 0xB8;
 sfr PSW     = 0xD0;
 sfr ACC     = 0xE0;
 sfr B       = 0xF0;
 


 
 
 
 
 
 
 
 
 
 


 
 sbit IT0  = 0x88;
 sbit IE0  = 0x89;
 sbit IT1  = 0x8A;
 sbit IE1  = 0x8B;
 sbit TR0  = 0x8C;
 sbit TF0  = 0x8D;
 sbit TR1  = 0x8E;
 sbit TF1  = 0x8F;
 


 
 
 
 
 
 
 
 
 
 
 
 
 


 
 sbit P1_0 = 0x90;
 sbit P1_1 = 0x91;
 sbit P1_2 = 0x92;
 sbit P1_3 = 0x93;
 sbit P1_4 = 0x94;
 sbit P1_5 = 0x95;
 sbit P1_6 = 0x96;
 sbit P1_7 = 0x97;
 
 sbit AIN0 = 0x90;        
 sbit AIN1 = 0x91;        
 


 
 sbit RI   = 0x98;
 sbit TI   = 0x99;
 sbit RB8  = 0x9A;
 sbit TB8  = 0x9B;
 sbit REN  = 0x9C;
 sbit SM2  = 0x9D;
 sbit SM1  = 0x9E;
 sbit SM0  = 0x9F;
 


 
 sbit EX0  = 0xA8;        
 sbit ET0  = 0xA9;        
 sbit EX1  = 0xAA;        
 sbit ET1  = 0xAB;        
 sbit ES   = 0xAC;        
 sbit ET2  = 0xAD;        
 
 sbit EA   = 0xAF;        
 


 
 sbit P3_0 = 0xB0;
 sbit P3_1 = 0xB1;
 sbit P3_2 = 0xB2;
 sbit P3_3 = 0xB3;
 sbit P3_4 = 0xB4;
 sbit P3_5 = 0xB5;
 
 sbit P3_7 = 0xB7;
 
 sbit RXD  = 0xB0;        
 sbit TXD  = 0xB1;        
 sbit INT0 = 0xB2;        
 sbit INT1 = 0xB3;        
 sbit T0   = 0xB4;        
 sbit T1   = 0xB5;        
 sbit AOUT = 0xB6;        
 


 
 sbit PX0  = 0xB8;
 sbit PT0  = 0xB9;
 sbit PX1  = 0xBA;
 sbit PT1  = 0xBB;
 sbit PS   = 0xBC;
 


 
 sbit P    = 0xD0;
 sbit FL   = 0xD1;
 sbit OV   = 0xD2;
 sbit RS0  = 0xD3;
 sbit RS1  = 0xD4;
 sbit F0   = 0xD5;
 sbit AC   = 0xD6;
 sbit CY   = 0xD7;
 



 
 
 
 
 
 
 

 
 
 
 
#line 10 "TCMain.c" /0
 
  
#line 1 "C:\KEIL\C51\INC\INTRINS.H" /0






 
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
#line 11 "TCMain.c" /0
 
 
 
 
 
 
 
 
 
 unsigned char code LEDDis[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x67};
 
 static char bdata StateREG;	 
 sbit DS1820ON=StateREG^0;	 
 
 void main(void)
 {
 void InitDS1820(void);
 void Delay_537(void);
 InitDS1820();
 P3=0;
 }
 
 void InitDS1820(void)		 
 {
  P3_7 = 1;		 
 _nop_();
  P3_7 = 0;		 
 Delay_537();
 }
 
 void Delay_537(void)	 
 {
 #pragma asm
 MOV R0,#FFH
 MOV R1,#02H
 TSR1:
 DJNZ R0,TSR1
 MOV R0,#FFH
 DJNZ R1,TSR1
 #pragma endasm
 }
