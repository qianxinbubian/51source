VERSION 5.00
Begin VB.UserControl MorphDisplay 
   AutoRedraw      =   -1  'True
   ClientHeight    =   675
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2565
   ScaleHeight     =   45
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   171
   ToolboxBitmap   =   "MorphLCD.ctx":0000
End
Attribute VB_Name = "MorphDisplay"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False


Option Explicit

' declares for creating, selecting, coloring and destroying the shaped LCD segment regions.
Private Declare Function CreatePolygonRgn Lib "gdi32.dll" (ByRef lpPoint As POINTAPI, ByVal nCount As Long, ByVal nPolyFillMode As Long) As Long
Private Declare Function CreateRectRgn Lib "gdi32" (ByVal x1 As Long, ByVal y1 As Long, ByVal x2 As Long, ByVal y2 As Long) As Long
Private Declare Function CreateSolidBrush Lib "gdi32.dll" (ByVal crColor As Long) As Long
Private Declare Function DeleteObject Lib "gdi32.dll" (ByVal hObject As Long) As Long
Private Declare Function FillRgn Lib "gdi32.dll" (ByVal hdc As Long, ByVal hRgn As Long, ByVal hBrush As Long) As Long
Private Declare Function FrameRgn Lib "gdi32.dll" (ByVal hdc As Long, ByVal hRgn As Long, ByVal hBrush As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Private Declare Function OffsetRgn Lib "gdi32.dll" (ByVal hRgn As Long, ByVal x As Long, ByVal Y As Long) As Long
Private Declare Function SelectClipRgn Lib "gdi32" (ByVal hdc As Long, ByVal hRgn As Long) As Long

' other graphics api declares.
Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal nXDest As Long, ByVal nYDest As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function CombineRgn Lib "gdi32.dll" (ByVal hDestRgn As Long, ByVal hSrcRgn1 As Long, ByVal hSrcRgn2 As Long, ByVal nCombineMode As Long) As Long
Private Declare Function CreateCompatibleBitmap Lib "gdi32" (ByVal hdc As Long, ByVal nWidth As Long, ByVal nHeight As Long) As Long
Private Declare Function CreateCompatibleDC Lib "gdi32" (ByVal hdc As Long) As Long
Private Declare Function CreateDCAsNull Lib "gdi32" Alias "CreateDCA" (ByVal lpDriverName As String, lpDeviceName As Any, lpOutput As Any, lpInitData As Any) As Long
Private Declare Function CreateDIBPatternBrushPt Lib "gdi32" (lpPackedDIB As Any, ByVal iUsage As Long) As Long
Private Declare Function CreateEllipticRgn Lib "gdi32" (ByVal x1 As Long, ByVal y1 As Long, ByVal x2 As Long, ByVal y2 As Long) As Long
Private Declare Function DeleteDC Lib "gdi32" (ByVal hdc As Long) As Long
Private Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
Private Declare Function GetDIBits Lib "gdi32" (ByVal aHDC As Long, ByVal hBitmap As Long, ByVal nStartScan As Long, ByVal nNumScans As Long, lpBits As Any, lpBI As BITMAPINFOHEADER, ByVal wUsage As Long) As Long
Private Declare Function GetObject Lib "gdi32" Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long
Private Declare Function GetObjectAPI Lib "gdi32" Alias "GetObjectA" (ByVal hObject As Long, ByVal nCount As Long, lpObject As Any) As Long
Private Declare Function GetObjectType Lib "gdi32" (ByVal hgdiobj As Long) As Long
Private Declare Function OleTranslateColor Lib "olepro32.dll" (ByVal OLE_COLOR As Long, ByVal hPalette As Long, pccolorref As Long) As Long
Private Declare Function SelectObject Lib "gdi32" (ByVal hdc As Long, ByVal hObject As Long) As Long
Private Declare Function SetBrushOrgEx Lib "gdi32" (ByVal hdc As Long, ByVal nXOrg As Long, ByVal nYOrg As Long, lppt As POINTAPI) As Long
Private Declare Function SetRect Lib "user32" (lpRect As RECT, ByVal x1 As Long, ByVal y1 As Long, ByVal x2 As Long, ByVal y2 As Long) As Long
Private Declare Function SetWindowRgn Lib "user32" (ByVal hwnd As Long, ByVal hRgn As Long, ByVal bRedraw As Long) As Long
Private Declare Function StretchDIBits Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal Y As Long, ByVal dx As Long, ByVal dy As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal wSrcWidth As Long, ByVal wSrcHeight As Long, lpBits As Any, lpBitsInfo As Any, ByVal wUsage As Long, ByVal dwRop As Long) As Long
Private Declare Function StretchBlt Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal nSrcWidth As Long, ByVal nSrcHeight As Long, ByVal dwRop As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (ByRef lpDest As Any, ByRef lpSource As Any, ByVal iLen As Long)
Private Declare Sub FillMemory Lib "kernel32.dll" Alias "RtlFillMemory" (Destination As Any, ByVal Length As Long, ByVal Fill As Byte)

'  for separator and colon positioning.
Private Type POINTAPI
   x                            As Long
   Y                            As Long
End Type

'  declares for gradient painting and bitmap tiling.
Private Type BITMAPINFOHEADER
   biSize                       As Long
   biWidth                      As Long
   biHeight                     As Long
   biPlanes                     As Integer
   biBitCount                   As Integer
   biCompression                As Long
   biSizeImage                  As Long
   biXPelsPerMeter              As Long
   biYPelsPerMeter              As Long
   biClrUsed                    As Long
   biClrImportant               As Long
End Type

Private Type BITMAP
   bmType                       As Long
   bmWidth                      As Long
   bmHeight                     As Long
   bmWidthBytes                 As Long
   bmPlanes                     As Integer
   bmBitsPixel                  As Integer
   bmBits                       As Long
End Type

Private Const DIB_RGB_COLORS    As Long = 0                 ' also used in gradient generation.
Private Const OBJ_BITMAP        As Long = 7                 ' used to determine if picture is a bitmap.
Private m_hBrush                As Long                     ' pattern brush for bitmap tiling.

'  used to define various graphics areas.
Private Type RECT
   Left                         As Long
   Top                          As Long
   Right                        As Long
   Bottom                       As Long
End Type

'  gradient generation constants.
Private Const RGN_DIFF          As Long = 4
Private Const PI                As Single = 3.14159265358979
Private Const TO_DEG            As Single = 180 / PI
Private Const TO_RAD            As Single = PI / 180
Private Const INT_ROT           As Long = 1000

'  gradient information for background.
Private BGuBIH                  As BITMAPINFOHEADER
Private BGlBits()               As Long

' enum tied to .Theme property.
Public Enum LCDThemeOptions
   [None] = 0
   [LED Hex Small] = 1
   [LED Hex Medium] = 2
   [LCD Trap Small] = 3
   [LCD Trap Medium] = 4
   [Rectangular Medium] = 5
   [Rectangular Small] = 6
End Enum

' enum tied to .ThousandsSeparator and .DecimalSeparator properties.
Public Enum SeparatorOptions
   [Comma] = 0
   [Period] = 1
End Enum

' enum tied to .SegmentStyle and .SegmentStyleExp properties.
Public Enum SegmentStyleOptions
   [Hexagonal] = 0
   [Trapezoidal] = 1
   [Rectangular] = 2
End Enum

' enum tied to .SegmentFillStyle property.
Public Enum SegmentFillStyleOptions
   [Filament] = 0
   [Solid] = 1
End Enum

'  enum tied to .PictureMode property.
Public Enum LCDPicModeOptions
   [Normal] = 0
   [Stretch] = 1
   [Tiled] = 2
End Enum

' holds all segment region pointers for hexagonal/trapezoidal/rectangular LCD display segment types.
Private LCDSegment(0 To 9)      As Long
Private LCDSegmentExp(0 To 9)   As Long

' pointers to the segment currently being created or displayed.
Private Const VERTICAL_HEXAGONAL_SEGMENT              As Long = 0
Private Const HORIZONTAL_HEXAGONAL_SEGMENT            As Long = 1
Private Const HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT As Long = 2
Private Const VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT   As Long = 3
Private Const HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT   As Long = 4
Private Const VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT  As Long = 5
Private Const VERTICAL_RECTANGULAR_SEGMENT            As Long = 6
Private Const HORIZONTAL_RECTANGULAR_SEGMENT          As Long = 7
Private Const DECIMAL_SEPARATOR_SEGMENT               As Long = 8
Private Const THOUSANDS_SEPARATOR_SEGMENT             As Long = 9

' pointers to which LCD digit type we're currently manipulating.
Private Const MAINVALUE         As Long = 0
Private Const EXPONENT          As Long = 1

' segment lit status constants.
Private Const SEGMENT_LIT       As String = "1"
Private Const SEGMENT_UNLIT     As String = "0"

' used by the DisplayValue routine to determine whether value should
' be fully redisplayed (as when property is changed in design mode).
Private Const FORCE_REDRAW_YES  As Boolean = True
Private Const FORCE_REDRAW_NO   As Boolean = False

Private LCDLitColorBrush        As Long                     '  color brush for lit segments.
Private LCDBurnInColorBrush     As Long                     '  color brush for 'burn-in' segments.
Private LCDLitColorBrushNeg     As Long                     '  for when value is negative.
Private LCDBurnInColorBrushNeg  As Long                     '  for when value is negative.
Private CurrentLitColorBrush    As Long                     '  which lit segment brush we're currently using.
Private CurrentBurnInColorBrush As Long                     '  which 'burn-in' brush we're currently using.

'  holds binary string patterns indicating which segments to "light up".
'  0-9, unlit segment, minus sign and hex A-F.  18 patterns total.
Private DisplayPattern(0 To 17) As String

Private Const MAX_DIGITS                   As Long = 50     '  maximum displayable number of digits.
Private DigitXPos(0 To MAX_DIGITS - 1)     As Long          '  X coordinate of each main value digit.
Private DigitXPosExp(0 To 4)               As Long          '  X coordinate of each exponent digit.
Private ThousandsFlag()                    As Boolean       '  thousands separator flag for after each digit.

' X and Y coordinates of the decimal separator.
Private DecimalSeparatorPos As POINTAPI

' X and Y coordinates of each 'dot' in the colon.
Private Type ColonCoordinateType
   TopPoint                     As POINTAPI
   BottomPoint                  As POINTAPI
End Type
Private ColonPos                As ColonCoordinateType

'  the widths and heights of main and exponent LCD digits.
Private DigitWidth              As Long                     ' width of a main value digit, in pixels.
Private DigitHeight             As Long                     ' height of a main value digit, in pixels.
Private DigitWidthExp           As Long                     ' width of an exponent digit, in pixels.
Private DigitHeightExp          As Long                     ' height of an exponent digit, in pixels.

Private ChangingPicture As Boolean                          ' so control knows to reblit new bg to virtual DC.
Private PreviousMainValue       As String                   ' used to determine whether to display new digit.
Private PreviousExponentValue   As String                   ' used to determine whether to display new digit.
Private PreviousNegative        As Boolean                  ' flag to determine sign of previously displayed value.

'  default property value constants.
Private Const m_def_BackAngle = 90                          ' horizontal gradient.
Private Const m_def_BackColor1 = &H0                        ' black background gradient start color.
Private Const m_def_BackColor2 = &H0                        ' black background gradient end color.
Private Const m_def_BackMiddleOut = True                    ' middle-out gradient display.
Private Const m_def_BorderWidth = 1                         ' 1-pixel wide border.
Private Const m_def_BorderColor = &HFF0000                  ' blue border color.
Private Const m_def_BurnInColor = &H505000                  ' dark cyan simulated segment burn-in color.
Private Const m_def_BurnInColorNeg = &H505000               ' dark cyan negative value burn-in color.
Private Const m_def_CurveBottomLeft = 0                     ' no curvature.
Private Const m_def_CurveBottomRight = 0                    ' no curvature.
Private Const m_def_CurveTopLeft = 0                        ' no curvature.
Private Const m_def_CurveTopRight = 0                       ' no curvature.
Private Const m_def_DecimalSeparator = 1                    ' period decimal separator.
Private Const m_def_InterDigitGap = 6                       ' 6 pixels between main LCD digits.
Private Const m_def_InterDigitGapExp = 2                    ' 2 pixels between exponent digits.
Private Const m_def_InterSegmentGap = 0                     ' no segment gap in main LCD digit segments.
Private Const m_def_InterSegmentGapExp = 0                  ' no segment gap in exponent digits.
Private Const m_def_NumDigits = 20                          ' 20 main value digits.
Private Const m_def_NumDigitsExp = 4                        ' 3 digits + minus sign.
Private Const m_def_PictureMode = 0                         ' normal picture display.
Private Const m_def_SegmentFillStyle = 1                    ' solid filled digits
Private Const m_def_SegmentHeight = 8                       ' main segments 8 pixels high.
Private Const m_def_SegmentHeightExp = 6                    ' exponent digit segments 4 pixels high.
Private Const m_def_SegmentLitColor = &HFFFF00              ' cyan lit positive value segment.
Private Const m_def_SegmentLitColorNeg = &HFFFF00           ' cyan lit negative value segment.
Private Const m_def_SegmentStyle = 2                        ' rectangular main digit segments.
Private Const m_def_SegmentStyleExp = 2                     ' rectangular exponent digit segments.
Private Const m_def_SegmentWidth = 3                        ' main segments 3 pixels high.
Private Const m_def_SegmentWidthExp = 3                     ' exponent segments 3 pixels wide.
Private Const m_def_ShowBurnIn = True                       ' show 'burned-in' segments.
Private Const m_def_ShowExponent = True                     ' show exponent.
Private Const m_def_ShowThousandsSeparator = False          ' don't show thousands separator.
Private Const m_def_Theme = 5                               ' 'rectangular medium' theme selected.
Private Const m_def_ThousandsGrouping = 3                   ' thousands separator every three digits.
Private Const m_def_ThousandsSeparator = 0                  ' comma thousands separator.
Private Const m_def_Value = "1234567890"                    ' displayed at first by default in design mode.
Private Const m_def_XOffset = 5                             ' 5 pixels from control left border.
Private Const m_def_XOffsetExp = 355                        ' 355 pixels from control left border.
Private Const m_def_YOffset = 5                             ' display main digits 5 pixels down from top edge.
Private Const m_def_YOffsetExp = 5                          ' display exponent 5 pixels from control top.

