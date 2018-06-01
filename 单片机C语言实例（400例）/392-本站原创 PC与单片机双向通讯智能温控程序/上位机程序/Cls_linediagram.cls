VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Cls_diagram"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Public HorzSplits As Long
Public VertSplits As Long
Public Max As Single 'Max value

Private ValueArray() As Single 'Array to hold values
Private mem_LineColor As Long
Private mem_GridColor As Long
Private mem_ShowGrid As Boolean
Private mem_pBox As PictureBox
Private mem_pBoxHeight As Long
Private mem_pBoxWidth As Long
Private mem_movingGrid As Boolean
Private StartPosition As Long 'Needed to not to display first zero values when starting a new diagram
Private GridPosition As Long

Public Enum ENUM_DIAGRAMTYPE
    TYPE_LINE = 0
    TYPE_POINT = 1
End Enum
Public DiagramType As ENUM_DIAGRAMTYPE 'Type of diagram (line or point)

Const const_tolerance = 0.0001 'Used to fix last line tolerance problem in some cases

Public Function InitDiagram(pBox As PictureBox, LineColor As Long, ShowGrid As Boolean, Optional GridColor As Variant, Optional MovingGrid As Variant)
    
    pBox.ScaleMode = vbPixels 'Set pixel scale mode
    
    mem_LineColor = LineColor
    mem_ShowGrid = ShowGrid
    
    mem_pBoxHeight = pBox.ScaleHeight
    mem_pBoxWidth = pBox.ScaleWidth
    
    'If user didn't give a grid color, we are using default (dark green) color
    If IsMissing(GridColor) Then
        mem_GridColor = RGB(0, 130, 0) 'Dark green
    Else:
        mem_GridColor = GridColor
    End If
    
    'If user didn't give a movingGrid parameter, we are using default (off)
    If IsMissing(MovingGrid) Then
        mem_movingGrid = False
    Else:
        mem_movingGrid = MovingGrid
    End If
    
    Set mem_pBox = pBox 'Save picturebox, so we dont need to ask it again
    
    'Allocate array to hold all diagram values (value per pixel)
    ReDim ValueArray(mem_pBoxWidth - 1)
    
    StartPosition = mem_pBoxWidth - 1
    
    GridPosition = 0
    
End Function
Public Sub AddValue(value As Single)
    
    Dim l As Long
    
    'Check if InitDiagram has not been executed yet
    If mem_pBox Is Nothing Then
        'Failed! (exit function)
        Exit Sub
    End If
    
    'Move all values from array one position lower
    For l = 1 To mem_pBoxWidth - 1
        ValueArray(l - 1) = ValueArray(l)
    Next
    
    'Max can't be 0 or smaller
    If Max <= 0 Then Max = 1
    
    'Add new value to the last element of array
    ValueArray(l - 1) = mem_pBoxHeight - ((value / Max) * mem_pBoxHeight)
    
    If StartPosition >= 1 Then StartPosition = StartPosition - 1
    
    GridPosition = GridPosition - 1
End Sub
Public Sub RePaint()

    Dim x As Single
    Dim y As Single
    Dim l As Long
    
    'Check if InitDiagram has not been executed yet
    If mem_pBox Is Nothing Then
        'Failed! (exit sub)
        Exit Sub
    End If

    'Create background image
    'First clear hole picture box, then draw grid if set, then draw diagram
    
    mem_pBox.Cls 'Clear picturebox
    
    'Draw grid if set
    If (mem_ShowGrid) Then
        mem_pBox.ForeColor = mem_GridColor
        
        'Draw vertical lines with or without using gridposition
        If (mem_movingGrid) Then
            For x = GridPosition To mem_pBoxWidth - 1 Step ((mem_pBoxWidth - 1) / (VertSplits + 1)) - const_tolerance
                mem_pBox.Line (x, 0)-(x, mem_pBoxHeight)
            Next
        Else:
            For x = 0 To mem_pBoxWidth - 1 Step ((mem_pBoxWidth - 1) / (VertSplits + 1)) - const_tolerance
                mem_pBox.Line (x, 0)-(x, mem_pBoxHeight)
            Next
        End If
        
        For y = 0 To mem_pBoxHeight - 1 Step ((mem_pBoxHeight - 1) / (HorzSplits + 1)) - const_tolerance
            mem_pBox.Line (0, y)-(mem_pBoxWidth, y)
        Next
        'Reset gridposition, when first line is not visible anymore
        If GridPosition <= -Int((mem_pBoxWidth - 1 / (HorzSplits + 1))) Then
            GridPosition = 0
        End If
    End If
    
    'Draw line diagram only if theres 2 or more values defined
    If StartPosition <= mem_pBoxWidth - 1 Then
        mem_pBox.ForeColor = mem_LineColor
        
        Select Case DiagramType
            
            Case TYPE_LINE
            For l = StartPosition + 1 To mem_pBoxWidth - 2
                mem_pBox.Line (l, ValueArray(l))-(l + 1, ValueArray(l + 1))
            Next
            
            Case TYPE_POINT
            For l = StartPosition + 1 To mem_pBoxWidth - 2
                mem_pBox.PSet (l + 1, ValueArray(l + 1))
            Next
        End Select
        
    End If

End Sub
