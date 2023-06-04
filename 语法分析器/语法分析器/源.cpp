#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<set>
#include<list>
#include<cstring>
#include<fstream>
#include<sstream>
//1.��ȡ�ķ�������洢
//2.��Ӧ�ķ���first��follow��
using namespace std;

set<char> nfirst;
set<char> follow;


list<string> l_grammar;

set<char> s_vn;
set<char> s_vt;

list<set<char>> all_first;
list<set<char>> all_follow;
//�洢ÿһ����first��follow

vector<char> v_left;
//�洢����ʽ��

void Init_grammar();
void creategrammar(string str);
void createv(string, set<char>);
void analy_first_onegrammar(char, string);
bool is_vt(char);
void create_all_first(char, char);
int loc_left(char s);

void Init_grammar() {
	ifstream ifs;
	string str;
	ifs.open("data.txt", ios::in);

	if (!ifs.is_open()) {
		cout << "�ļ���ʧ��" << endl;
	}

	while (getline(ifs, str)) {

		creategrammar(str);
		str = "";
	}

	ifs.close();

}

//���⴦��ÿһ���﷨
void creategrammar(string str) {
	int length = str.length();
	vector<char> v;
	set<char> s_ch;
	s_ch.insert('#');
	char s = str[0];


	v_left.push_back(s);
	all_first.push_back(s_ch);
	int flag0 = 3;
	int flag1 = 3;
	int flag2 = 0;
	string linshi = " ";
	linshi[0] = s;
	linshi += "->";
	//��������ʽ��

	for (int i = 0; i < length; i++) {
		if (str[i] == '|') {
			flag2 = 1;
			flag1 = i;
			for (int j = flag0; j < flag1; j++) {
				linshi += str[j];
			}
			l_grammar.push_back(linshi);
			linshi = " ";
			linshi[0] = s;
			linshi += "->";
			flag0 = flag1;
		}
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

//�洢����
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

void createfirst() {

	char s;
	char cmp;
	string one_str;
	list<string>::iterator ite_grammar;

	ite_grammar = l_grammar.begin();
	while (ite_grammar != l_grammar.end()) {
		analy_first_onegrammar((*ite_grammar)[0],*ite_grammar);
		ite_grammar++;
	}
}

void analy_first_onegrammar(char s, string str0) {
	char cmp;
	for (int i = 0; i < str0.length(); i++) {
		cmp = str0[i];
		if (is_vt(cmp) == 1) {
			create_all_first(s, cmp);
			return;
		}
	}
}

bool is_vt(char x) {

	if (s_vt.count(x) == 1) {
		return true;
	}
	return false;
}

bool is_vn(char x) {

	if (s_vn.count(x) == 1) {
		return true;
	}
	return false;
}

void create_all_first(char s, char ch) {

	int flag;
	flag = loc_left(s);
	list<set<char>>::iterator ite_all_first;
	ite_all_first = all_first.begin();

	for(int i = 0; i < flag; i++){
		ite_all_first++;
	}
	(*ite_all_first).insert(ch);
}

int loc_left(char s) {
	int flag = 0;
	vector<char>::iterator ite_left;

	ite_left = v_left.begin();
	//�ҵ���Ӧ�Ĳ���ʽ��λ��
	while (ite_left != v_left.end()) {

		if (s == *ite_left) {
			return flag;
		}
		flag++;
		ite_left++;
	}
}

//����Ƿ���ڲ���ʽs->@
bool checknull(char s) {
	string str0;
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	str0[0] = s;
	str0 += "->@";
	while (ite_grammar != l_grammar.end()) {
		if (str0 == *ite_grammar) {
			return true;
		}
		ite_grammar++;
	}
	return false;

}

void addset(char s1, char s2) {
	int loc1 = loc_left(s1);
	int loc2 = loc_left(s2);
	
	list<set<char>>::iterator ite0;
	list<set<char>>::iterator ite1;
	ite0 = all_first.begin();
	ite1 = all_first.begin();

	for (int i = 0; i < loc1; i++) {
		ite0++;
	}
	for (int i = 0; i < loc2; i++) {
		ite1++;
	}

	(*ite0).insert((*ite1).begin(), (*ite1).end());
}

void checkfirst() {
	list<string>::iterator ite_grammar;
	ite_grammar = l_grammar.begin();
	string str;
	char s;
	int flag;
	while (ite_grammar != l_grammar.end()) {
		str = *ite_grammar;
		s = str[0];
		for (int i = 3; i < str.length(); i++) {
			if (is_vn(str[i])) {
				if (true == checknull(s)) {
					addset(s, str[i]);
					continue;

				}
				else {
					addset(s, str[i]);
					break;
				}
				
			}
			else{
				break;
			}
		}

		ite_grammar++;
	}


}



int main() {

	list<string>::iterator ite;
	string s_vn, s_vt;
	string analy_str;

	Init_grammar();
	for (ite = l_grammar.begin(); ite != l_grammar.end(); ite++) {
		cout << *ite << endl;
	}



	cout << "�����ķ����ս�����ÿո�ָ���" << endl;
	getline(cin, s_vt);
	cout << "�����ķ��ķ��ս�����ÿո�ָ���" << endl;
	getline(cin, s_vn);

	createvt(s_vt);
	createvn(s_vn);

	//cout << "������������ַ���" << endl;
	//getline(cin, analy_str);

	createfirst();
	checkfirst();
	list<set<char>>::iterator ite0;
	for (ite0 = all_first.begin(); ite0 != all_first.end(); ite0++) {
		for (set<char>::iterator ite1 = (*ite0).begin(); ite1 != ((*ite0).end()); ite1++) {
			cout << *ite1 << endl;
		}
		
	}
	

	return 0;
}