VERSION 5.00
Object = "{6BF52A50-394A-11D3-B153-00C04F79FAA6}#1.0#0"; "wmp.dll"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   Caption         =   "锐志电子---红外遥控音乐播放器"
   ClientHeight    =   9195
   ClientLeft      =   60
   ClientTop       =   390
   ClientWidth     =   11415
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   9195
   ScaleWidth      =   11415
   StartUpPosition =   2  '屏幕中心
   Begin VB.Frame Frame5 
      Height          =   3375
      Left            =   165
      TabIndex        =   20
      Top             =   5775
      Width           =   11040
      Begin WMPLibCtl.WindowsMediaPlayer wmp1 
         Height          =   3090
         Left            =   90
         TabIndex        =   21
         Top             =   180
         Width           =   10845
         URL             =   ""
         rate            =   1
         balance         =   0
         currentPosition =   0
         defaultFrame    =   ""
         playCount       =   1
         autoStart       =   0   'False
         currentMarker   =   0
         invokeURLs      =   -1  'True
         baseURL         =   ""
         volume          =   50
         mute            =   0   'False
         uiMode          =   "full"
         stretchToFit    =   0   'False
         windowlessVideo =   0   'False
         enabled         =   -1  'True
         enableContextMenu=   -1  'True
         fullScreen      =   0   'False
         SAMIStyle       =   ""
         SAMILang        =   ""
         SAMIFilename    =   ""
         captioningID    =   ""
         enableErrorDialogs=   0   'False
         _cx             =   19129
         _cy             =   5450
      End
   End
   Begin VB.Frame Frame4 
      Caption         =   "歌曲信息"
      Height          =   4470
      Left            =   6840
      TabIndex        =   17
      Top             =   1290
      Width           =   4395
      Begin VB.TextBox Text3 
         Alignment       =   2  'Center
         BackColor       =   &H80000001&
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   18
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FFFF&
         Height          =   480
         Left            =   1725
         Locked          =   -1  'True
         TabIndex        =   28
         Text            =   "00"
         Top             =   3420
         Width           =   2370
      End
      Begin VB.TextBox Text2 
         Alignment       =   2  'Center
         BackColor       =   &H80000001&
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   18
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FFFF&
         Height          =   480
         Left            =   1725
         Locked          =   -1  'True
         TabIndex        =   23
         Text            =   "00:00"
         Top             =   2902
         Width           =   2370
      End
      Begin VB.TextBox Text1 
         Alignment       =   2  'Center
         BackColor       =   &H80000001&
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   18
            Charset         =   134
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FFFF&
         Height          =   480
         Left            =   1725
         Locked          =   -1  'True
         TabIndex        =   22
         Text            =   "00:00"
         Top             =   2385
         Width           =   2370
      End
      Begin VB.CommandButton Command3 
         Caption         =   "退出程序"
         Height          =   375
         Left            =   1005
         TabIndex        =   18
         Top             =   4005
         Width           =   2235
      End
      Begin VB.Label Label5 
         Caption         =   "当前音量"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   12
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   345
         Left            =   645
         TabIndex        =   27
         Top             =   3540
         Width           =   1080
      End
      Begin VB.Label Label4 
         Caption         =   "歌曲总时间"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   12
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   390
         TabIndex        =   25
         Top             =   3030
         Width           =   1350
      End
      Begin VB.Label Label2 
         Caption         =   "当前播放时间"
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   12
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Left            =   120
         TabIndex        =   24
         Top             =   2490
         Width           =   1530
      End
      Begin VB.Label Label1 
         BorderStyle     =   1  'Fixed Single
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   12
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1920
         Left            =   135
         TabIndex        =   19
         Top             =   255
         Width           =   4080
      End
   End
   Begin VB.Frame Frame2 
      Height          =   4425
      Left            =   3300
      TabIndex        =   15
      Top             =   1320
      Width           =   3450
      Begin VB.FileListBox File1 
         BackColor       =   &H00E0E0E0&
         Height          =   4050
         Hidden          =   -1  'True
         Left            =   90
         Pattern         =   $"Form1.frx":08CA
         TabIndex        =   16
         Top             =   225
         Width           =   3270
      End
   End
   Begin VB.Frame Frame1 
      Height          =   4440
      Left            =   105
      TabIndex        =   12
      Top             =   1320
      Width           =   3120
      Begin VB.DriveListBox Drive1 
         BackColor       =   &H00E0E0E0&
         Height          =   300
         Left            =   135
         TabIndex        =   14
         Top             =   195
         Width           =   2895
      End
      Begin VB.DirListBox Dir1 
         BackColor       =   &H00E0E0E0&
         Height          =   3660
         Left            =   105
         TabIndex        =   13
         Top             =   600
         Width           =   2895
      End
   End
   Begin VB.Frame fraFrames 
      Caption         =   "串口连接选项"
      Height          =   1170
      Index           =   2
      Left            =   45
      TabIndex        =   3
      Top             =   45
      Width           =   6720
      Begin VB.CommandButton Command1 
         Caption         =   "设置遥控键值"
         Height          =   360
         Left            =   5175
         TabIndex        =   26
         Top             =   195
         Width           =   1365
      End
      Begin VB.Timer Timer1 
         Enabled         =   0   'False
         Interval        =   1000
         Left            =   5910
         Top             =   675
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM4"
         Height          =   255
         Index           =   3
         Left            =   915
         TabIndex        =   10
         Top             =   585
         Width           =   915
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM3"
         Height          =   255
         Index           =   2
         Left            =   150
         TabIndex        =   9
         Top             =   585
         Width           =   900
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM2"
         Height          =   255
         Index           =   1
         Left            =   930
         TabIndex        =   8
         Top             =   285
         Width           =   915
      End
      Begin VB.OptionButton optComPort 
         Caption         =   "COM1"
         Height          =   255
         Index           =   0
         Left            =   135
         TabIndex        =   7
         Top             =   270
         Value           =   -1  'True
         Width           =   930
      End
      Begin VB.CommandButton cmdDisconnect 
         Caption         =   "断开"
         Enabled         =   0   'False
         Height          =   375
         Left            =   2955
         TabIndex        =   6
         Top             =   195
         Width           =   1035
      End
      Begin VB.CommandButton cmdConnect 
         Caption         =   "连接"
         Height          =   375
         Left            =   1995
         TabIndex        =   5
         Top             =   195
         Width           =   900
      End
      Begin VB.CommandButton cmdExit 
         Caption         =   "退出"
         Height          =   375
         Left            =   4125
         TabIndex        =   4
         Top             =   180
         Width           =   930
      End
      Begin MSCommLib.MSComm ComPort 
         Left            =   1875
         Top             =   540
         _ExtentX        =   1005
         _ExtentY        =   1005
         _Version        =   393216
         DTREnable       =   -1  'True
      End
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Caption         =   "空闲"
         Height          =   255
         Left            =   2430
         TabIndex        =   11
         Top             =   750
         Width           =   3075
      End
   End
   Begin VB.Frame fraFrames 
      Caption         =   "遥控显示窗口"
      Height          =   990
      Index           =   1
      Left            =   6945
      TabIndex        =   0
      Top             =   165
      Width           =   4350
      Begin VB.TextBox txtBarcode 
         Alignment       =   2  'Center
         BackColor       =   &H80000001&
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   21.75
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H0000FFFF&
         Height          =   600
         Left            =   1965
         Locked          =   -1  'True
         TabIndex        =   1
         Top             =   225
         Width           =   945
      End
      Begin VB.Label Label3 
         Caption         =   "当前键值："
         BeginProperty Font 
            Name            =   "宋体"
            Size            =   14.25
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   480
         Left            =   435
         TabIndex        =   2
         Top             =   345
         Width           =   1590
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private iniPath As String
Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpRetunedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As String, ByVal lplFileName As String) As Long
Dim sBarcodeTemp As String
Dim sInTemp As String
Dim k1
Dim k2
Dim k3
Dim k4
Dim k5
Dim k6
Dim k7
Dim k8
Dim k9
Dim k10
Dim k11
Dim k12
Dim k13
Dim hi
Dim ss

