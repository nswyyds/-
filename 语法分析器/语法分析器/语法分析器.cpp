#include<iostream>
#include<iomanip>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<set>
#include<list>
#include<stack>
#include<cstring>
#include<fstream>
#include<sstream>

using namespace std;

set<char> nfirst;
set<char> follow;
stack<char> analy_stack;


list<string> l_grammar;
//�﷨����

set<char> s_vn;
set<char> s_vt;
//�ս������ս������

list<set<char>> all_first;
list<set<char>> all_follow;
//�洢ÿһ����first��follow

vector<char> v_left;
//�洢����ʽ��
list<list<string>> l_table;
//Ԥ�������
void Init_grammar();//���⴦��ÿһ���﷨
void creategrammar(string str);//�����﷨����
void createvn(string s0);//�洢���ս����
void createvt(string s0);//�洢�ս����
void createv(string, set<char>);
void init_table();//��ʼ��Ԥ�������
int loc_x(list<list<string>>::iterator y, string str0);//���ַ���Ԥ��������к�����
int loc_y(string str0);//���ַ���Ԥ���������������
void insert_table(int x, int y, string st0);//��Ԥ��������Ӧλ�ò���Ԫ��
void createtable();//����Ԥ�������
void table_print();//��ӡԤ�������
string table_analy(char ch0, char ch1);//ȡ���������ж�Ӧ����ʽ
void init_stack();//��ʼ��ջ
string str_reverse(string str0);//�ַ�����ת
void push_stack(string str);//����ʽ�Ҳ���ת����ջ
string str_erase(string);//ȥ���ַ���ͷ��
void print_analy(string str0);//��ӡ��ʱջ�������ַ���
string str_erase_3(string str0);//�õ�����ʽ�Ҳ�
bool stack_analy(string str0);//����ջ��������

void Init_grammar() {
	ifstream ifs;
	string str;
	ifs.open("data.txt", ios::in);

	if (!ifs.is_open()) {
		cout << "�ļ���ʧ��" << endl;
	}

	while (getline(ifs, str)) {
		creategrammar(str);//������ʽ��ӵ�������
		str = "";
	}

	ifs.close();
}

void creategrammar(string str) {

	int flag0 = 3;
	int flag1 = 3;
	int flag2 = 0;
	//������־λ�������ʽ�Ҳ��Ŀ�ʼ��|���ŵ�λ��

	int length = str.length();
	char s = str[0];
	string linshi = " ";
	vector<char> v;
	set<char> s_ch;

	s_ch.insert(' ');
	v_left.push_back(s);
	all_first.push_back(s_ch);
	all_follow.push_back(s_ch);
	//Ϊ������ǰ����һ���ռ��ֹ������ָ��Խ��

	
	linshi[0] = s;
	linshi += "->";
	//��������ʽ��

	for (int i = 0; i < length; i++) {
		if (str[i] == '|') { //��ִ���|�Ĳ���ʽ
			flag2 = 1;
			flag1 = i;
			for (int j = flag0; j < flag1; j++) {
				linshi += str[j];
			}
			l_grammar.push_back(linshi);
			linshi = " ";
			linshi[0] = s;
			linshi += "->";
			flag0 = flag1 + 1;
		}

	}

	if (flag2 == 1) {
		for (int j = flag0; j < length; j++) {
			linshi += str[j];
		}
		l_grammar.push_back(linshi);
	}



	if (flag2 == 0) {
		l_grammar.push_back(str);
		return;
	}


	for (int i = flag0 + 1; i < length; i++) {
		linshi += str[i];
		l_grammar.push_back(linshi);
	}

}

void createvn(string s0) {
	int length = s0.length();
	for (int i = 0; i < length; i++) {
		if (s0[i] == ' ') {
			continue;
		}
		s_vn.insert(s0[i]);
	}
}

void createvt(string s0) {
	int length = s0.length();
	for (int i = 0; i < length; i++) {
		if (s0[i] == ' ') {
			continue;
		}
		s_vt.insert(s0[i]);
	}
}

