VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual v0.2 by Dnomd343"
   ClientHeight    =   6585
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   9390
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6585
   ScaleWidth      =   9390
   StartUpPosition =   2  '��Ļ����
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   360
      Left            =   5820
      TabIndex        =   4
      Top             =   5925
      Width           =   840
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   8280
      Top             =   5385
   End
   Begin VB.TextBox Text_debug 
      Height          =   5190
      Left            =   5505
      MultiLine       =   -1  'True
      TabIndex        =   3
      Top             =   495
      Width           =   2250
   End
   Begin VB.CommandButton Command_Get_Code 
      Caption         =   "���ɱ���"
      Height          =   465
      Left            =   210
      TabIndex        =   2
      Top             =   5970
      Width           =   975
   End
   Begin VB.CommandButton Command_Print 
      Caption         =   "�������"
      Height          =   465
      Left            =   3180
      TabIndex        =   1
      Top             =   5940
      Width           =   975
   End
   Begin VB.TextBox Text_Code 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "΢���ź�"
         Size            =   15.75
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   465
      Left            =   1200
      TabIndex        =   0
      Text            =   "4FEA13400"
      Top             =   5955
      Width           =   2040
   End
End
Attribute VB_Name = "Form_main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim print_now As Boolean ' ��ǵ�ǰ�Ƿ������ڻ���״̬
Dim click_x As Integer, click_y As Integer ' ��¼����λ�ö�Ӧ�����̱�� click_x -> 1 ~ 4 / click_y -> 1 ~ 5
Dim click_x_ As Integer, click_y_ As Integer ' ��¼����λ�õĴ�������
Dim mouse_x As Integer, mouse_y As Integer ' ��¼��ǰ���λ�úϷ���Ӧ�����̱��
Dim output As Case_size ' �����С
Dim case_main As Case_detail ' ������Ϣ
Dim exclude(-1 To 1, -1 To 1) As Boolean ' ��¼����λ����Χ�Ƿ��ܷ������� true -> ���� / false -> ��

Private Sub Command_Get_Code_Click()
  Text_Code = Get_Code(case_main)
End Sub

Private Sub Command1_Click()
  MsgBox Check_2x2_seat
End Sub

Private Sub Form_Load()
  print_now = False
  Dim i As Integer, j As Integer
  For i = 0 To 14
    case_main.kind(i) = 255
  Next i
  For i = 0 To 3
    For j = 0 To 4
      case_main.status(i, j) = 254
    Next j
  Next i

  output.start_x = 150
  output.start_y = 150

  output.square_width = 1000
  output.gap = 100
  style.block_line_width = 1
  style.case_line_width = 2
  style.block_line_color = RGB(0, 158, 240)
  style.case_line_color = RGB(0, 158, 240)
  style.block_color = RGB(225, 245, 255)
  style.case_color = RGB(248, 254, 255)
'  case_main.status(1, 1) = 254
'  case_main.status(1, 2) = 254
'  case_main.status(2, 1) = 254
'  case_main.status(2, 2) = 254
'  case_main.kind(0) = 255
  Call Output_case(Form_main, case_main, output)
  'Call Get_Code(case_main)