Function GetFromINI(AppName As String, KeyName As String, Filename As String) As String
   Dim RetStr As String
   RetStr = String(255, Chr(0))
   GetFromINI = Left(RetStr, GetPrivateProfileString(AppName, ByVal KeyName, "", RetStr, Len(RetStr), Filename))
End Function
Private Function Fun_FloppyDrive(sDrive As String) As Boolean
  On Error Resume Next
  Fun_FloppyDrive = Dir(sDrive) <> ""
  End Function


Private Sub cmdClear_Click()
    lstBarcodes.Clear
    txtBarcode.Text = ""
End Sub



Private Sub Command1_Click()
Form2.Show vbModal
End Sub



Private Sub Command3_Click()
End
End Sub



Private Sub Dir1_Change()
File1.Path = Dir1
End Sub

Private Sub Dir1_Click()
File1.Path = Dir1
End Sub

Private Sub Drive1_Change()
 On Error Resume Next
Dir1.Path = Drive1.Drive
  
   If Err <> 0 Then
       MsgBox "请选择有效磁盘！", vbOKOnly, "错误提示"
    End If

End Sub
Private Sub File1_Click()
On Error Resume Next
wmz = FileLen(File1.Path + "\" + File1.Filename)
rrr = Right(File1.Filename, 4)
rrr = LCase(rrr)
rr = Right$(File1.Filename, 3)
rr = LCase(rr)
If wmz < 1024 Then
filesize = Str(wmz) + "字节"
ElseIf wmz >= 1024 And wmz <= 1048576 Then
filesize = Str(Round((wmz / 1024), 2)) + "KB"
ElseIf wmz > 1048576 Then
filesize = Str(Round((wmz / 1048576), 2)) + "MB"
End If
'filesize = Str(Int((FileLen(File1.Path + "\" + File1.FileName)) / 1024)) + "KB"
wmp1.URL = File1.Path + "\" + File1
Label1.Caption = "当前文件：" + File1.Path + "\" + File1 + vbCrLf + vbCrLf + "文件大小：" + filesize + vbCrLf + vbCrLf
wmp1.Controls.play
Timer1.Enabled = True
Text3.Text = wmp1.settings.volume
End Sub




Private Sub Form_Load()
On Error Resume Next

iniPath = App.Path & "\hifans.ini"
k1 = GetFromINI("IRCODE", "KEYCODE1", iniPath)
k2 = GetFromINI("IRCODE", "KEYCODE2", iniPath)
k3 = GetFromINI("IRCODE", "KEYCODE3", iniPath)
k4 = GetFromINI("IRCODE", "KEYCODE4", iniPath)
k5 = GetFromINI("IRCODE", "KEYCODE5", iniPath)
k6 = GetFromINI("IRCODE", "KEYCODE6", iniPath)
k7 = GetFromINI("IRCODE", "KEYCODE7", iniPath)
k8 = GetFromINI("IRCODE", "KEYCODE8", iniPath)
k9 = GetFromINI("IRCODE", "KEYCODE9", iniPath)
k10 = GetFromINI("IRCODE", "KEYCODE10", iniPath)
k11 = GetFromINI("IRCODE", "KEYCODE11", iniPath)
k12 = GetFromINI("IRCODE", "KEYCODE12", iniPath)
k13 = GetFromINI("IRCODE", "KEYCODE13", iniPath)


'Form2.Show
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

Private Sub Form_Unload(Cancel As Integer)
'On Error Resume Next
'wmp1.Controls.stop
wmp1.Close
'Set wmp1 = Nothing
End
End Sub
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)

    '断开连接并退出
    If ComPort.PortOpen = True Then ComPort.PortOpen = False
    wmp1.Close

