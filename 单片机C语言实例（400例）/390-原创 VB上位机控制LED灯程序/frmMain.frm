VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "单片机上位机LED控制系统 ---------- WWW.RICHMCU.COM"
   ClientHeight    =   6750
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8415
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6750
   ScaleWidth      =   8415
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton Command5 
      Caption         =   "停止自动发送"
      Height          =   405
      Left            =   990
      TabIndex        =   47
      Top             =   5685
      Width           =   1815
   End
   Begin VB.CommandButton Command2 
      Caption         =   "自动发送00-FF数据"
      Height          =   405
      Left            =   990
      TabIndex        =   45
      Top             =   5085
      Width           =   1815
   End
   Begin VB.CommandButton Command1 
      Caption         =   "发送00-FF任意数据"
      Height          =   555
      Left            =   4215
      TabIndex        =   44
      Top             =   5220
      Width           =   1290
   End
   Begin VB.TextBox Text7 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   26.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   690
      Left            =   5670
      MaxLength       =   2
      TabIndex        =   43
      Text            =   "00"
      Top             =   5145
      Width           =   1185
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   2310
      Top             =   1770
   End
   Begin VB.TextBox Text3 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   26.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   675
      Left            =   3600
      TabIndex        =   31
      Text            =   "11111111"
      Top             =   2610
      Width           =   4560
   End
   Begin VB.TextBox Text2 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   26.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   675
      Left            =   3570
      TabIndex        =   30
      Text            =   "FF"
      Top             =   3450
      Width           =   4560
   End
   Begin VB.Frame Frame3 
      Caption         =   "P0口LED灯"
      Height          =   1125
      Left            =   3390
      TabIndex        =   21
      Top             =   240
      Width           =   4815
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   7
         Left            =   615
         TabIndex        =   40
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   6
         Left            =   1080
         TabIndex        =   39
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   5
         Left            =   1515
         TabIndex        =   38
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   4
         Left            =   1965
         TabIndex        =   37
         Top             =   720
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   3
         Left            =   2400
         TabIndex        =   36
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   2
         Left            =   2850
         TabIndex        =   35
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   1
         Left            =   3300
         TabIndex        =   34
         Top             =   735
         Width           =   285
      End
      Begin VB.CheckBox Check2 
         Height          =   225
         Index           =   0
         Left            =   3750
         TabIndex        =   33
         Top             =   735
         Width           =   285
      End
      Begin VB.Label Label16 
         Caption         =   "L0.0"
         Height          =   270
         Left            =   3645
         TabIndex        =   29
         Top             =   525
         Width           =   405
      End
      Begin VB.Label Label15 
         Caption         =   "L0.1"
         Height          =   240
         Left            =   3210
         TabIndex        =   28
         Top             =   525
         Width           =   495
      End
      Begin VB.Label Label14 
         Caption         =   "L0.2"
         Height          =   240
         Left            =   2760
         TabIndex        =   27
         Top             =   525
         Width           =   450
      End
      Begin VB.Label Label13 
         Caption         =   "L0.3"
         Height          =   270
         Left            =   2325
         TabIndex        =   26
         Top             =   525
         Width           =   480
      End
      Begin VB.Label Label12 
         Caption         =   "L0.4"
         Height          =   285
         Left            =   1875
         TabIndex        =   25
         Top             =   525
         Width           =   375
      End
      Begin VB.Label Label11 
         Caption         =   "L0.5"
         Height          =   285
         Left            =   1425
         TabIndex        =   24
         Top             =   525
         Width           =   495
      End
      Begin VB.Label Label10 
         Caption         =   "L0.6"
         Height          =   285
         Left            =   990
         TabIndex        =   23
         Top             =   525
         Width           =   405
      End
      Begin VB.Label Label9 
         Caption         =   "L0.7"
         Height          =   285
         Left            =   510
         TabIndex        =   22
         Top             =   525
         Width           =   360
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   7
         Left            =   585
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   6
         Left            =   1035
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   5
         Left            =   1470
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   4
         Left            =   1920
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   3
         Left            =   2370
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   2
         Left            =   2805
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   1
         Left            =   3255
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
      Begin VB.Shape Shape2 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   240
         Index           =   0
         Left            =   3705
         Shape           =   3  'Circle
         Top             =   225
         Width           =   285
      End
   End
   Begin VB.Frame fraFrames 
      Caption         =   "串口连接选项"
      Height          =   4410
      Index           =   2
      Left            =   135
      TabIndex        =   0
      Top             =   30
      Width           =   3030
      Begin VB.OptionButton optComPort 
         Caption         =   "COM16"
         Height          =   255
         Index           =   15
         Left            =   2145
         TabIndex        =   20
         Top             =   1005
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM15"
         Height          =   255
         Index           =   14
         Left            =   2145
         TabIndex        =   19
         Top             =   735
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM14"
         Height          =   255
         Index           =   13
         Left            =   2145
         TabIndex        =   18
         Top             =   465
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM13"
         Height          =   255
         Index           =   12
         Left            =   2145
         TabIndex        =   17
         Top             =   195
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM12"
         Height          =   255
         Index           =   11
         Left            =   1410
         TabIndex        =   16
         Top             =   990
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM11"
         Height          =   255
         Index           =   10
         Left            =   1410
         TabIndex        =   15
         Top             =   720
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM10"
         Height          =   255
         Index           =   9
         Left            =   1410
         TabIndex        =   14
         Top             =   465
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM9"
         Height          =   255
         Index           =   8
         Left            =   1410
         TabIndex        =   13
         Top             =   195
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM8"
         Height          =   255
         Index           =   7
         Left            =   735
         TabIndex        =   12
         Top             =   990
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM7"
         Height          =   255
         Index           =   6
         Left            =   735
         TabIndex        =   11
         Top             =   725
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM6"
         Height          =   255
         Index           =   5
         Left            =   735
         TabIndex        =   10
         Top             =   460
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM5"
         Height          =   255
         Index           =   4
         Left            =   735
         TabIndex        =   9
         Top             =   195
         Width           =   795
      End
      Begin MSCommLib.MSComm ComPort 
         Left            =   4920
         Top             =   120
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
         InputMode       =   1
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM4"
         Height          =   255
         Index           =   3
         Left            =   60
         TabIndex        =   8
         Top             =   975
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM3"
         Height          =   255
         Index           =   2
         Left            =   60
         TabIndex        =   7
         Top             =   715
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM2"
         Height          =   255
         Index           =   1
         Left            =   60
         TabIndex        =   6
         Top             =   455
         Width           =   795
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM1"
         Height          =   255
         Index           =   0
         Left            =   60
         TabIndex        =   5
         Top             =   195
         Value           =   -1  'True
         Width           =   795
      End
      Begin VB.CommandButton cmdDisconnect 
         Caption         =   "断开"
         Enabled         =   0   'False
         Height          =   375
         Left            =   1500
         TabIndex        =   3
         Top             =   2400
         Width           =   1230
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "连接"
         Height          =   375
         Left            =   105
         TabIndex        =   2
         Top             =   2385
         Width           =   1230
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "退出"
         Height          =   375
         Left            =   795
         TabIndex        =   1
         Top             =   3270
         Width           =   1230
      End
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Caption         =   "空闲"
         Height          =   240
         Left            =   60
         TabIndex        =   4
         Top             =   1485
         Width           =   2850
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "接收数据显示窗口"
      Height          =   2160
      Left            =   3435
      TabIndex        =   32
      Top             =   2205
      Width           =   4860
   End
   Begin VB.TextBox Text6 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H8000000A&
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   26.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   600
      Left            =   4125
      TabIndex        =   42
      Text            =   "FF"
      Top             =   255
      Visible         =   0   'False
      Width           =   360
   End
   Begin VB.TextBox Text4 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H80000016&
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   26.25
         Charset         =   134
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   600
      Left            =   3675
      TabIndex        =   41
      Text            =   "11111111"
      Top             =   270
      Visible         =   0   'False
      Width           =   360
   End
   Begin VB.TextBox Text1 
      Height          =   225
      Left            =   3645
      TabIndex        =   46
      Text            =   "Text1"
      Top             =   1020
      Visible         =   0   'False
      Width           =   135
   End
   Begin VB.Frame Frame2 
      Caption         =   "操作项"
      Height          =   2085
      Left            =   255
      TabIndex        =   48
      Top             =   4560
      Width           =   8025
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function sndPlaySound Lib "winmm.dll" Alias "sndPlaySoundA" _
    (ByVal lpszSoundName As String, ByVal uFlags As Long) As Long
    Const SND_SYNC = &H0
    Const SND_ASYNC = &H1
    Const SND_NODEFAULT = &H2
    Const SND_LOOP = &H8
    Const SND_NOSTOP = &H10
