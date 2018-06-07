VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "单片机串口继电器控制系统 ---------- WWW.RICHMCU.COM"
   ClientHeight    =   3345
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7410
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3345
   ScaleWidth      =   7410
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '屏幕中心
   Begin VB.CommandButton Command2 
      Caption         =   "继电器断开"
      Height          =   525
      Left            =   5295
      TabIndex        =   22
      Top             =   2250
      Width           =   1245
   End
   Begin VB.CommandButton Command1 
      Caption         =   "继电器吸合"
      Height          =   510
      Left            =   3690
      TabIndex        =   21
      Top             =   2250
      Width           =   1245
   End
   Begin VB.Frame Frame1 
      Caption         =   "控制面板"
      Height          =   3120
      Left            =   3120
      TabIndex        =   23
      Top             =   90
      Width           =   4140
      Begin VB.Label Label1 
         Caption         =   "继电器工作指示灯"
         Height          =   345
         Left            =   1245
         TabIndex        =   24
         Top             =   1470
         Width           =   1635
      End
      Begin VB.Shape Shape1 
         FillColor       =   &H00FFFFFF&
         FillStyle       =   0  'Solid
         Height          =   630
         Left            =   1605
         Shape           =   3  'Circle
         Top             =   675
         Width           =   765
      End
   End
   Begin VB.Frame fraFrames 
      Caption         =   "串口连接选项"
      Height          =   3210
      Index           =   2
      Left            =   30
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
         Left            =   1530
         TabIndex        =   3
         Top             =   1590
         Width           =   1230
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "连接"
         Height          =   375
         Left            =   75
         TabIndex        =   2
         Top             =   1575
         Width           =   1230
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "退出"
         Height          =   375
         Left            =   780
         TabIndex        =   1
         Top             =   2475
         Width           =   1230
      End
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Caption         =   "空闲"
         Height          =   240
         Left            =   105
         TabIndex        =   4
         Top             =   1305
         Width           =   2850
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


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
If ComPort.PortOpen = True Then

ComPort.Output = "1"
Shape1.FillColor = &HFF
End If
End Sub

Private Sub Command2_Click()

If ComPort.PortOpen = True Then
ComPort.Output = "2"
Shape1.FillColor = &HFFFFFF
End If
End Sub

Private Sub Form_Load()
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

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)

    '断开连接并退出
    If ComPort.PortOpen = True Then ComPort.PortOpen = False

End Sub