'  property variables.
Private m_BackAngle              As Single                  ' angle of background gradient.
Private m_BackColor1             As OLE_COLOR               ' first color of background gradient.
Private m_BackColor2             As OLE_COLOR               ' second color of background gradient.
Private m_BackMiddleOut          As Boolean                 ' if True, gradient displays in middle-out fashion.
Private m_BorderColor            As OLE_COLOR               ' border color.
Private m_BorderWidth            As Integer                 ' width, in pixels, of control border.
Private m_BurnInColor            As OLE_COLOR               ' color of simulated LCD digit 'burn-in'.
Private m_BurnInColorNeg         As OLE_COLOR               ' burn in color when value is negative.
Private m_CurveBottomLeft        As Long                    ' amount of curve for bottom left corner.
Private m_CurveBottomRight       As Long                    ' amount of curve for bottom right corner.
Private m_CurveTopLeft           As Long                    ' amount of curve for top left corner.
Private m_CurveTopRight          As Long                    ' amount of curve for top right corner.
Private m_DecimalSeparator       As SeparatorOptions        ' decimal separator character ("." in U.S.).
Private m_InterDigitGap          As Long                    ' # of pixels separating each main value digit.
Private m_InterDigitGapExp       As Long                    ' # of pixels separating each exponent digit.
Private m_InterSegmentGap        As Long                    ' # of pixels separating main value LCD segments.
Private m_InterSegmentGapExp     As Long                    ' # of pixels separating exponent LCD segments.
Private m_NumDigits              As Long                    ' number of digits to display.
Private m_NumDigitsExp           As Long                    ' number of exponent digits to display.
Private m_Picture                As Picture                 ' bitmap to be displayed in lieu of gradient.
Private m_PictureMode            As LCDPicModeOptions       ' normal, stretched or tiled bitmap display options.
Private m_SegmentFillStyle       As SegmentFillStyleOptions ' solid or filament-style segment styles.
Private m_SegmentHeight          As Long                    ' # of pixels in long dimension of main value segment.
Private m_SegmentHeightExp       As Long                    ' # of pixels in short dimension of exponent segment.
Private m_SegmentLitColor        As OLE_COLOR               ' the color of displayed (non burn-in) segments.
Private m_SegmentLitColorNeg     As OLE_COLOR               ' lit segment color when value is negative.
Private m_SegmentStyle           As SegmentStyleOptions     ' hexagonal, trapezoidal, or rectangular segments.
Private m_SegmentStyleExp        As SegmentStyleOptions     ' hexagonal, trapezoidal, or rectangular segments.
Private m_SegmentWidth           As Long                    ' # of pixels in short dimension of main value segment.
Private m_SegmentWidthExp        As Long                    ' # of pixels in short dimension of exponent segment.
Private m_ShowBurnIn             As Boolean                 ' if True, simulated digit 'burn-in' is displayed.
Private m_ShowExponent           As Boolean                 ' if True, exponent portion of value is shown.
Private m_ShowThousandsSeparator As Boolean                 ' display thousands separator? boolean.
Private m_Theme                  As LCDThemeOptions         ' user-definable and selectable display theme.
Private m_ThousandsGrouping      As Long                    ' how many digits between thousands separators.
Private m_ThousandsSeparator     As SeparatorOptions        ' thousands separator character ("," in U.S.).
Private m_Value                  As String                  ' the value to be displayed.
Private m_XOffset                As Long                    ' # of pixels from left to display main value.
Private m_XOffsetExp             As Long                    ' # of pixels from left to display exponent.
Private m_YOffset                As Long                    ' # of pixels from top to display main value.
Private m_YOffsetExp             As Long                    ' # of pixels from top to display exponent.

' declares for virtual background bitmap.
Private VirtualBackgroundDC     As Long                     ' handle of the created DC.
Private mMemoryBitmap           As Long                     ' Handle of the created bitmap.
Private mOrginalBitmap          As Long                     ' used in destroying virtual DC.

'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Events >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Private Sub UserControl_Initialize()

'*************************************************************************
'* initializes variables at the start of the control's existence.        *
'*************************************************************************

'  initialize the display patterns for the LCD segments.  Segment
'  positions on the 7-segment LCD start with #1 on top, go clockwise.
'  The center segment is #7.  A "1" means the segment is lit.
   DisplayPattern(0) = "1111110"    ' zero.
   DisplayPattern(1) = "0110000"    ' one.
   DisplayPattern(2) = "1101101"    ' two.
   DisplayPattern(3) = "1111001"    ' three.
   DisplayPattern(4) = "0110011"    ' four.
   DisplayPattern(5) = "1011011"    ' five.
   DisplayPattern(6) = "1011111"    ' six.
   DisplayPattern(7) = "1110000"    ' seven.
   DisplayPattern(8) = "1111111"    ' eight.
   DisplayPattern(9) = "1111011"    ' nine.
   DisplayPattern(10) = "0000000"   ' for display of 'burn-in' unused digits.
   DisplayPattern(11) = "0000001"   ' minus sign.
   DisplayPattern(12) = "1110111"   ' Hex "A".
   DisplayPattern(13) = "0011111"   ' Hex "b". (have to do it lowercase so as not to confuse it with "8".)
   DisplayPattern(14) = "1001110"   ' Hex "C".
   DisplayPattern(15) = "0111101"   ' Hex "d". (have to do it lowercase so as not to confuse it with "0".)
   DisplayPattern(16) = "1001111"   ' Hex "E".
   DisplayPattern(17) = "1000111"   ' Hex "F".

'  initialize the decimal separator location to 'no decimal separator'.
   DecimalSeparatorPos.x = -1

'  initialize the colon location to 'no colon'.
   ColonPos.TopPoint.x = -1

End Sub

Private Sub UserControl_Resize()

'*************************************************************************
'* just used in design mode at the moment.                               *
'*************************************************************************

   CalculateBackGroundGradient
   RedrawControl

End Sub

Private Sub UserControl_Show()

'*************************************************************************
'* dimension the thousands flag to match size of .NumDigits property.    *
'*************************************************************************

   ReDim ThousandsFlag(0 To m_NumDigits - 1)

'  for showing the control when placed on form in design mode.
   If Not Ambient.UserMode Then
      InitLCDDisplayCharacteristics
      RedrawControl
   End If

End Sub

Private Sub UserControl_Terminate()

'*************************************************************************
'* destroys all active objects and regions prior to control termination. *
'*************************************************************************

   Dim i As Long    ' loop variable.

'  delete digit segment region objects.
   For i = 0 To 9
      DeleteObject LCDSegment(i)
      DeleteObject LCDSegmentExp(i)
   Next i

'  delete digit segment color brushes.
   DeleteObject LCDLitColorBrush
   DeleteObject LCDBurnInColorBrush
   DeleteObject LCDLitColorBrushNeg
   DeleteObject LCDBurnInColorBrushNeg

'  destroy the virtual DC's used in background storage.
   DestroyVirtualDC
   DestroyPattern

End Sub

'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Graphics >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Private Sub DisplayValue(sValue As String, ByVal ForceDisplay As Boolean)

'*************************************************************************
'* displays the value stored in the .Value property.  The 'ForceDisplay' *
'* parameter is used to force a redisplay when a property is changed.    *
'*************************************************************************

   Dim sMainValue     As String    ' non-exponential, main value.
   Dim sExponentValue As String    ' up to n_NumDigitsExp long exponent (including sign).
   Dim IsBase10       As Boolean   ' not using this function return value now.

'  just in case, so exponent "E" and hex digits get processed correctly.
   sValue = UCase(sValue)

'  a catch for if there isn't a 0 in front of the decimal separator in a
'  fractional value.  (That won't happen if control is used correctly.)
   If Left(sValue, 1) = "." Then
      sValue = "0" & sValue
   End If

'  determine the color brush to use when displaying the value.
   If Val(sValue) < 0 Then

'     value is negative; use negative value color brushes.
      CurrentLitColorBrush = LCDLitColorBrushNeg
      CurrentBurnInColorBrush = LCDBurnInColorBrushNeg

'     a word about the PreviousNegative stuff... Normally individual display digits are drawn
'     only if the digit has changed from the previous displayed digit in the same position.
'     This saves a great deal of time in my timing tests.  However, when the sign is changed
'     when a new value is displayed, we have to force a redraw of every digit, even if they're
'     the same as in previous display.  That way a displayed value can't be a mishmash of
'     positive and negative value colors.
      If Not PreviousNegative Then
         PreviousMainValue = String(m_NumDigits, "X")
         PreviousExponentValue = String(m_NumDigitsExp, "X")
      End If
      PreviousNegative = True

   Else

'     value is positive; use regular (positive number) color brushes.
      CurrentLitColorBrush = LCDLitColorBrush
      CurrentBurnInColorBrush = LCDBurnInColorBrush

      If PreviousNegative Then
         PreviousMainValue = String(m_NumDigits, "X")
         PreviousExponentValue = String(m_NumDigitsExp, "X")
      End If
      PreviousNegative = False

   End If

'  separate possible exponent from overall value.
   IsBase10 = SeparateMainValueAndExponent(sValue, sMainValue, sExponentValue)

'  display the main value, and also the exponent if permitted by the ShowExponent property.
   DisplayMainValue sMainValue, ForceDisplay
   If m_ShowExponent Then
      DisplayExponentValue sExponentValue, ForceDisplay
   End If

End Sub

Private Function SeparateMainValueAndExponent(ByVal sValue As String, ByRef sMainValue As String, ByRef ExponentValue As String) As Boolean

'*************************************************************************
'* if it's an exponential base-10 number, separates the exponent and the *
'* main value.  Otherwise returns the value and spaces for exponent.     *
'*************************************************************************

   Dim ExpPos       As Long      ' position in value of exponent symbols E+ or E-.
   Dim ExponentSign As String    ' what sign the exponent is (+ or -).

   If InStr(sValue, "E+") > 0 Or InStr(sValue, "E-") > 0 Then

'     since we see an "E" followed by a "+" or "-" we know it's the base-10
'     exponent "E", not hex "E".  Separate the main value and exponent.
      ExpPos = InStr(sValue, "E+")
      If ExpPos = 0 Then
         ExpPos = InStr(sValue, "E-")
      End If
      sMainValue = Left(sValue, ExpPos - 1)
      ExponentSign = Mid(sValue, ExpPos + 1, 1)
      ExponentValue = Right(sValue, Len(sValue) - ExpPos - 1)

'     grab appropriate part of exponent depending on whether
'     we're displaying calculation result exponent or seconds.
      If InStr(sValue, ":") = 0 Then
         ExponentValue = Right(String(m_NumDigitsExp - 1, "0") & ExponentValue, m_NumDigitsExp - 1)
      Else
         ExponentValue = Right(String(m_NumDigitsExp - 1, "0") & ExponentValue, m_NumDigitsExp)
      End If
      If ExponentSign = "-" Then
         ExponentValue = ExponentSign & ExponentValue
      Else
         ExponentValue = " " & ExponentValue
      End If
      SeparateMainValueAndExponent = True

   Else

'     could be hex, or a non-exponential decimal value.  Just return the original value.
      sMainValue = sValue
      ExponentValue = String(m_NumDigitsExp, " ")

   End If

End Function

Private Sub DisplayMainValue(strValue As String, ByVal ForceDisplay As Boolean)

'*************************************************************************
'* displays non-exponent portion of value stored in .Value property.     *
'*************************************************************************

   Dim s            As String    ' right-justified version of value.
   Dim i            As Long      ' loop variable.
   Dim CurrentDigit As String    ' the current digit being displayed.

   s = strValue

'  determine thousands separator placement, if they are to be displayed.
   If m_ShowThousandsSeparator Then
      DetermineThousandsSeparatorPlacement s
      DisplayThousandsSeparators
   End If

'  erase old decimal separator if displayed; display new one if necessary.
   ProcessDecimalSeparator s

'  erase old colon if displayed; display new one if necessary.
   ProcessColon s

'  pad the value with leading spaces.
   s = Right(Space(m_NumDigits) & s, m_NumDigits)

'  display the main value.  Only draw a digit if the particular digit being
'  drawn is different from digit the previous time value was displayed.
   Select Case m_SegmentStyle

      Case [Hexagonal]
         For i = 1 To m_NumDigits
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousMainValue, i, 1)) Or ForceDisplay Then
               DisplayHexagonalSegmentDigit CurrentDigit, LCDSegment(), _
                                            DigitXPos(i - 1), m_YOffset, _
                                            m_SegmentHeight, m_SegmentWidth, _
                                            m_InterSegmentGap, DigitWidth, DigitHeight
            End If
         Next i

      Case [Trapezoidal]
         For i = 1 To m_NumDigits
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousMainValue, i, 1)) Or ForceDisplay Then
               DisplayTrapezoidalSegmentDigit CurrentDigit, LCDSegment(), _
                                              DigitXPos(i - 1), m_YOffset, _
                                              m_SegmentHeight, m_SegmentWidth, _
                                              m_InterSegmentGap, DigitWidth, DigitHeight
            End If
         Next i

      Case [Rectangular]
         For i = 1 To m_NumDigits
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousMainValue, i, 1)) Or ForceDisplay Then
               DisplayRectangularSegmentDigit CurrentDigit, LCDSegment(), _
                                              DigitXPos(i - 1), m_YOffset, _
                                              m_SegmentHeight, m_SegmentWidth, _
                                              m_InterSegmentGap, DigitWidth, DigitHeight
            End If
         Next i

   End Select

   UserControl.Refresh

'  save the displayed value so we know which LCD digits
'  to update the next time we need to display a value.
   PreviousMainValue = s

End Sub

Private Sub DisplayExponentValue(strValue As String, ByVal ForceDisplay As Boolean)

'*************************************************************************
'* displays exponent portion of value stored in .Value property.         *
'*************************************************************************

   Dim s            As String    ' right-justified version of exponent.
   Dim i            As Long      ' loop variable.
   Dim CurrentDigit As String    ' the current digit we're displaying.

   s = Right(Space(m_NumDigitsExp) & strValue, m_NumDigitsExp)

'  display the exponent.  Only draw a digit if the particular digit being
'  drawn is different from digit the previous time value was displayed.
   Select Case m_SegmentStyleExp

      Case [Hexagonal]
         For i = 1 To m_NumDigitsExp
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousExponentValue, i, 1)) Or ForceDisplay Then
               DisplayHexagonalSegmentDigit CurrentDigit, LCDSegmentExp(), _
                                            DigitXPosExp(i - 1), m_YOffsetExp, _
                                            m_SegmentHeightExp, m_SegmentWidthExp, _
                                            m_InterSegmentGapExp, DigitWidthExp, DigitHeightExp
            End If
         Next i

      Case [Trapezoidal]
         For i = 1 To m_NumDigitsExp
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousExponentValue, i, 1)) Or ForceDisplay Then
               DisplayTrapezoidalSegmentDigit CurrentDigit, LCDSegmentExp(), _
                                              DigitXPosExp(i - 1), m_YOffsetExp, _
                                              m_SegmentHeightExp, m_SegmentWidthExp, _
                                              m_InterSegmentGapExp, DigitWidthExp, DigitHeightExp
            End If
         Next i

      Case [Rectangular]
         For i = 1 To m_NumDigitsExp
            CurrentDigit = Mid(s, i, 1)
            If (CurrentDigit <> Mid(PreviousExponentValue, i, 1)) Or ForceDisplay Then
               DisplayRectangularSegmentDigit CurrentDigit, LCDSegmentExp(), _
                                              DigitXPosExp(i - 1), m_YOffsetExp, _
                                              m_SegmentHeightExp, m_SegmentWidthExp, _
                                              m_InterSegmentGapExp, DigitWidthExp, DigitHeightExp
            End If
         Next i

   End Select

   UserControl.Refresh

'  save the displayed value so we know which LCD digits
'  to update the next time we need to display a value.
   PreviousExponentValue = s

End Sub

Private Sub DetermineThousandsSeparatorPlacement(ByVal sVal As String)

'*************************************************************************
'* determines which digits in the main value get a thousands separator   *
'* afterwards.  ThousandsFlag() elements that correspond to LCD digits   *
'* that need to be followed by a thousands separator are set to True.    *
'* Thanks to Redbird77 for optimizing this routine.                      *
'*************************************************************************

   Dim i    As Long      ' loop variable.
   Dim p1   As Long      ' position of first non-zero/minus sign digit.
   Dim p2   As Long      ' position of decimal separator.
   Dim sTmp As String    ' padded value.

'  reset the ThousandsFlag boolean array to all False.  (Remember that False
'  is an integer (2-byte) value equal to zero, so this is a lightning-quick
'  way to set a dynamic boolean array, as opposed to looping.)
   FillMemory ThousandsFlag(0), 2 * m_NumDigits, False

'  Add decimal point to end if whole number.
   If InStr(sVal, ".") = 0 Then
      sVal = sVal & "."
   End If

'  left-pad value.
   sTmp = Right$(String$(m_NumDigits, "0") & sVal, m_NumDigits)

'  get position of decimal point.
   p2 = InStr(sTmp, ".")

'  get position of first non-zero and non-minus sign digit.
   p1 = m_NumDigits - Len(sVal) + 1
   If Mid$(sTmp, p1, 1) = "-" Then
      p1 = p1 + 1
   End If

'  flag appropriate digits that receive a decimal separator after them.
   For i = p2 - (m_ThousandsGrouping + 1) To p1 Step -m_ThousandsGrouping
      ThousandsFlag(i) = True
   Next i

End Sub

Private Sub DisplayThousandsSeparators()

