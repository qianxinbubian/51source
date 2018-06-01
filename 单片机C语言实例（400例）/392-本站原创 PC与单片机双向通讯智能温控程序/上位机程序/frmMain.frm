VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmMain 
   Caption         =   "串口单片机温度控制系统 ---------- WWW.RICHMCU.COM"
   ClientHeight    =   9090
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9450
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   9090
   ScaleWidth      =   9450
   StartUpPosition =   2  '屏幕中心
   Begin zhinengwenkong.ucHistogram ucHistogram1 
      Height          =   5085
      Left            =   120
      TabIndex        =   25
      Top             =   3930
      Width           =   9270
      _ExtentX        =   16351
      _ExtentY        =   8969
      GridTickHorizontal=   5
      GridTickVertical=   10
      Max             =   99
   End
   Begin VB.TextBox Text2 
      Height          =   330
      Left            =   6870
      TabIndex        =   22
      Top             =   1605
      Width           =   1275
   End
   Begin VB.TextBox Text1 
      Height          =   225
      Left            =   1305
      TabIndex        =   21
      Top             =   435
      Visible         =   0   'False
      Width           =   135
   End
   Begin VB.Frame Frame1 
      Caption         =   "操作项"
      Height          =   990
      Left            =   210
      TabIndex        =   16
      Top             =   2730
      Width           =   4200
      Begin VB.CommandButton Command8 
         Caption         =   "退出设定"
         Height          =   375
         Left            =   3000
         TabIndex        =   20
         Top             =   330
         Width           =   870
      End
      Begin VB.CommandButton Command3 
         Caption         =   "增加"
         Height          =   375
         Left            =   2040
         TabIndex        =   19
         Top             =   330
         Width           =   870
      End
      Begin VB.CommandButton Command2 
         Caption         =   "减少"
         Height          =   375
         Left            =   120
         TabIndex        =   18
         Top             =   330
         Width           =   870
      End
      Begin VB.CommandButton Command4 
         Caption         =   "设定温度"
         Height          =   375
         Left            =   1080
         TabIndex        =   17
         Top             =   330
         Width           =   870
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "状态显示窗口"
      Height          =   1680
      Left            =   4530
      TabIndex        =   13
      Top             =   2130
      Width           =   4905
      Begin VB.Label Label4 
         Caption         =   "设置"
         Height          =   300
         Left            =   1935
         TabIndex        =   23
         Top             =   1320
         Width           =   615
      End
      Begin VB.Shape Shape3 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   555
         Left            =   1575
         Shape           =   3  'Circle
         Top             =   465
         Width           =   1155
      End
      Begin VB.Label Label3 
         Caption         =   "超温"
         Height          =   285
         Left            =   3090
         TabIndex        =   15
         Top             =   1320
         Width           =   480
      End
      Begin VB.Label Label2 
         Caption         =   "正常"
         Height          =   255
         Left            =   810
         TabIndex        =   14
         Top             =   1320
         Width           =   495
      End
      Begin VB.Shape Shape2 
         BackStyle       =   1  'Opaque
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   570
         Left            =   2850
         Shape           =   3  'Circle
         Top             =   450
         Width           =   810
      End
      Begin VB.Shape Shape1 
         BackColor       =   &H00FFFFFF&
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   555
         Left            =   705
         Shape           =   3  'Circle
         Top             =   420
         Width           =   675
      End
   End
   Begin zhinengwenkong.MorphDisplay Mo 
      Height          =   1155
      Left            =   6870
      TabIndex        =   12
      Top             =   375
      Width           =   1410
      _ExtentX        =   2487
      _ExtentY        =   2037
      BurnInColor     =   96
      BurnInColorNeg  =   96
      InterDigitGapExp=   5
      InterSegmentGap =   5
      NumDigits       =   2
      SegmentHeight   =   20
      SegmentHeightExp=   10
      SegmentLitColor =   65535
      SegmentLitColorNeg=   255
      SegmentStyle    =   0
      SegmentWidth    =   8
      SegmentWidthExp =   8
      Theme           =   2
      Value           =   "000"
      XOffsetExp      =   490
   End
   Begin VB.Frame fraFrames 
      Caption         =   "串口连接选项"
      Height          =   3705
      Index           =   2
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4350
      Begin VB.Timer Timer2 
         Interval        =   300
         Left            =   1170
         Top             =   1875
      End
      Begin VB.Timer Timer1 
         Interval        =   1000
         Left            =   1440
         Top             =   975
      End
      Begin MSCommLib.MSComm ComPort 
         Left            =   4920
         Top             =   120
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM4"
         Height          =   255
         Index           =   3
         Left            =   480
         TabIndex        =   10
         Top             =   1320
         Width           =   1335
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM3"
         Height          =   255
         Index           =   2
         Left            =   480
         TabIndex        =   9
         Top             =   1080
         Width           =   1335
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM2"
         Height          =   255
         Index           =   1
         Left            =   480
         TabIndex        =   8
         Top             =   840
         Width           =   1335
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM1"
         Height          =   255
         Index           =   0
         Left            =   480
         TabIndex        =   7
         Top             =   600
         Value           =   -1  'True
         Width           =   1335
      End
      Begin VB.CommandButton cmdDisconnect 
         Caption         =   "断开"
         Enabled         =   0   'False
         Height          =   375
         Left            =   2280
         TabIndex        =   5
         Top             =   1065
         Width           =   1845
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "连接"
         Height          =   375
         Left            =   2280
         TabIndex        =   4
         Top             =   600
         Width           =   1845
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "退出"
         Height          =   375
         Left            =   2280
         TabIndex        =   3
         Top             =   1590
         Width           =   1845
      End
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Caption         =   "空闲"
         Height          =   255
         Left            =   330
         TabIndex        =   6
         Top             =   2205
         Width           =   3570
      End
   End
   Begin VB.Frame fraFrames 
      Caption         =   "温度显示窗口"
      Height          =   1905
      Index           =   1
      Left            =   4545
      TabIndex        =   0
      Top             =   195
      Width           =   4860
      Begin VB.TextBox txtBarcode 
         Alignment       =   2  'Center
         Height          =   300
         Left            =   3105
         Locked          =   -1  'True
         TabIndex        =   1
         Top             =   465
         Visible         =   0   'False
         Width           =   705
      End
      Begin VB.Label Label5 
         Caption         =   "报警设定温度："
         Height          =   240
         Left            =   795
         TabIndex        =   24
         Top             =   1425
         Width           =   1320
      End
      Begin VB.Label Label1 
         Caption         =   "当前温度："
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   15.75
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   435
         Left            =   585
         TabIndex        =   11
         Top             =   570
         Width           =   1725
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


