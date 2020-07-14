#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>
using namespace std;

ifstream File_Input;
ofstream File_Output;
const unsigned char Up = 1, Down = 2, Left = 3, Right = 4;

struct block_struct {
	unsigned char address;  //0~19
	unsigned char style;  //0:2*2  1:2*1  2:1*2  3:1*1
};
unsigned char table[20];  //0~9:block[?]  0xA:space  0xFF:empty
unsigned char space[2];  //space[0~1]��λ��
struct block_struct block[10];  //0:2*2  1~5:1*2,2*1  6~9:1*1

list <unsigned int> int_list;  //���б� ��Source��
list <unsigned int> Hash[0x10000];  //��ϣ������
unsigned int Now_Move;  //Ŀǰ���ڽ��м���Ĳ��ֱ��

vector <unsigned int> List;  //��������Ķ���
vector <unsigned int> Layer_Num;  //���ڲ�ı��
vector <list <unsigned int> > Source;  //�����ֱ��

int group_size;  //����������Ĵ�С
int min_steps;  //���������ٲ���
int farthest_steps;  //������Զ���ֵĲ���
vector <unsigned int> Solutions;  //���н�
vector <unsigned int> min_Solutions;  //�������ٲ���
vector <unsigned int> Solutions_path;  //���ٲ��ⷨ
vector <unsigned int> farthest_cases;  //��Զ�Ĳ��ֽⷨ

int target_steps;  //����Ŀ�겼�ֵ����ٲ���
vector <unsigned int> target_path;  //����Ŀ�겼�ֵ����ٲ��ⷨ
vector <vector <unsigned int> > target_data;  //target_data[Layer_num][num]
vector <vector <unsigned int> > target_source;  //target_source[next_index]

vector <unsigned int> all_data;  //���в��ֵı���
vector <vector <unsigned int> > style_data;  //style_0 ~ style_5�µ����б���
vector <vector <vector <unsigned int> > > group_data;  //group[style][grout_index]�µ����б���

void debug();
void Find_All_Case();
void Data_Output();
unsigned int Change_int (char str[8]);
string Change_str (unsigned int dat);
void Output_Graph (unsigned int Code);
void Analyse_Code (unsigned int Code);
void Add_Case (unsigned int Code);
void Calculate (unsigned int Start_Code);
bool Check (unsigned int Code);
unsigned int Get_Code();
void Find_Next();
bool Check_Empty (unsigned char address,unsigned char dir,unsigned char num);
void Move_Block (unsigned char num,unsigned char dir_1,unsigned char dir_2);
void Fill_Block (unsigned char addr, unsigned char style, unsigned char filler);
vector <unsigned int> Search_Path (unsigned int target_num);
void Analyse_Case (unsigned int Start_Code);
void Analyse_target (unsigned int target);

int main(){
	Analyse_Case(0x4FEA134);
	Analyse_target(0x4F2E9C4);
	return 0;

	cout<<"Welcome to HRD-Calculator by Dnomd343"<<endl;
	cout<<"Please Input the Code:";
}

vector <unsigned int> Search_Path (unsigned int target_num) {  //��������Ŀ�겼�ֵ�һ�����·�� ����vector��
	vector <unsigned int> path;
	int temp = -1;
	path.push_back(target_num);  //·���м���Ŀ�겼��
	while (temp != 0) {
		temp = path[path.size() - 1];
		path.push_back(*Source[temp].begin());
	}
	path.pop_back();  //ȥ���ظ��ĸ�����
	temp = path.size() / 2;  //��������·��
	for (int i = 0; i < temp; i++) {
		swap(path[i], path[path.size() - i - 1]);
	}
	for (int  i = 0; i < path.size(); i++){  //����Ÿĳɲ��ֵı���
		path[i] = List[path[i]];
	}
	return path;
}

void Analyse_target (unsigned int target) {
	bool get_it = false;
	unsigned int temp, target_num;
	list <unsigned int>::iterator poi;
	target_steps = -1;
	target_path.clear();
	web_target.clear();
	web_target_source.clear();
	for (int i = 0; i < List.size(); i++){  //����target�ı�ŵ�target_num
		if (List[i] == target){
			get_it = true;
			target_num = i;
			target_steps = Layer_Num[i];
			break;
		}
	}
	if (get_it == false) {return;}  //�Ҳ������target �˳�
	target_path = Search_Path(target_num);

}

