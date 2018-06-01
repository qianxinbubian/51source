VERSION 5.00
Begin VB.UserControl ucHistogram 
   BackColor       =   &H80000012&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   720
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2565
   FillColor       =   &H00008000&
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H0000FF00&
   ScaleHeight     =   48
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   171
   ToolboxBitmap   =   "ucHistogram.ctx":0000
   Begin VB.PictureBox picHist 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00000000&
      BorderStyle     =   0  'None
      FillColor       =   &H0000FF00&
      ForeColor       =   &H0000FF00&
      Height          =   465
      Left            =   45
      ScaleHeight     =   465
      ScaleWidth      =   1050
      TabIndex        =   0
      Top             =   45
      Visible         =   0   'False
      Width           =   1050
   End
End
Attribute VB_Name = "ucHistogram"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

Option Explicit

Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Const SRCCOPY = &HCC0020
Private lPoint() As Long
Private lCurrentPoint As Long
Private lJump As Integer
Private lHSpacing As Long
Private lVSpacing As Long

Const m_def_Appearance = 1
Const m_def_BorderStyle = 1
Const m_def_DrawWidth = 1
Const m_def_GridTickHorizontal = 10
Const m_def_GridTickVertical = 25
Const m_def_NumberOfPoints = 100
Const m_def_BackColor = &H80000012
Const m_def_GridColour = &H8000&
Const m_def_Max = 100
Const m_def_ForeColor = &HFF00&

Dim m_Appearance As Integer
Dim m_BorderStyle As Integer
Dim m_DrawWidth As Integer
Dim m_GridTickHorizontal As Integer
Dim m_GridTickVertical As Integer
Dim m_NumberOfPoints As Long
Dim m_BackColor As OLE_COLOR
Dim m_GridColour As OLE_COLOR
Dim m_Max As Long
Dim m_ForeColor As OLE_COLOR

Event Click()

Private Sub DrawHistogram()
    
    Dim lRet As Long
    Dim lAdjustHeight As Long
    
    On Local Error Resume Next
    
    picHist.Cls
    For lRet = picHist.ScaleHeight To 0 Step -lVSpacing
        picHist.Line (0, lRet)-(picHist.ScaleWidth, lRet), m_GridColour
    Next
    For lRet = 0 To picHist.ScaleWidth Step lHSpacing
        picHist.Line (lRet, 0)-(lRet, picHist.ScaleHeight), m_GridColour
    Next
    
    picHist.DrawWidth = m_DrawWidth
    lAdjustHeight = (UserControl.Height / m_Max)
    
    For lRet = 1 To m_NumberOfPoints
        If lPoint(lRet) > -1 Then
            picHist.Line ((lRet * lJump), (lAdjustHeight * (m_Max - lPoint(lRet - 1))))-(((lRet * lJump) + lJump), (lAdjustHeight * (m_Max - lPoint(lRet)))), m_ForeColor
        Else
            Exit For
        End If
    Next
    With picHist
        lRet = BitBlt(UserControl.hdc, -8, 0, .ScaleWidth, .ScaleHeight, .hdc, 0, 0, SRCCOPY)
    End With

End Sub

Public Sub NextPoint(ByVal Value As Long)
    
    Dim lRet As Long
    
    On Local Error Resume Next
    
    If Value <= m_Max Then
        If lCurrentPoint > m_NumberOfPoints Then
            For lRet = 1 To m_NumberOfPoints
                lPoint(lRet - 1) = lPoint(lRet)
            Next
            lPoint(m_NumberOfPoints) = Value
            lCurrentPoint = m_NumberOfPoints
        Else
            lPoint(lCurrentPoint) = Value
            lCurrentPoint = lCurrentPoint + 1
        End If
        Call DrawHistogram
    End If
       
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,0
Public Property Get Appearance() As Integer
Attribute Appearance.VB_Description = "Returns/sets whether or not an object is painted at run time with 3-D effects."
    Appearance = m_Appearance
End Property

Public Property Let Appearance(ByVal New_Appearance As Integer)
    m_Appearance = New_Appearance
    PropertyChanged "Appearance"
    UserControl.Appearance = m_Appearance
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,1
Public Property Get DrawWidth() As Integer
Attribute DrawWidth.VB_Description = "Returns/sets the line width for output from graphics methods."
    DrawWidth = m_DrawWidth
End Property

Public Property Let DrawWidth(ByVal New_DrawWidth As Integer)
    m_DrawWidth = New_DrawWidth
    PropertyChanged "DrawWidth"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14,0,0,10
Public Property Get GridTickHorizontal() As Variant
    GridTickHorizontal = m_GridTickHorizontal
End Property

Public Property Let GridTickHorizontal(ByVal New_GridTickHorizontal As Variant)
    m_GridTickHorizontal = New_GridTickHorizontal
    PropertyChanged "GridTickHorizontal"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=14,0,0,10
Public Property Get GridTickVertical() As Variant
    GridTickVertical = m_GridTickVertical
End Property

Public Property Let GridTickVertical(ByVal New_GridTickVertical As Variant)
    m_GridTickVertical = New_GridTickVertical
    PropertyChanged "GridTickVertical"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,1