'Option Explicit

Private iniPath As String
Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpRetunedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As String, ByVal lplFileName As String) As Long
Private Declare Function sndPlaySound Lib "winmm.dll" Alias "sndPlaySoundA" _
    (ByVal lpszSoundName As String, ByVal uFlags As Long) As Long
    Const SND_SYNC = &H0
    Const SND_ASYNC = &H1
    Const SND_NODEFAULT = &H2
    Const SND_LOOP = &H8
    Const SND_NOSTOP = &H10
Dim hi
Dim ss
Dim sBarcodeTemp As String
Dim sInTemp As String
Function GetFromINI(AppName As String, KeyName As String, Filename As String) As String
   Dim RetStr As String
   RetStr = String(255, Chr(0))
   GetFromINI = Left(RetStr, GetPrivateProfileString(AppName, ByVal KeyName, "", RetStr, Len(RetStr), Filename))
End Function

Private Sub cmdClear_Click()
    lstBarcodes.Clear
    txtBarcode.Text = ""
End Sub

Private Sub cmdConnect_Click()

    '查找指定端口
    Dim i As Integer
    For i = 1 To 4
        If optComPort(i - 1).Value = True Then
            ComPort.CommPort = i
            Exit For '跳出循环
        End If
    Next
    
    If ComPort.PortOpen = True Then ComPort.PortOpen = False  '如果端口打开则先关闭
    ComPort.PortOpen = True  '然后打开
    
    '状态信息
    lblStatus = "已连接..."
    Text1.Text = "EXIT"
    
    
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
Dim strsent As String
strsent = Trim(txtsent.Text)
If (strsent = "") Then
MsgBox "发送数据不能为空!", vbOKOnly, "提示"
txtsent.SetFocus
Else
ComPort.Output = strsent
'MsgBox "已经执行发送!", vbOKOnly, "提示"
End If
End Sub