Dim hi, j, zd
Dim strData As String
Dim bytInput() As Byte
Dim bytSendByte() As Byte    '发送二进制数据
Dim sBarcodeTemp As String
Dim sInTemp As String

Public Function DecimaltoHex(ByVal Dec As Long) As String

    Dim Hex As String

    Dim r As Long

    Dim Maks As Boolean

    If Dec > 0 Then

        Do While Dec <> 0

            r = Abs(Dec Mod 16)

            Dec = Dec \ 16

            Hex = IIf(r > 9, Chr(55 + r), r) & Hex

        Loop

    Else

        Maks = True

        Do While Dec <> 0

            r = 15 - Abs(Dec Mod 16) - Maks

            Maks = r = 16

            r = IIf(r = 16, 0, r)

            Dec = Dec \ 16

            Hex = IIf(r > 9, Chr(55 + r), r) & Hex

        Loop

    End If

   DecimaltoHex = IIf(Len(Hex) = 0, "00", Right("00" & Hex, 2))

End Function

Private Sub Check1_Click(Index As Integer)
Text1.Text = ""
Select Case Index
Case Index:
If Check1.Item(Index).Value = 1 Then
Shape1.Item(Index).FillColor = &HFF
Else
Shape1.Item(Index).FillColor = &HFFFFFF
End If
End Select
For ck = 0 To 7
If Check1.Item(7 - ck).Value = 1 Then
ckv = 0
Else
ckv = 1
End If
Text1.Text = Text1.Text & ckv
Next ck
End Sub

