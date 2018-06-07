/*
================================================================================
File Name : GUI_Basic.c
Author    : LiYOng
Date      : 2008-12-12 15:51
Version   : 1.0
Decription: This file contains some basic functions for GUI, It need the LCD
            Drive functions
================================================================================
*/
#include "GUI_Basic.H"
#include "GUI_Type.H"
#include "fontlib.h"
/*
================================================================================
Function     : GUI_DrawRectangle( )
Description  : Draw a rectangle
Input        : -pRect, point to a rectangle structure
output       : None
================================================================================
*/
void GUI_DrawRectangle( RECT* pRect )
{
	LINE line;

	line.xs = pRect->xs;
	line.xe = pRect->xe;
	line.ys = pRect->ys;
	line.ye = pRect->ys;
	line.Color = pRect->Color;
	LCDDrawHRLine( &line );

	line.xe = pRect->xs;
	line.ye = pRect->ye;
	LCDDrawHRLine( &line );

	line.xs = pRect->xe;
	line.ys = pRect->ye;
	LCDDrawHRLine( &line );

	line.xe = pRect->xe;
	line.ye = pRect->ys;
	LCDDrawHRLine( &line );
}
/*
================================================================================
Function     : GUI_DrawLine( )
Description  : Draw a line
Input        : -pLine, point to a line structure
output       : None
================================================================================
*/
void GUI_DrawLine( LINE* pLine )
{
	INT32S   dx;						// ֱ��x���ֵ����
	INT32S   dy;          	// ֱ��y���ֵ����
	INT32S    dx_sym;				// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	INT32S    dy_sym;				// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	INT32S   dx_x2;					// dx*2ֵ���������ڼӿ������ٶ�
	INT32S   dy_x2;					// dy*2ֵ���������ڼӿ������ٶ�
	INT32S   di;						// ���߱���

	POINT    point;
	LINE     line;

	line.xs = pLine->xs;
	line.ys = pLine->ys;
	line.xe = pLine->xe;
	line.ye = pLine->ye;
	line.Color = pLine->Color;

  point.Color = pLine->Color;

	dx = line.xe - line.xs;
  dy = line.ye - line.ys;

 /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
	if( dx > 0 )					// �ж�x�᷽��
	{
		dx_sym = 1;					// dx>0������dx_sym=1
	}
	else
	{
		if( dx < 0 )
		{
			dx_sym = -1;		  // dx<0������dx_sym=-1
		}
		else
		{
			LCDDrawHRLine( &line );
			return;
		}
	}

	if( dy > 0 )							// �ж�y�᷽��
	{
		dy_sym = 1;					// dy>0������dy_sym=1
	}
	else
	{
		if( dy < 0 )
		{
			dy_sym = -1;				// dy<0������dy_sym=-1
		}
		else
		{  // dy==0����ˮƽ�ߣ���һ��
			LCDDrawHRLine( &line );
			return;
		}
	}

	/* ��dx��dyȡ����ֵ */
	dx = dx_sym * dx;
	dy = dy_sym * dy;

	/* ����2����dx��dyֵ */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

 /* ʹ��Bresenham�����л�ֱ�� */
	if( dx >= dy )						// ����dx>=dy����ʹ��x��Ϊ��׼
	{
		di = dy_x2 - dx;
    while( line.xs != line.xe )
    {
			point.x = line.xs;
			point.y = line.ys;
			LCDDrawPoint( &point );
			line.xs += dx_sym;
			if( di < 0 )
			{
				di += dy_x2;			// �������һ���ľ���ֵ
			}
			else
			{
				di += dy_x2 - dx_x2;
				line.ys += dy_sym;
			}
    }
		LCDDrawPoint( &point );		// ��ʾ���һ��
	}
	else								// ����dx<dy����ʹ��y��Ϊ��׼
	{
		di = dx_x2 - dy;
    while( line.ys != line.ye )
    {
			point.x = line.xs;
			point.y = line.ys;
			LCDDrawPoint( &point );
			line.ys += dy_sym;
			if(di<0)
			{
				di += dx_x2;
			}
			else
			{
				di += dx_x2 - dy_x2;
				line.xs += dx_sym;
			}
    }
		LCDDrawPoint( &point );		// ��ʾ���һ��
	}
}
/*
================================================================================
Name: PrintFont
Function: Display a character at a special area
Input:	1.Xs : Start position X
		2.Ys : Start position Y
		3.pFont : A pointer of a font structure
		4.Character : The ASCII code of the character.
Output: None
Note: The start position is inputted as a parameter, And the end position is calculated by the FONT
		structure.
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void	GUI_DisplayFont( INT8U	Xs, INT8U Ys, FONT* pFont, char Character )
{
	BitBlock	Block;
	INT32U	Bytes;
	INT8U DataBuffer[64];
	INT8U i;
	const unsigned char *offset;

	Block.Height = pFont->Height;
	Block.Width = pFont->Width;
	Block.Color = pFont->Color;
	Block.BackColor = pFont->BackColor;
	Block.xs = Xs;
	Block.ys = Ys;

	Bytes = pFont->Width >> 3;
	if( pFont->Width & 0x07 )
	{
	 	Bytes ++;
	}
	Bytes *= pFont->Height;
	Bytes *= Character - ' ';

	if( pFont->Height == 18 )
	{
	 	offset = (const unsigned char*)&FontLib_18;
	}
	else if( pFont->Height == 14 )
	{
	 	offset = (const unsigned char*)&FontLib_14;
	}
	else
	{
		return;
	}
	offset += Bytes;
	for( i = 0; i < 36; i ++ )
	{
		//DataBuffer[i] = pgm_read_byte( offset + i );	
	}

	
	Block.pData = DataBuffer;

	PrintBitBlock( &Block );
}
/*
========================================================================================================
Name: DisplayStr
Function: Display a character at a special area
Input:
    1.Xs : Start position X
		2.Ys : Start position Y
		3.pFont : A pointer of a font structure
		4.Str : The start address of a string
Output: None
Note: The start position is inputted as a parameter, And the end position is calculated by the FONT
		structure.
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void	GUI_DisplayStr( INT8U xs, INT8U ys, FONT* pFont, char* Str )
{
	while( *Str )
	{
		GUI_DisplayFont( xs, ys, pFont, *Str );
	 	Str ++;
		xs += pFont->Width;
	}
}
/*
================================================================================
Name: GUI_DrawCircle( )
Function: Display a cycle at a special area
Input:	-pCycle, A pinter point to a cycle structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawCircle( CIRCLE* pCircle )
{
   INT8S  draw_x0, draw_y0;			// ��ͼ���������
   INT8S  draw_x1, draw_y1;
   INT8S  draw_x2, draw_y2;
   INT8S  draw_x3, draw_y3;
   INT8S  draw_x4, draw_y4;
   INT8S  draw_x5, draw_y5;
   INT8S  draw_x6, draw_y6;
   INT8S  draw_x7, draw_y7;
   INT8S  xx, yy;					// ��Բ���Ʊ���

   INT8S  di;						// ���߱���
   POINT point;

   point.Color = pCircle->Color;

   /* �������� */
   if(0 == pCircle->r ) return;

   /* �����8�������(0��45��90��135��180��225��270��)��������ʾ */
   point.x = draw_x0 = draw_x1 = pCircle->x;
   point.y = draw_y0 = draw_y1 = pCircle->y + pCircle->r;

   if( draw_y0 < GUI_LCM_YMAX ) LCDDrawPoint( &point );	// 90��

   point.x = draw_x2 = draw_x3 = pCircle->x;
   point.y = draw_y2 = draw_y3 = pCircle->y - pCircle->r;
   if( draw_y2 >= 0 ) LCDDrawPoint( &point );			// 270��


   point.x = draw_x4 = draw_x6 = pCircle->x + pCircle->r;
   point.y = draw_y4 = draw_y6 = pCircle->y;
   if(draw_x4<GUI_LCM_XMAX) LCDDrawPoint( &point );	// 0��

   point.x = draw_x5 = draw_x7 = pCircle->x - pCircle->r;
   point.y = draw_y5 = draw_y7 = pCircle->y;
   if(draw_x5>=0) LCDDrawPoint( &point );			// 180��
   if(1==pCircle->r) return;					// ���뾶Ϊ1������Բ����


   /* ʹ��Bresenham�����л�Բ */
   di = 3 - 2*pCircle->r;					// ��ʼ�����߱���

   xx = 0;
   yy = pCircle->r;
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;
	  }
	  else
	  {  di += 4*(xx - yy) + 10;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;
	  }

	  xx++;
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;


	  /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )
	  {
	  	point.x = draw_x0;
	  	point.y = draw_y0;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x1>=0)&&(draw_y1>=0) )
	  {
	  	point.x = draw_x1;
	  	point.y = draw_y1;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x2;
	  	point.y = draw_y2;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x3;
	  	point.y = draw_y3;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )
	  {
	  	point.x = draw_x4;
	  	point.y = draw_y4;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )
	  {
	  	point.x = draw_x5;
	  	point.y = draw_y5;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x6;
	  	point.y = draw_y6;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x7;
	  	point.y = draw_y7;
	  	LCDDrawPoint( &point );
	  }
	}
}
/*
================================================================================
Name: GUI_DrawCircleFill( )
Function: Display a cycle at a special area and fill its area
Input:	-pCycle, A pinter point to a cycle structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawCircleFill( CIRCLE* pCircle )
{
   INT8S  draw_x0, draw_y0;			// ��ͼ���������
   INT8S  draw_x1, draw_y1;
   INT8S  draw_x2, draw_y2;
   INT8S  draw_x3, draw_y3;
   INT8S  draw_x4, draw_y4;
   INT8S  draw_x5, draw_y5;
   INT8S  draw_x6, draw_y6;
   INT8S  draw_x7, draw_y7;
   INT8S  fill_x0, fill_y0;			// �������ı�����ʹ�ô�ֱ�����
   INT8S  fill_x1;
   INT8S  xx, yy;					// ��Բ���Ʊ���

   INT8S  di;						// ���߱���
   POINT   point;
   LINE    line;

   point.Color = pCircle->Color;
   line.Color = pCircle->Color;



   /* �������� */
   if(0==pCircle->r) return;

   /* �����4�������(0��90��180��270��)��������ʾ */
   point.x = draw_x0 = draw_x1 = pCircle->x;
   point.y = draw_y0 = draw_y1 = pCircle->y + pCircle->r;
   if(draw_y0<GUI_LCM_YMAX)
   {
      LCDDrawPoint( &point );
   }

   point.x = draw_x2 = draw_x3 = pCircle->x;
   point.y = draw_y2 = draw_y3 = pCircle->y - pCircle->r;
   if(draw_y2>=0)
   {
   		LCDDrawPoint( &point );
   }

   point.x = draw_x4 = draw_x6 = pCircle->x + pCircle->r;
   point.y = draw_y4 = draw_y6 = pCircle->y;
   if(draw_x4<GUI_LCM_XMAX)
   {
   		LCDDrawPoint( &point );	// 0��
      fill_x1 = draw_x4;
   }
   else
   {
   		fill_x1 = GUI_LCM_XMAX;
   }
   fill_y0 = pCircle->y;							// �������������ʼ��fill_x0
   fill_x0 = pCircle->x - pCircle->r;						// �����������������fill_y1
   if(fill_x0<0) fill_x0 = 0;
	 line.xs = fill_x0;
	 line.ys = fill_y0;
	 line.ye = fill_y0;
	 line.xe = fill_x1;
   LCDDrawHRLine( &line );

   point.x = draw_x5 = draw_x7 = pCircle->x - pCircle->r;
   point.y = draw_y5 = draw_y7 = pCircle->y;
   if(draw_x5>=0)
   {
   		LCDDrawPoint( &point );	// 180��
   }
   if(1==pCircle->r) return;


   /* ʹ��Bresenham�����л�Բ */
   di = 3 - 2*pCircle->r;							// ��ʼ�����߱���

   xx = 0;
   yy = pCircle->r;
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;
	  }
	  else
	  {  di += 4*(xx - yy) + 10;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;
	  }

	  xx++;
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;


	  /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )
	  {
	  	point.x = draw_x0;
	  	point.y = draw_y0;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x1>=0)&&(draw_y1>=0) )
	  {
	  	point.x = draw_x1;
	  	point.y = draw_y1;
	  	LCDDrawPoint( &point );
	  }

	  /* �ڶ���ˮֱ�����(�°�Բ�ĵ�) */
	  if(draw_x1>=0)
	  {  /* �������������ʼ��fill_x0 */
	     fill_x0 = draw_x1;
	     /* �������������ʼ��fill_y0 */
	     fill_y0 = draw_y1;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* �����������������fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x1;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;

         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x2;
	  	point.y = draw_y2;
	  	LCDDrawPoint( &point );
	  }

	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x3;
	  	point.y = draw_y3;
	  	LCDDrawPoint( &point );
	  }

	  /* ���ĵ㴹ֱ�����(�ϰ�Բ�ĵ�) */
	  if(draw_x3>=0)
	  {  /* �������������ʼ��fill_x0 */
	     fill_x0 = draw_x3;
	     /* �������������ʼ��fill_y0 */
	     fill_y0 = draw_y3;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* �����������������fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x3;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;

         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )
	  {
	  	point.x = draw_x4;
	  	point.y = draw_y4;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )
	  {
	  	point.x = draw_x5;
	  	point.y = draw_y5;
	  	LCDDrawPoint( &point );
	  }

	  /* �����㴹ֱ�����(�ϰ�Բ�ĵ�) */
	  if(draw_x5>=0)
	  {  /* �������������ʼ��fill_x0 */
	     fill_x0 = draw_x5;
	     /* �������������ʼ��fill_y0 */
	     fill_y0 = draw_y5;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* �����������������fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x5;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x6;
	  	point.y = draw_y6;
	  	LCDDrawPoint( &point );
	  }

	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x7;
	  	point.y = draw_y7;
	  	LCDDrawPoint( &point );
	  }

	  /* �ڰ˵㴹ֱ�����(�ϰ�Բ�ĵ�) */
	  if(draw_x7>=0)
	  {  /* �������������ʼ��fill_x0 */
	     fill_x0 = draw_x7;
	     /* �������������ʼ��fill_y0 */
	     fill_y0 = draw_y7;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* �����������������fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x7;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }

   }
}
/*
================================================================================
Name: GUI_DrawEllipse( )
Function: Display a ellipse at a special area
Input:	-pCycle, A pinter point to a ellipse structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawEllipse( ELLIPSE* pEllipse )
{
   INT32S  draw_x0, draw_y0;			// ��ͼ���������
   INT32S  draw_x1, draw_y1;
   INT32S  draw_x2, draw_y2;
   INT32S  draw_x3, draw_y3;
   INT32S  xx, yy;					// ��ͼ���Ʊ���

   INT32S  center_x, center_y;		// ��Բ���ĵ��������
   INT32S  radius_x, radius_y;		// ��Բ�İ뾶��x��뾶��y��뾶
   INT32S  radius_xx, radius_yy;		// �뾶��ƽ��ֵ
   INT32S  radius_xx2, radius_yy2;	// �뾶��ƽ��ֵ������
   INT32S  di;						// ������߱���

   POINT   point;

   point.Color = pEllipse->Color;

   /* �������� */
   if( (pEllipse->xs == pEllipse->xe ) ||
   	   (pEllipse->ys == pEllipse->ye ) ) return;

   /* �������Բ���ĵ����� */
   center_x = (pEllipse->xs + pEllipse->xe) >> 1;
   center_y = (pEllipse->ys + pEllipse->ye) >> 1;

   /* �������Բ�İ뾶��x��뾶��y��뾶 */
   if(pEllipse->xs > pEllipse->xe)
   {  radius_x = (pEllipse->xs - pEllipse->xe) >> 1;
   }
   else
   {  radius_x = (pEllipse->xe - pEllipse->xs) >> 1;
   }
   if(pEllipse->ys > pEllipse->ye)
   {  radius_y = (pEllipse->ys - pEllipse->ye) >> 1;
   }
   else
   {  radius_y = (pEllipse->ye - pEllipse->ys) >> 1;
   }

   /* ����뾶ƽ��ֵ */
   radius_xx = radius_x * radius_x;
   radius_yy = radius_y * radius_y;

   /* ����뾶ƽ��ֵ��2ֵ */
   radius_xx2 = radius_xx<<1;
   radius_yy2 = radius_yy<<1;

   /* ��ʼ����ͼ���� */
   xx = 0;
   yy = radius_y;

   di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// ��ʼ�����߱���

   /* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
   draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
   draw_y0 = draw_y1 = center_y + radius_y;
   draw_y2 = draw_y3 = center_y - radius_y;

	 point.x = draw_x0;
	 point.y = draw_y0;
	 LCDDrawPoint( &point );
	 point.x = draw_x2;
	 point.y = draw_y2;
	 LCDDrawPoint( &point );	// ��y���ϵ������˵�

   while( (radius_yy*xx) < (radius_xx*yy) )
   {  if(di<0)
	  {  di+= radius_yy2*(2*xx+3);
	  }
	  else
	  {  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
	  }

	  xx ++;						// x���1

	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );
   }

   di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
   while(yy>=0)
   {  if(di<0)
	  {  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

	     xx ++;						// x���1
	     draw_x0++;
	     draw_x1--;
	     draw_x2++;
	     draw_x3--;
	  }
	  else
	  {  di += radius_xx2*3 - 2*radius_xx2*yy;
	  }

	  yy--;
 	  draw_y0--;
	  draw_y1--;
	  draw_y2++;
	  draw_y3++;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );
   }
}
/*
================================================================================
Name: GUI_DrawEllipseFill( )
Function: Display a ellipse at a special area and fill its area
Input:	-pCycle, A pinter point to a ellipse structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawEllipseFill( ELLIPSE* pEllipse )
{
	INT32S  draw_x0, draw_y0;			// ��ͼ���������
	INT32S  draw_x1, draw_y1;
	INT32S  draw_x2, draw_y2;
	INT32S  draw_x3, draw_y3;
	INT32S  xx, yy;					      // ��ͼ���Ʊ���

	INT32S  center_x, center_y;		// ��Բ���ĵ��������
	INT32S  radius_x, radius_y;		// ��Բ�İ뾶��x��뾶��y��뾶
	INT32S  radius_xx, radius_yy;		// �뾶��ƽ��ֵ
	INT32S  radius_xx2, radius_yy2;	// �뾶��ƽ��ֵ������
	INT32S  di;						// ������߱���

	POINT point;
	LINE  line;

	point.Color = pEllipse->Color;
	line.Color = pEllipse->Color;

	/* �������� */
	if( (pEllipse->xs==pEllipse->xe) ||
	    (pEllipse->ys==pEllipse->ye) ) return;

	/* �������Բ���ĵ����� */
	center_x = (pEllipse->xs + pEllipse->xe) >> 1;
	center_y = (pEllipse->ys + pEllipse->ye) >> 1;

	/* �������Բ�İ뾶��x��뾶��y��뾶 */
	if(pEllipse->xs > pEllipse->xe)
	{
		radius_x = (pEllipse->xs - pEllipse->xe) >> 1;
	}
	else
	{
		radius_x = (pEllipse->xe - pEllipse->xs) >> 1;
	}
	if(pEllipse->ys > pEllipse->ye)
	{
		radius_y = (pEllipse->ys - pEllipse->ye) >> 1;
	}
	else
	{
		radius_y = (pEllipse->ye - pEllipse->ys) >> 1;
	}

	/* ����뾶��ƽ��ֵ */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* ����뾶��4ֵ */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* ��ʼ����ͼ���� */
	xx = 0;
	yy = radius_y;

	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// ��ʼ�����߱���

	/* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;

	point.x = draw_x0;
	point.y = draw_y0;
	LCDDrawPoint( &point );
	point.x = draw_x2;
	point.y = draw_y2;
	LCDDrawPoint( &point );// ��y���ϵ������˵�

   while( (radius_yy*xx) < (radius_xx*yy) )
   {  if(di<0)
	  {  di+= radius_yy2*(2*xx+3);
	  }
	  else
	  {  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
	  }

	  xx ++;						// x���1

	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );

	  /* ��y���ѱ仯��������� */
	  if(di>=0)
	  {
	  	line.xs = draw_x0;
	  	line.xe = draw_x1;
	  	line.ys = line.ye = draw_y0;
	  	LCDDrawHRLine( &line );
	  	line.xs = draw_x2;
	  	line.xe = draw_x3;
	  	line.ys = line.ye = draw_y2;
	  	LCDDrawHRLine( &line );

	  }
   }

   di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy +
        radius_yy2*xx - radius_xx2*radius_yy;
   while(yy>=0)
   {  if(di<0)
	  {  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

	     xx ++;						// x���1
	     draw_x0++;
	     draw_x1--;
	     draw_x2++;
	     draw_x3--;
	  }
		else
		{  di += radius_xx2*3 - 2*radius_xx2*yy;
		}

		yy--;
		draw_y0--;
		draw_y1--;
		draw_y2++;
		draw_y3++;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );

	  /* y���ѱ仯��������� */
		line.xs = draw_x0;
		line.xe = draw_x1;
		line.ys = line.ye = draw_y0;
		LCDDrawHRLine( &line );
		line.xs = draw_x2;
		line.xe = draw_x3;
		line.ys = line.ye = draw_y2;
		LCDDrawHRLine( &line );
   }
}
/*
================================================================================
Name: GUI_Inital( )
Function: Initialize GUI with single color
Input:	-Color, Initialize color
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void GUI_Inital( TCOLOR Color )
{
	DOLLOP dollop;

	dollop.xs = 0;
	dollop.xe = GUI_LCM_XMAX;
	dollop.ys = 0;
	dollop.ye = GUI_LCM_YMAX;
	dollop.Color = Color;

	LCDDrawDollop( &dollop );
}	
/*
================================================================================
=======================================End of file==============================
================================================================================
*/


