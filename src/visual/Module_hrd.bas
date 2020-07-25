Attribute VB_Name = "Module_hrd"
Option Explicit
Type Case_detail
  status(0 To 3, 0 To 4) As Integer ' 255 -> undefined ; 254 -> space
  kind(0 To 14) As Integer ' 0 -> 2 * 2 ; 1 -> 2 * 1 ; 2 -> 1 * 2 ; 3 -> 1 * 1
  code As String ' length -> 9
End Type
Public Parse_data As Case_detail ' ��������ķ�������
Public Sub Output_case(obj, case_data As Case_detail, case_output As Case_size) ' ������Ĳ�����ʾ��obj��
  Dim i As Integer, x As Integer, y As Integer
  Dim block_type As Integer
  Dim exclude(0 To 3, 0 To 4) ' �ų��Ѿ��������Ŀ�
  Dim print_x As Integer, print_y As Integer ' ��ʾ����ʼλ��
  Dim print_width As Integer, print_height As Integer ' ��ʾ�Ŀ�Ⱥ͸߶�
  For y = 0 To 4 ' ��ʼ��exclude
    For x = 0 To 3
      exclude(x, y) = False
    Next x
  Next y
  ' ��ʾ�����
  Call Print_Block(obj, case_output.start_x, case_output.start_y, case_output.square_width * 4 + case_output.gap * 5, case_output.square_width * 5 + case_output.gap * 6, style.case_line_width, style.case_color, style.case_line_color)
  For y = 0 To 4 ' ����20��λ��
    For x = 0 To 3
      If exclude(x, y) = False And case_data.status(x, y) <> 254 Then ' δ�����ֹ��Ҹÿ鲻Ϊ��
        print_x = x * (case_output.square_width + case_output.gap) + case_output.gap + case_output.start_x ' ������ʼλ��
        print_y = y * (case_output.square_width + case_output.gap) + case_output.gap + case_output.start_y
        block_type = case_data.kind(case_data.status(x, y)) ' �õ��������
        If block_type = 0 Then ' 2 * 2
          print_width = case_output.square_width * 2 + case_output.gap
          print_height = case_output.square_width * 2 + case_output.gap
          exclude(x + 1, y) = True ' ����Ϊ�ѷ���
          exclude(x, y + 1) = True
          exclude(x + 1, y + 1) = True
        ElseIf block_type = 1 Then ' 2 * 1
          print_width = case_output.square_width * 2 + case_output.gap
          print_height = case_output.square_width
          exclude(x + 1, y) = True ' ����Ϊ�ѷ���
        ElseIf block_type = 2 Then ' 1 * 2
          print_width = case_output.square_width
          print_height = case_output.square_width * 2 + case_output.gap
          exclude(x, y + 1) = True ' ����Ϊ�ѷ���
        ElseIf block_type = 3 Then ' 1 * 1
          print_width = case_output.square_width
          print_height = case_output.square_width
        End If
        ' ��ʾ�ҵ��Ŀ�
        Call Print_Block(obj, print_x, print_y, print_width, print_height, style.block_line_width, style.block_color, style.block_line_color)
      End If
    Next x
  Next y
End Sub
Public Sub Print_Block(obj, print_start_x, print_start_y, print_width, print_height, print_line_width, print_color, print_line_color) ' ��ӡ��������ľ��ε�obj��
  If print_width < 0 Or print_height < 0 Then Exit Sub
  obj.FillStyle = 0
  obj.DrawWidth = print_line_width
  obj.FillColor = print_color
  obj.Line (print_start_x, print_start_y)-(print_start_x + print_width, print_start_y + print_height), print_color, B
  obj.Line (print_start_x, print_start_y)-(print_start_x + print_width, print_start_y + print_height), print_line_color, B
End Sub
Function change_str(dat As Integer) As String ' ����һ��ʮ������λ ת��Ϊ�ַ�������
  If dat <= 9 And dat >= 0 Then
    change_str = str(dat)
  ElseIf dat >= 10 And dat <= 15 Then
    change_str = Chr(dat + 55)
  Else
    change_str = "-"
  End If
  change_str = Trim(change_str)
End Function
Function change_int(dat As String) As Integer ' ����һ��ʮ������λ�ַ��� ת��Ϊint����
  If Asc(dat) >= 48 And Asc(dat) <= 57 Then change_int = Int(dat) ' 0 - 9
  If Asc(dat) >= 65 And Asc(dat) <= 70 Then change_int = Asc(dat) - 55 ' A - F
  If Asc(dat) >= 97 And Asc(dat) <= 102 Then change_int = Asc(dat) - 87 ' a - f