Private Sub Check2_Click(Index As Integer)
If ComPort.PortOpen = False Then
 MsgBox "请先连接串口，然后在操作", vbInformation, "锐志电子温馨提示"
 Exit Sub
 End If
Text4.Text = ""
Select Case Index
Case Index:
If Check2.Item(Index).Value = 1 Then
Shape2.Item(Index).FillColor = &HFF
Else
Shape2.Item(Index).FillColor = &HFFFFFF
End If
End Select
For ck = 0 To 7
If Check2.Item(7 - ck).Value = 1 Then
ckv = 0
Else
ckv = 1
End If
Text4.Text = Text4.Text & ckv
Next ck
 longth = strHexToByteArray(Text6, bytSendByte())
        
        If longth > 0 Then
            ComPort.Output = bytSendByte
        End If
        


End Sub

Private Sub cmdConnect_Click()

    '查找指定端口
    Dim i As Integer
    For i = 1 To 16
        If optComPort(i - 1).Value = True Then
            ComPort.CommPort = i
            Exit For '跳出循环
        End If
    Next
    
    If ComPort.PortOpen = True Then ComPort.PortOpen = False  '如果端口打开则先关闭
    ComPort.PortOpen = True  '然后打开
    
    '状态信息
    lblStatus = "已连接..."
    ''Text1.Text = "EXIT"
    
    
    cmdConnect.Enabled = False
    cmdDisconnect.Enabled = True
    
End Sub

