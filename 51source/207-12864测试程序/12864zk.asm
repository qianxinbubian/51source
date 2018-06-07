;12864�����ֿ⺺����ʾ��ʾ����
;***************************************************************************
;*  saxmcu@126.com		          www.saxmcu.com                   *
;*  laosong     www.saxmcu.com �����������½��վ               	   *
;*  P0.0--P0.7�ڽ�J21��                                                    *  
;*  P2.0----��J20��0������                                                 *
;*  P2.1----��J20��1������                                                 *
;*  P2.2----��J20��2������                                                 *
;*  P2.3----��J20��4������                                                 *
;*  P2.5----��J20��6������                                                 *
;***************************************************************************

RS EQU P2.0
          RW EQU P2.1
          E  EQU P2.2
          PSB EQU P2.3
          RST EQU P2.5
;-----------------------------------------------
          LCD_X  EQU 30H
          LCD_Y  EQU 31H
          COUNT  EQU 32H
          COUNT1 EQU 33H
          COUNT2 EQU 34H
          COUNT3 EQU 35H
;-----------------------------------------------
       LCD_DATA  EQU 36H
       LCD_DATA1 EQU 37H
       LCD_DATA2 EQU 38H
          STORE  EQU 39H

;-----------------------------------------------
  ORG 0000H
  LJMP MAIN
  ORG 0100H
;-----------------------------------------------
MAIN:
     MOV SP,#5FH
            CLR  RST             ;��λ
            LCALL DELAY4
            SETB RST
     NOP
            SETB PSB             ;ͨѶ��ʽΪ8λ���ݲ���

;********************��ʼ��**********************
LGS0:     MOV A,#34H  ;34H--����ָ�����
     LCALL SEND_I
     MOV A,#30H  ;30H--����ָ�����
     LCALL SEND_I
     MOV A,#01H  ;�����ʾ
     LCALL SEND_I
            MOV A,#06H  ;ָ��������д����ȡʱ�������ƶ�����
     LCALL SEND_I        ;DDRAM �ĵ�ַ������(AC)��1
     MOV A,#0CH  ;����ʾ,�ع��,����˸
     LCALL SEND_I
;===============================================
TU_PLAY1:
            MOV  DPTR,#TU_TAB1      ;��ʾͼ��
     LCALL PHO_DISP
     LCALL DELAY3

;=================================================
;;��ʾ���ֺ��ַ�
;����80ms����ʱ��ʹ���ܹ��������ʾ�Ĺ���
;���ݺ�����ʾ����ֶ�д�루˳��д�룩
;=================================================
HAN_WR2:
            LCALL CLEAR_P
HAN_WR2A:
            MOV DPTR,#TAB1A ;��ʾ���ֺ��ַ�
     MOV COUNT,#10H      ;��ַ��������Ϊ16��
     MOV A,#80H          ;��һ����ʼ��ַ
     LCALL SEND_I
            LCALL   QUSHU
HAN_WR2B:
            MOV DPTR,#TAB1B ;��ʾ���ֺ��ַ�
     MOV COUNT,#10H      ;��ַ��������Ϊ16��
     MOV A,#90H          ;�ڶ�����ʼ��ַ
     LCALL SEND_I
            LCALL QUSHU
HAN_WR2C:
            MOV DPTR,#TAB1C ;��ʾ���ֺ��ַ�
     MOV COUNT,#10H      ;��ַ��������Ϊ16��
     MOV A,#88H          ;��������ʼ��ַ
     LCALL SEND_I
            LCALL QUSHU
HAN_WR2D:
            MOV DPTR,#TAB1D  ;��ʾ���ֺ��ַ�
     MOV COUNT,#10H       ;��ַ��������Ϊ16��
     MOV A,#98H           ;��������ʼ��ַ
     LCALL SEND_I
            LCALL QUSHU
            LCALL DELAY3
            LCALL FLASH
            LCALL CLEAR_P
            JMP  TU_PLAY2
;----------------------------------------------
;TU_PLAY1:
            MOV  DPTR,#TU_TAB1      ;��ʾͼ��
     LCALL PHO_DISP
     LCALL DELAY3
           