void init_table() {
	set<char>::iterator ite;
	list<list<string>>::iterator ite_table;
	list<string> l_s;
	list<string> l_l;
	string s = "";

	l_l.push_back("");
	int set_length = 1;
	//��ʼ�����һ��
	vector<char>::iterator ite_left;
	ite_left = v_left.begin();
	while (ite_left != v_left.end()) {

		s += *ite_left;
		l_l.push_back(s);
		s = "";
		set_length++;
		ite_left++;
		
	}

	l_table.push_back(l_l);

	for (int i = 0; i < set_length; i++) {
		l_s.push_back("");
	}
	list<string>::iterator ite_l_s;
	ite_l_s = l_s.begin();
	for (ite = s_vt.begin(); ite != s_vt.end(); ite++) {

		*ite_l_s = "";
		*ite_l_s += *ite;
		l_table.push_back(l_s);

	}

	*ite_l_s = "";
	*ite_l_s += "#";
	l_table.push_back(l_s);
}

int loc_x(list<list<string>>::iterator y,string str0) {
	list<string>::iterator x;
	int flag = 0;
	x = (*y).begin();
	while (x != (*y).end()) {
		if (*x == str0) {
			return flag;
		}
		flag++;
		x++;
	}
	return -1;
}

int loc_y(string str0) {
	list<list<string>>::iterator y;
	list<string>::iterator y_y;
	y = l_table.begin();
	y_y = (*y).begin();

	int flag = 0;
	while (y != l_table.end()) {
		if (*y_y == str0) {
			return flag;
		}
		flag++;
		y++;
		if (y != l_table.end()) {
			y_y = (*y).begin();
		}
		else {
			cout << "����ʧ�ܣ�����δ֪�ַ�" << endl;
			exit(-1);
		}
		
	}
	cout << "����ʧ�ܣ�����δ֪�ַ�" << endl;
	exit(-1);
}

void insert_table(int x, int y, string st0) {
	list<list<string>>::iterator ite_y;
	list<string>::iterator ite_x;

	ite_y = l_table.begin();

	for (int i = 0; i < y; i++) {
		ite_y++;
	}

	ite_x = (*ite_y).begin();

	for (int i = 0; i < x; i++) {
		ite_x++;
	}
	*ite_x = "";
	*ite_x += st0;

}

//�ҵ���Ӧ��ĸ��first����
list<set<char>>::iterator loc_first(char ch0) {
	list<string>::iterator ite_grammar;
	set<char> x;
	ite_grammar = l_grammar.begin();
	int loc = 0;
	char pre_ch = ' ';
	while (ite_grammar != l_grammar.end()) {
		if ((*ite_grammar)[0] != ch0 && pre_ch != (*ite_grammar)[0]) {
			pre_ch = (*ite_grammar)[0];
			loc++;
		}
		if ((*ite_grammar)[0] == ch0) {
			break;
		}

		ite_grammar++;
	}

	list<set<char>>::iterator ite_all_first;
	ite_all_first = all_first.begin();

	for (int i = 0; i < loc; i++) {
		ite_all_first++;
	}
	return ite_all_first;
}

//�ж��Ƿ����->@
bool is_null(char ch0) {
	string str = " ";
	str[0] = ch0;
	str += "->@";
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	while (ite_grammar != l_grammar.end()) {
		if (*ite_grammar == str) {
			return true;
		}
		ite_grammar++;
	}
	return false;

}

set<char> firstofright;
//A->�� �� ��!=@ ʱ��first����
void right_first(string str0) {
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	firstofright.clear();
	bool flag0 = false;//�Ƿ�����ս��
	bool flag1 = false;//�Ƿ���ڷ��ս��
	char ch;
	int flag = 0;
	int loc_vt = 0;
	int length = str0.length();
	for (int i = 3; i < length; i++) {

		if (s_vt.count(str0[i]) == 1) {
			flag0 = true;
		}
		if (s_vn.count(str0[i]) == 1) {
			flag1 = true;
		}

	}

	list<set<char>>::iterator ite_first;
	if (flag0 == false) {
		if (flag1 == true) {
			char ch1 = str0[3];
			ite_first = loc_first(str0[3]);
			firstofright.insert((*ite_first).begin(), (*ite_first).end());
			for (int i = 4; i < length; i++) {
				if (is_null(str0[i - 1]) == true) {
					ite_first = loc_first(str0[i]);
					firstofright.insert((*ite_first).begin(), (*ite_first).end());
				}
				else {
					return;
				}
			}
		}
		else {
			cout << "�ķ�����δ֪�ַ�������" << endl;
			exit(-1);
		}
	}

	else if (flag0 == true) {
		if (flag1 == false) {
			firstofright.insert(str0[3]);
			return;
		}
		else {
			
			for (int i = 3; i < length; i++) {
				if (s_vt.count(str0[i]) == 1) {
					loc_vt = i;
					break;
				}
			}

			if (loc_vt == 3) {
				firstofright.insert(str0[3]);
				return;
			}
			else {
					char ch1 = str0[3];
					ite_first = loc_first(str0[3]);
					firstofright.insert((*ite_first).begin(), (*ite_first).end());
					for (int i = 4; i < loc_vt; i++) {
						if (is_null(str0[i - 1]) == true) {
							ite_first = loc_first(str0[i]);
							firstofright.insert((*ite_first).begin(), (*ite_first).end());
						}
						else {
							return;
						}
					}
					firstofright.insert(str0[loc_vt]);
			}
		}
	}

}