void Analyse_Case (unsigned int Start_Code) {  //��һ�����ֽ��з���
	unsigned int i, first_solution;
	bool get_it = false;
	if (Check(Start_Code) == false) {return;}
	Calculate(Start_Code);  //ͨ�����㽨�����б�
	min_steps = -1;
	farthest_steps = -1;
	group_size=List.size();
	Solutions.clear();
	min_Solutions.clear();
	farthest_cases.clear();
	Solutions_path.clear();

	for (i = 0; i < List.size(); i++) {  //��������������Ԫ��
		Analyse_Code(List[i]);
		if (block[0].address == 13) {  //����ǰ����Ϊ��Ч��
			if (get_it == false){
				min_steps = Layer_Num[i];  //��һ���ҵ��Ľ�Ϊ���ٲ���
				first_solution = i;
				get_it = true;
			}
			Solutions.push_back(List[i]);
			if (Layer_Num[i] == min_steps) {
				min_Solutions.push_back(List[i]);
			}
		}
	}

	farthest_steps=Layer_Num[Layer_Num.size()-1];  //������Զ���ֵĲ���
	for (i = Layer_Num.size() - 1; i > 0; i--) {  //�ҵ�������Զ�Ĳ���
		if(Layer_Num[i] != farthest_steps) {break;}
		farthest_cases.push_back(List[i]);
	}
	if (min_steps != -1) {Solutions_path = Search_Path(first_solution);}
}

void Calculate (unsigned int Start_Code) {  //������������
	unsigned int i;
	for (i = 0; i <= 0xFFFF; i++) {Hash[i].clear();}  //��ʼ��
	List.clear();
	Layer_Num.clear();
	Source.clear();

	Hash[(Start_Code>>4) & 0xFFFF].push_back(0);  //�����ʼ����
	List.push_back(Start_Code);
	Layer_Num.push_back(0);
	Source.push_back(int_list);
	Now_Move = 0;  //����Ŀ��ָ����ڵ�
	while (Now_Move != List.size()) {  //���й����������
		Analyse_Code(List[Now_Move]);  //����Ŀ�겼��
		Find_Next();  //���ݽ���������������Ӳ���
		Now_Move++;
	}
}

void Add_Case (unsigned int Code) {  //���������������
    list <unsigned int>::iterator poi;  //���������
	poi = Hash[(Code>>4) & 0xFFFF].begin();  //����poiΪ���������ʼ��
	while (poi != Hash[(Code>>4) & 0xFFFF].end()) {  //����������
		if (Code == List[*poi]) {  //�������ظ�
            if ((Layer_Num[*poi] - Layer_Num[Now_Move]) == 1) {  //����һ��
				Source[*poi].push_back(Now_Move);  //���븸�ڵ��б�
			}
            return;  //�ظ� �˳�
        }
		++poi;
	}
	Hash[(Code>>4) & 0xFFFF].push_back(List.size());  //�������������������
	List.push_back(Code);  //���������������
	Layer_Num.push_back(Layer_Num[Now_Move] + 1);  //��Ӷ�Ӧ�Ĳ���
	Source.push_back(int_list);  //��ʼ���丸�ڵ��б�
	Source[Source.size()-1].push_back(Now_Move);  //�����ڽ��������Ĳ������Ϊ���ڵ�
}

void Fill_Block (unsigned char addr, unsigned char style, unsigned char filler) {  //��ָ���������table��ָ����λ��
	if (style == 0) {table[addr] = table[addr + 1] = table[addr + 4] = table[addr + 5] = filler;}  //2*2
	else if (style == 1) {table[addr] = table[addr + 1] = filler;}  //2*1
	else if (style == 2) {table[addr] = table[addr + 4] = filler;}  //1*2
	else if (style == 3) {table[addr] = filler;}  //1*1
}

void Move_Block (unsigned char num, unsigned char dir_1, unsigned char dir_2) {  //��Ҫ���ƶ��鲢���ƶ���ı��봫��Add_Case
	unsigned char i, addr, addr_bak;
	addr = block[num].address;
	addr_bak = addr;

	if (dir_1 == Up) {addr -= 4;}  //��һ���ƶ�
	else if (dir_1==Down) {addr += 4;}
	else if (dir_1==Left) {addr--;}
	else if (dir_1==Right) {addr++;}
	if (dir_2 == Up) {addr -= 4;}  //�ڶ����ƶ�
	else if (dir_2 == Down) {addr += 4;}
	else if (dir_2 == Left) {addr--;}
	else if (dir_2 == Right) {addr++;}

	Fill_Block(addr_bak, block[num].style, 0xA);  //�޸� tableΪ�ƶ����״̬
	Fill_Block(addr, block[num].style, num);
	block[num].address = addr;

	Add_Case(Get_Code());  //���ɱ��벢����Add_Case

	block[num].address = addr_bak;
	Fill_Block(addr, block[num].style, 0xA);  //��ԭ tableԭ����״̬
	Fill_Block(addr_bak, block[num].style, num);
}