End
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

'串口时间
Private Sub ComPort_OnComm()
    '如果已经接收数据，则继续
   On Error Resume Next
    If ComPort.CommEvent <> comEvReceive Then Exit Sub
     
    '读取数据
    sInTemp = ComPort.Input

    
    sBarcodeTemp = sBarcodeTemp & sInTemp
    
    'Text1.Text = sBarcodeTemp

    If Right$(sBarcodeTemp, 2) = vbNewLine Then
    
        'If chkWedge Then SendKeys sBarcodeTemp
     sBarcodeTemp = Left$(sBarcodeTemp, Len(sBarcodeTemp) - 2)
        
        
        '把条码放到文本框
       txtBarcode = sBarcodeTemp
       Form2.Text7 = sBarcodeTemp
        'Mo.Value = txtBarcode
       If txtBarcode = k6 Then
        If File1.ListIndex < File1.ListCount - 1 Then
        File1.ListIndex = File1.ListIndex + 1
        Call File1_Click
End If
End If
          If txtBarcode = k5 Then
          If File1.ListIndex > 0 Then
        File1.ListIndex = File1.ListIndex - 1
        Call File1_Click
 End If
 End If
If txtBarcode = k7 Then
 wmp1.settings.volume = wmp1.settings.volume + 3
 Text3.Text = wmp1.settings.volume
       End If