'*************************************************************************
'* display needed thousands separators and erase any others.             *
'*************************************************************************

   Dim i    As Long   ' loop index.
   Dim r    As Long   ' bitblt function call return.
   Dim xPos As Long   ' x coordinate of thousands separator.
   Dim yPos As Long   ' y coordinate of thousands separator.

'  determine the Y coordinate of the thousands separator. If digit
'  segment style is rectangular, 1 is subtracted from Y coordinate.
   Select Case m_ThousandsSeparator
      Case [Period]
         yPos = m_YOffset + DigitHeight - m_SegmentWidth + 1 + (m_SegmentStyle = [Rectangular])
      Case [Comma]
         yPos = m_YOffset + DigitHeight - m_SegmentWidth - 1
   End Select

   For i = 0 To m_NumDigits - 1

'     calculate the starting X coordinates for the thousands separator.
      Select Case m_ThousandsSeparator
         Case [Period]
            xPos = DigitXPos(i) + DigitWidth + (m_InterDigitGap \ 2) - (m_SegmentWidth \ 2)
         Case [Comma]
            xPos = DigitXPos(i) + (DigitWidth) - (m_SegmentWidth)
      End Select

      If ThousandsFlag(i) Then
'        display a thousands separator.
         DisplaySegment LCDSegment(THOUSANDS_SEPARATOR_SEGMENT), xPos, yPos, SEGMENT_LIT
      Else
'        make sure a possible previous thousands separator is erased.
         Select Case m_ThousandsSeparator
            Case [Period]
               r = BitBlt(hdc, xPos, yPos, m_SegmentWidth, m_SegmentWidth, _
                          VirtualBackgroundDC, xPos, yPos, vbSrcCopy)
            Case [Comma]
               EraseIrregularRegion THOUSANDS_SEPARATOR_SEGMENT, xPos, yPos
         End Select
      End If

   Next i

End Sub

Private Sub ProcessColon(ByRef s As String)

'*************************************************************************
'* erases old colon, if one was displayed.  Displays new colon if needed *
'* and removes colon from passed value string.                           *
'* NOTE:  To use this control as a clock display, set the number of main *
'* value digits to 4, and the number of exponent digits to 2.  Pass the  *
'* time (either 12- or 24-hour mode) to the .Value property like this:   *
'* HH:MMe+SS, where HH:MM is the hours:minutes, and SS is the seconds.   *
'* The 'e+' tricks control into displaying the seconds in the exponent   *
'* part of the display.  If you wish to just display hours and minutes,  *
'* set the .ShowExponent property to False and just send HH:MM.          *
'*************************************************************************

   EraseOldColon
   DrawNewColon s

End Sub

Private Sub EraseOldColon()

'*************************************************************************
'* erases previously drawn colon by drawing background over it.          *
'*************************************************************************

   Dim r As Long    ' bitblt function call return.

'  only bother if there was actually a displayed colon.
   If ColonPos.TopPoint.x > -1 Then
      r = BitBlt(hdc, ColonPos.TopPoint.x, ColonPos.TopPoint.Y, m_SegmentWidth, m_SegmentWidth, _
                 VirtualBackgroundDC, ColonPos.TopPoint.x, ColonPos.TopPoint.Y, vbSrcCopy)
      r = BitBlt(hdc, ColonPos.BottomPoint.x, ColonPos.BottomPoint.Y, m_SegmentWidth, m_SegmentWidth, _
                 VirtualBackgroundDC, ColonPos.BottomPoint.x, ColonPos.BottomPoint.Y, vbSrcCopy)
   End If

End Sub

Private Sub DrawNewColon(ByRef s As String)

'*************************************************************************
'* draws new colon in correct location.                                  *
'*************************************************************************

   Dim i As Long    ' position of colon within value to be displayed.

'  check for existence of colon in value to be displayed.
   i = InStr(s, ":")

   If i > 0 Then

'     if colon needed, calculate the starting X and Y coordinates of each 'dot'.
      ColonPos.TopPoint.x = DigitXPos(i - 1 + (m_NumDigits - Len(s))) + DigitWidth + (m_InterDigitGap \ 2) - (m_SegmentWidth \ 2) + 1
      ColonPos.BottomPoint.x = ColonPos.TopPoint.x
      ColonPos.TopPoint.Y = m_YOffset + m_SegmentHeight \ 2 + m_SegmentWidth \ 2
      ColonPos.BottomPoint.Y = ColonPos.TopPoint.Y + m_SegmentHeight

'     display the colon.
      DisplaySegment LCDSegment(DECIMAL_SEPARATOR_SEGMENT), ColonPos.TopPoint.x, ColonPos.TopPoint.Y, SEGMENT_LIT
      DisplaySegment LCDSegment(DECIMAL_SEPARATOR_SEGMENT), ColonPos.BottomPoint.x, ColonPos.BottomPoint.Y, SEGMENT_LIT

'     remove the colon from the numeric string to be displayed.
      s = Left(s, i - 1) & Right(s, Len(s) - i)

   Else

'     flag it so control knows no colon has been drawn.
      ColonPos.TopPoint.x = -1

   End If

End Sub

Private Sub ProcessDecimalSeparator(ByRef s As String)

'*************************************************************************
'* erases old decimal point, if one was displayed.  Displays new decimal *
'* point if needed and removes decimal point from passed value string.   *
'*************************************************************************

   EraseOldDecimalSeparator
   DisplayNewDecimalSeparator s

End Sub

Private Sub EraseOldDecimalSeparator()

'*************************************************************************
'* erases previously drawn decimal point by drawing background over it.  *
'*************************************************************************

   Dim r As Long    ' bitblt function call return.

'  only bother if there was actually a displayed decimal separator.
   If DecimalSeparatorPos.x > -1 Then
      Select Case m_DecimalSeparator
         Case [Period]
            r = BitBlt(hdc, DecimalSeparatorPos.x, DecimalSeparatorPos.Y, m_SegmentWidth, m_SegmentWidth, _
                       VirtualBackgroundDC, DecimalSeparatorPos.x, DecimalSeparatorPos.Y, vbSrcCopy)
         Case [Comma]
            EraseIrregularRegion DECIMAL_SEPARATOR_SEGMENT, DecimalSeparatorPos.x, DecimalSeparatorPos.Y
      End Select
   End If

End Sub

Private Sub EraseIrregularRegion(ByVal RegionIndex As Long, ByVal xPos As Long, ByVal yPos As Long)

'*************************************************************************
'* erases a non-rectangular region by using SelectClipRgn to select the  *
'* desired clipping region.  The subsequent BitBlt blits to the entire   *
'* control, but only the selected clipping region is actually updated    *
'* with control background graphics.  The clipping region is then reset. *
'* I do this with comma separators because the bottom of the comma will  *
'* oftentimes be underneath the lower right corner of the preceding      *
'* digit and a straightforward rectangular blit would erase that lower   *
'* right corner of the preceding digit.  Thanks to LaVolpe for the tip.  *
'*************************************************************************

   Dim r               As Long    ' bitblt function call return.
   Dim CommaClipRegion As Long    ' clipping region for bitblt.

'  move the comma region to the decimal separator position.
   OffsetRgn LCDSegment(RegionIndex), xPos, yPos

'  select a clipping region consisting of the comma decimal separator segment.
   CommaClipRegion = SelectClipRgn(hdc, LCDSegment(RegionIndex))

'  blit the whole background back to the control.  Since the comma clipping region has been
'  selected, only that portion of the background will actually be drawn, thereby erasing the comma.
   r = BitBlt(hdc, 0, 0, ScaleWidth, ScaleHeight, VirtualBackgroundDC, 0, 0, vbSrcCopy)

'  remove the clipping region constraint from the control.
   SelectClipRgn hdc, ByVal 0&

'  delete the selected clipping region.
   DeleteObject CommaClipRegion

'  reset the comma region coordinates to 0,0.
   OffsetRgn LCDSegment(RegionIndex), -xPos, -yPos

End Sub

Private Sub DisplayNewDecimalSeparator(ByRef s As String)

'*************************************************************************
'* draws new decimal separator in correct location.                      *
'*************************************************************************

   Dim i As Long    ' position of decimal point within value to be displayed.

'  check for existence of decimal point in value to be displayed.
   i = InStr(s, ".")

   If i > 0 Then

'     calculate the starting X and Y coordinates for the decimal point.  If digit
'     segment style is rectangular, 1 is subtracted from the Y coordinate.  These
'     coordinates are retained for erasing the decimal separator when necessary.
      Select Case m_DecimalSeparator
         Case [Period]
            DecimalSeparatorPos.x = DigitXPos(i - 1 + (m_NumDigits - Len(s))) + DigitWidth + (m_InterDigitGap \ 2) - (m_SegmentWidth \ 2)
            DecimalSeparatorPos.Y = m_YOffset + DigitHeight - m_SegmentWidth + 1 + (m_SegmentStyle = [Rectangular])
         Case [Comma]
            DecimalSeparatorPos.x = DigitXPos(i - 1 + (m_NumDigits - Len(s))) + (DigitWidth) - (m_SegmentWidth)
            DecimalSeparatorPos.Y = m_YOffset + DigitHeight - m_SegmentWidth - 1
      End Select

'     display the decimal separator.
      DisplaySegment LCDSegment(DECIMAL_SEPARATOR_SEGMENT), DecimalSeparatorPos.x, DecimalSeparatorPos.Y, SEGMENT_LIT

'     remove the decimal separator from the numeric string to be displayed.
      s = Left(s, i - 1) & Right(s, Len(s) - i)

   Else

'     flag it so control knows no decimal separator has been drawn.
      DecimalSeparatorPos.x = -1

   End If

End Sub

Private Sub DisplayHexagonalSegmentDigit(ByVal strDigit As String, ByRef LCD() As Long, _
                                         ByVal OffsetX As Long, ByVal OffsetY As Long, _
                                         ByVal SegmentHeight As Long, ByVal SegmentWidth As Long, _
                                         ByVal SegmentGap As Long, _
                                         ByVal DigWidth As Long, ByVal DigHeight As Long)

'*************************************************************************
'* displays one hex-segment display digit according to string pattern.   *
'*************************************************************************

   Dim Digit               As Long    ' the display pattern index of the current digit to draw.
   Dim r                   As Long    ' bitblt function call return.

'  used to avoid unnecessary recalculations of segment gap multiples.  Just a speed
'  tweak for situations where the display must be updated quickly (as in a counter).
   Dim DoubleSegmentGap    As Long
   Dim TripleSegmentGap    As Long
   Dim QuadrupleSegmentGap As Long
   Dim HalfSegmentWidth    As Long

   DoubleSegmentGap = 2 * SegmentGap
   TripleSegmentGap = 3 * SegmentGap
   QuadrupleSegmentGap = 4 * SegmentGap
   HalfSegmentWidth = SegmentWidth \ 2

'  blit the appropriate portion of the background over the digit position to 'erase' old digit.
   r = BitBlt(hdc, OffsetX, OffsetY, DigWidth, DigHeight, VirtualBackgroundDC, OffsetX, OffsetY, vbSrcCopy)

'  get the appropriate segment display pattern for the digit.
   Digit = GetDisplayPatternIndex(strDigit)
   If Digit = -1 Then
      Exit Sub
   End If

'  segment 1 (top)
   DisplaySegment LCD(HORIZONTAL_HEXAGONAL_SEGMENT), _
                  OffsetX + HalfSegmentWidth + SegmentGap, _
                  OffsetY, _
                  Mid(DisplayPattern(Digit), 1, 1)

'  segment 2 (top right)
   DisplaySegment LCD(VERTICAL_HEXAGONAL_SEGMENT), _
                  OffsetX + SegmentHeight + DoubleSegmentGap, _
                  OffsetY + HalfSegmentWidth + SegmentGap, _
                  Mid(DisplayPattern(Digit), 2, 1)

'  segment 3 (bottom right)
   DisplaySegment LCD(VERTICAL_HEXAGONAL_SEGMENT), _
                  OffsetX + SegmentHeight + DoubleSegmentGap, _
                  OffsetY + SegmentHeight + HalfSegmentWidth + TripleSegmentGap, _
                  Mid(DisplayPattern(Digit), 3, 1)

'  segment 4 (bottom)
   DisplaySegment LCD(HORIZONTAL_HEXAGONAL_SEGMENT), _
                  OffsetX + HalfSegmentWidth + SegmentGap, _
                  OffsetY + (2 * SegmentHeight) + QuadrupleSegmentGap, _
                  Mid(DisplayPattern(Digit), 4, 1)

'  segment 5 (bottom left)
   DisplaySegment LCD(VERTICAL_HEXAGONAL_SEGMENT), _
                  OffsetX, _
                  OffsetY + SegmentHeight + HalfSegmentWidth + TripleSegmentGap, _
                  Mid(DisplayPattern(Digit), 5, 1)

'  segment 6 (top left)
   DisplaySegment LCD(VERTICAL_HEXAGONAL_SEGMENT), _
                  OffsetX, _
                  OffsetY + HalfSegmentWidth + SegmentGap, _
                  Mid(DisplayPattern(Digit), 6, 1)

'  segment 7 (center)
   DisplaySegment LCD(HORIZONTAL_HEXAGONAL_SEGMENT), _
                  OffsetX + HalfSegmentWidth + SegmentGap, _
                  OffsetY + SegmentHeight + DoubleSegmentGap, _
                  Mid(DisplayPattern(Digit), 7, 1)

End Sub

Private Sub DisplayTrapezoidalSegmentDigit(ByVal strDigit As String, ByRef LCD() As Long, _
                                           ByVal OffsetX As Long, ByVal OffsetY As Long, _
                                           ByVal SegmentHeight As Long, ByVal SegmentWidth As Long, _
                                           ByVal SegmentGap As Long, ByVal DigWidth As Long, _
                                           ByVal DigHeight As Long)

'*************************************************************************
'* displays a trapezoidal-segment display digit according to pattern.    *
'*************************************************************************

   Dim Digit As Long    ' the display pattern index of the current digit to draw.
   Dim r As Long        ' bitblt function call return.

'  used to avoid unnecessary recalculations of segment gap multiples.  Just a speed
'  tweak for situations where the display must be updated quickly (as in a counter).
   Dim DoubleSegmentGap    As Long
   Dim TripleSegmentGap    As Long
   Dim QuadrupleSegmentGap As Long

   DoubleSegmentGap = 2 * SegmentGap
   TripleSegmentGap = 3 * SegmentGap
   QuadrupleSegmentGap = 4 * SegmentGap

'  blit the appropriate portion of the background over the digit position to 'erase' old digit.
   r = BitBlt(hdc, OffsetX, OffsetY, DigWidth, DigHeight, VirtualBackgroundDC, OffsetX, OffsetY, vbSrcCopy)

'  get the appropriate segment display pattern for the digit.
   Digit = GetDisplayPatternIndex(strDigit)
   If Digit = -1 Then
      Exit Sub
   End If