End Function
Public Function Get_Code(case_data As Case_detail) As String ' ��ȡ���� �������ݱ�������
  Dim x As Integer, y As Integer, num As Integer
  Dim block_type As Integer
  Dim range(1 To 16) As Integer
  Dim exclude(0 To 3, 0 To 4) ' �ų��Ѿ��������Ŀ�
  For num = 1 To 16 ' ��ʼ��range
    range(num) = 0
  Next num
  For y = 0 To 4 ' ��ʼ��exclude
    For x = 0 To 3
      exclude(x, y) = False
    Next x
  Next y
  num = 1
  For y = 0 To 4 ' ����20��λ��
    For x = 0 To 3
      If exclude(x, y) = False Then ' δ�����ֹ��Ҹÿ鲻Ϊ��
        If case_data.status(x, y) <> 254 Then
          block_type = case_data.kind(case_data.status(x, y)) ' �õ��������
          If block_type = 0 Then ' 2 * 2
            Get_Code = change_str(x + y * 4)
            exclude(x + 1, y) = True
            exclude(x, y + 1) = True
            exclude(x + 1, y + 1) = True
          ElseIf block_type = 1 Then ' 2 * 1
            range(num) = 1
            num = num + 1
            exclude(x + 1, y) = True
          ElseIf block_type = 2 Then ' 1 * 2
            range(num) = 2
            num = num + 1
            exclude(x, y + 1) = True
          ElseIf block_type = 3 Then ' 1 * 1
            range(num) = 3
            num = num + 1
          End If
        Else ' �ո�
          range(num) = 0
          num = num + 1
        End If
      End If
    Next x
  Next y
  For num = 1 To 16
    Get_Code = Get_Code & change_str(range(num) * 4 + range(num + 1)) ' ÿ����ת��Ϊһ��ʮ������λ
    num = num + 1
  Next num
End Function
Public Function Parse_Code(code As String) As Boolean ' ������� ���������Parse_data�� ������󷵻�false
  Dim space_num As Integer
  Dim i As Integer, num As Integer
  Dim x As Integer, y As Integer
  Dim range(1 To 16) As Integer
  Parse_Code = False
  For i = 1 To 8 ' �����8λ�и��16��
    num = change_int(Mid(code, i + 1, 1)) ' ȡ��iλ��תΪ����
    range(i * 2) = num Mod 4
    range(i * 2 - 1) = (num - num Mod 4) / 4 Mod 4
  Next i
  For x = 0 To 3 ' ��ʼ��status
    For y = 0 To 4
      Parse_data.status(x, y) = 255
    Next y
  Next x
  For i = 0 To 14 ' ��ʼ��kind
    Parse_data.kind(i) = 255
  Next i
  num = 0
  For i = 1 To 16 ' ͳ��������0�ĸ���
    If range(i) = 0 Then num = num + 1
  Next i
  If num < 2 Then GoTo code_err ' 0�ĸ���������������
  num = change_int(Mid(code, 1, 1))
  If num > 14 Or num Mod 4 = 3 Then GoTo code_err ' �ų�2 * 2��Խ�����
  x = num Mod 4
  y = Int(num / 4)
  Parse_data.kind(0) = 0 ' ����2 * 2����
  Parse_data.status(x, y) = 0
  Parse_data.status(x, y + 1) = 0
  Parse_data.status(x + 1, y) = 0
  Parse_data.status(x + 1, y + 1) = 0
  num = 0: x = 0: y = 0
  For i = 1 To 16
    While Parse_data.status(x, y) <> 255 ' �ҵ���һ��δ�����λ��
      x = x + 1
      If x = 4 Then ' ������ĩ
        x = 0 ' �ƶ�����һ����ʼ
        y = y + 1
        If y = 5 Then ' ������20����λ Խ��
          If space_num < 2 Then GoTo code_err ' �ո�������� ����
          For num = i To 16 ' ������±����Ƿ�Ϊ0
            'Form_main.Text_Focus = Form_main.Text_Focus & range(num)
            If range(num) <> 0 Then GoTo code_err ' ���ַ�0 �������
          Next num
          GoTo code_right ' ȫΪ0 ������ȷ
        End If
      End If
    Wend
    If range(i) = 0 Then ' space
      space_num = space_num + 1
      Parse_data.status(x, y) = 254
    ElseIf range(i) = 1 Then ' 2 * 1
      If x = 3 Then GoTo code_err ' Խ�����
      If Parse_data.status(x + 1, y) <> 255 Then GoTo code_err ' �����ص�
      num = num + 1
      Parse_data.kind(num) = 1
      Parse_data.status(x, y) = num
      Parse_data.status(x + 1, y) = num
    ElseIf range(i) = 2 Then ' 1 * 2
      If y = 4 Then GoTo code_err ' Խ�����
      If Parse_data.status(x, y + 1) <> 255 Then GoTo code_err ' �����ص�
      num = num + 1
      Parse_data.kind(num) = 2
      Parse_data.status(x, y) = num
      Parse_data.status(x, y + 1) = num
    ElseIf range(i) = 3 Then ' 1 * 1
      num = num + 1
      Parse_data.kind(num) = 3
      Parse_data.status(x, y) = num
    End If
  Next i
code_right:
  Parse_Code = True
code_err:
End Function