void Find_Next() {  //Ѱ�������ƶ��ķ�ʽ���ύ��Move_Block
	bool Can_Move[10];
    unsigned char i, addr;
	for (i = 0; i < 10; i++) {Can_Move[i] = false;}
	for (i = 0; i <= 1; i++) {  //Ѱ��λ�ڿո���Χ�����п�
		addr = space[i];
		if (addr > 3)
			if (table[addr - 4] != 0xA) {Can_Move[table[addr - 4]] = true;}
		if (addr < 16)
			if (table[addr + 4] != 0xA) {Can_Move[table[addr + 4]] = true;}
		if (addr % 4 != 0)
			if (table[addr - 1] != 0xA) {Can_Move[table[addr - 1]] = true;}
		if (addr % 4 != 3) {
			if (table[addr + 1] != 0xA) {Can_Move[table[addr + 1]] = true;}}
	}

	for (i = 0; i <= 9; i++) {
		if (Can_Move[i] == true) {  //���ÿ���ܿ����ƶ�
			addr = block[i].address;
			if (block[i].style == 0) {  //2*2
				if ((Check_Empty(addr, Up, 1) == true) &&
					(Check_Empty(addr + 1,Up, 1) == true)) {Move_Block(i, Up, 0);}

				if ((Check_Empty(addr, Down, 2) == true) &&
					(Check_Empty(addr + 1,Down, 2) == true)) {Move_Block(i, Down, 0);}

				if ((Check_Empty(addr, Left, 1) == true) &&
					(Check_Empty(addr + 4, Left, 1) == true)) {Move_Block(i, Left, 0);}

				if ((Check_Empty(addr, Right, 2) == true) &&
					(Check_Empty(addr + 4, Right, 2) == true)) {Move_Block(i, Right, 0);}
			}
			else if (block[i].style == 1) {  //2*1
				if ((Check_Empty(addr, Up, 1) == true) &&
					(Check_Empty(addr + 1, Up, 1) == true)) {Move_Block(i, Up, 0);}

				if ((Check_Empty(addr, Down, 1) == true) &&
					(Check_Empty(addr + 1, Down, 1) == true)) {Move_Block(i, Down, 0);}

				if (Check_Empty(addr, Left, 1) == true) {
					Move_Block(i, Left, 0);
					if (Check_Empty(addr, Left, 2) == true) {Move_Block(i, Left, Left);}
				}

				if (Check_Empty(addr, Right, 2) == true) {
					Move_Block(i, Right, 0);
					if (Check_Empty(addr, Right, 3) == true) {Move_Block(i, Right, Right);}
				}
			}
			else if (block[i].style == 2) {  //1*2
				if (Check_Empty(addr, Up, 1) == true) {
					Move_Block(i, Up, 0);
					if (Check_Empty(addr, Up, 2) == true) {Move_Block(i, Up, Up);}
				}

				if (Check_Empty(addr, Down, 2) == true) {
					Move_Block(i, Down, 0);
					if (Check_Empty(addr, Down, 3) == true) {Move_Block(i, Down, Down);}
				}

				if ((Check_Empty(addr, Left, 1) == true) &&
					(Check_Empty(addr + 4, Left, 1) == true)) {Move_Block(i, Left, 0);}

				if ((Check_Empty(addr, Right, 1) == true) &&
					(Check_Empty(addr + 4, Right, 1) == true)) {Move_Block(i, Right, 0);}
			}
			else if (block[i].style == 3) {  //1*1
				if (Check_Empty(addr, Up, 1) == true) {
					Move_Block(i, Up, 0);
					if (Check_Empty(addr - 4, Up, 1) == true) {Move_Block(i, Up, Up);}
					if (Check_Empty(addr - 4, Left, 1) == true) {Move_Block(i, Up, Left);}
					if (Check_Empty(addr - 4, Right, 1) == true) {Move_Block(i, Up, Right);}
				}

				if (Check_Empty(addr, Down, 1) == true) {
					Move_Block(i, Down, 0);
					if (Check_Empty(addr + 4, Down, 1) == true) {Move_Block(i, Down, Down);}
					if (Check_Empty(addr + 4, Left, 1) == true) {Move_Block(i, Down, Left);}
					if (Check_Empty(addr + 4, Right, 1) == true) {Move_Block(i, Down, Right);}
				}

				if (Check_Empty(addr, Left, 1) == true) {
					Move_Block(i, Left, 0);
					if (Check_Empty(addr - 1, Up, 1) == true) {Move_Block(i, Left, Up);}
					if (Check_Empty(addr - 1, Down, 1) == true) {Move_Block(i, Left, Down);}
					if (Check_Empty(addr - 1, Left, 1) == true) {Move_Block(i, Left, Left);}
				}

				if (Check_Empty(addr, Right, 1) == true) {
					Move_Block(i, Right, 0);
					if (Check_Empty(addr + 1, Up, 1) == true) {Move_Block(i, Right, Up);}
					if (Check_Empty(addr + 1, Down, 1) == true) {Move_Block(i, Right, Down);}
					if (Check_Empty(addr + 1, Right, 1) == true) {Move_Block(i, Right, Right);}
				}
			}
		}
	}
}