End Sub
Private Sub Command_Print_Click()
  If Len(Text_Code) <> 9 Then MsgBox "��������", , "��ʾ": Exit Sub
  If Parse_Code(Text_Code) = True Then
    case_main = Parse_data
    Call Output_case(Form_main, case_main, output)
  Else
    MsgBox "��������", , "��ʾ"
  End If
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) ' ������갴��
  Dim x_ As Integer, y_ As Integer, num As Integer
  Dim raw_x As Single, raw_y As Single
  Dim addr_x As Integer, addr_y As Integer
  Dim clear As Boolean
  clear = False
  raw_x = Get_block_x(X) ' �õ����λ���������ϵı��
  raw_y = Get_block_y(Y)
  If raw_x < 1 Or raw_x > 4 Or raw_y < 1 Or raw_y > 5 Then Exit Sub ' ����������� �˳�
  addr_x = Int(raw_x) - 1 ' �����λ��Ϊ��϶��ӳ�䵽����/�Ϸ�
  addr_y = Int(raw_y) - 1
  num = case_main.status(addr_x, addr_y) ' �õ���Ż�254
  If num <> 254 Then ' ��λ�ò�Ϊ��
    If Int(raw_x) <> raw_x And Int(raw_y) <> raw_y Then ' ����������϶�ཻ��
      If num = 0 And Get_addr_x(num) = addr_x And Get_addr_y(num) = addr_y Then clear = True ' �ж��Ƿ�����2 * 2��������
    ElseIf Int(raw_x) <> raw_x Then ' �����������϶
      If Get_addr_x(num) = addr_x Then '�ж��Ƿ����Ƿ���������
        If case_main.kind(num) = 0 Or case_main.kind(num) = 1 Then clear = True
      End If
    ElseIf Int(raw_y) <> raw_y Then ' �����ں����϶
      If Get_addr_y(num) = addr_y Then '�ж��Ƿ����Ƿ���������
        If case_main.kind(num) = 0 Or case_main.kind(num) = 2 Then clear = True
      End If
    Else ' �����ڼ�϶
      clear = True
    End If
  End If
  If clear = True Then ' ������ʶΪ�����������
    Call Clear_block(num) ' ���������
    Call Output_case(Form_main, case_main, output) ' ˢ����ʾ����
    Exit Sub ' �˳�
  End If
  If Int(raw_x) <> raw_x Or Int(raw_y) <> raw_y Then Exit Sub ' ����ڼ�϶�� �˳�
  click_x = raw_x: click_y = raw_y ' ��¼��Ч�ĵ��λ�ñ��
  click_x_ = X: click_y_ = Y ' ��¼�������ʵλ��
  num = case_main.status(click_x - 1, click_y - 1) ' ���λ�ö�Ӧ�ı���
  For y_ = -1 To 1
    For x_ = -1 To 1
      exclude(x_, y_) = False ' ȫ����ʼ��Ϊ�ɷ���
    Next x_
  Next y_
  If case_main.kind(0) <> 255 Then exclude(-1, -1) = True: exclude(-1, 1) = True: exclude(1, -1) = True: exclude(1, 1) = True ' ��2 * 2���Ѵ��� ���ĽǶ����Ϊ���ɷ���
  If click_x = 1 Then exclude(-1, -1) = True: exclude(-1, 0) = True: exclude(-1, 1) = True ' ����������
  If click_x = 4 Then exclude(1, -1) = True: exclude(1, 0) = True: exclude(1, 1) = True ' ����������
  If click_y = 1 Then exclude(-1, -1) = True: exclude(0, -1) = True: exclude(1, -1) = True ' �����������
  If click_y = 5 Then exclude(-1, 1) = True: exclude(0, 1) = True: exclude(1, 1) = True ' �����������
  For y_ = -1 To 1
    For x_ = -1 To 1
      If click_x + x_ >= 1 And click_x + x_ <= 4 And click_y + y_ >= 1 And click_y + y_ <= 5 Then ' ��ֹԽ��
        If case_main.status(click_x + x_ - 1, click_y + y_ - 1) <> 254 Then exclude(x_, y_) = True ' ��Ϊ�յı��Ϊ���ɷ���
      End If
    Next x_
  Next y_
  print_now = True ' �������ģʽ
  Call Form_MouseMove(Button, Shift, X, Y) ' ��������ƶ��¼�
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) ' �ƶ����
  Dim delta_x As Integer, delta_y As Integer
  Dim print_x As Integer, print_y As Integer
  Dim print_width As Integer, print_height As Integer
  If print_now = False Then Exit Sub ' �ж��Ƿ��ڻ���ģʽ
  Call Output_case(Form_main, case_main, output) ' ������һ�ε���ʾ
  delta_x = signed(Get_block_x(X) - click_x) ' ��¼���λ������λ�õ���Է���
  delta_y = signed(Get_block_y(Y) - click_y)
  mouse_x = click_x: mouse_y = click_y ' Ĭ��
  If Abs(delta_x) + Abs(delta_y) = 1 Then ' �������������
    If exclude(delta_x, delta_y) = False Then ' Ŀ��λ�ÿɷ�������
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
  End If
  If Abs(delta_x) + Abs(delta_y) = 2 Then ' �����б�Խ�
    If exclude(delta_x, 0) = True And exclude(0, delta_y) = False Then mouse_y = click_y + delta_y ' ����߱�����
    If exclude(delta_x, 0) = False And exclude(0, delta_y) = True Then mouse_x = click_x + delta_x
    If exclude(delta_x, delta_y) = True And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then ' ��б�ǲ��ɷ��ö������ǿյ�
      If Abs(click_x_ - X) > Abs(click_y_ - Y) Then ' x�᷽��λ�ƽϴ�
        mouse_x = click_x + delta_x
      Else ' y�᷽��λ�ƽϴ�
        mouse_y = click_y + delta_y
      End If
    End If
    If exclude(delta_x, delta_y) = False And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then ' ������λ�þ��ɷ���
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
  End If
  print_x = (min(click_x, mouse_x) - 1) * (output.square_width + output.gap) + output.gap + output.start_x ' ���������ʼλ��
  print_y = (min(click_y, mouse_y) - 1) * (output.square_width + output.gap) + output.gap + output.start_y
  If Abs(click_x - mouse_x) = 1 Then print_width = output.square_width * 2 + output.gap Else print_width = output.square_width ' ������ƿ��
  If Abs(click_y - mouse_y) = 1 Then print_height = output.square_width * 2 + output.gap Else print_height = output.square_width ' ������Ƹ߶�
  Call Print_Block(Form_main, print_x, print_y, print_width, print_height, style.block_line_width, style.block_color, style.block_line_color) ' ����
End Sub
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) ' �ͷ���갴��
  Dim i As Integer, j As Integer, num As Integer
  Dim block_x As Integer, block_y As Integer
  Dim block_width As Integer, block_height As Integer
  If print_now = False Then Exit Sub ' �ж��Ƿ��ڻ���ģʽ
  print_now = False ' �˳�����ģʽ
  block_x = min(click_x, mouse_x) - 1 ' ����������ӵ���ʼλ��
  block_y = min(click_y, mouse_y) - 1
  block_width = Abs(click_x - mouse_x) + 1 ' ����������ӵĿ�͸�
  block_height = Abs(click_y - mouse_y) + 1
    If block_width = 2 And block_height = 2 Then ' ���Ƶ�������2 * 2
    If Get_empty_num >= 6 Then ' ��ǰ������6����λ
      case_main.kind(0) = 0
      case_main.status(block_x, block_y) = 0
      case_main.status(block_x, block_y + 1) = 0
      case_main.status(block_x + 1, block_y) = 0
      case_main.status(block_x + 1, block_y + 1) = 0
    End If
  ElseIf block_width = 2 And block_height = 1 Then ' ���Ƶ�������2 * 1
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 4 Then ' ��ǰ������δ�����������ٴ���4����λ
      case_main.kind(num) = 1
      case_main.status(block_x, block_y) = num
      case_main.status(block_x + 1, block_y) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  ElseIf block_width = 1 And block_height = 2 Then ' ���Ƶ�������1 * 2
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 4 Then ' ��ǰ������δ�����������ٴ���4����λ
      case_main.kind(num) = 2
      case_main.status(block_x, block_y) = num
      case_main.status(block_x, block_y + 1) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  ElseIf block_width = 1 And block_height = 1 Then ' ���Ƶ�������1 * 1
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 3 Then ' ��ǰ������δ�����������ٴ���3����λ
      case_main.kind(num) = 3
      case_main.status(block_x, block_y) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  End If
  Call Output_case(Form_main, case_main, output) ' ˢ����ʾ����
End Sub
Private Sub Clear_block(num As Integer) ' ���ݱ�����������Ϣ
  Dim addr_x As Integer, addr_y As Integer
  addr_x = Get_addr_x(num) ' �õ��������Ͻ�λ��
  addr_y = Get_addr_y(num)
  If case_main.kind(num) = 0 Then ' 2 * 2
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x, addr_y + 1) = 254
    case_main.status(addr_x + 1, addr_y) = 254
    case_main.status(addr_x + 1, addr_y + 1) = 254
  ElseIf case_main.kind(num) = 1 Then ' 2 * 1
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x + 1, addr_y) = 254
  ElseIf case_main.kind(num) = 2 Then ' 1 * 2
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x, addr_y + 1) = 254
  ElseIf case_main.kind(num) = 3 Then ' 1 * 1
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
  End If