If txtBarcode = k2 Then
   If Drive1.ListIndex < Drive1.ListCount Then
   Drive1.ListIndex = Drive1.ListIndex + 1
   Call Drive1_Change
   End If
   End If
   
 If txtBarcode = k1 Then

   If Drive1.ListIndex > 0 Then
   Drive1.ListIndex = Drive1.ListIndex - 1
   Call Drive1_Change
   End If
   End If
   
    If txtBarcode = k4 Then

   If Dir1.ListIndex < Dir1.ListCount - 1 Then
   Dir1.ListIndex = Dir1.ListIndex + 1
   End If
   End If
   If txtBarcode = k3 Then

   If Dir1.ListIndex > -1 Then
   Dir1.ListIndex = Dir1.ListIndex - 1
   End If
   End If
   
    If txtBarcode = k9 Then

   
 Dir1.Path = Dir1.List(Dir1.ListIndex)
 End If
If txtBarcode = k10 Then

 On Error Resume Next
 Dir1.Path = "c:\"
 Dir1.Path = Drive1
 End If
   If txtBarcode = k8 Then


        wmp1.settings.volume = wmp1.settings.volume - 3
        Text3.Text = wmp1.settings.volume
        End If
        
        If txtBarcode = k13 Then
          If wmp1.fullScreen = True Then

        wmp1.fullScreen = False
        Else
        wmp1.fullScreen = True
        End If
        
        End If
        
          
      If txtBarcode = k12 Then

If wmp1.settings.mute = True Then
        wmp1.settings.mute = False
        Else
       wmp1.settings.mute = True
       End If
       End If
      If txtBarcode = k11 Then

If wmp1.playState = wmppsPlaying Then
wmp1.Controls.Pause
Else
If wmp1.playState = wmppsPaused Then
wmp1.Controls.play
End If
   End If
   End If
        '放到列表框
       'lstBarcodes.AddItem sBarcodeTemp, 0
        
        
       sBarcodeTemp = ""
        
        
        '状态
        lblStatus = "连接控制器成功！"

    End If
       
End Sub

Private Sub Timer1_Timer()

Text2 = wmp1.currentMedia.durationString
Text1 = wmp1.Controls.currentPositionString

End Sub
Private Sub wmp1_PlayStateChange(ByVal NewState As Long)
If NewState = 1 Then '1为停止(一曲播完)
 If File1.ListIndex < File1.ListCount - 1 Then
        File1.ListIndex = File1.ListIndex + 1
    
        Call File1_Click
     Pause (1)
End If
If File1.ListIndex = File1.ListCount - 1 Then
File1.ListIndex = 0
Call File1_Click
Pause (1)
End If
'wmp1.Controls.play
End If

End Sub
Public Sub Pause(interval)
Current = Timer
Do While Timer - Current < Val(interval)
DoEvents
Loop
End Sub
