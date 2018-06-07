#include <reg52.h>
#include "LCD1602.h"


//LCD��ʼ��
void InitLCD()
{
	LCDReset();
	InputMode(0x06); 	//������ʽ������λ
	DispControl(0x0c);	//��ʾ�������أ���˸��
	FunctionSet(0x38);	//8λ��2�У�5��7
	}

void main()
{
	InitP0(0);
	InitLCD();
	
	while(1)
	{
		//��ʾ"Welcome!"
		DispCharacter(0,4,'W');
		DispCharacter(0,5,'e');
		DispCharacter(0,6,'l');
		DispCharacter(0,7,'c');
		DispCharacter(0,8,'o');
		DispCharacter(0,9,'m');
		DispCharacter(0,10,'e');
		DispCharacter(0,11,'!');
		//��ʾ"03-01-2007"
		DispCharacter(1,3,'0');
		DispCharacter(1,4,'6');
		DispCharacter(1,5,'-');
		DispCharacter(1,6,'2');
		DispCharacter(1,7,'8');
		DispCharacter(1,8,'-');
		DispCharacter(1,9,'2');
		DispCharacter(1,10,'0');
		DispCharacter(1,11,'0');
		DispCharacter(1,12,'9');
		}
	}

	