bool Check_Empty (unsigned char address, unsigned char dir, unsigned char num) {  //�ж�ָ��λ���Ƿ�Ϊ�ո� �����ǿո������Ч����false
	unsigned char x, y, addr;
	if (address > 19) {return false;}  //����λ�ò�����

	x = address % 4;
	y = (address - x) / 4;
	if (dir == Up) {  //�Ϸ�
		if (y < num) {return false;}
		addr = address - num * 4;
	}
	if (dir == Down) {  //�·�
		if (y + num > 4) {return false;}
		addr = address + num * 4;
	}
	if (dir == Left) {  //���
		if (x < num) {return false;}
		addr = address - num;
	}
	if (dir == Right) {  //�Ҳ�
		if(x + num > 3){return false;}
		addr = address + num;
	}

	if (table[addr] == 0xA) {
		return true;
	} else {
		return false;
	}
}

unsigned int Get_Code() {  //���ɱ���
	bool temp[20];
	unsigned int Code = 0;
	unsigned char i, addr, style;

	for (i = 0; i < 20; i++) {temp[i] = false;}  //��ʼ��
	temp[block[0].address] = temp[block[0].address + 1] =
		temp[block[0].address + 4] = temp[block[0].address + 5] = true;

	Code |= block[0].address<<24;  //2*2���λ��
	addr = 0;
	for (i = 1; i <= 11; i++) {
		while(temp[addr] == true){  //�ҵ���һ��δ���Ŀո�
			if (addr < 19) {
				addr++;
			} else {
				return 0;
			}
		}
		if (table[addr] == 0xA) {  //�ո�
			temp[addr] = true;
		} else {
			style = block[table[addr]].style;
			if (style == 1) {  //2*1
				temp[addr] = temp[addr + 1] = true;
				Code |= 1<<(24 - i * 2);
			}
			else if (style == 2) {  //1*2
				temp[addr] = temp[addr + 4] = true;
				Code |= 2<<(24 - i * 2);
			}
			else if (style == 3) {  //1*1
				temp[addr] = true;
				Code |= 3<<(24 - i * 2);
			}
		}
	}
	return Code;
}