Private Sub cmdDisconnect_Click()

    '断开连接
    If ComPort.PortOpen = True Then ComPort.PortOpen = False
    
    lblStatus = "已断开..."
    
    cmdDisconnect.Enabled = False
    cmdConnect.Enabled = True
    
End Sub

Private Sub cmdExit_Click()

    '先断开端口再退出程序
    If ComPort.PortOpen = True Then ComPort.PortOpen = False
        Unload Me
    End

End Sub

Private Sub Command1_Click()
If ComPort.PortOpen = False Then
 MsgBox "请先连接串口，然后在操作", vbInformation, "锐志电子温馨提示"
 Exit Sub
 End If
 longth = strHexToByteArray(Text7, bytSendByte())
        
        If longth > 0 Then
            ComPort.Output = bytSendByte
        End If
End Sub

Public Function StrtoHex(ByVal strs As String) As String 'str to 16
Dim abytS() As Byte
Dim bytTemp As Byte
Dim strTemp As String
Dim lLocation As Long
abytS = StrConv(strs, vbFromUnicode)
For lLocation = 0 To UBound(abytS)
bytTemp = abytS(lLocation)
strTemp = Hex(bytTemp)
strTemp = Right("00" & strTemp, 2)
StrtoHex = StrtoHex & strTemp
Next lLocation
End Function
Public Function HextoStr(ByVal strs As String) As String '16 to str
Dim i    As Integer, tmp       As String
If Len(strs) Mod 2 Then Exit Function
For i = 1 To Len(strs) Step 2
          n = Val("&H" & Mid(strs, i, 2))
          If n < 0 Or n > 127 Then
                  n = Val("&H" & Mid(strs, i, 4))
                    i = i + 2
          End If
          tmp = tmp & Chr(n)
Next i
HextoStr = tmp
End Function





Private Sub Command2_Click()
If ComPort.PortOpen = False Then
 MsgBox "请先连接串口，然后在操作", vbInformation, "锐志电子温馨提示"
 Exit Sub
 End If


zd = 0
Timer1.Enabled = True
End Sub




Public Function D_To_B(ByVal Dec As Long) As String
    Do
        D_To_B = Dec Mod 2 & D_To_B
        Dec = Dec \ 2
    Loop While Dec
End Function

Public Function B_To_D(ByVal Bin As String) As Integer
    Dim i As Long
    For i = 1 To Len(Bin)
        B_To_D = B_To_D * 2 + Val(Mid(Bin, i, 1))
    Next i
End Function

Public Function H_To_B(ByVal Hex As String) As String
    Dim i As Long
    Dim B As String
    
    Hex = UCase(Hex)
    For i = 1 To Len(Hex)
        Select Case Mid(Hex, i, 1)
            Case "0": B = B & "0000"
            Case "1": B = B & "0001"
            Case "2": B = B & "0010"
            Case "3": B = B & "0011"
            Case "4": B = B & "0100"
            Case "5": B = B & "0101"
            Case "6": B = B & "0110"
            Case "7": B = B & "0111"
            Case "8": B = B & "1000"
            Case "9": B = B & "1001"
            Case "A": B = B & "1010"
            Case "B": B = B & "1011"
            Case "C": B = B & "1100"
            Case "D": B = B & "1101"
            Case "E": B = B & "1110"
            Case "F": B = B & "1111"
        End Select
    Next i
    While Left(B, 1) = "0"
        B = Right(B, Len(B) - 1)
    Wend
    H_To_B = Format(B, "00000000")
End Function

Public Function B_To_H(ByVal Bin As String) As String
    Dim i As Long
    Dim H As String
    If Len(Bin) Mod 4 <> 0 Then
        Bin = String(4 - Len(Bin) Mod 4, "0" & Bin)
    End If
    
    For i = 1 To Len(Bin) Step 4
        Select Case Mid(Bin, i, 4)
            Case "0000": H = H & "0"
            Case "0001": H = H & "1"
            Case "0010": H = H & "2"
            Case "0011": H = H & "3"
            Case "0100": H = H & "4"
            Case "0101": H = H & "5"
            Case "0110": H = H & "6"
            Case "0111": H = H & "7"
            Case "1000": H = H & "8"
            Case "1001": H = H & "9"
            Case "1010": H = H & "A"
            Case "1011": H = H & "B"
            Case "1100": H = H & "C"
            Case "1101": H = H & "D"
            Case "1110": H = H & "E"
            Case "1111": H = H & "F"
        End Select
    Next i
    B_To_H = H