;----------------------------------------------
TU_PLAY2:
     MOV DPTR,#TU_TAB2   ;��ʾͼ��
     LCALL PHO_DISP
     LCALL DELAY3
         
;----------------------------------------------
TU_PLAY3:
     MOV DPTR,#TU_TAB4   ;��ʾͼ��
     LCALL PHO_DISP
     LCALL DELAY3
         
;-----------------------------------------------
;��ʾ����
;-----------------------------------------------
LATPLAY1:
            MOV A,#01H         ;����
            LCALL SEND_I
     MOV LCD_DATA1,#0CCH  ;��ʾ����
     MOV LCD_DATA2,#0CCH
     LCALL LAT_DISP
     LCALL DELAY3
            LCALL CLEAR_P

KU_PLAY2:

            LJMP  TU_PLAY1
      
;===============================================
;ȫ����ʾͼ���ӳ���
;===============================================
PHO_DISP:
         MOV  COUNT3,#02H
     MOV  LCD_X,#80H
PHO_DISP1:
         MOV  LCD_Y,#80H
     MOV  COUNT2,#20H
PHO_DISP2:
         MOV  COUNT1,#10H
     LCALL WR_ZB
PHO_DISP3:
            CLR A
     MOVC A,@A+DPTR
     LCALL SEND_D
     INC DPTR
     DJNZ COUNT1,PHO_DISP3
     INC LCD_Y
     DJNZ COUNT2,PHO_DISP2
     MOV LCD_X,#88H
     DJNZ COUNT3,PHO_DISP1

     MOV A,#36H
     LCALL SEND_I
     MOV A,#30H
     LCALL SEND_I
            RET
;----------------------------------------------
CLRRAM:
          MOV LCD_DATA1,#00H  ;GDRAMд0�ӳ���
     MOV LCD_DATA2,#00H
     LCALL LAT_DISP
     RET
;==============================================
;��ʾ�����ӳ���
;==============================================
LAT_DISP:
         MOV COUNT3,#02H
     MOV LCD_X,#80H
LAT_DISP1:
         MOV LCD_Y,#80H
     CLR F0
     MOV COUNT2,#20H
LAT_DISP2:
         MOV COUNT1,#10H
     LCALL WR_ZB
LAT_DISP3:
         JB F0,LAT_DISP32
     MOV LCD_DATA,LCD_DATA1
     AJMP LAT_DISP31
LAT_DISP32:
         MOV LCD_DATA,LCD_DATA2
LAT_DISP31:
         MOV A,LCD_DATA
     LCALL SEND_D
     DJNZ COUNT1,LAT_DISP31
     INC LCD_Y
     CPL F0
     DJNZ COUNT2,LAT_DISP2
     MOV LCD_X,#88H
     DJNZ COUNT3,LAT_DISP1

     MOV A,#36H
     LCALL SEND_I
     MOV A,#30H
     LCALL SEND_I
     RET
;---------------------------------------------
WR_ZB:
         MOV A,#34H
     LCALL SEND_I
     MOV A,LCD_Y
     LCALL SEND_I
     MOV A,LCD_X
     LCALL SEND_I
     MOV A,#30H
     LCALL SEND_I
     RET

;===============================================
FLASH:
            MOV  A,#08H       ;�ر���ʾ
            LCALL SEND_I
            LCALL  DELAY5
            MOV  A,#0CH   ;����ʾ,�ع��,����˸
     LCALL SEND_I
            LCALL  DELAY5
            MOV  A,#08H       ;�ر���ʾ
            LCALL SEND_I
            LCALL  DELAY5
            MOV  A,#0CH   ;����ʾ,�ع��,����˸
     LCALL SEND_I
            LCALL  DELAY5
            MOV  A,#08H       ;�ر���ʾ
            LCALL SEND_I
            LCALL  DELAY5
                RET
;==================================================
;����
;==================================================
CLEAR_P:
            MOV A,#01H           ;����
     LCALL SEND_I
            MOV A,#34H
     LCALL SEND_I
     MOV A,#30H
     LCALL SEND_I
            RET