End Sub
Private Function Check_2x2_seat() As Boolean
  Dim i As Integer, j As Integer
  Check_2x2_seat = False
  'If case_main.kind(0) = 0 Then Exit Function
  For j = 0 To 3
    For i = 0 To 2
      If case_main.status(i, j) = 254 And case_main.status(i, j + 1) = 254 And case_main.status(i + 1, j) = 254 And case_main.status(i + 1, j + 1) = 254 Then Check_2x2_seat = True
    Next i
  Next j
End Function
Private Function Get_empty_num() As Integer ' ͳ�Ƶ�ǰ��λ����
  Dim i As Integer, j As Integer
  Get_empty_num = 0
  For i = 0 To 3
    For j = 0 To 4
      If case_main.status(i, j) = 254 Then Get_empty_num = Get_empty_num + 1
    Next j
  Next i
End Function
Private Function Get_empty_seat() As Integer ' ��case_main.kind���ҵ���λ
  Dim i As Integer
  Get_empty_seat = 0
  For i = 1 To 14
    If case_main.kind(i) = 255 Then ' ��λ����δ����
      Get_empty_seat = i
      Exit For
    End If
  Next i
End Function
Private Function signed(dat As Single) ' �õ����� ����-1 / 0 / 1
  If Abs(dat) <> 0 Then signed = dat / Abs(dat) Else signed = 0
End Function
Private Function min(dat_1 As Integer, dat_2 As Integer) As Integer ' ���ؽ�С��ֵ
  If dat_1 > dat_2 Then min = dat_2 Else min = dat_1
End Function
Private Function Get_addr_x(num As Integer) As Integer ' �ҵ���ŵ����Ͻ�x���� ���Ϊ�������򷵻�255
  Dim i As Integer, j As Integer
  Get_addr_x = 255
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = num Then
        Get_addr_x = i
        Exit Function
      End If
    Next i
  Next j
End Function
Private Function Get_addr_y(num As Integer) As Integer ' �ҵ���ŵ����Ͻ�y���� ���Ϊ�������򷵻�255
  Dim i As Integer, j As Integer
  Get_addr_y = 255
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = num Then
        Get_addr_y = j
        Exit Function
      End If
    Next i
  Next j
End Function
Private Function Get_block_x(dat As Single) As Single ' �������λ�����ڶ�Ӧ�����ϵĺ����� ����Ϸ���*.5 ��Խ�緵��0 ��Խ�緵��5
  dat = dat - output.start_x ' ȥ����ʼƫ��
  Dim i As Integer
  For i = 1 To 4
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_x = i ' �����������
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_x = i + 0.5 ' ��������Ӽ��
  Next i
  If dat > (output.gap + output.square_width) * 4 Then Get_block_x = 5 ' ��Խ��
  If dat < output.gap Then Get_block_x = 0 ' ��Խ��
  dat = dat + output.start_x
End Function
Private Function Get_block_y(dat As Single) As Single ' �������λ�����ڶ�Ӧ�����ϵ������� ����Ϸ���*.5 ��Խ�緵��0 ��Խ�緵��6
  dat = dat - output.start_y ' ȥ����ʼƫ��
  Dim i As Integer
  For i = 1 To 5
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_y = i ' �����������
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_y = i + 0.5 ' ��������Ӽ��
  Next i
  If dat > (output.gap + output.square_width) * 5 Then Get_block_y = 6 ' ��Խ��
  If dat < output.gap Then Get_block_y = 0 ' ��Խ��
  dat = dat + output.start_y
End Function

Private Sub Text_Code_KeyPress(KeyAscii As Integer)
  If KeyAscii = 13 Then Call Command_Print_Click
End Sub

Private Sub Timer1_Timer()
  Dim i As Integer, j As Integer
  Dim debug_dat As String
  debug_dat = ""
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = 254 Then
        debug_dat = debug_dat & "- "
      Else
        debug_dat = debug_dat & case_main.status(i, j) & " "
      End If
    Next i
    debug_dat = debug_dat & vbCrLf
  Next j
  
  For i = 0 To 14
    debug_dat = debug_dat & Trim(i) & ": " & case_main.kind(i) & vbCrLf
  Next i
  Text_debug = debug_dat
End Sub