End Function
Function Long2Bin(Data As Long) As String
    Dim tmp As String
    tmp = ""
    tmp = tmp & IIf(Data And 32768, "1", "0")
    tmp = tmp & IIf(Data And 16384, "1", "0")
    tmp = tmp & IIf(Data And 8192, "1", "0")
    tmp = tmp & IIf(Data And 4096, "1", "0")
    tmp = tmp & IIf(Data And 2048, "1", "0")
    tmp = tmp & IIf(Data And 1024, "1", "0")
    tmp = tmp & IIf(Data And 512, "1", "0")
    tmp = tmp & IIf(Data And 256, "1", "0")
    tmp = tmp & IIf(Data And 128, "1", "0")
    tmp = tmp & IIf(Data And 64, "1", "0")
    tmp = tmp & IIf(Data And 32, "1", "0")
    tmp = tmp & IIf(Data And 16, "1", "0")
    tmp = tmp & IIf(Data And 8, "1", "0")
    tmp = tmp & IIf(Data And 4, "1", "0")
    tmp = tmp & IIf(Data And 2, "1", "0")
    tmp = tmp & IIf(Data And 1, "1", "0")
    Long2Bin = tmp
End Function






Private Sub Command5_Click()
Timer1.Enabled = False
End Sub

Private Sub Form_Load()


    '端口循环计数器
    Dim iComPort As Integer

    '错误陷阱
    On Error GoTo CommErrorHandle
    
    '尝试列表存在端口
    For iComPort = 1 To 16
        ComPort.CommPort = iComPort '指定端口号
        If ComPort.PortOpen = True Then ComPort.PortOpen = False '如打开先关闭
        ComPort.PortOpen = True  '尝试打开
        ComPort.PortOpen = False '确认成功关闭
    Next
    
    '端口配置
    ComPort.InputLen = 1    '1 个字符产生接收事件
    ComPort.RThreshold = 1  '1 个字符产生接收事件
    
    '跳出错误
    Exit Sub
CommErrorHandle:

    '68   = 设备无效
    '8002 = 端口号无效
    '8012 = 端口无法打开
    If Err = 68 Or Err = 8002 Or Err = 8012 Then
        '端口无效时则禁止单击连接按钮
        optComPort(iComPort - 1).Enabled = False
    End If
    
    '继续错误
    Resume Next
    
End Sub

Private Sub ComPort_OnComm()
'如果已经接收数据，则继续
    On Error Resume Next
    If ComPort.CommEvent <> comEvReceive Then Exit Sub
    Dim intInputLen As Integer
    Select Case Me.ComPort.CommEvent
        Case comEvReceive
            '此处添加处理接收的代码
            ComPort.InputMode = comInputModeBinary '二进制接收
            intInputLen = ComPort.InBufferCount
            ReDim bytInput(intInputLen)
            bytInput = ComPort.Input
            
            jieshou
           
    End Select
End Sub

Public Function jieshou() '接收数据处理为16进制
    Dim i As Integer
    For i = 0 To UBound(bytInput)
        If Len(Hex(bytInput(i))) = 1 Then
            strData = strData & "0" & Hex(bytInput(i))
            'Debug.Print strData
        Else
            strData = strData & Hex(bytInput(i))
          
        End If
       
         Text3 = Hex(bytInput(i))
           Text2 = Right$("00" & Text3, 2)
            Text3 = H_To_B(Text3)
            If Text2 = "00" Then
            Text3 = "00000000"
            End If
            