;==================================================
;���ȡ��������ʾ
;==================================================
QUSHU:
            CLR A
     MOVC A,@A+DPTR       ;���ȡ����
     LCALL SEND_D          ;����ʾ
     INC DPTR
            LCALL DELAY4           ;��ʱ80ms,
            DJNZ COUNT,QUSHU
     RET

;===============================================
;д�����ӳ���
;RS=1,RW=0,E=������,D0-D7=����
;===============================================
SEND_D:
     LCALL CHK_BUSY    ;д�����ӳ���
     SETB RS
     CLR RW
     MOV P0,A
     SETB E
     NOP
     NOP
     CLR E
     RET
;===============================================
;дָ���ӳ���
;RS=0,RW=0,E=������,D0-D7=ָ����
;===============================================
SEND_I:
     LCALL CHK_BUSY
     CLR RS
     CLR RW
     MOV P0,A
     SETB E
     NOP
     NOP
     CLR E
     RET
;================================================
;�������ӳ���
;RS=1,RW=1,E=H,D0-D7=����
;================================================
READ_D:
    LCALL CHK_BUSY    ;�������ӳ���
    SETB RS
    SETB RW
    SETB E
    NOP
    MOV A,P0
    CLR E
    MOV STORE,A
    RET
;================================================
;;��æµ�ӳ���
;RS=0,RW=1,E=H,D0-D7=״̬��
;================================================
CHK_BUSY:
        MOV P0,#0FFH    ;��æµ�ӳ���
    CLR RS
    SETB RW
    SETB E
    JB P0.7,$
    CLR E
    RET
;================================================
;��ʱ�ӳ���

DELAY3:
    MOV R5,#16H
DEL31:    MOV R6,#100
DEL32:    MOV R7,#0FFH
DEL33:    DJNZ R7,DEL33
    DJNZ R6,DEL32
    DJNZ R5,DEL31
    RET

DELAY2:
           MOV R6,#0CH
DEL21:    MOV R7,#18H
DEL22:    DJNZ R7,DEL22
    DJNZ R6,DEL21
    RET

DELAY1:
           MOV R6,#06H
DEL11:    MOV R7,#08H
DEL12:    DJNZ R7,DEL12
    DJNZ R6,DEL11
           RET

DELAY4:
           MOV R6,#100
DEL41:    MOV R7,#200
DEL42:    DJNZ R7,DEL42
    DJNZ R6,DEL41
    RET

DELAY5:
           MOV R5,#05H
DEL51:    MOV R6,#100
DEL52:    MOV R7,#0FFH
DEL53:    DJNZ R7,DEL53
    DJNZ R6,DEL52
    DJNZ R5,DEL51
    RET
;***********************************************
TAB1:
TAB1A:     DB  '    ��۱�����    '    ;��ʾ�ڵ�һ��
TAB1C:     DB  '��ѧ��Ƭ����һվ'    ;��ʾ�ڵ�����
TAB1B:     DB  '  WWW.saxmcu.COM  '    ;��ʾ�ڵڶ���
TAB1D:     DB  'TEL 371-66987238'    ;��ʾ�ڵ�����
;*-------------------------------------------------------------------- *
;* Bitmap�������ݱ�                                                    *
;* ͼƬ: E:\ͼ��8.bmp,����ȡģ���λ,��������:�����Ҵ��ϵ���         *
;* ͼƬ�ߴ�: 128 * 64                                                  *
;*-------------------------------------------------------------------- *
TU_TAB1:                                   ; ���ݱ�
;--  ����:  0  --
;--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16   --
DB  000H,007H,008H,010H,010H,010H,008H,007H,000H,0F0H,008H,004H,004H,004H,008H,0F0H

