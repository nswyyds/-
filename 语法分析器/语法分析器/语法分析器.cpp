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
//语法链表

set<char> s_vn;
set<char> s_vt;
//终结符与非终结符集合

list<set<char>> all_first;
list<set<char>> all_follow;
//存储每一个的first与follow

vector<char> v_left;
//存储产生式左部
list<list<string>> l_table;
//预测分析表
void Init_grammar();//特殊处理每一句语法
void creategrammar(string str);//创建语法链表
void createvn(string s0);//存储非终结符号
void createvt(string s0);//存储终结符号
void createv(string, set<char>);
void init_table();//初始化预测分析表
int loc_x(list<list<string>>::iterator y, string str0);//求字符在预测分析表中横坐标
int loc_y(string str0);//求字符在预测分析表中纵坐标
void insert_table(int x, int y, string st0);//在预测分析表对应位置插入元素
void createtable();//创建预测分析表
void table_print();//打印预测分析表
string table_analy(char ch0, char ch1);//取出分析表中对应产生式
void init_stack();//初始化栈
string str_reverse(string str0);//字符串反转
void push_stack(string str);//产生式右部反转后入栈
string str_erase(string);//去除字符串头部
void print_analy(string str0);//打印此时栈与余留字符串
string str_erase_3(string str0);//得到产生式右部
bool stack_analy(string str0);//分析栈分析过程

void Init_grammar() {
	ifstream ifs;
	string str;
	ifs.open("data.txt", ios::in);

	if (!ifs.is_open()) {
		cout << "文件打开失败" << endl;
	}

	while (getline(ifs, str)) {
		creategrammar(str);//将产生式添加到链表中
		str = "";
	}

	ifs.close();
}

void creategrammar(string str) {

	int flag0 = 3;
	int flag1 = 3;
	int flag2 = 0;
	//三个标志位保存产生式右部的开始，|符号的位置

	int length = str.length();
	char s = str[0];
	string linshi = " ";
	vector<char> v;
	set<char> s_ch;

	s_ch.insert(' ');
	v_left.push_back(s);
	all_first.push_back(s_ch);
	all_follow.push_back(s_ch);
	//为各表提前分配一个空间防止迭代器指针越界

	
	linshi[0] = s;
	linshi += "->";
	//保留产生式左部

	for (int i = 0; i < length; i++) {
		if (str[i] == '|') { //拆分带有|的产生式
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
	//初始化表第一行
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
			cout << "分析失败，出现未知字符" << endl;
			exit(-1);
		}
		
	}
	cout << "分析失败，出现未知字符" << endl;
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

//找到对应字母的first集合
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

//判断是否存在->@
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
//A->α 且 α!=@ 时求first集合
void right_first(string str0) {
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	firstofright.clear();
	bool flag0 = false;//是否存在终结符
	bool flag1 = false;//是否存在非终结符
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
			cout << "文法遇到未知字符，请检查" << endl;
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

		str0 = *ite_grammar;//保存每一句文法
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
			//初始化follow链表与集合的迭代器
			ite_follow_char++;

			while (ite_follow_char != (*ite_follow).end()) {
				ch2 = *ite_follow_char;
				str_y += ch2;
				y = loc_y(str_y);
				if (y == -1) {
					cout << "终结符输入有误，请检查" << endl;
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
	cout << "预测分析表如下：" << endl;
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
	
	//设置宽度为十位，且主函数中设置了左对齐，不够自动补齐。
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
	
	cout << "具体分析过程如下：" << endl;
	cout << "栈" << "\t" << "余留符号串" << "\t" << "下一步动作" << endl;
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
				cout  <<"匹配栈顶和当前符号" << ch_str << ",成功" << endl;
			}
			else {
				cout << "待分析字符串中出现未知符号，识别失败" << endl;
				return false;
			}
		}

		if (s_vn.count(ch_stack) == 1) {
			string str_linshi = table_analy(ch_stack, ch_str);
			if (str_linshi == "") {
				cout << "无法找出推导的产生式，分析失败" << endl;
				return false;
			}
			if (str_linshi[3] == '@') {
				
				cout << "用" << str_linshi[0] << "->@" << endl;
				ch_stack = analy_stack.top();
				print_analy(str);
				analy_stack.pop();
				
				continue;
			}
			else {
				string str_er3 = str_erase_3(str_linshi);
				cout << "用" << str_linshi<< "," << str_er3 << "逆序进栈" << endl;
				push_stack(str_linshi);
			}

		}



		print_analy(str);
		ch_stack = analy_stack.top();
		analy_stack.pop();

	}
	if (ch_stack == ch_str && ch_stack == '#') {
		cout << "分析成功!!!" << endl;
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
	cout << "文法：" << endl;
	for (ite = l_grammar.begin(); ite != l_grammar.end(); ite++) {
		cout << *ite << endl;
	}

	cout << "输入文法的终结符（用空格分隔）" << endl;
	getline(cin, s_vt);
	cout << "输入文法的非终结符（用空格分隔）" << endl;
	getline(cin, s_vn);

	createvt(s_vt);
	createvn(s_vn);

	int i = (s_vn.size() + 1) / 2;
	//保存集合的长度

	list<set<char>>::iterator ite_first;
	list<set<char>>::iterator ite_follow;
	ite_first = all_first.begin();
	ite_follow = all_follow.begin();

	cout << i <<":" << endl;
	cout << "输入first集合（形如A：b c）" << endl;
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
	cout << "输入follow集合（形如A：b c）" << endl;

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

	cout << "请输入您需要分析的句子：";
	string str_analy;
	getline(cin, str_analy);
	cout << endl;
	str_analy += '#';
	stack_analy(str_analy);

	return 0;
}