Private Sub Command2_Click()
ComPort.Output = "4"
End Sub

Private Sub Command3_Click()
ComPort.Output = "5"
End Sub

Private Sub Command4_Click()
ComPort.Output = "3"
End Sub

Private Sub Command8_Click()
ComPort.Output = "6"
iniPath = App.Path & "\hifans.ini"
hfs = WritePrivateProfileString("TEMP", "SETTEMP", Mo.Tag, iniPath)
End Sub

'串口时间
Private Sub ComPort_OnComm()
    '如果已经接收数据，则继续
    On Error Resume Next
    If ComPort.CommEvent <> comEvReceive Then Exit Sub
     
    '读取数据
    sInTemp = ComPort.Input
If sInTemp = "O" Then
Text1.Text = "OK"
Shape3.FillColor = &HFF
 End If
 
 If sInTemp = "N" Then
Text1.Text = "EXIT"
Shape3.FillColor = &HFFFFFF
iniPath = App.Path & "\hifans.ini"
hfs = WritePrivateProfileString("TEMP", "SETTEMP", Mo.Tag, iniPath)
 End If
 If Text1.Text = "OK" Then
 Label1.Caption = "报警温度："
 Mo.Tag = Mo.Value
 End If
 Text2.Text = Mo.Tag
 If Text1.Text = "EXIT" Then Label1.Caption = "当前温度："
    sBarcodeTemp = sBarcodeTemp & sInTemp

    '判断是否完成
    If Right$(sBarcodeTemp, 2) = vbNewLine Then
    
        'If chkWedge Then SendKeys sBarcodeTemp
        
       sBarcodeTemp = Left$(sBarcodeTemp, Len(sBarcodeTemp) - 3)
        
        
        '放到文本框
        txtBarcode = sBarcodeTemp
       Mo.Value = txtBarcode
        
        
       'If Text1.Text = "EXIT" Then Text2.Text = txtBarcode
        'If Text1.Text = "OK" Then Text2.Text = txtBarcode
       ' If Text1.Text = "OK" Then Text2.Text = txtBarcode
      ' If  Then
       If txtBarcode >= hi And Label1.Caption = "当前温度：" Then
        'ComPort.Output = "4"
      Shape2.FillColor = &HFF
        Shape1.FillColor = &HFFFFFF
        
       ss = 1
       Else
        'ComPort.Output = "5"
        
         Shape1.FillColor = &HFF
          Shape2.FillColor = &HFFFFFF
          ss = 0
StopTheSoundNOW = sndPlaySound(soundfile$, wFlags%)
      End If
       If Label1.Caption = "报警温度：" Then
        Shape3.FillColor = &HFF
         Shape2.FillColor = &HFFFFFF
        Shape1.FillColor = &HFFFFFF
        End If

        '放到列表框
        'lstBarcodes.AddItem sBarcodeTemp, 0
        
        '清空临时变量
        sBarcodeTemp = ""
        
        '状态
        lblStatus = "连接温度控制器成功！"
       If lblStatus = "连接温度控制器成功！" Then
        hi = GetFromINI("TEMP", "SETTEMP", iniPath)
    Text2.Text = hi
    End If

    End If
       
End Sub

Private Sub Form_Load()

iniPath = App.Path & "\hifans.ini"
 txtBarcode = 0
 
 'r = GetFromINI("SETUP", "SOUNDCARD", iniPath)
    'z = GetFromINI("SETUP", "SOUNDDEVICES", iniPath)
    hi = GetFromINI("TEMP", "SETTEMP", iniPath)
    '端口循环计数器
    Dim iComPort As Integer

    '错误陷阱
    On Error GoTo CommErrorHandle
    
    '尝试列表存在端口
    For iComPort = 1 To 4
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

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)

    '断开连接并退出
    If ComPort.PortOpen = True Then ComPort.PortOpen = False

End Sub

Private Sub Timer1_Timer()
If ss = 1 Then
soundfile$ = App.Path + "\BUZZ5.wav"
    wFlags% = SND_ASYNC Or SND_NODEFAULT
    HaHa = sndPlaySound(soundfile$, wFlags%)
    End If
End Sub

Private Sub Timer2_Timer()
On Error Resume Next
ucHistogram1.NextPoint txtBarcode
End Sub