void Analyse_Code (unsigned int Code) {  //������뵽 table[20] block[10] space[2]��
	unsigned char i, addr, style;
	unsigned char num_space = 0, num_type_1 = 0, num_type_2 = 5;
	space[0] = space[1] = 0xFF;  //��ʼ��
	for (i = 0; i < 20; i++) {table[i] = 0xFF;}
	for (i = 0; i <= 9; i++) {
		block[i].address = 0xFF;
		block[i].style = 0xFF;
	}

	block[0].address = 0xF & (Code>>24);  //��ʼ����
	if (block[0].address > 14) {goto err;}  //2*2��Խ�� �˳�
	block[0].style = 0;  //����2*2��Ĳ���
	Fill_Block(block[0].address, 0, 0x0);
	addr = 0;
	for (i = 0; i < 11; i++) {  //���� 10����
		while (table[addr] != 0xFF){  //���������տ�
			if (addr < 19) {
				addr++;
			} else {
				break;
			}
		}
		style = 0x3 & (Code>>(22 - i * 2));  //0:space  1:2*1  2:1*2  3:1*1
		if (style == 0) {  //space
			table[addr] = 0xA;
			space[num_space] = addr;
			if (num_space == 0) {num_space++;}
		}
		if (style == 1) {  //2*1
			if (num_type_1 < 5) {num_type_1++;}
			if (addr > 18) {goto err;}  //2*1��Խ��
			block[num_type_1].style = 1;
			block[num_type_1].address = addr;
			table[addr] = table[addr + 1] = num_type_1;
		}
		if (style == 2) {  //1*2
			if (num_type_1 < 5) {num_type_1++;}
			if (addr > 15) {goto err;}  //1*2��Խ��
			block[num_type_1].style = 2;
			block[num_type_1].address = addr;
			table[addr] = table[addr + 4] = num_type_1;
		}
		if (style == 3) {  //1*1
			if (num_type_2 < 9) {num_type_2++;}
			block[num_type_2].style = 3;
			block[num_type_2].address = addr;
			table[addr] = num_type_2;
		}
	}
	err:;
}

bool Check (unsigned int Code) {  //�������Ƿ�Ϸ� ��ȷ����true ���󷵻�false
	bool temp[20];
	unsigned char addr, i;
	Analyse_Code(Code);

	for (i = 0; i < 20; i++){temp[i] = false;}  //��ʼ��
	for (i = 0; i < 20; i++) {  //���table�����Ƿ�Ϸ�
		if (table[i] > 10) {return false;}
	}

	if (block[0].style != 0) {return false;}  //���2*2��
	for (i = 1; i <= 5; i++) {  //���2*1��1*2��
		if ((block[i].style != 1) && (block[i].style != 2)) {return false;}
	}
	for (i = 6; i <= 9; i++) {  //���1*1��
		if (block[i].style != 3) {return false;}
	}
	for (i = 0; i <= 1; i++) { //���ո�
		if (space[i] > 19) {
			return false;
		} else {
			temp[space[i]] = true;
		}
	}

	addr = block[0].address;  //���2*2��
	if ((addr > 14) || (addr%4 == 3)) {return false;}
	if ((temp[addr] == true) || (temp[addr + 1] == true) ||
		(temp[addr + 4] == true) || (temp[addr + 5] == true)) {return false;}
	temp[addr] = temp[addr + 1] = temp[addr + 4] = temp[addr + 5] = true;
	for (i = 1; i <= 5; i++) {  //���2*1��1*2��
		addr = block[i].address;
		if (block[i].style == 1) {
			if ((addr > 18) || (addr % 4 == 3)) {return false;}
			if ((temp[addr] == true) || (temp[addr + 1] == true)) {return false;}
			temp[addr] = temp[addr + 1] = true;
		}
		if (block[i].style == 2) {
			if (addr > 15) {return false;}
			if ((temp[addr] == true) || (temp[addr + 4] == true)) {return false;}
			temp[addr] = temp[addr + 4] = true;
		}
	}
	for (i = 6; i <= 9; i++) {  //���1*1��
		addr = block[i].address;
		if (addr > 19) {return false;}
		if (temp[addr] == true) {return false;}
		temp[addr] = true;
	}
	return true;
}