Public Property Get NumberOfPoints() As Long
    NumberOfPoints = m_NumberOfPoints
End Property

Public Property Let NumberOfPoints(ByVal New_NumberOfPoints As Long)
    m_NumberOfPoints = New_NumberOfPoints
    PropertyChanged "NumberOfPoints"
End Property

Private Sub UserControl_InitProperties()
    
    m_Appearance = m_def_Appearance
    m_BorderStyle = m_def_BorderStyle
    m_DrawWidth = m_def_DrawWidth
    m_GridTickHorizontal = m_def_GridTickHorizontal
    m_GridTickVertical = m_def_GridTickVertical
    m_NumberOfPoints = m_def_NumberOfPoints
    m_BackColor = m_def_BackColor
    m_GridColour = m_def_GridColour
    m_Max = m_def_Max
    m_ForeColor = m_def_ForeColor
    
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)

    Dim lRet As Long
    
    With PropBag
        m_Appearance = .ReadProperty("Appearance", m_def_Appearance)
        m_BorderStyle = .ReadProperty("BorderStyle", m_def_BorderStyle)
        m_DrawWidth = .ReadProperty("DrawWidth", m_def_DrawWidth)
        m_GridTickHorizontal = .ReadProperty("GridTickHorizontal", m_def_GridTickHorizontal)
        m_GridTickVertical = .ReadProperty("GridTickVertical", m_def_GridTickVertical)
        m_NumberOfPoints = .ReadProperty("NumberOfPoints", m_def_NumberOfPoints)
        m_BackColor = .ReadProperty("BackColor", m_def_BackColor)
        m_GridColour = .ReadProperty("GridColour", m_def_GridColour)
        m_Max = .ReadProperty("Max", m_def_Max)
        m_ForeColor = .ReadProperty("ForeColor", m_def_ForeColor)
    End With
    
    ReDim lPoint(m_NumberOfPoints)
    lPoint(0) = 0
    
    For lRet = 1 To m_NumberOfPoints
        lPoint(lRet) = -1
    Next
    lCurrentPoint = 1
    With UserControl
        .BorderStyle = m_BorderStyle
        .Appearance = m_Appearance
        .BackColor = m_BackColor
    End With
    picHist.BackColor = m_BackColor
    
End Sub

Private Sub UserControl_Resize()
    
    On Local Error Resume Next
    
    With UserControl
        picHist.Move 0, 0, .ScaleWidth + 8, .ScaleHeight
        lJump = CInt(picHist.ScaleWidth / m_NumberOfPoints) + 1
        lVSpacing = CLng(picHist.ScaleHeight / (m_Max / m_GridTickVertical))
        lHSpacing = CLng(picHist.ScaleWidth / m_NumberOfPoints) * m_GridTickHorizontal
    End With
    
End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)

    With PropBag
        Call .WriteProperty("Appearance", m_Appearance, m_def_Appearance)
        Call .WriteProperty("BorderStyle", m_BorderStyle, m_def_BorderStyle)
        Call .WriteProperty("DrawWidth", m_DrawWidth, m_def_DrawWidth)
        Call .WriteProperty("GridTickHorizontal", m_GridTickHorizontal, m_def_GridTickHorizontal)
        Call .WriteProperty("GridTickVertical", m_GridTickVertical, m_def_GridTickVertical)
        Call .WriteProperty("NumberOfPoints", m_NumberOfPoints, m_def_NumberOfPoints)
        Call .WriteProperty("BackColor", m_BackColor, m_def_BackColor)
        Call .WriteProperty("GridColour", m_GridColour, m_def_GridColour)
        Call .WriteProperty("Max", m_Max, m_def_Max)
        Call .WriteProperty("ForeColor", m_ForeColor, m_def_ForeColor)
    End With
    
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
    BackColor = m_BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
    m_BackColor = New_BackColor
    PropertyChanged "BackColor"
    UserControl.BackColor = m_BackColor
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,1
Public Property Get GridColour() As OLE_COLOR
    GridColour = m_GridColour
End Property

Public Property Let GridColour(ByVal New_GridColour As OLE_COLOR)
    m_GridColour = New_GridColour
    PropertyChanged "GridColour"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,100
Public Property Get Max() As Long
    Max = m_Max
End Property

Public Property Let Max(ByVal New_Max As Long)
    m_Max = New_Max
    PropertyChanged "Max"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
    ForeColor = m_ForeColor
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
    m_ForeColor = New_ForeColor
    PropertyChanged "ForeColor"
    UserControl.ForeColor = m_ForeColor
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,0
Public Property Get BorderStyle() As Integer
Attribute BorderStyle.VB_Description = "Returns/sets the border style for an object."
    BorderStyle = m_BorderStyle
End Property

Public Property Let BorderStyle(ByVal New_BorderStyle As Integer)
    m_BorderStyle = New_BorderStyle
    PropertyChanged "BorderStyle"
    UserControl.BorderStyle = m_BorderStyle
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=5
Public Sub Refresh()
Attribute Refresh.VB_Description = "Forces a complete repaint of a object."
    Call DrawHistogram
End Sub