void createtable() {
	
	init_table();
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	
	int x, y;
	char ch0;
	char ch1;
	char ch2;
	string str0;
	string str_x;
	string str_y;

	while (ite_grammar != l_grammar.end()) {

		str0 = *ite_grammar;//����ÿһ���ķ�
		str_x = "";
		str_y = "";
		str_x += str0[0];

		if (str0[3] != '@') {
			right_first(str0);
			set<char>::iterator ite_right_first;
			ite_right_first = firstofright.begin();
			while (ite_right_first != firstofright.end()) {
				str_x = "";
				str_x += str0[0];
				if (*ite_right_first != ' ') {
					str_y += *ite_right_first;
					int x = loc_x(l_table.begin(), str_x);
					int y = loc_y(str_y);
					str_x = "";
					str_y = "";
					insert_table(x, y, str0);
				}
				ite_right_first++;
			}
			ite_grammar++;
			continue;
		}
		
		else {
			x = loc_x(l_table.begin(), str_x);
			list<set<char>>::iterator ite_follow = all_follow.begin();
			for (int i = 0; i < x - 1; i++) {
				ite_follow++;
			}
			set<char>::iterator ite_follow_char = (*ite_follow).begin();
			//��ʼ��follow�����뼯�ϵĵ�����
			ite_follow_char++;

			while (ite_follow_char != (*ite_follow).end()) {
				ch2 = *ite_follow_char;
				str_y += ch2;
				y = loc_y(str_y);
				if (y == -1) {
					cout << "�ս��������������" << endl;
					return;
					exit(-1);
				}
				string str_follow = " ";
				str_follow[0] = str0[0];
				str_follow += "->@";
				insert_table(x, y, str_follow);
				str_y = "";
				ite_follow_char++;
				continue;
			}
			ite_grammar++;
		}

		

	}
}

void table_print() {
	cout << "Ԥ����������£�" << endl;
	list<list<string>>::iterator ite0;
	ite0 = l_table.begin();
	for (list<string>::iterator ite1 = (*ite0).begin()++; ite1 != ((*ite0).end()); ite1++) {
		cout << " " << *ite1 << "\t";
	}

	cout << endl;

	for (ite0++; ite0 != l_table.end(); ite0++) {
		for (list<string>::iterator ite1 = (*ite0).begin(); ite1 != ((*ite0).end()); ite1++) {
			if (*ite1 != "") {
				cout.width(9);
				cout << *ite1;
				continue;
			}
			cout.width(8);
			cout << " ";
		}
		cout << endl;
	}
}

string table_analy(char ch0, char ch1) {
	string str0 = "";
	string str1 = "";
	str0 += ch0;
	str1 += ch1;

	int x = loc_x(l_table.begin(), str0);
	int y = loc_y(str1);

	list<list<string>>::iterator ite_y = l_table.begin();
	list<string>::iterator ite_x;

	for (int i = 0; i < y; i++) {
		ite_y++;
	}
	ite_x = (*ite_y).begin();
	for (int i = 0; i < x; i++) {
		ite_x++;
	}

	return *ite_x;

}

void init_stack() {
	list<string>::iterator ite_grammar = l_grammar.begin();
	analy_stack.push('#');
	analy_stack.push((*ite_grammar)[0]);
}

string str_reverse(string str0) {
	int length = str0.length();
	string str_r = str0;
	for (int i = 0; i < str0.length(); i++) {
		str_r[i] = str0[length - 1];
		length--;
	}
	return str_r;
}

void push_stack(string str) {
	
	int length = str.length();
	string str_r;
	str_r = str_reverse(str);

	for (int i = 0; i < length - 3; i++) {
		analy_stack.push(str_r[i]);
	}
}