void Output_Graph (unsigned int Code) {  //��������ͼ�λ��������ͼ
	const unsigned char Square_Width = 4;
	const string Square_str = "[]";  //or "u2588"
	unsigned int i, k, m;
	unsigned int start_x, start_y, end_x, end_y;
	bool Graph[(Square_Width + 1) * 4 + 1][(Square_Width + 1) * 5 + 1];  //����ʽ�������ͼ��
	if (Check(Code) == false) {
		cout<<"The Code is Wrong!"<<endl;
		return;
	}

	Analyse_Code(Code);  //�Ƚ�������
	for (k = 0; k <= (Square_Width + 1) * 5; k++) {  //��ʼ��
		for (i = 0; i <= (Square_Width + 1) * 4; i++) {
			Graph[i][k] = false;
		}
	}

	for (m = 0; m <= 9; m++) {  //�����Ÿ���
		start_x = (block[m].address % 4) * (Square_Width + 1) + 1;  //����ÿ�����Ͻ�����
		start_y = int(block[m].address / 4) * (Square_Width + 1) + 1;
		if ((block[m].style == 0) || (block[m].style == 1)) {  //����ÿ�����½Ǻ�����
			end_x = start_x + Square_Width * 2;  //2*2 or 2*1
		} else {
			end_x = start_x + Square_Width - 1;  //1*2 or 1*1
		}
		if ((block[m].style == 0) || (block[m].style == 2)) {  //����ÿ�����½�������
			end_y = start_y + Square_Width * 2;  //2*2 or 1*2
		} else {
			end_y = start_y + Square_Width - 1;  //2*1 or 1*1
		}
		for (i = start_x; i <= end_x; i++) {  //���ÿ鸲�ǵ�����
			for(k = start_y; k <= end_y; k++) {
				Graph[i][k] = true;
			}
		}
	}

	for (i = 1; i <= (Square_Width + 1) * 4 + 3; i++) {cout<<Square_str;}  //��ʾ�ϱ߿�
	cout<<endl<<Square_str;
	for (k = 0; k <= (Square_Width + 1) * 5; k++){  //��ʾͼ������
		for (i = 0; i <= (Square_Width + 1) * 4; i++) {
			if (Graph[i][k] == false){
				cout<<"  ";
			} else {
				cout<<Square_str;
			}
		}
		cout<<Square_str<<endl<<Square_str;  //��ʾ���ұ߿�
	}
	for (i = 1; i <= (Square_Width + 1) * 4 + 2; i++) {cout<<Square_str;}  //��ʾ�±߿�
	cout<<endl;
}

string Change_str (unsigned int dat) {  //����������ת��Ϊ�ַ�
	unsigned char i, bit;
	string str = "";
	for (i = 0; i < 7; i++) {
		bit = 0xF & dat>>(6 - i)*4;  //���뵥��ʮ������λ
		if ((bit >= 0) && (bit <= 9)) {str += bit + 48;}  //0~9
		if ((bit >= 0xA) && (bit <= 0xF)) {str += bit + 55;}  //A~F
	}
	return str;
}

unsigned int Change_int (char str[8]) {  //�������ַ�ת��Ϊint
	unsigned int i, dat = 0;
	for (i = 0; i < 7; i++) {
		if ((str[i] >= 48) && (str[i] <= 57)) {dat = dat | (str[i] - 48)<<(24 - i * 4);}  //0~9
		if ((str[i] >= 65) && (str[i] <= 70)) {dat = dat | (str[i] - 55)<<(24 - i * 4);}  //A~F
		if ((str[i] >= 97) && (str[i] <= 102)) {dat = dat | (str[i] - 87)<<(24 - i * 4);}  //a~f
	}
	return dat;
}

void debug() {  //�����ǰ table[20] block[10] space[2]��״̬
	unsigned int i;
	for (i = 0; i < 20; i++) {
		if (table[i] != 0xA) {
			cout<<int(table[i])<<" ";
		} else {
			cout<<"A ";
		}
		if (i == 3) {cout<<"        00 01 02 03"<<endl;}
		if (i == 7) {cout<<"        04 05 06 07"<<endl;}
		if (i == 11) {cout<<"        08 09 10 11"<<endl;}
		if (i == 15) {cout<<"        12 13 14 15"<<endl;}
		if (i == 19) {cout<<"        16 17 18 19"<<endl;}
	}
	cout<<endl<<"space[0] -> address="<<int(space[0])<<endl;
	cout<<"space[1] -> address="<<int(space[1])<<endl<<endl;
	for (i = 0; i <= 9; i++) {
		cout<<"block["<<i<<"] -> address="<<int(block[i].address)<<" style="<<int(block[i].style)<<endl;
	}
}

void Find_All_Case(){  //Right_File's MD5: 4A0179C6AEF266699A73E41ECB4D87C1
	File_Output.open("All_Case.txt");
	unsigned int i;
	for(i=0;i<0xFFFFFFF;i=i+4){
		if(Check(i)==true){File_Output<<Change_str(i)<<endl;}
	}
	File_Output.close();
}
void Data_Output(){
    unsigned int i;
    list<unsigned int>::iterator poi;
    File_Output.open("Data_Output.txt");
    for(i=0;i<List.size();i++){
        File_Output<<i<<" -> "<<Change_str(List[i])<<" ("<<Layer_Num[i]<<")";
        poi=Source[i].begin();
        while(poi!=Source[i].end()){File_Output<<"  ["<<*poi<<"]";++poi;}
        File_Output<<endl;
    }
    File_Output.close();
}
