VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual v1.0 by Dnomd343"
   ClientHeight    =   6495
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6495
   ScaleWidth      =   4815
   StartUpPosition =   2  '��Ļ����
   Begin VB.Timer Timer 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   4305
      Top             =   5040
   End
   Begin VB.CommandButton Command_OK 
      Caption         =   "OK"
      Height          =   465
      Left            =   3615
      TabIndex        =   3
      Top             =   5880
      Width           =   1050
   End
   Begin VB.CommandButton Command_Clear 
      Caption         =   "Clear"
      Height          =   465
      Left            =   150
      TabIndex        =   2
      Top             =   5900
      Width           =   1050
   End
   Begin VB.TextBox Text_Focus 
      Height          =   270
      Left            =   6000
      TabIndex        =   0
      Top             =   0
      Width           =   180
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
      Left            =   1400
      MaxLength       =   9
      TabIndex        =   1
      Text            =   "---"
      Top             =   5900
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
Dim timer_ctl As Integer
Dim exclude(-1 To 1, -1 To 1) As Boolean ' ��¼����λ����Χ�Ƿ��ܷ������� true -> ���� / false -> ��
Private Sub Form_Load() ' �����ʼ��
  Call Init_case
  print_now = False
  output.start_x = 150
  output.start_y = 150
  output.gap = 100
  output.square_width = 1000
  style.block_line_width = 1
  style.case_line_width = 2
  style.block_line_color = RGB(0, 158, 240)
  style.case_line_color = RGB(0, 158, 240)
  style.block_color = RGB(225, 245, 255)
  style.case_color = RGB(248, 254, 255)
  Call Output_case(Form_main, case_main, output) ' ��ʾ����
End Sub
Private Sub Command_Clear_Click() ' �����ǰ��ʾ
  Call Init_case
  Call Output_case(Form_main, case_main, output) ' ˢ����ʾ����
End Sub
Private Sub Command_OK_Click() ' ��ɰ�ť
  If Try_parse_code(Text_Code) = False Then
    MsgBox "�������"
  Else
    Text_Code = UCase(Text_Code) & String(9 - Len(Text_Code), "0") ' �޸�Ϊ��д����0
    Text_Focus.SetFocus ' ���߽���
  End If
End Sub
Private Sub Text_Code_DblClick() ' ˫���ı��� �������
  Text_Code = ""
  Text_Code.ForeColor = vbBlack
End Sub
Private Sub Text_Code_GotFocus() ' �ı���õ�����
  If Text_Code = "---" Then Text_Code = "" ' ����δ�������������
End Sub
Private Sub Text_Code_LostFocus() ' �ı���ʧȥ����
  If Text_Code = "" Then ' δ��д����
    Text_Code = "---"
    Call Try_get_code
  Else
    If Text_Code.ForeColor = vbBlack Then Text_Code = UCase(Text_Code) ' ȫ����Ϊ��д
  End If
End Sub
Private Sub Text_Code_KeyPress(KeyAscii As Integer) ' �ı�����������
  Dim code As String
  If KeyAscii = 13 Then ' �س���
    Call Command_OK_Click ' ģ����OK��ť
  Else
    timer_ctl = 200 ' ������timer��ʾ
    Timer.Enabled = True
  End If
End Sub
Private Function Try_parse_code(code As String) As Boolean ' ���Խ���
  Dim i As Integer, dat As String, flag As Boolean
  If Len(code) > 9 Then Try_parse_code = False: Exit Function ' ���볬9λ �˳�
  For i = 1 To Len(code) ' �����ַ���
    dat = Mid(code, i, 1)
    flag = False
    If Asc(dat) >= 48 And Asc(dat) <= 57 Then flag = True ' 0 - 9
    If Asc(dat) >= 65 And Asc(dat) <= 70 Then flag = True ' A - F
    If Asc(dat) >= 97 And Asc(dat) <= 102 Then flag = True ' a - f
    If flag = False Then Try_parse_code = False: Exit Function ' ������ڲ��Ϸ��ַ� �˳�
  Next i
  If Len(code) < 9 Then code = code & String(9 - Len(code), "0") ' ��0��9λ
  If Parse_Code(code) = True Then ' ������ȷ
    case_main = Parse_data
    Call Output_case(Form_main, case_main, output) ' ˢ����ʾ����
    Try_parse_code = True
  Else
    Try_parse_code = False
    Exit Function
  End If
End Function
Private Sub Try_get_code() ' ���Ի�ȡ����
  Dim code As String
  If case_main.kind(0) <> 0 Then Exit Sub ' 2 * 2�黹δȷ�� �˳�
  code = Get_Code(case_main)
  While Right(code, 1) = "0" ' ȥ���󷽵�0
    code = Left(code, Len(code) - 1)
    If Len(code) = 0 Then Text_Code = "0": Exit Sub ' ��ȫΪ0 ����һ���˳�
  Wend
  Text_Code = code
End Sub
Private Sub Timer_Timer()
  timer_ctl = timer_ctl - 1
  If timer_ctl = -1 Then Timer.Enabled = False ' ���д�������
  If Text_Code = "" Then ' �Ѿ���ɾ��
    Call Init_case
    Call Output_case(Form_main, case_main, output)
    Exit Sub
  End If
  If Try_parse_code(Text_Code) = False Then ' ����������ȷ����ʾ��ͬ��ɫ
    Text_Code.ForeColor = vbRed
  Else
    Text_Code.ForeColor = vbBlack
  End If
End Sub
Private Sub Form_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single) ' ������갴��
  Text_Focus.SetFocus
  Dim x_ As Integer, y_ As Integer, num As Integer
  Dim raw_x As Single, raw_y As Single
  Dim addr_x As Integer, addr_y As Integer
  Dim clear As Boolean
  clear = False
  raw_x = Get_block_x(x) ' �õ����λ���������ϵı��
  raw_y = Get_block_y(y)
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
    Text_Code = "---"
    Call Try_get_code
    Exit Sub ' �˳�
  End If
  If Int(raw_x) <> raw_x Or Int(raw_y) <> raw_y Then Exit Sub ' ����ڼ�϶�� �˳�
  click_x = raw_x: click_y = raw_y ' ��¼��Ч�ĵ��λ�ñ��
  click_x_ = x: click_y_ = y ' ��¼�������ʵλ��
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
  Call Form_MouseMove(Button, Shift, x, y) ' ��������ƶ��¼�
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single) ' �ƶ����
  Dim delta_x As Integer, delta_y As Integer
  Dim print_x As Integer, print_y As Integer
  Dim print_width As Integer, print_height As Integer
  If print_now = False Then Exit Sub ' �ж��Ƿ��ڻ���ģʽ
  Call Output_case(Form_main, case_main, output) ' ������һ�ε���ʾ
  delta_x = signed(Get_block_x(x) - click_x) ' ��¼���λ������λ�õ���Է���
  delta_y = signed(Get_block_y(y) - click_y)
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
      If Abs(click_x_ - x) > Abs(click_y_ - y) Then ' x�᷽��λ�ƽϴ�
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
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single) ' �ͷ���갴��
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
  Call Try_get_code
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
Private Sub Init_case() ' ��ʼ������
  Dim i As Integer, j As Integer
  For i = 0 To 14
    case_main.kind(i) = 255
  Next i
  For i = 0 To 3
    For j = 0 To 4
      case_main.status(i, j) = 254
    Next j
  Next i
End Sub
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