;*-------------------------------------------------------------------- *
;* Bitmap�������ݱ�                                                    *
;* ͼƬ: E:\ͼ��12.bmp,����ȡģ���λ,��������:�����Ҵ��ϵ���        *
;* ͼƬ�ߴ�: 128 * 64                                                  *
;*-------------------------------------------------------------------- *
TU_TAB2:                                   ; ���ݱ�

	db   40h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 03h, 65h,0E8h, 80h, 00h, 01h
	db	 9Ah,0D6h,0B4h, 42h, 00h, 00h, 00h, 00h, 00h, 00h, 34h,0DAh,0D0h, 00h, 00h, 01h
	db	0A4h, 00h, 02h, 10h, 6Bh, 30h, 00h, 00h, 00h, 00h, 8Bh,0FFh, 30h, 00h, 00h, 01h
	db	 42h,0D6h,0B0h, 42h, 80h, 00h, 00h, 00h, 00h, 16h, 77h,0FEh,0C0h, 00h, 00h, 01h
	db	 42h,0D6h,0B0h, 42h, 80h, 00h, 00h, 00h, 00h, 16h, 77h,0FEh,0C0h, 00h, 00h, 01h
	db	0B9h, 20h, 04h, 00h, 00h, 02h, 10h, 00h, 00h, 00h,0BCh,0BCh, 00h, 00h, 00h, 01h
	db	 46h,0D6h,0B1h,0ACh, 6Bh, 30h, 00h, 00h, 02h, 29h,0CBh, 40h, 00h, 00h, 00h, 01h
	db	0FFh,0F9h, 04h, 01h, 00h, 00h, 00h, 00h, 01h, 16h,0B4h,0A0h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0F2h, 50h, 00h, 40h, 00h, 00h, 00h, 29h, 4Bh, 00h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0F2h, 50h, 00h, 40h, 00h, 00h, 00h, 29h, 4Bh, 00h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0BDh, 82h, 94h, 00h, 40h, 00h, 00h,0C9h, 30h, 02h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0FBh,0B0h, 00h, 08h, 00h, 00h, 05h, 16h, 44h, 40h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0FFh,0EFh, 90h, 40h, 00h, 00h, 00h, 21h, 00h, 04h,0C8h, 30h, 00h, 01h
	db	0A5h,0FFh,0FFh,0FDh, 6Bh, 00h, 00h, 00h, 00h,0C8h, 00h, 99h, 16h,0FAh, 40h, 01h
	db	0A5h,0FFh,0FFh,0FDh, 6Bh, 00h, 00h, 00h, 00h,0C8h, 00h, 99h, 16h,0FAh, 40h, 01h
	db	 82h, 2Fh,0FFh,0FFh,0E8h, 84h, 00h, 00h, 00h, 00h, 08h, 00h,0DFh,0FFh,0ECh, 01h
	db	 24h,0C1h, 7Fh,0FFh,0F4h, 30h, 00h, 00h, 00h, 00h, 00h,0A7h,0FFh,0FFh,0FCh, 01h
	db	 42h, 28h, 05h,0BFh,0FBh, 80h, 00h, 00h, 00h, 00h, 04h, 1Bh,0FFh,0FFh,0FFh, 81h
	db	 98h,0C1h, 4Ah, 53h,0ECh, 48h, 00h, 00h, 00h, 00h, 40h, 7Fh,0FFh,0FFh,0FFh,0E1h
	db	 98h,0C1h, 4Ah, 53h,0ECh, 48h, 00h, 00h, 00h, 00h, 40h, 7Fh,0FFh,0FFh,0FFh,0E1h
	db	 42h, 28h, 00h, 0Ch, 77h, 82h, 00h, 00h, 00h, 00h, 33h,0BFh,0FFh,0FFh,0FFh, 89h
	db	 18h,0C1h, 4Ah, 51h, 9Bh, 74h, 00h, 00h, 00h, 00h, 04h,0FFh, 3Fh,0FFh,0DFh,0F1h
	db	0C2h, 28h, 00h, 0Ch, 04h, 80h, 00h, 00h, 00h, 00h, 40h,0FEh, 1Fh,0F9h,0C3h,0F9h
	db	 1Ah,0D6h,0B5h,0A1h, 6Bh, 75h, 80h, 00h, 00h, 00h, 8Fh,0FCh, 3Fh,0F2h, 0Fh, 1Fh
	db	 1Ah,0D6h,0B5h,0A1h, 6Bh, 75h, 80h, 00h, 00h, 00h, 8Fh,0FCh, 3Fh,0F2h, 0Fh, 1Fh
	db	0C5h, 29h, 79h, 8Ch, 00h, 88h, 00h, 00h, 00h, 00h, 03h,0F8h, 1Fh,0C8h, 52h, 81h
	db	 22h,0FFh,0FFh,0F2h, 6Bh, 35h, 80h, 00h, 00h, 00h, 37h,0C2h, 3Fh,0FBh,0FCh, 09h
	db	 9Ah,0FFh,0FFh,0FFh, 00h, 48h, 00h, 00h, 00h, 00h, 07h,0D8h, 1Fh,0FFh,0E3h, 81h
	db	 9Ah,0FFh,0FFh,0FFh, 00h, 48h, 00h, 00h, 00h, 00h, 07h,0D8h, 1Fh,0FFh,0E3h, 81h
	db	 47h,0FFh,0FFh,0FFh,0F3h, 35h, 80h, 00h, 00h, 00h, 8Fh,0C1h, 0Fh,0FFh,0ACh, 01h
	db	0B9h,0FFh,0FFh,0FFh,0FCh, 4Ah, 20h, 00h, 00h, 00h, 0Fh,0A4h, 07h,0FEh, 40h, 01h
	db	 5Fh,0FFh,0C7h,0FFh,0FCh,0B5h, 80h, 00h, 00h, 00h, 4Fh,0DAh,0FFh,0F8h, 00h, 01h
	db	0A7h,0FFh,0B7h,0FFh,0FFh, 8Ah, 20h, 00h, 00h, 00h, 0Fh,0FFh,0EFh, 40h, 00h, 01h
	db	0A7h,0FFh,0B7h,0FFh,0FFh, 8Ah, 20h, 00h, 00h, 00h, 0Fh,0FFh,0EFh, 40h, 00h, 01h
	db	 7Fh,0FFh, 47h,0FDh, 7Fh,0C5h, 80h, 00h, 00h, 00h, 3Fh,0DAh,0D0h, 00h, 00h, 01h
	db	0A7h,0FEh, 37h,0FFh, 7Ch,0FAh, 50h, 00h, 00h, 00h, 0Ch,0A4h, 08h, 00h, 00h, 01h
	db	0DFh,0FEh, 43h,0FFh,0F8h,0C1h, 80h, 00h, 00h, 00h, 48h, 01h, 00h, 00h, 00h, 01h
	db	 27h,0FFh, 4Bh,0FFh,0FFh,0FAh, 2Ch, 00h, 00h, 00h, 03h, 58h, 20h, 00h, 00h, 01h
	db	 27h,0FFh, 4Bh,0FFh,0FFh,0FAh, 2Ch, 00h, 00h, 00h, 03h, 58h, 20h, 00h, 00h, 01h
	db	 9Bh,0FFh,0C3h,0FFh,0FFh,0F1h,0C0h, 00h, 00h, 00h, 08h, 00h, 00h, 00h, 00h, 01h
	db	 64h, 3Fh,0FDh,0FFh, 6Bh, 7Ch, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 03h,0D7h,0FFh,0FDh, 10h, 01h,0A0h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0D8h, 29h, 4Fh,0A2h, 80h,0B5h, 90h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0D8h, 29h, 4Fh,0A2h, 80h,0B5h, 90h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 05h, 10h,0B0h, 4Ch, 0Bh, 08h, 40h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0A0h,0C9h, 0Ah, 00h, 00h, 42h, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 04h, 10h, 40h, 21h, 64h, 09h,0A0h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0A2h,0C1h, 35h, 8Ch, 00h, 44h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0A2h,0C1h, 35h, 8Ch, 00h, 44h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 00h, 16h, 00h, 21h, 14h, 09h,0A0h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0A5h, 00h,0B5h, 80h, 83h, 44h, 0Ch, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 00h, 26h, 00h, 12h, 08h, 09h,0A0h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0DAh, 00h,0B4h, 40h, 60h, 44h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0DAh, 00h,0B4h, 40h, 60h, 44h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h,0C8h, 00h, 00h, 04h, 01h,0A0h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0A2h, 20h, 89h, 91h, 10h, 8Ah, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 44h, 06h, 00h, 00h, 00h, 30h, 40h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 44h, 06h, 00h, 00h, 00h, 30h, 40h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
