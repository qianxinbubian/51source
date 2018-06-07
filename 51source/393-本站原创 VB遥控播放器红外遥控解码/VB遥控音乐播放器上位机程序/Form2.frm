VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "遥控键值设置"
   ClientHeight    =   8085
   ClientLeft      =   45
   ClientTop       =   375
   ClientWidth     =   4665
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8085
   ScaleWidth      =   4665
   StartUpPosition =   2  '屏幕中心
   Begin VB.OptionButton Opt13 
      Height          =   255
      Left            =   3840
      TabIndex        =   41
      Top             =   6420
      Width           =   720
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   12
      Left            =   1440
      TabIndex        =   40
      Text            =   "Text1"
      Top             =   6285
      Width           =   1950
   End
   Begin VB.OptionButton Opt12 
      Height          =   255
      Left            =   3840
      TabIndex        =   38
      Top             =   5955
      Width           =   495
   End
   Begin VB.OptionButton Opt1 
      Height          =   255
      Left            =   3840
      TabIndex        =   37
      Top             =   255
      Width           =   495
   End
   Begin VB.OptionButton Opt11 
      Height          =   255
      Left            =   3840
      TabIndex        =   36
      Top             =   5435
      Width           =   495
   End
   Begin VB.OptionButton Opt10 
      Height          =   255
      Left            =   3840
      TabIndex        =   35
      Top             =   4917
      Width           =   495
   End
   Begin VB.OptionButton Opt9 
      Height          =   255
      Left            =   3840
      TabIndex        =   34
      Top             =   4399
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   11
      Left            =   1440
      TabIndex        =   31
      Text            =   "Text1"
      Top             =   5775
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   10
      Left            =   1440
      TabIndex        =   30
      Text            =   "Text1"
      Top             =   5265
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   9
      Left            =   1440
      TabIndex        =   29
      Text            =   "Text1"
      Top             =   4761
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   8
      Left            =   1440
      TabIndex        =   27
      Text            =   "Text1"
      Top             =   4257
      Width           =   1950
   End
   Begin VB.OptionButton Opt8 
      Height          =   255
      Left            =   3840
      TabIndex        =   25
      Top             =   3881
      Width           =   495
   End
   Begin VB.OptionButton Opt7 
      Height          =   255
      Left            =   3840
      TabIndex        =   24
      Top             =   3363
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   7
      Left            =   1440
      TabIndex        =   22
      Text            =   "Text1"
      Top             =   3753
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   6
      Left            =   1440
      TabIndex        =   21
      Text            =   "Text1"
      Top             =   3249
      Width           =   1950
   End
   Begin VB.OptionButton Opt6 
      Height          =   255
      Left            =   3840
      TabIndex        =   19
      Top             =   2845
      Width           =   495
   End
   Begin VB.OptionButton Opt5 
      Height          =   255
      Left            =   3840
      TabIndex        =   18
      Top             =   2327
      Width           =   495
   End
   Begin VB.OptionButton Opt4 
      Height          =   255
      Left            =   3840
      TabIndex        =   17
      Top             =   1809
      Width           =   495
   End
   Begin VB.OptionButton Opt3 
      Height          =   255
      Left            =   3840
      TabIndex        =   16
      Top             =   1291
      Width           =   495
   End
   Begin VB.OptionButton Opt2 
      Height          =   255
      Left            =   3840
      TabIndex        =   15
      Top             =   773
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   5
      Left            =   1440
      TabIndex        =   14
      Text            =   "Text1"
      Top             =   2745
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   4
      Left            =   1440
      TabIndex        =   13
      Text            =   "Text1"
      Top             =   2241
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   3
      Left            =   1440
      TabIndex        =   12
      Text            =   "Text1"
      Top             =   1737
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   2
      Left            =   1440
      TabIndex        =   11
      Text            =   "Text1"
      Top             =   1233
      Width           =   1950
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   1
      Left            =   1440
      TabIndex        =   10
      Text            =   "Text1"
      Top             =   729
      Width           =   1950
   End
   Begin VB.TextBox Text7 
      Height          =   330
      Left            =   675
      TabIndex        =   9
      Text            =   "Text7"
      Top             =   7485
      Visible         =   0   'False
      Width           =   1695
   End
   Begin VB.CommandButton Command2 
      Caption         =   "取消"
      Height          =   390
      Left            =   2325
      TabIndex        =   8
      Top             =   6870
      Width           =   1200
   End
   Begin VB.CommandButton Command1 
      Caption         =   "保存"
      Height          =   360
      Left            =   825
      TabIndex        =   7
      Top             =   6900
      Width           =   1200
   End
   Begin VB.TextBox Text1 
      Height          =   390
      Index           =   0
      Left            =   1440
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   225
      Width           =   1950
   End
   Begin VB.Label Label13 
      Caption         =   "全屏/窗口切换"
      Height          =   315
      Left            =   165
      TabIndex        =   39
      Top             =   6420
      Width           =   1185
   End
   Begin VB.Label Label12 
      Caption         =   "静音"
      Height          =   315
      Left            =   735
      TabIndex        =   33
      Top             =   5850
      Width           =   600
   End
   Begin VB.Label Label11 
      Caption         =   "播放/暂停"
      Height          =   255
      Left            =   405
      TabIndex        =   32
      Top             =   5355
      Width           =   990
   End
   Begin VB.Label Label10 
      Caption         =   "文件夹退出"
      Height          =   345
      Left            =   300
      TabIndex        =   28
      Top             =   4905
      Width           =   1155
   End
   Begin VB.Label Label9 
      Caption         =   "文件夹进入"
      Height          =   285
      Left            =   315
      TabIndex        =   26
      Top             =   4305
      Width           =   930
   End
   Begin VB.Label Label8 
      Caption         =   "音量-"
      Height          =   345
      Left            =   720
      TabIndex        =   23
      Top             =   3795
      Width           =   675
   End
   Begin VB.Label Label7 
      Caption         =   "音量+"
      Height          =   270
      Left            =   705
      TabIndex        =   20
      Top             =   3345
      Width           =   900
   End
   Begin VB.Label Label6 
      Caption         =   "歌曲后选"
      Height          =   240
      Left            =   390
      TabIndex        =   6
      Top             =   2850
      Width           =   885
   End
   Begin VB.Label Label5 
      Caption         =   "歌曲前选"
      Height          =   285
      Left            =   375
      TabIndex        =   5
      Top             =   2325
      Width           =   1065
   End
   Begin VB.Label Label4 
      Caption         =   "文件夹后选"
      Height          =   255
      Left            =   270
      TabIndex        =   4
      Top             =   1860
      Width           =   960
   End
   Begin VB.Label Label3 
      Caption         =   "文件夹前选"
      Height          =   300
      Left            =   210
      TabIndex        =   3
      Top             =   1350
      Width           =   1140
   End
   Begin VB.Label Label2 
      Caption         =   "磁盘目录后选"
      Height          =   330
      Left            =   135
      TabIndex        =   2
      Top             =   765
      Width           =   1305
   End
   Begin VB.Label Label1 
      Caption         =   "磁盘目录前选"
      Height          =   300
      Left            =   165
      TabIndex        =   0
      Top             =   345
      Width           =   1275
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private iniPath As String
Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, lpKeyName As Any, ByVal lpDefault As String, ByVal lpRetunedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As String, ByVal lplFileName As String) As Long
Dim hi
Dim ss
'Private Const SWP_NOSIZE = &H1
'Private Const SWP_NOMOVE = &H2
'Private Const HWND_TOPMOST = -1
'Private Const HWND_NOTOPMOST = -2
'Private Declare Function SetWindowPos Lib "user32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long