'  segment 1 (top)
   DisplaySegment LCD(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX + SegmentGap, _
                  OffsetY, _
                  Mid(DisplayPattern(Digit), 1, 1)

'  segment 2 (top right)
   DisplaySegment LCD(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX + SegmentHeight + DoubleSegmentGap - SegmentWidth, _
                  OffsetY + SegmentGap, _
                  Mid(DisplayPattern(Digit), 2, 1)

'  segment 3 (bottom right)
   DisplaySegment LCD(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX + SegmentHeight + DoubleSegmentGap - SegmentWidth, _
                  OffsetY + SegmentHeight + TripleSegmentGap, _
                  Mid(DisplayPattern(Digit), 3, 1)

'  segment 4 (bottom)
   DisplaySegment LCD(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX + SegmentGap, _
                  OffsetY + (2 * SegmentHeight) + QuadrupleSegmentGap - SegmentWidth, _
                  Mid(DisplayPattern(Digit), 4, 1)

'  segment 5 (bottom left)
   DisplaySegment LCD(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX, _
                  OffsetY + SegmentHeight + TripleSegmentGap, _
                  Mid(DisplayPattern(Digit), 5, 1)

'  segment 6 (top left)
   DisplaySegment LCD(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT), _
                  OffsetX, _
                  OffsetY + SegmentGap, _
                  Mid(DisplayPattern(Digit), 6, 1)

'  segment 7 (middle)
   DisplaySegment LCD(HORIZONTAL_HEXAGONAL_SEGMENT), _
                  OffsetX + SegmentGap, _
                  OffsetY + SegmentHeight + DoubleSegmentGap - (SegmentWidth \ 2), _
                  Mid(DisplayPattern(Digit), 7, 1)

End Sub

Private Sub DisplayRectangularSegmentDigit(ByVal strDigit As String, ByRef LCD() As Long, _
                                           ByVal OffsetX As Long, ByVal OffsetY As Long, _
                                           ByVal SegmentHeight As Long, ByVal SegmentWidth As Long, _
                                           ByVal SegmentGap As Long, ByVal DigWidth As Long, _
                                           ByVal DigHeight As Long)

'*************************************************************************
'* displays a rectangular-segment display digit according to pattern.    *
'*************************************************************************

   Dim Digit As Long    ' the display pattern index of the current digit to draw.
   Dim r     As Long    ' bitblt function call return.

'  used to avoid unnecessary recalculations of segment gap multiples.  Just a speed
'  tweak for situations where the display must be updated quickly (as in a counter).
   Dim DoubleSegmentGap    As Long
   Dim TripleSegmentGap    As Long
   Dim QuadrupleSegmentGap As Long
   Dim DoubleSegmentWidth  As Long

   DoubleSegmentGap = 2 * SegmentGap
   TripleSegmentGap = 3 * SegmentGap
   QuadrupleSegmentGap = 4 * SegmentGap
   DoubleSegmentWidth = 2 * SegmentWidth

'  blit the appropriate portion of the background over the digit position to 'erase' old digit.
   r = BitBlt(hdc, OffsetX, OffsetY, DigWidth, DigHeight, VirtualBackgroundDC, OffsetX, OffsetY, vbSrcCopy)

'  get the appropriate segment display pattern for the digit.
   Digit = GetDisplayPatternIndex(strDigit)
   If Digit = -1 Then
      Exit Sub
   End If

'  segment 1 (top)
   DisplaySegment LCD(HORIZONTAL_RECTANGULAR_SEGMENT), _
                  OffsetX + SegmentWidth + SegmentGap - 1, _
                  OffsetY, _
                  Mid(DisplayPattern(Digit), 1, 1)

'  segment 2 (top right)
   DisplaySegment LCD(VERTICAL_RECTANGULAR_SEGMENT), _
                  OffsetX + SegmentWidth + SegmentHeight + DoubleSegmentGap - 2, _
                  OffsetY + SegmentWidth + SegmentGap - 1, _
                  Mid(DisplayPattern(Digit), 2, 1)

'  segment 3 (bottom right)
   DisplaySegment LCD(VERTICAL_RECTANGULAR_SEGMENT), _
                  OffsetX + SegmentWidth + SegmentHeight + DoubleSegmentGap - 2, _
                  OffsetY + SegmentHeight + DoubleSegmentWidth + TripleSegmentGap - 3, _
                  Mid(DisplayPattern(Digit), 3, 1)

'  segment 4 (bottom)
   DisplaySegment LCD(HORIZONTAL_RECTANGULAR_SEGMENT), _
                  OffsetX + SegmentWidth + SegmentGap - 1, _
                  OffsetY + (2 * SegmentHeight) + DoubleSegmentWidth + QuadrupleSegmentGap - 4, _
                  Mid(DisplayPattern(Digit), 4, 1)

'  segment 5 (bottom left)
   DisplaySegment LCD(VERTICAL_RECTANGULAR_SEGMENT), _
                  OffsetX, _
                  OffsetY + SegmentHeight + DoubleSegmentWidth + TripleSegmentGap - 3, _
                  Mid(DisplayPattern(Digit), 5, 1)

'  segment 6 (top left)
   DisplaySegment LCD(VERTICAL_RECTANGULAR_SEGMENT), _
                  OffsetX, _
                  OffsetY + SegmentWidth + SegmentGap - 1, _
                  Mid(DisplayPattern(Digit), 6, 1)

'  segment 7 (center)
   DisplaySegment LCD(HORIZONTAL_RECTANGULAR_SEGMENT), _
                  OffsetX + SegmentWidth + SegmentGap - 1, _
                  OffsetY + SegmentHeight + SegmentWidth + DoubleSegmentGap - 2, _
                  Mid(DisplayPattern(Digit), 7, 1)

End Sub

Private Function GetDisplayPatternIndex(ByVal strDigit As String) As Long

'*************************************************************************
'* returns correct segment lighting pattern index for supplied digit.    *
'*************************************************************************

   If strDigit = " " And m_ShowBurnIn Then
'     for showing unlit digits in burn-in display mode.
      GetDisplayPatternIndex = 10
   ElseIf strDigit = " " Then
'     if not showing burn-in pattern, don't mess with an unlit digit at all.
      GetDisplayPatternIndex = -1
   ElseIf strDigit = "-" Then
'     the pattern index for the minus sign.
      GetDisplayPatternIndex = 11
   ElseIf InStr("ABCDEF", strDigit) Then
'     the pattern index for the appropriate hex value A-F.
      GetDisplayPatternIndex = Asc(strDigit) - 53
   Else
'     the appropriate pattern index for the supplied digit.
      GetDisplayPatternIndex = Val(strDigit)
   End If

End Function

Private Sub DisplaySegment(ByVal Segment As Long, ByVal StartX As Long, ByVal StartY As Long, ByVal LitStatus As String)

'*************************************************************************
'* displays one segment of an LCD digit according to its fill style.     *
'*************************************************************************

'  position the segment region in the correct location.
   OffsetRgn Segment, StartX, StartY

   If LitStatus = SEGMENT_UNLIT And m_ShowBurnIn Then
'     if segment is unlit but burn-in mode is active, display as unlit according to fill mode.
      If m_SegmentFillStyle = [Solid] Then
         FillRgn hdc, Segment, CurrentBurnInColorBrush
      Else
         FrameRgn hdc, Segment, CurrentBurnInColorBrush, 1, 1
      End If
   Else
      If LitStatus = SEGMENT_LIT Then
'        otherwise, if segment is lit, display according to fill mode.
         If m_SegmentFillStyle = [Solid] Then
            FillRgn hdc, Segment, CurrentLitColorBrush
         Else
            FrameRgn hdc, Segment, CurrentLitColorBrush, 1, 1
         End If
      End If
   End If

'  reset the region location to (0, 0) to prepare for the next segment draw.
   OffsetRgn Segment, -StartX, -StartY

End Sub

Private Function CreateHexRegion(ByVal cx As Long, ByVal cy As Long) As Long

'*************************************************************************
'* Author: LaVolpe                                                       *
'* creates a horizontal/vertical hex region with perfectly smooth edges. *
'* the cx & cy parameters are respective width & height of the region.   *
'* passed values may be modified which coder can use for other purposes  *
'* like drawing borders or calculating the client/clipping region.       *
'*************************************************************************

   Dim tpts(0 To 7) As POINTAPI    ' holds polygon region vertices.

   If cy > cx Then             ' vertical hex vs horizontal

'     absolute minimum width & height of a hex region
      If cx < 4 Then
         cx = 4
      End If
'     ensure width is even
      If cx Mod 2 Then
         cx = cx - 1
      End If

'     calculate the vertical hex.
      tpts(0).x = cx \ 2              ' bot apex
      tpts(0).Y = cy
      tpts(1).x = cx                  ' bot right
      tpts(1).Y = cy - tpts(0).x
      tpts(2).x = cx                  ' top right
      tpts(2).Y = tpts(0).x - 1
      tpts(3).x = tpts(0).x           ' top apex
      tpts(3).Y = -1
'     add an extra point & modify; trial & error shows without this
'     added point, getting a nice smooth diagonal edge is impossible
      tpts(4).x = tpts(0).x - 1       ' added
      tpts(4).Y = 0
      tpts(5).x = 0                   ' top left
      tpts(5).Y = tpts(2).Y
      tpts(6).x = 0                   ' bot left
      tpts(6).Y = tpts(1).Y
      tpts(7) = tpts(0)               ' bot apex, close polygon

   Else

'     absolute minimum width & height of a hex region
      If cy < 4 Then
         cy = 4
      End If

'     ensure height is even
      If cy Mod 2 Then
         cy = cy - 1
      End If

'     calculate the horizontal hex.
      tpts(0).x = 0                   ' left apex
      tpts(0).Y = cy \ 2
      tpts(1).x = tpts(0).Y           ' bot left
      tpts(1).Y = cy
      tpts(2).x = cx - tpts(0).Y      ' bot right
      tpts(2).Y = tpts(1).Y
      tpts(3).x = cx                  ' right apex
      tpts(3).Y = tpts(0).Y
'     add an extra point & modify; trial & error shows without this
'     added point, getting a nice smooth diagonal edge is impossible
      tpts(4).x = cx
      tpts(4).Y = tpts(3).Y - 1
      tpts(5).x = tpts(2).x + 1       ' top right
      tpts(5).Y = 0
      tpts(6).x = tpts(1).x - 1       ' top left
      tpts(6).Y = 0
      tpts(7).x = tpts(0).x           ' left apex, close polygon
      tpts(7).Y = tpts(0).Y - 1

   End If

   CreateHexRegion = CreatePolygonRgn(tpts(0), UBound(tpts) + 1, 2)

End Function

Private Function CreateDiagRectRegion(ByVal cx As Long, ByVal cy As Long, SideAStyle As Integer, SideBStyle As Integer) As Long

'**************************************************************************
'* Author: LaVolpe                                                        *
'* the cx & cy parameters are the respective width & height of the region *
'* the passed values may be modified which coder can use for other purp-  *
'* oses like drawing borders or calculating the client/clipping region.   *
'* SideAStyle is -1, 0 or 1 depending on horizontal/vertical shape,       *
'*            reflects the left or top side of the region                 *
'*            -1 draws left/top edge like /                               *
'*            0 draws left/top edge like  |                               *
'*            1 draws left/top edge like  \                               *
'* SideBStyle is -1, 0 or 1 depending on horizontal/vertical shape,       *
'*            reflects the right or bottom side of the region             *
'*            -1 draws right/bottom edge like \                           *
'*            0 draws right/bottom edge like  |                           *
'*            1 draws right/bottom edge like  /                           *
'**************************************************************************

   Dim tpts(0 To 4) As POINTAPI    ' holds polygonal region vertices.

   If cx > cy Then ' horizontal

'     absolute minimum width & height of a trapezoid
      If Abs(SideAStyle + SideBStyle) = 2 Then ' has 2 opposing slanted sides
         If cx < cy * 2 Then cy = cx \ 2
      End If

      If SideAStyle < 0 Then
         tpts(0).x = cy - 1
         tpts(1).x = -1
      ElseIf SideAStyle > 0 Then
         tpts(1).x = cy
      End If
      tpts(1).Y = cy

      tpts(2).x = cx + Abs(SideBStyle < 0)
      If SideBStyle > 0 Then tpts(2).x = tpts(2).x - cy
      tpts(2).Y = cy

      tpts(3).x = cx + Abs(SideBStyle < 0)
      If SideBStyle < 0 Then tpts(3).x = tpts(3).x - cy

   Else

'     absolute minimum width & height of a trapezoid
      If Abs(SideAStyle + SideBStyle) = 2 Then ' has 2 opposing slanted sides
         If cy < cx * 2 Then cx = cy \ 2
      End If

      If SideAStyle < 0 Then
         tpts(0).Y = cx - 1
         tpts(3).Y = -1
      ElseIf SideAStyle > 0 Then
         tpts(3).Y = cx - 1
         tpts(0).Y = -1
      End If

      tpts(1).Y = cy
      If SideBStyle < 0 Then tpts(1).Y = tpts(1).Y - cx
      tpts(2).x = cx

      tpts(2).Y = cy
      If SideBStyle > 0 Then tpts(2).Y = tpts(2).Y - cx
      tpts(3).x = cx

   End If

   tpts(4) = tpts(0)

   CreateDiagRectRegion = CreatePolygonRgn(tpts(0), UBound(tpts) + 1, 2)

End Function

Private Sub RedrawControl()

'*************************************************************************
'* master routine for painting of MorphDisplay control.                  *
'*************************************************************************

   SetBackGround                             ' display background gradient or bitmap.
   CreateBorder                              ' display border if width > 0.
   DisplayValue m_Value, FORCE_REDRAW_YES    ' display the value; force value redraw.

   UserControl.Refresh

End Sub

Private Function TranslateColor(ByVal oClr As OLE_COLOR, Optional hPal As Long = 0) As Long

'*************************************************************************
'* converts color long COLORREF for api coloring purposes.               *
'*************************************************************************

   If OleTranslateColor(oClr, hPal, TranslateColor) Then
      TranslateColor = -1
   End If

End Function

Private Sub InitLCDDisplayCharacteristics()

'*************************************************************************
'* initializes gradients, picture, and border.                           *
'*************************************************************************

   Dim r As Long    ' bitblt function call return.

   ReDim ThousandsFlag(0 To m_NumDigits - 1)

'  create a virtual bitmap that will hold the background gradient or picture.  Portions of
'  this virtual bitmap are blitted to the control background to restore the background
'  gradient/picture when display digits are changed.  Saves time over repainting whole control
'  when we're just doing things like changing a digit or moving the decimal separator.
   CreateVirtualBackgroundDC

'  determine digital display background gradient that may be used in the control.
   CalculateBackGroundGradient

'  transfer background, whether bitmap or gradient, to control and virtual bitmap.
   If IsPictureThere(m_Picture) Then
      DisplayPicture
      CreateBorder
'     transfer the picture (with border) to the virtual DC bitmap.
      r = BitBlt(VirtualBackgroundDC, 0, 0, ScaleWidth, ScaleHeight, hdc, 0, 0, vbSrcCopy)
   Else
'     paint the gradient onto the virtual DC bitmap.
      Call StretchDIBits(VirtualBackgroundDC, _
                         0, 0, _
                         ScaleWidth, ScaleHeight, _
                         0, 0, _
                         ScaleWidth, ScaleHeight, _
                         BGlBits(0), BGuBIH, _
                         DIB_RGB_COLORS, _
                         vbSrcCopy)
'     transfer the gradient in the virtual bitmap to the usercontrol.
      r = BitBlt(hdc, 0, 0, ScaleWidth, ScaleHeight, VirtualBackgroundDC, 0, 0, vbSrcCopy)
      CreateBorder
   End If

'  create the segment regions and color brushes.
   CreateLCDSegments
   CreateLCDColorBrushes

'  calculate the x-offset of each main value and exponent digit.
   MapLCDDigits

End Sub

Private Sub MapLCDDigits()

'*************************************************************************
'* maps out the starting x coordinate of each main and exponent digit.   *
'*************************************************************************

   MapMainDigits
   MapExponentDigits

End Sub

Private Sub MapMainDigits()

'*************************************************************************
'* maps the starting x position of each main LCD digit.                  *
'*************************************************************************

   Dim i As Long

'  determine the height and width of a main value LCD digit, based on segment style.
   Select Case m_SegmentStyle

      Case [Hexagonal]
         DigitWidth = m_SegmentHeight + m_SegmentWidth + (2 * m_InterSegmentGap)
         DigitHeight = (2 * m_SegmentHeight) + m_SegmentWidth + (4 * m_InterSegmentGap)

      Case [Trapezoidal]
         DigitWidth = m_SegmentHeight + (2 * m_InterSegmentGap)
         DigitHeight = (2 * m_SegmentHeight) + (4 * m_InterSegmentGap)

      Case [Rectangular]
         DigitWidth = m_SegmentHeight + (2 * m_SegmentWidth) + (2 * m_InterSegmentGap) - 3
         DigitHeight = (2 * m_SegmentHeight) + (3 * m_SegmentWidth) + (4 * m_InterSegmentGap) - 4

   End Select

'  calculate and store the x offset for each display digit.
   DigitXPos(0) = m_BorderWidth + m_XOffset
   For i = 1 To m_NumDigits - 1
      DigitXPos(i) = DigitXPos(i - 1) + DigitWidth + m_InterDigitGap
   Next i

End Sub

Private Sub MapExponentDigits()

'*************************************************************************
'* maps the starting x position of each exponent LCD digit.              *
'*************************************************************************

   Dim i As Long    ' loop variable.

'  determine the height and width of an exponent LCD digit, based on segment style.
   Select Case m_SegmentStyleExp

      Case [Hexagonal]
         DigitWidthExp = m_SegmentHeightExp + m_SegmentWidthExp + (2 * m_InterSegmentGapExp)
         DigitHeightExp = (2 * m_SegmentHeightExp) + m_SegmentWidthExp + (4 * m_InterSegmentGapExp)

      Case [Trapezoidal]
         DigitWidthExp = m_SegmentHeightExp + (2 * m_InterSegmentGapExp)
         DigitHeightExp = (2 * m_SegmentHeightExp) + (4 * m_InterSegmentGapExp)

      Case [Rectangular]
         DigitWidthExp = m_SegmentHeightExp + (2 * m_SegmentWidthExp) + (2 * m_InterSegmentGapExp) - 3
         DigitHeightExp = (2 * m_SegmentHeightExp) + (3 * m_SegmentWidthExp) + (4 * m_InterSegmentGapExp) - 4

   End Select

'  calculate and store the x offset for each display digit.
   DigitXPosExp(0) = m_BorderWidth + m_XOffsetExp
   For i = 1 To m_NumDigitsExp - 1
      DigitXPosExp(i) = DigitXPosExp(i - 1) + DigitWidthExp + m_InterDigitGapExp
   Next i

End Sub

Private Sub CreateLCDColorBrushes()

'*************************************************************************
'* creates brushes used by FillRgn and FrameRgn to color LCD segments.   *
'*************************************************************************

'  delete any previously created color brushes.
   If LCDLitColorBrush Then DeleteObject LCDLitColorBrush
   If LCDBurnInColorBrush Then DeleteObject LCDBurnInColorBrush
   If LCDLitColorBrushNeg Then DeleteObject LCDLitColorBrushNeg
   If LCDBurnInColorBrushNeg Then DeleteObject LCDBurnInColorBrushNeg

'  generate the color brush to fill the lit segment objects with.
   LCDLitColorBrush = CreateSolidBrush(TranslateColor(m_SegmentLitColor))

'  generate the color brush to fill the 'burn-in' segment objects with.
   LCDBurnInColorBrush = CreateSolidBrush(TranslateColor(m_BurnInColor))

'  generate the color brush for lit negative-number segments.
   LCDLitColorBrushNeg = CreateSolidBrush(TranslateColor(m_SegmentLitColorNeg))

'  generate the color brush for negative value 'burn-in' segments.
   LCDBurnInColorBrushNeg = CreateSolidBrush(TranslateColor(m_BurnInColorNeg))

End Sub

Private Sub CreateLCDSegments()

'*************************************************************************
'* creates the shaped regions that define LCD digit segments.            *
'*************************************************************************

   CreateRectangularLCDSegments MAINVALUE
   CreateRectangularLCDSegments EXPONENT

   CreateHexagonalLCDSegments MAINVALUE
   CreateHexagonalLCDSegments EXPONENT

   CreateTrapezoidalLCDSegments MAINVALUE
   CreateTrapezoidalLCDSegments EXPONENT

   CreateDecimalSeparatorSegment
   CreateThousandsSeparatorSegment

End Sub

Private Sub CreateDecimalSeparatorSegment()

'*************************************************************************
'* creates a comma or period decimal separator shaped region.            *
'*************************************************************************

   Select Case m_DecimalSeparator

      Case [Period]
         CreatePeriodSeparator DECIMAL_SEPARATOR_SEGMENT

      Case [Comma]
         CreateCommaSeparator DECIMAL_SEPARATOR_SEGMENT

   End Select

End Sub

Private Sub CreateThousandsSeparatorSegment()

'*************************************************************************
'* creates a comma or period decimal separator shaped region.            *
'*************************************************************************

   Select Case m_ThousandsSeparator

      Case [Period]
         CreatePeriodSeparator THOUSANDS_SEPARATOR_SEGMENT

      Case [Comma]
         CreateCommaSeparator THOUSANDS_SEPARATOR_SEGMENT

   End Select

End Sub

Private Sub CreatePeriodSeparator(ByVal SegmentIndex As Long)

'*************************************************************************
'* creates a period-shaped region for thousands or decimal separator.    *
'*************************************************************************

'  delete segments if they exist before recreating.
   If LCDSegment(SegmentIndex) Then DeleteObject LCDSegment(SegmentIndex)
   If LCDSegmentExp(SegmentIndex) Then DeleteObject LCDSegmentExp(SegmentIndex)

   LCDSegment(SegmentIndex) = CreateRectRgn(0, 0, m_SegmentWidth - 1, m_SegmentWidth - 1)
   LCDSegmentExp(SegmentIndex) = CreateRectRgn(0, 0, m_SegmentWidthExp - 1, m_SegmentWidthExp - 1)

End Sub

Private Sub CreateCommaSeparator(ByVal SegmentIndex As Long)

'*************************************************************************
'* creates a comma-shaped region for thousands or decimal separator.     *
'*************************************************************************

   Dim tpts(0 To 3) As POINTAPI    ' vertices for shaped region.

'  delete segments if they exist before recreating.
   If LCDSegment(SegmentIndex) Then DeleteObject LCDSegment(SegmentIndex)
   If LCDSegmentExp(SegmentIndex) Then DeleteObject LCDSegmentExp(SegmentIndex)

'  create the main value comma region.
   tpts(0).x = m_SegmentHeight \ 2                       ' top left corner.
   tpts(0).Y = m_YOffset - m_SegmentWidth                ' tweak by redbird77

   tpts(1).x = tpts(0).x + m_SegmentWidth                ' top right corner.
   tpts(1).Y = tpts(0).Y

   tpts(2).x = m_SegmentWidth                            ' bottom right corner.
   tpts(2).Y = tpts(0).Y + (m_SegmentHeight)

   tpts(3).x = 0                                         ' bottom left corner.
   tpts(3).Y = tpts(2).Y

   LCDSegment(SegmentIndex) = CreatePolygonRgn(tpts(0), UBound(tpts) + 1, 2)

'  create the exponent comma region.
   tpts(0).x = m_SegmentHeightExp \ 2                    ' top left corner.
   tpts(0).Y = m_YOffsetExp - m_SegmentWidthExp

   tpts(1).x = tpts(0).x + m_SegmentWidthExp             ' top right corner.
   tpts(1).Y = tpts(0).Y

   tpts(2).x = m_SegmentWidthExp                         ' bottom right corner.
   tpts(2).Y = tpts(0).Y + (m_SegmentHeightExp)

   tpts(3).x = 0                                         ' bottom left corner.
   tpts(3).Y = tpts(2).Y

   LCDSegmentExp(SegmentIndex) = CreatePolygonRgn(tpts(0), UBound(tpts) + 1, 2)

End Sub

Private Sub CreateRectangularLCDSegments(SegmentType As Long)

'*************************************************************************
'* creates the vertical and horizontal rectangular LCD segments.         *
'*************************************************************************

   Select Case SegmentType

      Case MAINVALUE
         If LCDSegment(VERTICAL_RECTANGULAR_SEGMENT) Then DeleteObject LCDSegment(VERTICAL_RECTANGULAR_SEGMENT)
         If LCDSegment(HORIZONTAL_RECTANGULAR_SEGMENT) Then DeleteObject LCDSegment(HORIZONTAL_RECTANGULAR_SEGMENT)
         LCDSegment(VERTICAL_RECTANGULAR_SEGMENT) = CreateRectRgn(0, 0, m_SegmentWidth - 1, m_SegmentHeight - 1)
         LCDSegment(HORIZONTAL_RECTANGULAR_SEGMENT) = CreateRectRgn(0, 0, m_SegmentHeight - 1, m_SegmentWidth - 1)

      Case EXPONENT
         If LCDSegmentExp(VERTICAL_RECTANGULAR_SEGMENT) Then DeleteObject LCDSegmentExp(VERTICAL_RECTANGULAR_SEGMENT)
         If LCDSegmentExp(HORIZONTAL_RECTANGULAR_SEGMENT) Then DeleteObject LCDSegmentExp(HORIZONTAL_RECTANGULAR_SEGMENT)
         LCDSegmentExp(VERTICAL_RECTANGULAR_SEGMENT) = CreateRectRgn(0, 0, m_SegmentWidthExp - 1, m_SegmentHeightExp - 1)
         LCDSegmentExp(HORIZONTAL_RECTANGULAR_SEGMENT) = CreateRectRgn(0, 0, m_SegmentHeightExp - 1, m_SegmentWidthExp - 1)

   End Select

End Sub

Private Sub CreateHexagonalLCDSegments(SegmentType As Long)

'*************************************************************************
'* creates the vertical and horizontal hexagonal LCD segments.           *
'*************************************************************************

   Select Case SegmentType

      Case MAINVALUE
         If LCDSegment(VERTICAL_HEXAGONAL_SEGMENT) Then DeleteObject LCDSegment(VERTICAL_HEXAGONAL_SEGMENT)
         If LCDSegment(HORIZONTAL_HEXAGONAL_SEGMENT) Then DeleteObject LCDSegment(HORIZONTAL_HEXAGONAL_SEGMENT)
         LCDSegment(VERTICAL_HEXAGONAL_SEGMENT) = CreateHexRegion(m_SegmentWidth, m_SegmentHeight)
         LCDSegment(HORIZONTAL_HEXAGONAL_SEGMENT) = CreateHexRegion(m_SegmentHeight, m_SegmentWidth)

      Case EXPONENT
         If LCDSegmentExp(VERTICAL_HEXAGONAL_SEGMENT) Then DeleteObject LCDSegmentExp(VERTICAL_HEXAGONAL_SEGMENT)
         If LCDSegmentExp(HORIZONTAL_HEXAGONAL_SEGMENT) Then DeleteObject LCDSegmentExp(HORIZONTAL_HEXAGONAL_SEGMENT)
         LCDSegmentExp(VERTICAL_HEXAGONAL_SEGMENT) = CreateHexRegion(m_SegmentWidthExp, m_SegmentHeightExp)
         LCDSegmentExp(HORIZONTAL_HEXAGONAL_SEGMENT) = CreateHexRegion(m_SegmentHeightExp, m_SegmentWidthExp)

   End Select

End Sub

Private Sub CreateTrapezoidalLCDSegments(SegmentType As Long)

'*************************************************************************
'* creates the vertical and horizontal trapezoidal LCD segments.         *
'*************************************************************************

   Select Case SegmentType

      Case MAINVALUE
         If LCDSegment(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegment(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegment(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegment(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegment(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegment(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegment(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegment(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT)
         LCDSegment(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentHeight, m_SegmentWidth, 1, 1)
         LCDSegment(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentWidth, m_SegmentHeight, -1, -1)
         LCDSegment(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentHeight, m_SegmentWidth, -1, -1)
         LCDSegment(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentWidth, m_SegmentHeight, 1, 1)

      Case EXPONENT
         If LCDSegmentExp(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegmentExp(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegmentExp(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegmentExp(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegmentExp(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegmentExp(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT)
         If LCDSegmentExp(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT) Then DeleteObject LCDSegmentExp(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT)
         LCDSegmentExp(HORIZONTAL_DOWNWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentHeightExp, m_SegmentWidthExp, 1, 1)
         LCDSegmentExp(VERTICAL_LEFTWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentWidthExp, m_SegmentHeightExp, -1, -1)
         LCDSegmentExp(HORIZONTAL_UPWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentHeightExp, m_SegmentWidthExp, -1, -1)
         LCDSegmentExp(VERTICAL_RIGHTWARD_TRAPEZOIDAL_SEGMENT) = CreateDiagRectRegion(m_SegmentWidthExp, m_SegmentHeightExp, 1, 1)

   End Select

End Sub

Private Sub CalculateBackGroundGradient()

'*************************************************************************
'* calculate the gradient for the background.  Even if a picture is used *
'* instead of a gradient, this allows control user to switch back and    *
'* forth between those two options in design or runtime modes.           *
'*************************************************************************

   CalculateGradient ScaleWidth, ScaleHeight, _
                     TranslateColor(m_BackColor1), TranslateColor(m_BackColor2), _
                     m_BackAngle, m_BackMiddleOut, _
                     BGuBIH, BGlBits()

End Sub

Private Sub SetBackGround()

'*************************************************************************
'* displays control's background gradient or picture in initial draw.    *
'*************************************************************************

   If IsPictureThere(m_Picture) Then
'     if the .Picture property has been defined, it takes precedence over gradient.
      DisplayPicture
   Else
'     paint the gradient onto the actual usercontrol DC.  Most subsequent repaints are handled
'     by blitting the appropriate gradient portions from the virtual bitmap's DC to the usercontrol.
'     Thanks to RedBird77 for tweaking this to work correctly with wide borders!
      Call StretchDIBits(hdc, m_BorderWidth, m_BorderWidth, _
                         ScaleWidth - (m_BorderWidth * 2), _
                         ScaleHeight - (m_BorderWidth * 2), _
                         m_BorderWidth, m_BorderWidth, _
                         ScaleWidth - (m_BorderWidth * 2), _
                         ScaleHeight - (m_BorderWidth * 2), _
                         BGlBits(0), BGuBIH, _
                         DIB_RGB_COLORS, vbSrcCopy)
   End If

End Sub

Private Sub DisplayPicture()

'*************************************************************************
'* if the .Picture property is defined, paints the picture onto the      *
'* control.  If tiling or stretching is indicated, that is performed.    *
'*************************************************************************

   Select Case m_PictureMode
      Case [Normal]
         Set UserControl.Picture = m_Picture
      Case [Tiled]
         SetPattern m_Picture
         Tile hdc, m_BorderWidth, m_BorderWidth, ScaleWidth - m_BorderWidth, ScaleHeight - m_BorderWidth
      Case [Stretch]
         StretchPicture
   End Select

End Sub

Private Sub StretchPicture()
   
'*************************************************************************
'* stretch bitmap to fit control background.  Thanks to LaVolpe for the  *
'* suggestion and AllAPI.net / VBCity.com for the learning to do it.     *
'*************************************************************************

   Dim TempBitmap As BITMAP       ' bitmap structure that temporarily holds picture.
   Dim CreateDC As Long           ' used in creating temporary bitmap structure virtual DC.
   Dim TempBitmapDC As Long       ' virtual DC of temporary bitmap structure.
   Dim TempBitmapOld As Long      ' used in destroying temporary bitmap structure virtual DC.
   Dim r As Long                  ' result long for StretchBlt call.

'  create a temporary bitmap and DC to place the picture in.
   GetObjectAPI m_Picture.Handle, Len(TempBitmap), TempBitmap
   CreateDC = CreateDCAsNull("DISPLAY", ByVal 0&, ByVal 0&, ByVal 0&)
   TempBitmapDC = CreateCompatibleDC(CreateDC)
   TempBitmapOld = SelectObject(TempBitmapDC, m_Picture.Handle)

'  streeeeeeeetch it...
   r = StretchBlt(hdc, _
                  m_BorderWidth, m_BorderWidth, _
                  ScaleWidth - m_BorderWidth, ScaleHeight - m_BorderWidth, _
                  TempBitmapDC, _
                  0, 0, _
                  TempBitmap.bmWidth, TempBitmap.bmHeight, _
                  vbSrcCopy)

'  destroy temporary bitmap DC.
   SelectObject TempBitmapDC, TempBitmapOld
   DeleteDC TempBitmapDC
   DeleteDC CreateDC

End Sub

Private Function IsPictureThere(ByVal Pic As StdPicture) As Boolean

'*************************************************************************
'* checks for existence of a picture.  Thanks to Roger Gilchrist.        *
'*************************************************************************

   If Not Pic Is Nothing Then
      If Pic.Height <> 0 Then
         IsPictureThere = Pic.Width <> 0
      End If
   End If

End Function

Private Sub CreateBorder()

'*************************************************************************
'* draws the border around the control, using appropriate curvatures.    *
'*************************************************************************

   Dim r       As Long   ' return variable for BitBlt.
   Dim hRgn1   As Long   ' the outer region of the border.
   Dim hRgn2   As Long   ' the inner region of the border.
   Dim hBrush  As Long   ' the solid-color brush used to paint the combined border regions.

'  create the outer region.
   hRgn1 = pvGetRoundedRgn(0, 0, _
                           ScaleWidth, ScaleHeight, _
                           m_CurveTopLeft, m_CurveTopRight, _
                           m_CurveBottomLeft, m_CurveBottomRight)
'  create the inner region.
   hRgn2 = pvGetRoundedRgn(m_BorderWidth, m_BorderWidth, _
                           ScaleWidth - m_BorderWidth, ScaleHeight - m_BorderWidth, _
                           m_CurveTopLeft, m_CurveTopRight, _
                           m_CurveBottomLeft, m_CurveBottomRight)

'  combine the outer and inner regions.
   CombineRgn hRgn2, hRgn1, hRgn2, RGN_DIFF

'  create the solid brush pattern used to color the combined regions.
   hBrush = CreateSolidBrush(TranslateColor(m_BorderColor))

'  color the combined regions.
   FillRgn hdc, hRgn2, hBrush

'  set the container's visibility region.
   SetWindowRgn hwnd, hRgn1, True

'  delete created objects to restore memory.
   DeleteObject hBrush
   DeleteObject hRgn1
   DeleteObject hRgn2

'  if we are redrawing the control because of a change to the .Picture property,
'  this is the time to re-blit the new picture/border to the virtual DC. I do
'  it here because I blit the entire control surface, including border, when
'  using a picture background as opposed to a gradient.
   If ChangingPicture Then
      r = BitBlt(VirtualBackgroundDC, 0, 0, ScaleWidth, ScaleHeight, hdc, 0, 0, vbSrcCopy)
      ChangingPicture = False
   End If

End Sub

Private Function pvGetRoundedRgn(ByVal x1 As Long, ByVal y1 As Long, ByVal x2 As Long, ByVal y2 As Long, _
                                 ByVal TopLeftRadius As Long, ByVal TopRightRadius As Long, _
                                 ByVal BottomLeftRadius As Long, ByVal BottomRightRadius As Long) As Long

'*************************************************************************
'* allows each corner of the container to have its own curvature.        *
'* Code by Carles P.V.                                                   *
'*************************************************************************

   Dim hRgnMain As Long   ' the original "starting point" region.
   Dim hRgnTmp1 As Long   ' the first region that defines a corner's radius.
   Dim hRgnTmp2 As Long   ' the second region that defines a corner's radius.

'  bounding region.
   hRgnMain = CreateRectRgn(x1, y1, x2, y2)

'  top-left corner.
   hRgnTmp1 = CreateRectRgn(x1, y1, x1 + TopLeftRadius, y1 + TopLeftRadius)
   hRgnTmp2 = CreateEllipticRgn(x1, y1, x1 + 2 * TopLeftRadius, y1 + 2 * TopLeftRadius)
   CombineRegions hRgnTmp1, hRgnTmp2, hRgnMain

'  top-right corner.
   hRgnTmp1 = CreateRectRgn(x2, y1, x2 - TopRightRadius, y1 + TopRightRadius)
   hRgnTmp2 = CreateEllipticRgn(x2 + 1, y1, x2 + 1 - 2 * TopRightRadius, y1 + 2 * TopRightRadius)
   CombineRegions hRgnTmp1, hRgnTmp2, hRgnMain

'  bottom-left corner.
   hRgnTmp1 = CreateRectRgn(x1, y2, x1 + BottomLeftRadius, y2 - BottomLeftRadius)
   hRgnTmp2 = CreateEllipticRgn(x1, y2 + 1, x1 + 2 * BottomLeftRadius, y2 + 1 - 2 * BottomLeftRadius)
   CombineRegions hRgnTmp1, hRgnTmp2, hRgnMain

'  bottom-right corner.
   hRgnTmp1 = CreateRectRgn(x2, y2, x2 - BottomRightRadius, y2 - BottomRightRadius)
   hRgnTmp2 = CreateEllipticRgn(x2 + 1, y2 + 1, x2 + 1 - 2 * BottomRightRadius, y2 + 1 - 2 * BottomRightRadius)
   CombineRegions hRgnTmp1, hRgnTmp2, hRgnMain

   pvGetRoundedRgn = hRgnMain

End Function

Private Sub CombineRegions(ByVal Region1 As Long, ByVal Region2 As Long, ByVal MainRegion As Long)

'*************************************************************************
'* combines outer/inner rectangular regions for border painting.         *
'*************************************************************************

   CombineRgn Region1, Region1, Region2, RGN_DIFF
   CombineRgn MainRegion, MainRegion, Region1, RGN_DIFF
   DeleteObject Region1
   DeleteObject Region2

End Sub

Private Sub CalculateGradient(Width As Long, Height As Long, _
                              ByVal Color1 As Long, ByVal Color2 As Long, _
                              ByVal Angle As Single, ByVal bMOut As Boolean, _
                              ByRef uBIH As BITMAPINFOHEADER, ByRef lBits() As Long)

'*************************************************************************
'* Carles P.V.'s routine, modified by Matthew R. Usner for middle-out    *
'* gradient capability.  Also modified to just calculate the gradient,   *
'* not draw it.  Original submission at PSC, txtCodeID=60580.            *
'*************************************************************************

   Dim lGrad()   As Long, lGrad2() As Long

   Dim lClr      As Long
   Dim R1        As Long, G1 As Long, b1 As Long
   Dim R2        As Long, G2 As Long, b2 As Long
   Dim dR        As Long, dG As Long, dB As Long

   Dim Scan      As Long
   Dim i         As Long, j As Long, k As Long
   Dim jIn       As Long
   Dim iEnd      As Long, jEnd As Long
   Dim Offset    As Long

   Dim lQuad     As Long
   Dim AngleDiag As Single
   Dim AngleComp As Single

   Dim g         As Long
   Dim luSin     As Long, luCos As Long
 
   If (Width > 0 And Height > 0) Then

'     when angle is >= 91 and <= 270, the colors
'     invert in MiddleOut mode.  This corrects that.
      If bMOut And Angle >= 91 And Angle <= 270 Then
         g = Color1
         Color1 = Color2
         Color2 = g
      End If

'     -- Right-hand [+] (ox=0º)
      Angle = -Angle + 90

'     -- Normalize to [0º;360º]
      Angle = Angle Mod 360
      If (Angle < 0) Then
         Angle = 360 + Angle
      End If

'     -- Get quadrant (0 - 3)
      lQuad = Angle \ 90

'     -- Normalize to [0º;90º]
        Angle = Angle Mod 90

'     -- Calc. gradient length ('distance')
      If (lQuad Mod 2 = 0) Then
         AngleDiag = Atn(Width / Height) * TO_DEG
      Else
         AngleDiag = Atn(Height / Width) * TO_DEG
      End If
      AngleComp = (90 - Abs(Angle - AngleDiag)) * TO_RAD
      Angle = Angle * TO_RAD
      g = Sqr(Width * Width + Height * Height) * Sin(AngleComp) 'Sinus theorem

'     -- Decompose colors
      If (lQuad > 1) Then
         lClr = Color1
         Color1 = Color2
         Color2 = lClr
      End If
      R1 = (Color1 And &HFF&)
      G1 = (Color1 And &HFF00&) \ 256
      b1 = (Color1 And &HFF0000) \ 65536
      R2 = (Color2 And &HFF&)
      G2 = (Color2 And &HFF00&) \ 256
      b2 = (Color2 And &HFF0000) \ 65536

'     -- Get color distances
      dR = R2 - R1
      dG = G2 - G1
      dB = b2 - b1

'     -- Size gradient-colors array
      ReDim lGrad(0 To g - 1)
      ReDim lGrad2(0 To g - 1)

'     -- Calculate gradient-colors
      iEnd = g - 1
      If (iEnd = 0) Then
'        -- Special case (1-pixel wide gradient)
         lGrad2(0) = (b1 \ 2 + b2 \ 2) + 256 * (G1 \ 2 + G2 \ 2) + 65536 * (R1 \ 2 + R2 \ 2)
      Else
         For i = 0 To iEnd
            lGrad2(i) = b1 + (dB * i) \ iEnd + 256 * (G1 + (dG * i) \ iEnd) + 65536 * (R1 + (dR * i) \ iEnd)
         Next i
      End If

'     'if' block added by Matthew R. Usner - accounts for possible MiddleOut gradient draw.
      If bMOut Then
         k = 0
         For i = 0 To iEnd Step 2
            lGrad(k) = lGrad2(i)
            k = k + 1
         Next i
         For i = iEnd - 1 To 1 Step -2
            lGrad(k) = lGrad2(i)
            k = k + 1
         Next i
      Else
         For i = 0 To iEnd
            lGrad(i) = lGrad2(i)
         Next i
      End If

'     -- Size DIB array
      ReDim lBits(Width * Height - 1) As Long
      iEnd = Width - 1
      jEnd = Height - 1
      Scan = Width

'     -- Render gradient DIB
      Select Case lQuad

         Case 0, 2
            luSin = Sin(Angle) * INT_ROT
            luCos = Cos(Angle) * INT_ROT
            Offset = 0
            jIn = 0
            For j = 0 To jEnd
               For i = 0 To iEnd
                  lBits(i + Offset) = lGrad((i * luSin + jIn) \ INT_ROT)
               Next i
               jIn = jIn + luCos
               Offset = Offset + Scan
            Next j

         Case 1, 3
            luSin = Sin(90 * TO_RAD - Angle) * INT_ROT
            luCos = Cos(90 * TO_RAD - Angle) * INT_ROT
            Offset = jEnd * Scan
            jIn = 0
            For j = 0 To jEnd
               For i = 0 To iEnd
                  lBits(i + Offset) = lGrad((i * luSin + jIn) \ INT_ROT)
               Next i
               jIn = jIn + luCos
               Offset = Offset - Scan
            Next j

      End Select

'     -- Define DIB header
      With uBIH
         .biSize = 40
         .biPlanes = 1
         .biBitCount = 32
         .biWidth = Width
         .biHeight = Height
      End With

   End If

End Sub

' next 3 routines adapted from Carles P.V.'s class titled "DIB Brush - Easy
' Image Tiling Using FillRect" at Planet Source Code, txtCodeId=40585.

Private Function SetPattern(Picture As StdPicture) As Boolean

'*************************************************************************
'* creates the brush pattern for tiling into the control.  By Carles P.V.*
'*************************************************************************

   Dim tBI       As BITMAP
   Dim tBIH      As BITMAPINFOHEADER
   Dim Buff()    As Byte 'Packed DIB

   Dim lhDC      As Long
   Dim lhOldBmp  As Long

   If (GetObjectType(Picture) = OBJ_BITMAP) Then

'     -- Get image info
      GetObject Picture, Len(tBI), tBI

'     -- Prepare DIB header and redim. Buff() array
      With tBIH
         .biSize = Len(tBIH) '40
         .biPlanes = 1
         .biBitCount = 24
         .biWidth = tBI.bmWidth
         .biHeight = tBI.bmHeight
         .biSizeImage = ((.biWidth * 3 + 3) And &HFFFFFFFC) * .biHeight
      End With
      ReDim Buff(1 To Len(tBIH) + tBIH.biSizeImage) '[Header + Bits]

'     -- Create DIB brush
      lhDC = CreateCompatibleDC(0)
      If (lhDC <> 0) Then
         lhOldBmp = SelectObject(lhDC, Picture)

'        -- Build packed DIB:
'        - Merge Header
         CopyMemory Buff(1), tBIH, Len(tBIH)
'        - Get and merge DIB Bits
         GetDIBits lhDC, Picture, 0, tBI.bmHeight, Buff(Len(tBIH) + 1), tBIH, DIB_RGB_COLORS

         SelectObject lhDC, lhOldBmp
         DeleteDC lhDC

'        -- Create brush from packed DIB
         DestroyPattern
         m_hBrush = CreateDIBPatternBrushPt(Buff(1), DIB_RGB_COLORS)
      End If

   End If

   SetPattern = (m_hBrush <> 0)

End Function

Private Sub Tile(ByVal hdc As Long, ByVal x1 As Long, ByVal y1 As Long, ByVal x2 As Long, ByVal y2 As Long)

'*************************************************************************
'* performs the tiling of the bitmap on the control.  By Carles P.V.     *
'*************************************************************************

   Dim TileRect As RECT
   Dim PtOrg    As POINTAPI

   If (m_hBrush <> 0) Then
      SetRect TileRect, x1, y1, x2, y2
      SetBrushOrgEx hdc, x1, y1, PtOrg
'     -- Tile image
      FillRect hdc, TileRect, m_hBrush
   End If

End Sub

Private Sub DestroyPattern()

'*************************************************************************
'* destroys the pattern brush used to tile the bitmap.  By Carles P.V.   *
'*************************************************************************

   If (m_hBrush <> 0) Then
      DeleteObject m_hBrush
      m_hBrush = 0
   End If

End Sub

Private Sub CreateVirtualBackgroundDC()

'*************************************************************************
'* creates a virtual bitmap, with its own DC, that will hold a copy of   *
'* the control's background gradient (or picture).  This is used by      *
'* BitBlt to update just the part of the control's background that is    *
'* changed when an individual display digit has changed.  This allows    *
'* for lightning-quick updates of the background and display of indiv-   *
'* idual digits without having to repaint the whole control.             *
'*************************************************************************

'  safety net that makes sure virtual DC doesn't already exist.
   If IsCreated Then
      DestroyVirtualDC
   End If

'  Create a memory device context to use.
   VirtualBackgroundDC = CreateCompatibleDC(hdc)

'  define it as a bitmap so that drawing can be performed to the virtual DC.
   mMemoryBitmap = CreateCompatibleBitmap(hdc, ScaleWidth, ScaleHeight)
   mOrginalBitmap = SelectObject(VirtualBackgroundDC, mMemoryBitmap)

End Sub

Private Function IsCreated() As Boolean

'*************************************************************************
'* checks the handle of the created DC and returns if it exists.         *
'*************************************************************************

   IsCreated = (VirtualBackgroundDC <> 0)

End Function

Private Sub DestroyVirtualDC()

'*************************************************************************
'* eliminates the virtual background dc bitmap on control's termination. *
'*************************************************************************

   If Not IsCreated Then
      Exit Sub
   End If

   SelectObject VirtualBackgroundDC, mOrginalBitmap
   DeleteObject mMemoryBitmap
   DeleteDC VirtualBackgroundDC
   VirtualBackgroundDC = -1

End Sub

'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Properties >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
'<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Private Sub UserControl_InitProperties()

'*************************************************************************
'* sets default usercontrol properties.                                  *
'*************************************************************************

   Set m_Picture = LoadPicture("")
   m_BackAngle = m_def_BackAngle
   m_BackColor1 = m_def_BackColor1
   m_BackColor2 = m_def_BackColor2
   m_BackMiddleOut = m_def_BackMiddleOut
   m_BorderColor = m_def_BorderColor
   m_BorderWidth = m_def_BorderWidth
   m_BurnInColor = m_def_BurnInColor
   m_BurnInColorNeg = m_def_BurnInColorNeg
   m_CurveBottomLeft = m_def_CurveBottomLeft
   m_CurveBottomRight = m_def_CurveBottomRight
   m_CurveTopLeft = m_def_CurveTopLeft
   m_CurveTopRight = m_def_CurveTopRight
   m_DecimalSeparator = m_def_DecimalSeparator
   m_InterDigitGap = m_def_InterDigitGap
   m_InterDigitGapExp = m_def_InterDigitGapExp
   m_InterSegmentGap = m_def_InterSegmentGap
   m_InterSegmentGapExp = m_def_InterSegmentGapExp
   m_NumDigits = m_def_NumDigits
   m_NumDigitsExp = m_def_NumDigitsExp
   m_PictureMode = m_def_PictureMode
   m_SegmentFillStyle = m_def_SegmentFillStyle
   m_SegmentLitColor = m_def_SegmentLitColor
   m_SegmentLitColorNeg = m_def_SegmentLitColorNeg
   m_SegmentHeight = m_def_SegmentHeight
   m_SegmentHeightExp = m_def_SegmentHeightExp
   m_SegmentStyle = m_def_SegmentStyle
   m_SegmentStyleExp = m_def_SegmentStyleExp
   m_SegmentWidth = m_def_SegmentWidth
   m_SegmentWidthExp = m_def_SegmentWidthExp
   m_ShowBurnIn = m_def_ShowBurnIn
   m_ShowExponent = m_def_ShowExponent
   m_ShowThousandsSeparator = m_def_ShowThousandsSeparator
   m_Theme = m_def_Theme
   m_ThousandsGrouping = m_def_ThousandsGrouping
   m_ThousandsSeparator = m_def_ThousandsSeparator
   m_Value = m_def_Value
   m_XOffset = m_def_XOffset
   m_XOffsetExp = m_def_XOffsetExp
   m_YOffset = m_def_YOffset
   m_YOffsetExp = m_def_YOffsetExp

End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)

'*************************************************************************
'* read properties in the property bag.                                  *
'*************************************************************************

   With PropBag
      Set m_Picture = PropBag.ReadProperty("Picture", Nothing)
      m_BackAngle = .ReadProperty("BackAngle", m_def_BackAngle)
      m_BackColor1 = .ReadProperty("BackColor1", m_def_BackColor1)
      m_BackColor2 = .ReadProperty("BackColor2", m_def_BackColor2)
      m_BackMiddleOut = .ReadProperty("BackMiddleOut", m_def_BackMiddleOut)
      m_BorderColor = .ReadProperty("BorderColor", m_def_BorderColor)
      m_BorderWidth = .ReadProperty("BorderWidth", m_def_BorderWidth)
      m_BurnInColor = .ReadProperty("BurnInColor", m_def_BurnInColor)
      m_BurnInColorNeg = .ReadProperty("BurnInColorNeg", m_def_BurnInColorNeg)
      m_CurveBottomLeft = .ReadProperty("CurveBottomLeft", m_def_CurveBottomLeft)
      m_CurveBottomRight = .ReadProperty("CurveBottomRight", m_def_CurveBottomRight)
      m_CurveTopLeft = .ReadProperty("CurveTopLeft", m_def_CurveTopLeft)
      m_CurveTopRight = .ReadProperty("CurveTopRight", m_def_CurveTopRight)
      m_DecimalSeparator = .ReadProperty("DecimalSeparator", m_def_DecimalSeparator)
      m_InterDigitGap = .ReadProperty("InterDigitGap", m_def_InterDigitGap)
      m_InterDigitGapExp = .ReadProperty("InterDigitGapExp", m_def_InterDigitGapExp)
      m_InterSegmentGap = .ReadProperty("InterSegmentGap", m_def_InterSegmentGap)
      m_InterSegmentGapExp = .ReadProperty("InterSegmentGapExp", m_def_InterSegmentGapExp)
      m_NumDigits = PropBag.ReadProperty("NumDigits", m_def_NumDigits)
      m_NumDigitsExp = PropBag.ReadProperty("NumDigitsExp", m_def_NumDigitsExp)
      m_PictureMode = .ReadProperty("PictureMode", m_def_PictureMode)
      m_SegmentFillStyle = .ReadProperty("SegmentFillStyle", m_def_SegmentFillStyle)
      m_SegmentLitColor = .ReadProperty("SegmentLitColor", m_def_SegmentLitColor)
      m_SegmentLitColorNeg = .ReadProperty("SegmentLitColorNeg", m_def_SegmentLitColorNeg)
      m_SegmentHeight = .ReadProperty("SegmentHeight", m_def_SegmentHeight)
      m_SegmentHeightExp = .ReadProperty("SegmentHeightExp", m_def_SegmentHeightExp)
      m_SegmentStyle = .ReadProperty("SegmentStyle", m_def_SegmentStyle)
      m_SegmentStyleExp = .ReadProperty("SegmentStyleExp", m_def_SegmentStyleExp)
      m_SegmentWidth = .ReadProperty("SegmentWidth", m_def_SegmentWidth)
      m_SegmentWidthExp = .ReadProperty("SegmentWidthExp", m_def_SegmentWidthExp)
      m_ShowBurnIn = .ReadProperty("ShowBurnIn", m_def_ShowBurnIn)
      m_ShowExponent = .ReadProperty("ShowExponent", m_def_ShowExponent)
      m_ShowThousandsSeparator = .ReadProperty("ShowThousandsSeparator", m_def_ShowThousandsSeparator)
      m_Theme = .ReadProperty("Theme", m_def_Theme)
      m_ThousandsGrouping = .ReadProperty("ThousandsGrouping", m_def_ThousandsGrouping)
      m_ThousandsSeparator = .ReadProperty("ThousandsSeparator", m_def_ThousandsSeparator)
      m_Value = .ReadProperty("Value", m_def_Value)
      m_XOffset = .ReadProperty("XOffset", m_def_XOffset)
      m_XOffsetExp = .ReadProperty("XOffsetExp", m_def_XOffsetExp)
      m_YOffset = .ReadProperty("YOffset", m_def_YOffset)
      m_YOffsetExp = .ReadProperty("YOffsetExp", m_def_YOffsetExp)
   End With

'  if hexagonal or trapezoidal segment style, for LaVolpe's region shaping
'  code to work properly, width and height must be even numbers of pixels.
   If m_SegmentStyle = Hexagonal Or m_SegmentStyle = Trapezoidal Then
      If m_SegmentWidth Mod 2 Then m_SegmentWidth = m_SegmentWidth + 1
      If m_SegmentHeight Mod 2 Then m_SegmentHeight = m_SegmentHeight + 1
   End If

   InitLCDDisplayCharacteristics

End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)

'*************************************************************************
'* write the properties in the property bag.                             *
'*************************************************************************

   With PropBag
      .WriteProperty "BackAngle", m_BackAngle, m_def_BackAngle
      .WriteProperty "BackColor1", m_BackColor1, m_def_BackColor1
      .WriteProperty "BackColor2", m_BackColor2, m_def_BackColor2
      .WriteProperty "BackMiddleOut", m_BackMiddleOut, m_def_BackMiddleOut
      .WriteProperty "BorderColor", m_BorderColor, m_def_BorderColor
      .WriteProperty "BorderWidth", m_BorderWidth, m_def_BorderWidth
      .WriteProperty "BurnInColor", m_BurnInColor, m_def_BurnInColor
      .WriteProperty "BurnInColorNeg", m_BurnInColorNeg, m_def_BurnInColorNeg
      .WriteProperty "CurveBottomLeft", m_CurveBottomLeft, m_def_CurveBottomLeft
      .WriteProperty "CurveBottomRight", m_CurveBottomRight, m_def_CurveBottomRight
      .WriteProperty "CurveTopLeft", m_CurveTopLeft, m_def_CurveTopLeft
      .WriteProperty "CurveTopRight", m_CurveTopRight, m_def_CurveTopRight
      .WriteProperty "DecimalSeparator", m_DecimalSeparator, m_def_DecimalSeparator
      .WriteProperty "InterDigitGap", m_InterDigitGap, m_def_InterDigitGap
      .WriteProperty "InterDigitGapExp", m_InterDigitGapExp, m_def_InterDigitGapExp
      .WriteProperty "InterSegmentGap", m_InterSegmentGap, m_def_InterSegmentGap
      .WriteProperty "InterSegmentGapExp", m_InterSegmentGapExp, m_def_InterSegmentGapExp
      .WriteProperty "NumDigits", m_NumDigits, m_def_NumDigits
      .WriteProperty "NumDigitsExp", m_NumDigitsExp, m_def_NumDigitsExp
      .WriteProperty "Picture", m_Picture, Nothing
      .WriteProperty "PictureMode", m_PictureMode, m_def_PictureMode
      .WriteProperty "SegmentFillStyle", m_SegmentFillStyle, m_def_SegmentFillStyle
      .WriteProperty "SegmentHeight", m_SegmentHeight, m_def_SegmentHeight
      .WriteProperty "SegmentHeightExp", m_SegmentHeightExp, m_def_SegmentHeightExp
      .WriteProperty "SegmentLitColor", m_SegmentLitColor, m_def_SegmentLitColor
      .WriteProperty "SegmentLitColorNeg", m_SegmentLitColorNeg, m_def_SegmentLitColorNeg
      .WriteProperty "SegmentStyle", m_SegmentStyle, m_def_SegmentStyle
      .WriteProperty "SegmentStyleExp", m_SegmentStyleExp, m_def_SegmentStyleExp
      .WriteProperty "SegmentWidth", m_SegmentWidth, m_def_SegmentWidth
      .WriteProperty "SegmentWidthExp", m_SegmentWidthExp, m_def_SegmentWidthExp
      .WriteProperty "ShowBurnIn", m_ShowBurnIn, m_def_ShowBurnIn
      .WriteProperty "ShowExponent", m_ShowExponent, m_def_ShowExponent
      .WriteProperty "ShowThousandsSeparator", m_ShowThousandsSeparator, m_def_ShowThousandsSeparator
      .WriteProperty "Theme", m_Theme, m_def_Theme
      .WriteProperty "ThousandsGrouping", m_ThousandsGrouping, m_def_ThousandsGrouping
      .WriteProperty "ThousandsSeparator", m_ThousandsSeparator, m_def_ThousandsSeparator
      .WriteProperty "Value", m_Value, m_def_Value
      .WriteProperty "XOffset", m_XOffset, m_def_XOffset
      .WriteProperty "XOffsetExp", m_XOffsetExp, m_def_XOffsetExp
      .WriteProperty "YOffset", m_YOffset, m_def_YOffset
      .WriteProperty "YOffsetExp", m_YOffsetExp, m_def_YOffsetExp
   End With

End Sub

Public Property Get BackAngle() As Single
Attribute BackAngle.VB_Description = "The angle, in degrees, of the colors in the control's background gradient."
   BackAngle = m_BackAngle
End Property

Public Property Let BackAngle(ByVal New_BackAngle As Single)
   m_BackAngle = New_BackAngle
   PropertyChanged "BackAngle"
   RedrawControl
End Property

Public Property Get BackColor1() As OLE_COLOR
Attribute BackColor1.VB_Description = "The first color of the background gradient."
   BackColor1 = m_BackColor1
End Property

Public Property Let BackColor1(ByVal New_BackColor1 As OLE_COLOR)
   m_BackColor1 = New_BackColor1
   PropertyChanged "BackColor1"
   CalculateBackGroundGradient
   RedrawControl
End Property

Public Property Get BackColor2() As OLE_COLOR
Attribute BackColor2.VB_Description = "The second color of the background gradient."
   BackColor2 = m_BackColor2
End Property

Public Property Let BackColor2(ByVal New_BackColor2 As OLE_COLOR)
   m_BackColor2 = New_BackColor2
   PropertyChanged "BackColor2"
   CalculateBackGroundGradient
   RedrawControl
End Property

Public Property Get BackMiddleOut() As Boolean
Attribute BackMiddleOut.VB_Description = "If True, the background gradient is middle-out (Color1 > Color2 > Color1)."
   BackMiddleOut = m_BackMiddleOut
End Property

Public Property Let BackMiddleOut(ByVal New_BackMiddleOut As Boolean)
   m_BackMiddleOut = New_BackMiddleOut
   PropertyChanged "BackMiddleOut"
   RedrawControl
End Property

Public Property Get BorderColor() As OLE_COLOR
Attribute BorderColor.VB_Description = "The color of the control's border."
   BorderColor = m_BorderColor
End Property

Public Property Let BorderColor(ByVal New_BorderColor As OLE_COLOR)
   m_BorderColor = New_BorderColor
   PropertyChanged "BorderColor"
   RedrawControl
End Property

Public Property Get BorderWidth() As Integer
Attribute BorderWidth.VB_Description = "The width, in pixels, of the control's border."
   BorderWidth = m_BorderWidth
End Property

Public Property Let BorderWidth(ByVal New_BorderWidth As Integer)
   m_BorderWidth = New_BorderWidth
   PropertyChanged "BorderWidth"
   RedrawControl
End Property

Public Property Get BurnInColor() As OLE_COLOR
Attribute BurnInColor.VB_Description = "The color of 'burned-in' segments.  If the .ShowBurnIn property is True, this helps the display look more like a physical LED/LCD."
   BurnInColor = m_BurnInColor
End Property

Public Property Let BurnInColor(ByVal New_BurnInColor As OLE_COLOR)
   m_BurnInColor = New_BurnInColor
   PropertyChanged "BurnInColor"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get BurnInColorNeg() As OLE_COLOR
Attribute BurnInColorNeg.VB_Description = "The color to paint unlit segments in .ShowBurnIn=True display mode if the value being displayed is negative."
   BurnInColorNeg = m_BurnInColorNeg
End Property

Public Property Let BurnInColorNeg(ByVal New_BurnInColorNeg As OLE_COLOR)
   m_BurnInColorNeg = New_BurnInColorNeg
   PropertyChanged "BurnInColorNeg"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get CurveBottomLeft() As Long
Attribute CurveBottomLeft.VB_Description = "The amount of curvature of the bottom left corner of the control."
   CurveBottomLeft = m_CurveBottomLeft
End Property

Public Property Let CurveBottomLeft(ByVal New_CurveBottomLeft As Long)
   m_CurveBottomLeft = New_CurveBottomLeft
   PropertyChanged "CurveBottomLeft"
   RedrawControl
End Property

Public Property Get CurveBottomRight() As Long
Attribute CurveBottomRight.VB_Description = "The amount of curvature of the bottom right corner of the control."
   CurveBottomRight = m_CurveBottomRight
End Property

Public Property Let CurveBottomRight(ByVal New_CurveBottomRight As Long)
   m_CurveBottomRight = New_CurveBottomRight
   PropertyChanged "CurveBottomRight"
   RedrawControl
End Property

Public Property Get CurveTopLeft() As Long
Attribute CurveTopLeft.VB_Description = "The amount of curvature of the top left corner of the control."
   CurveTopLeft = m_CurveTopLeft
End Property

Public Property Let CurveTopLeft(ByVal New_CurveTopLeft As Long)
   m_CurveTopLeft = New_CurveTopLeft
   PropertyChanged "CurveTopLeft"
   RedrawControl
End Property

Public Property Get CurveTopRight() As Long
Attribute CurveTopRight.VB_Description = "The amount of curvature of the top right corner of the control."
   CurveTopRight = m_CurveTopRight
End Property

Public Property Let CurveTopRight(ByVal New_CurveTopRight As Long)
   m_CurveTopRight = New_CurveTopRight
   PropertyChanged "CurveTopRight"
   RedrawControl
End Property

Public Property Get DecimalSeparator() As SeparatorOptions
Attribute DecimalSeparator.VB_Description = "The character to use as the decimal point character: a period or a comma."
   DecimalSeparator = m_DecimalSeparator
End Property

Public Property Let DecimalSeparator(ByVal New_DecimalSeparator As SeparatorOptions)
   m_DecimalSeparator = New_DecimalSeparator
   PropertyChanged "DecimalSeparator"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get InterDigitGap() As Long
Attribute InterDigitGap.VB_Description = "The number of pixels separating each display digit in the main value."
   InterDigitGap = m_InterDigitGap
End Property

Public Property Let InterDigitGap(ByVal New_InterDigitGap As Long)
   m_InterDigitGap = New_InterDigitGap
   PropertyChanged "InterDigitGap"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get InterDigitGapExp() As Long
Attribute InterDigitGapExp.VB_Description = "The number of pixels separating each display digit in the exponent value."
   InterDigitGapExp = m_InterDigitGapExp
End Property

Public Property Let InterDigitGapExp(ByVal New_InterDigitGapExp As Long)
   m_InterDigitGapExp = New_InterDigitGapExp
   PropertyChanged "InterDigitGapExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get InterSegmentGap() As Long
Attribute InterSegmentGap.VB_Description = "The number of pixels separating individual segments in a main value display digit."
   InterSegmentGap = m_InterSegmentGap
End Property

Public Property Let InterSegmentGap(ByVal New_InterSegmentGap As Long)
   m_InterSegmentGap = New_InterSegmentGap
   PropertyChanged "InterSegmentGap"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get InterSegmentGapExp() As Long
Attribute InterSegmentGapExp.VB_Description = "The number of pixels separating individual segments in an exponent value display digit."
   InterSegmentGapExp = m_InterSegmentGapExp
End Property

Public Property Let InterSegmentGapExp(ByVal New_InterSegmentGapExp As Long)
   m_InterSegmentGapExp = New_InterSegmentGapExp
   PropertyChanged "InterSegmentGapExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get NumDigits() As Long
Attribute NumDigits.VB_Description = "The maximum number of main value digits to display."
   NumDigits = m_NumDigits
End Property

Public Property Let NumDigits(ByVal New_NumDigits As Long)
   m_NumDigits = New_NumDigits
   PropertyChanged "NumDigits"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get NumDigitsExp() As Long
Attribute NumDigitsExp.VB_Description = "The maximum number of exponent value digits to display."
   NumDigitsExp = m_NumDigitsExp
End Property

Public Property Let NumDigitsExp(ByVal New_NumDigitsExp As Long)
   m_NumDigitsExp = New_NumDigitsExp
   PropertyChanged "NumDigitsExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get Picture() As Picture
Attribute Picture.VB_Description = "The bitmap to display in the control's background in lieu of a gradient."
   Set Picture = m_Picture
End Property

Public Property Set Picture(ByVal New_Picture As Picture)
   Set m_Picture = New_Picture
   PropertyChanged "Picture"
   ChangingPicture = True
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get PictureMode() As LCDPicModeOptions
Attribute PictureMode.VB_Description = "The method used to display the background bitmap - Regular, Tiled or Stretched."
   PictureMode = m_PictureMode
End Property

Public Property Let PictureMode(ByVal New_PictureMode As LCDPicModeOptions)
   m_PictureMode = New_PictureMode
   PropertyChanged "PictureMode"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentFillStyle() As SegmentFillStyleOptions
Attribute SegmentFillStyle.VB_Description = "Determines whether the digit segments are painted in regular (filled-in) or filament (outline) style."
   SegmentFillStyle = m_SegmentFillStyle
End Property

Public Property Let SegmentFillStyle(ByVal New_SegmentFillStyle As SegmentFillStyleOptions)
   m_SegmentFillStyle = New_SegmentFillStyle
   PropertyChanged "SegmentFillStyle"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentHeight() As Long
Attribute SegmentHeight.VB_Description = "The height, in pixels, of the long dimension of a main value digit segment."
   SegmentHeight = m_SegmentHeight
End Property

Public Property Let SegmentHeight(ByVal New_SegmentHeight As Long)
   m_SegmentHeight = New_SegmentHeight
   PropertyChanged "SegmentHeight"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentHeightExp() As Long
Attribute SegmentHeightExp.VB_Description = "The height, in pixels, of the long dimension of an exponent digit segment."
   SegmentHeightExp = m_SegmentHeightExp
End Property

Public Property Let SegmentHeightExp(ByVal New_SegmentHeightExp As Long)
   m_SegmentHeightExp = New_SegmentHeightExp
   PropertyChanged "SegmentHeightExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentLitColor() As OLE_COLOR
Attribute SegmentLitColor.VB_Description = "The color with which to display an activated LCD digit segment when the value to display is positive."
Attribute SegmentLitColor.VB_ProcData.VB_Invoke_Property = ";General Graphics"
   SegmentLitColor = m_SegmentLitColor
End Property

Public Property Let SegmentLitColor(ByVal New_SegmentLitColor As OLE_COLOR)
   m_SegmentLitColor = New_SegmentLitColor
   PropertyChanged "SegmentLitColor"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentLitColorNeg() As OLE_COLOR
Attribute SegmentLitColorNeg.VB_Description = "The color with which to display an activated LCD digit segment when the value to display is negative."
   SegmentLitColorNeg = m_SegmentLitColorNeg
End Property

Public Property Let SegmentLitColorNeg(ByVal New_SegmentLitColorNeg As OLE_COLOR)
   m_SegmentLitColorNeg = New_SegmentLitColorNeg
   PropertyChanged "SegmentLitColorNeg"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentStyle() As SegmentStyleOptions
Attribute SegmentStyle.VB_Description = "The shape of main value digit segments: Rectangular, Hexagonal or Trapezoidal."
   SegmentStyle = m_SegmentStyle
End Property

Public Property Let SegmentStyle(ByVal New_SegmentStyle As SegmentStyleOptions)
   m_SegmentStyle = New_SegmentStyle
   PropertyChanged "SegmentStyle"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentStyleExp() As SegmentStyleOptions
Attribute SegmentStyleExp.VB_Description = "The shape of exponent digit segments: Rectangular, Hexagonal or Trapezoidal."
   SegmentStyleExp = m_SegmentStyleExp
End Property

Public Property Let SegmentStyleExp(ByVal New_SegmentStyleExp As SegmentStyleOptions)
   m_SegmentStyleExp = New_SegmentStyleExp
   PropertyChanged "SegmentStyleExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentWidth() As Long
Attribute SegmentWidth.VB_Description = "The length, in pixels, of the short dimension of an individual main value display digit segment."
Attribute SegmentWidth.VB_ProcData.VB_Invoke_Property = ";Main Value"
   SegmentWidth = m_SegmentWidth
End Property

Public Property Let SegmentWidth(ByVal New_SegmentWidth As Long)
   m_SegmentWidth = New_SegmentWidth
   PropertyChanged "SegmentWidth"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get SegmentWidthExp() As Long
Attribute SegmentWidthExp.VB_Description = "The length, in pixels, of the short dimension of an individual exponent display digit segment."
   SegmentWidthExp = m_SegmentWidthExp
End Property

Public Property Let SegmentWidthExp(ByVal New_SegmentWidthExp As Long)
   m_SegmentWidthExp = New_SegmentWidthExp
   PropertyChanged "SegmentWidthExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get ShowBurnIn() As Boolean
Attribute ShowBurnIn.VB_Description = "If True, unlit segments are displayed with a faint ""burn-in' color to simulate a physical LED/LCD display."
   ShowBurnIn = m_ShowBurnIn
End Property

Public Property Let ShowBurnIn(ByVal New_ShowBurnIn As Boolean)
   m_ShowBurnIn = New_ShowBurnIn
   PropertyChanged "ShowBurnIn"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get ShowExponent() As Boolean
Attribute ShowExponent.VB_Description = "If True, an exponent is shown.  Set to False when using control as a simple counter."
   ShowExponent = m_ShowExponent
End Property

Public Property Let ShowExponent(ByVal New_ShowExponent As Boolean)
   m_ShowExponent = New_ShowExponent
   PropertyChanged "ShowExponent"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get ShowThousandsSeparator() As Boolean
Attribute ShowThousandsSeparator.VB_Description = "If True, groups of thousands are separated by the character in the .ThousandsSeparator property (comma or period)."
   ShowThousandsSeparator = m_ShowThousandsSeparator
End Property

Public Property Let ShowThousandsSeparator(ByVal New_ShowThousandsSeparator As Boolean)
   m_ShowThousandsSeparator = New_ShowThousandsSeparator
   PropertyChanged "ShowThousandsSeparator"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get Theme() As LCDThemeOptions
Attribute Theme.VB_Description = "One of several predefined display styles."
   Theme = m_Theme
End Property

Public Property Let Theme(ByVal New_Theme As LCDThemeOptions)

'*************************************************************************
'* selects one of 6 predefined display themes.  Add your own themes too. *
'*************************************************************************

   m_Theme = New_Theme
   PropertyChanged "Theme"

   Select Case m_Theme

      Case [LED Hex Small]
         m_BackAngle = 90
         m_BackColor1 = &H0
         m_BackColor2 = &H0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &H60&
         m_BurnInColorNeg = &H60&
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 4
         m_InterDigitGapExp = 2
         m_InterSegmentGap = 0
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         m_PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 8
         m_SegmentHeightExp = 5
         m_SegmentLitColor = &HFF&
         m_SegmentLitColorNeg = &HFF&
         m_SegmentStyle = Hexagonal
         m_SegmentStyleExp = Rectangular
         m_SegmentWidth = 4
         m_SegmentWidthExp = 3
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 335
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 5715
         UserControl.Height = 450

      Case [LED Hex Medium]
         m_BackAngle = 90
         m_BackColor1 = &H0
         m_BackColor2 = &H0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &H60&
         m_BurnInColorNeg = &H60&
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 6
         m_InterDigitGapExp = 2
         m_InterSegmentGap = 1
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         m_PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 12
         m_SegmentHeightExp = 8
         m_SegmentLitColor = &HFF&
         m_SegmentLitColorNeg = &HFF&
         m_SegmentStyle = Hexagonal
         m_SegmentStyleExp = Rectangular
         m_SegmentWidth = 4
         m_SegmentWidthExp = 4
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 490
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 8355
         UserControl.Height = 630

      Case [LCD Trap Small]
         m_BackAngle = 90
         m_BackColor1 = &HE0E0E0
         m_BackColor2 = &HE0E0E0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &HD0D0D0
         m_BurnInColorNeg = &HD0D0D0
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 4
         m_InterDigitGapExp = 2
         m_InterSegmentGap = 1
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         m_PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 12
         m_SegmentHeightExp = 6
         m_SegmentLitColor = &H0
         m_SegmentLitColorNeg = &H0
         m_SegmentStyle = Trapezoidal
         m_SegmentStyleExp = Rectangular
         m_SegmentWidth = 4
         m_SegmentWidthExp = 3
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 370
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 6525
         UserControl.Height = 570

      Case [LCD Trap Medium]
         m_BackAngle = 90
         m_BackColor1 = &HE0E0E0
         m_BackColor2 = &HE0E0E0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &HD0D0D0
         m_BurnInColorNeg = &HD0D0D0
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 6
         m_InterDigitGapExp = 2
         m_InterSegmentGap = 1
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         m_PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 14
         m_SegmentHeightExp = 8
         m_SegmentLitColor = &H0
         m_SegmentLitColorNeg = &H0
         m_SegmentStyle = Trapezoidal
         m_SegmentStyleExp = Rectangular
         m_SegmentWidth = 4
         m_SegmentWidthExp = 4
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 455
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 7875
         UserControl.Height = 630

      Case [Rectangular Medium]
         m_BackAngle = 90
         m_BackColor1 = &H0
         m_BackColor2 = &H0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &H505000
         m_BurnInColorNeg = &H505000
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 6
         m_InterDigitGapExp = 2
         m_InterSegmentGap = 0
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 8
         m_SegmentHeightExp = 6
         m_SegmentLitColor = &HFFFF00
         m_SegmentLitColorNeg = &HFFFF00
         m_SegmentStyle = [Rectangular]
         m_SegmentStyleExp = [Rectangular]
         m_SegmentWidth = 3
         m_SegmentWidthExp = 3
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 355
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 6045
         UserControl.Height = 465

      Case [Rectangular Small]
         m_BackAngle = 90
         m_BackColor1 = &H0
         m_BackColor2 = &H0
         m_BackMiddleOut = True
         m_BorderColor = &HFF0000
         m_BorderWidth = 1
         m_BurnInColor = &H5050&
         m_BurnInColorNeg = &H5050&
         m_CurveBottomLeft = 0
         m_CurveBottomRight = 0
         m_CurveTopLeft = 0
         m_CurveTopRight = 0
         m_DecimalSeparator = [Period]
         m_InterDigitGap = 4
         m_InterDigitGapExp = 3
         m_InterSegmentGap = 0
         m_InterSegmentGapExp = 0
         m_NumDigits = 20
         m_NumDigitsExp = 4
         Set m_Picture = Nothing
         m_PictureMode = [Normal]
         m_SegmentFillStyle = Solid
         m_SegmentHeight = 4
         m_SegmentHeightExp = 4
         m_SegmentLitColor = &HFFFF&
         m_SegmentLitColorNeg = &HFFFF&
         m_SegmentStyle = Rectangular
         m_SegmentStyleExp = Rectangular
         m_SegmentWidth = 2
         m_SegmentWidthExp = 2
         m_ShowBurnIn = True
         m_ShowExponent = True
         m_ShowThousandsSeparator = False
         m_ThousandsGrouping = 3
         m_ThousandsSeparator = [Comma]
         m_XOffset = 5
         m_XOffsetExp = 195
         m_YOffset = 5
         m_YOffsetExp = 5
         UserControl.Width = 3525
         UserControl.Height = 300

   End Select

   InitLCDDisplayCharacteristics
   RedrawControl
   DisplayValue m_Value, FORCE_REDRAW_YES

End Property

Public Property Get ThousandsGrouping() As Long
Attribute ThousandsGrouping.VB_Description = "The number of digits between thousands groups in a value (in the U.S., for example, the value is 3)."
   ThousandsGrouping = m_ThousandsGrouping
End Property

Public Property Let ThousandsGrouping(ByVal New_ThousandsGrouping As Long)
   m_ThousandsGrouping = New_ThousandsGrouping
   PropertyChanged "ThousandsGrouping"
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get ThousandsSeparator() As SeparatorOptions
Attribute ThousandsSeparator.VB_Description = "The character to be used to separate groups of digits (a comma or period)."
   ThousandsSeparator = m_ThousandsSeparator
End Property

Public Property Let ThousandsSeparator(ByVal New_ThousandsSeparator As SeparatorOptions)
   m_ThousandsSeparator = New_ThousandsSeparator
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
   PropertyChanged "ThousandsSeparator"
End Property

Public Property Get Value() As String
Attribute Value.VB_Description = "The value to be shown in the digital display."
Attribute Value.VB_ProcData.VB_Invoke_Property = ";General Graphics"
   Value = m_Value
End Property

Public Property Let Value(ByVal New_Value As String)
   m_Value = New_Value
   DisplayValue m_Value, FORCE_REDRAW_NO
   PropertyChanged "Value"
End Property

Public Property Get XOffset() As Long
Attribute XOffset.VB_Description = "The number of pixels from the left edge of the control to start displaying main value digits."
   XOffset = m_XOffset
End Property

Public Property Let XOffset(ByVal New_XOffset As Long)
   m_XOffset = New_XOffset
   PropertyChanged "XOffset"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get XOffsetExp() As Long
Attribute XOffsetExp.VB_Description = "The number of pixels from the left edge of the control to start displaying exponent digits."
   XOffsetExp = m_XOffsetExp
End Property

Public Property Let XOffsetExp(ByVal New_XOffsetExp As Long)
   m_XOffsetExp = New_XOffsetExp
   PropertyChanged "XOffsetExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get YOffset() As Long
Attribute YOffset.VB_Description = "The number of pixels from the top side of the control to display the LCD main value digits (does not take border into account)."
Attribute YOffset.VB_ProcData.VB_Invoke_Property = ";Main Value"
   YOffset = m_YOffset
End Property

Public Property Let YOffset(ByVal New_YOffset As Long)
   m_YOffset = New_YOffset
   PropertyChanged "YOffset"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property

Public Property Get YOffsetExp() As Long
Attribute YOffsetExp.VB_Description = "The number of pixels from the top side of the control to display the LCD exponent digits (does not take border into account)."
Attribute YOffsetExp.VB_ProcData.VB_Invoke_Property = ";Exponent Value"
   YOffsetExp = m_YOffsetExp
End Property

Public Property Let YOffsetExp(ByVal New_YOffsetExp As Long)
   m_YOffsetExp = New_YOffsetExp
   PropertyChanged "YOffsetExp"
   InitLCDDisplayCharacteristics
   DisplayValue m_Value, FORCE_REDRAW_YES
End Property