;*-------------------------------------------------------------------- *

;*-------------------------------------------------------------------- *
;* Bitmap�������ݱ�                                                    *
;* ͼƬ: E:\ͼ��13.bmp,����ȡģ���λ,��������:�����Ҵ��ϵ���        *
;* ͼƬ�ߴ�: 128 * 64                                                  *
;*-------------------------------------------------------------------- *
TU_TAB4:                                   ; Ư�����￴����


	db  0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 11h,0FFh, 80h, 40h, 08h, 30h, 10h, 20h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 68h, 3Fh,0FFh, 08h, 30h, 11h,0F8h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 68h, 3Fh,0FFh, 08h, 30h, 11h,0F8h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h,0FFh, 00h, 00h, 7Fh,0FEh, 7Dh, 08h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 11h, 6Bh, 07h,0FCh, 16h, 30h, 2Dh,0F8h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Dh,0FFh, 04h, 0Ch, 16h, 30h, 2Dh, 08h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 00h, 3Fh,0FFh, 16h, 30h, 2Dh,0F8h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 00h, 3Fh,0FFh, 16h, 30h, 2Dh,0F8h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch,0FFh, 30h, 01h, 66h,0FEh, 2Dh, 26h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 1Ch, 00h, 03h,0F0h, 76h, 82h, 2Dh, 28h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Dh,0FFh, 82h, 11h, 08h, 82h, 11h, 10h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 8Bh, 04h, 11h, 16h, 82h, 2Dh, 28h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Ch, 8Bh, 04h, 11h, 16h, 82h, 2Dh, 28h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0Dh, 18h,0B8h, 1Fh, 60h,0FEh, 41h,0C6h, 00h
	db	 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 07h, 8Fh,0FFh, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 08h, 70h, 40h,0EFh, 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 08h, 70h, 40h,0EFh, 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 00h, 10h, 00h, 80h, 10h, 60h, 00h, 0Fh,0FFh, 30h, 0Ch, 00h, 80h, 00h, 00h, 00h
	db	 00h, 20h, 18h, 38h, 20h, 60h, 00h, 00h, 60h, 08h, 0Ch, 1Fh,0FCh, 1Ch, 00h, 00h
	db	 00h,0C0h, 64h, 45h, 00h, 60h, 00h, 03h,0FCh, 0Bh,0FFh, 00h, 80h, 3Eh, 00h, 00h
	db	 00h,0C1h, 00h, 01h, 00h, 10h, 00h, 00h, 80h, 00h, 0Ch, 08h, 88h, 3Eh, 00h, 00h
	db	 00h,0C1h, 00h, 01h, 00h, 10h, 00h, 00h, 80h, 00h, 0Ch, 08h, 88h, 3Eh, 00h, 00h
	db	 01h, 01h, 00h, 00h,0C0h, 10h, 00h, 1Fh,0FFh,0BDh, 8Ch, 06h,0B0h, 3Eh, 00h, 00h
	db	 02h, 0Eh, 18h, 1Ch,0C0h, 10h, 00h, 01h, 00h, 04h, 4Ch, 7Fh,0FEh, 1Ch, 00h, 00h
	db	 02h, 16h, 64h, 22h, 20h, 10h, 00h, 03h,0FCh, 04h, 4Ch, 01h,0C0h, 1Ch, 00h, 00h
	db	 02h, 16h, 64h, 22h, 20h, 10h, 00h, 03h,0FCh, 04h, 4Ch, 01h,0C0h, 1Ch, 00h, 00h
	db	 04h,0E8h, 83h, 41h, 1Ch, 08h, 00h, 0Dh, 04h, 04h, 0Ch, 06h,0B0h, 00h, 00h, 00h
	db	 07h, 10h, 8Fh, 47h, 03h, 88h, 00h, 11h,0FCh, 04h, 3Ch, 08h, 88h, 1Ch, 00h, 00h
	db	 00h, 20h, 6Ch, 26h, 01h, 78h, 00h, 01h, 04h, 0Ah, 00h, 70h, 86h, 1Ch, 00h, 00h
	db	 00h,0C0h, 18h, 1Ch, 00h, 80h, 00h, 01h,0FCh, 31h,0FFh, 00h, 80h, 00h, 00h, 00h
	db	 00h,0C0h, 18h, 1Ch, 00h, 80h, 00h, 01h,0FCh, 31h,0FFh, 00h, 80h, 00h, 00h, 00h
	db	 00h,0C1h, 87h, 80h, 00h, 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 01h, 06h, 78h, 3Ch, 20h, 60h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 01h, 08h, 07h,0C2h,0C0h, 60h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 01h, 08h, 00h, 01h,0C0h, 60h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 01h, 08h, 00h, 01h,0C0h, 60h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 01h, 08h, 18h, 01h,0C0h, 60h, 00h, 00h, 00h,0F1h,0F3h, 87h,0C0h, 00h, 00h, 00h
	db	 00h,0C6h, 18h,0C2h, 20h, 80h, 00h, 00h, 03h, 0Eh, 1Ch, 78h, 70h, 00h, 00h, 00h
	db	 00h,0C1h, 00h,0C2h, 00h, 80h, 00h, 00h, 04h, 44h, 11h, 10h, 38h, 00h, 00h, 00h
	db	 00h, 26h,0E0h, 04h, 01h, 00h, 00h, 00h, 04h, 80h, 12h, 00h, 38h, 00h, 00h, 00h
	db	 00h, 26h,0E0h, 04h, 01h, 00h, 00h, 00h, 04h, 80h, 12h, 00h, 38h, 00h, 00h, 00h
	db	 00h, 11h, 18h, 38h, 03h, 00h, 00h, 00h, 04h, 80h, 12h, 00h, 38h, 00h, 00h, 00h
	db	 00h, 0Eh, 87h,0C2h, 1Ch, 00h, 00h, 00h, 04h, 00h, 10h, 00h, 38h, 00h, 00h, 00h
	db	 00h, 01h,0E0h, 04h,0E0h, 00h, 00h, 00h, 03h, 00h, 1Ch, 00h, 70h, 00h, 00h, 00h
	db	 00h, 1Eh, 18h, 3Fh, 1Eh, 00h, 00h, 00h, 00h, 80h, 32h, 00h,0C0h, 00h, 00h, 00h
	db	 00h, 1Eh, 18h, 3Fh, 1Eh, 00h, 00h, 00h, 00h, 80h, 32h, 00h,0C0h, 00h, 00h, 00h
	db	 00h, 38h, 07h,0C0h, 0Fh, 00h, 00h, 00h, 00h, 40h, 61h, 01h, 80h, 00h, 00h, 00h
	db	 00h,0E9h,0FFh,0FFh, 0Dh, 80h, 00h, 00h, 00h, 31h,0C0h, 87h, 00h, 00h, 00h, 00h
	db	 01h,0E9h, 1Bh,0B9h, 0Dh,0E0h, 00h, 00h, 00h, 0Bh, 80h, 6Eh, 00h, 00h, 00h, 00h
	db	 03h,0C9h,0F0h, 1Fh, 0Ch,0F0h, 00h, 00h, 00h, 06h, 00h, 18h, 00h, 00h, 00h, 00h
	db	 03h,0C9h,0F0h, 1Fh, 0Ch,0F0h, 00h, 00h, 00h, 06h, 00h, 18h, 00h, 00h, 00h, 00h
	db	 07h, 06h,0E0h, 06h, 10h, 78h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 07h,0C1h, 00h, 00h, 20h,0F8h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 07h,0E0h, 80h, 00h,0C1h,0F8h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 07h,0E0h, 80h, 00h,0C1h,0F8h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
	db	 80h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 01h
	db	0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh,0FFh


end
