#define uchar unsigned char
#define uint  unsigned int
/*---------------------------------------------------------------------
------------------����LCD����----------------------------------------*/
sbit    LCD_RS= P2^0;
sbit    LCD_RW= P2^1;
sbit    LCD_E=  P2^2;
#define LCD_DATA  P0
/*---------------------------------------------------------------------
--------------------�ӳ���˵��---------------------------------------*/
void initialization(void);
void write_dictate(uchar i,j);
void write_data(uchar i);
uchar read_estate(void);
uchar read_data(void);
void displayonechar(uchar x,y,ddata);
void displaylistchar(uchar x,y,uchar code *ddata);
void delay400ms(void);
void delay(uint a);