string str_erase(string str0) {

	int length = str0.length();
	string str_rtn = " ";
	str_rtn[0] = str0[1];
	for (int i = 2; i < length; i++) {
		str_rtn += str0[i];
	}
	return str_rtn;
}

void print_analy(string str0) {
	stack<char> print_stack = analy_stack;
	string str = " ";

	char ch;
	while (!print_stack.empty()) {
		ch = print_stack.top();
		str += ch;
		print_stack.pop();
	}
	str = str_reverse(str);
	
	//���ÿ��Ϊʮλ����������������������룬�����Զ����롣
	cout << setw(10) << str;
	cout << setw(10) << str0;
}

string str_erase_3(string str0) {
	string str = " ";
	str[0] = str0[3];
	int length = str0.length();
	for (int i = 4; i < length; i++) {
		str += str0[i];
	}
	return str;
}

bool stack_analy(string str0) {
	
	cout << "��������������£�" << endl;
	cout << "ջ" << "\t" << "�������Ŵ�" << "\t" << "��һ������" << endl;
	string str = str0;
	char ch_str = str[0];
	init_stack();

	print_analy(str);
	char ch_stack = analy_stack.top();
	analy_stack.pop();

	while (ch_stack != '#') {
		if (s_vt.count(ch_stack) == 1) {
			if (ch_stack == ch_str) {
				char c_first = ch_str;
				str = str_erase(str);
				ch_str = str[0];
				cout  <<"ƥ��ջ���͵�ǰ����" << ch_str << ",�ɹ�" << endl;
			}
			else {
				cout << "�������ַ����г���δ֪���ţ�ʶ��ʧ��" << endl;
				return false;
			}
		}

		if (s_vn.count(ch_stack) == 1) {
			string str_linshi = table_analy(ch_stack, ch_str);
			if (str_linshi == "") {
				cout << "�޷��ҳ��Ƶ��Ĳ���ʽ������ʧ��" << endl;
				return false;
			}
			if (str_linshi[3] == '@') {
				
				cout << "��" << str_linshi[0] << "->@" << endl;
				ch_stack = analy_stack.top();
				print_analy(str);
				analy_stack.pop();
				
				continue;
			}
			else {
				string str_er3 = str_erase_3(str_linshi);
				cout << "��" << str_linshi<< "," << str_er3 << "�����ջ" << endl;
				push_stack(str_linshi);
			}

		}



		print_analy(str);
		ch_stack = analy_stack.top();
		analy_stack.pop();

	}
	if (ch_stack == ch_str && ch_stack == '#') {
		cout << "�����ɹ�!!!" << endl;
		return true;
	}
	return false;
}

int main() {

	list<string>::iterator ite;
	string s_vn, s_vt;
	string analy_str;
	string str_first;
	string str_follow;
	Init_grammar();

	cout.setf(std::ios::left);
	cout << "�ķ���" << endl;
	for (ite = l_grammar.begin(); ite != l_grammar.end(); ite++) {
		cout << *ite << endl;
	}

	cout << "�����ķ����ս�����ÿո�ָ���" << endl;
	getline(cin, s_vt);
	cout << "�����ķ��ķ��ս�����ÿո�ָ���" << endl;
	getline(cin, s_vn);

	createvt(s_vt);
	createvn(s_vn);

	int i = (s_vn.size() + 1) / 2;
	//���漯�ϵĳ���

	list<set<char>>::iterator ite_first;
	list<set<char>>::iterator ite_follow;
	ite_first = all_first.begin();
	ite_follow = all_follow.begin();

	cout << i <<":" << endl;
	cout << "����first���ϣ�����A��b c��" << endl;
	while (i > 0) {
		
		getline(cin, str_first);
		for (int k = 2; k < str_first.size(); k++) {
			if (str_first[k] != ' ') {
				(*ite_first).insert(str_first[k]);
			}
		}
		ite_first++;
		i--;
	}
	cout << "����follow���ϣ�����A��b c��" << endl;

	i = (s_vn.size() + 1) / 2;
	while (i > 0) {
		getline(cin, str_follow);
		for (int k = 2; k < str_follow.size(); k++) {
			if (str_follow[k] != ' ') {
				(*ite_follow).insert(str_follow[k]);	
			}
		}
		ite_follow++;
		i--;
	}

	createtable();
	table_print();

	cout << "����������Ҫ�����ľ��ӣ�";
	string str_analy;
	getline(cin, str_analy);
	cout << endl;
	str_analy += '#';
	stack_analy(str_analy);

	return 0;
}