For ii = 1 To 8
df = Mid$(Text3, ii, 1)
    If df = 0 Then
    Shape2.Item(7 - (ii - 1)).FillColor = &HFF
    'Check2.Item(7 - (ii - 1)).Value = 1
    Else
    Shape2.Item(7 - (ii - 1)).FillColor = &HFFFFFF
     'Check2.Item(7 - (ii - 1)).Value = 0
    End If
    Next ii
    Next
    'Text2 = strData
   
End Function

'**********************************
'字符表示的十六进制数转化为相应的整数
'错误则返回  -1
'**********************************

Function ConvertHexChr(str As String) As Integer
    
    Dim test As Integer
    
    test = Asc(str)
    If test >= Asc("0") And test <= Asc("9") Then
        test = test - Asc("0")
    ElseIf test >= Asc("a") And test <= Asc("f") Then
        test = test - Asc("a") + 10
    ElseIf test >= Asc("A") And test <= Asc("F") Then
        test = test - Asc("A") + 10
    Else
        test = -1                                       '出错信息
    End If
    ConvertHexChr = test
    
End Function




Function strHexToByteArray(strText As String, bytByte() As Byte) As Integer
    
    Dim HexData As Integer          '十六进制(二进制)数据字节对应值
    Dim hstr As String * 1          '高位字符
    Dim lstr As String * 1          '低位字符
    Dim HighHexData As Integer      '高位数值
    Dim LowHexData As Integer       '低位数值
    Dim HexDataLen As Integer       '字节数
    Dim StringLen As Integer        '字符串长度
    Dim Account As Integer          '计数
        
    strTestn = ""                   '设初值
    HexDataLen = 0
    strHexToByteArray = 0
    
    StringLen = Len(strText)
    Account = StringLen \ 2
    ReDim bytByte(Account)
    
    For n = 1 To StringLen
    
        Do                                              '清除空格
            hstr = Mid(strText, n, 1)
            n = n + 1
            If (n - 1) > StringLen Then
                HexDataLen = HexDataLen - 1
                
                Exit For
            End If
        Loop While hstr = " "
        
        Do
            lstr = Mid(strText, n, 1)
            n = n + 1
            If (n - 1) > StringLen Then
                HexDataLen = HexDataLen - 1
                
                Exit For
            End If
        Loop While lstr = " "
        n = n - 1
        If n > StringLen Then
            HexDataLen = HexDataLen - 1
            Exit For
        End If
        
        HighHexData = ConvertHexChr(hstr)
        LowHexData = ConvertHexChr(lstr)
        
        If HighHexData = -1 Or LowHexData = -1 Then     '遇到非法字符中断转化
            HexDataLen = HexDataLen - 1
            
            Exit For
        Else
            
            HexData = HighHexData * 16 + LowHexData
            bytByte(HexDataLen) = HexData
            HexDataLen = HexDataLen + 1
            
            
        End If
                        
    Next n
    
    If HexDataLen > 0 Then                              '修正最后一次循环改变的数值
        HexDataLen = HexDataLen - 1
        ReDim Preserve bytByte(HexDataLen)
    Else
        ReDim Preserve bytByte(0)
    End If
    
    
    If StringLen = 0 Then                               '如果是空串,则不会进入循环体
        strHexToByteArray = 0
    Else
        strHexToByteArray = HexDataLen + 1
    End If
    
    
End Function



Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)

    '断开连接并退出
    If ComPort.PortOpen = True Then ComPort.PortOpen = False

End Sub



Private Sub Text4_Change()
Text6.Text = B_To_H(Text4.Text)
End Sub
Private Sub Pause(interval)
Current = Timer
Do While Timer - Current < Val(interval)
DoEvents
Loop
End Sub

Private Sub Timer1_Timer()
Text1.Text = DecimaltoHex(zd)
longth = strHexToByteArray(Text1, bytSendByte())
        
        If longth > 0 Then
            ComPort.Output = bytSendByte
        End If
        zd = zd + 1
If zd > 255 Then
zd = 0
End If
   
End Sub