'SetWindowPos hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE '取消置顶




 



Function GetFromINI(AppName As String, KeyName As String, Filename As String) As String
   Dim RetStr As String
   RetStr = String(255, Chr(0))
   GetFromINI = Left(RetStr, GetPrivateProfileString(AppName, ByVal KeyName, "", RetStr, Len(RetStr), Filename))
End Function
Private Sub Command1_Click()
iniPath = App.Path & "\hifans.ini"
hfs1 = WritePrivateProfileString("IRCODE", "KEYCODE1", Text1(0), iniPath)
hfs2 = WritePrivateProfileString("IRCODE", "KEYCODE2", Text1(1), iniPath)
hfs3 = WritePrivateProfileString("IRCODE", "KEYCODE3", Text1(2), iniPath)
hfs4 = WritePrivateProfileString("IRCODE", "KEYCODE4", Text1(3), iniPath)
hfs5 = WritePrivateProfileString("IRCODE", "KEYCODE5", Text1(4), iniPath)
hfs6 = WritePrivateProfileString("IRCODE", "KEYCODE6", Text1(5), iniPath)
hfs7 = WritePrivateProfileString("IRCODE", "KEYCODE7", Text1(6), iniPath)
hfs8 = WritePrivateProfileString("IRCODE", "KEYCODE8", Text1(7), iniPath)
hfs9 = WritePrivateProfileString("IRCODE", "KEYCODE9", Text1(8), iniPath)
hfs10 = WritePrivateProfileString("IRCODE", "KEYCODE10", Text1(9), iniPath)
hfs11 = WritePrivateProfileString("IRCODE", "KEYCODE11", Text1(10), iniPath)
hfs12 = WritePrivateProfileString("IRCODE", "KEYCODE12", Text1(11), iniPath)
hfs13 = WritePrivateProfileString("IRCODE", "KEYCODE13", Text1(12), iniPath)
MsgBox "遥控键值已成功保存！", vbInformation, "成功提示"
End Sub

Private Sub Command2_Click()
Unload Form2
End Sub

Private Sub Form_Load()
On Error Resume Next
'Opt1.Value = True
iniPath = App.Path & "\hifans.ini"
For i = 0 To Text1.Count - 1
Text1(i).Locked = True
Dim aa As String
aa = "KEYCODE" & Trim(Str(i + 1))
Text1(i) = GetFromINI("IRCODE", aa, iniPath)
Next i

'SetWindowPos hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE '设置顶
End Sub

Private Sub Text7_Change()
If Opt1.Value = True Then
Text1(0).Text = Text7.Text
End If
If Opt2.Value = True Then
Text1(1).Text = Text7.Text
End If
If Opt3.Value = True Then
Text1(2).Text = Text7.Text
End If
If Opt4.Value = True Then
Text1(3).Text = Text7.Text
End If
If Opt5.Value = True Then
Text1(4).Text = Text7.Text
End If

If Opt6.Value = True Then
Text1(5).Text = Text7.Text
End If
If Opt7.Value = True Then
Text1(6).Text = Text7.Text
End If
If Opt8.Value = True Then
Text1(7).Text = Text7.Text
End If
If Opt9.Value = True Then
Text1(8).Text = Text7.Text
End If
If Opt10.Value = True Then
Text1(9).Text = Text7.Text
End If
If Opt11.Value = True Then
Text1(10).Text = Text7.Text
End If
If Opt12.Value = True Then
Text1(11).Text = Text7.Text
End If
If Opt13.Value = True Then
Text1(12).Text = Text7.Text
End If

End Sub
