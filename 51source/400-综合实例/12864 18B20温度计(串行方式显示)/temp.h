//函数声明:
void LCD_SEND(unsigned char send_data);
unsigned char LCD_RECEIVE();
unsigned char LCD_READ_BF();
void LCD_WRITE_IR(unsigned char LCD_COMMAND);
void LCD_WRITE_DR(unsigned char LCD_DATA);
void LCD_INIT();
//函数声明
bit DS1820_Reset();
void DS1820_WriteData(unsigned char wData);
unsigned char DS1820_ReadData();
void read_temp();

void delay100us();
void DispCode();
void display();