#pragma once
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define NONE 0
#define RIGHT 1
#define NORMAL 0
#define CONTRARIETY -1
#include <iostream>
#include <vector>
#include <string>
#include<stdlib.h>
#include<iomanip>
#include <map>
#include <fstream>
using namespace std;

typedef int status;

class Solution;
class Clause;

extern vector<string> CONSTANT_NAME; //��������
extern vector<string> VARIABLE_NAME; //��������
extern vector<string> PREDICATE_NAME; //ν������
extern vector<Clause> CLAUSE_SET; //�Ӿ伯
extern Clause TARGET_CLAUSE; //Ŀ���Ӿ�

enum class IDENTIFIER_TYPE {CONSTANT, VARIABLE};
class Identifier //��ʶ��
{
	friend bool operator < (Identifier a, Identifier b);
public:
	bool operator > (Identifier b) { return this->index > b.index; }
	bool operator < (Identifier b) { return this->index < b.index; }
	IDENTIFIER_TYPE type; //��ʶ������
	int index; //�ڳ�������������е��±�
};

class Atom //���Ӿ�
{
public:
	Atom();
	vector<Identifier> Idens; //��ʶ��
	bool positive;
	int index; //��ν�������е��±�
};

class Clause //�Ӿ�
{
	friend class Solution;
	friend ostream& operator <<(ostream& out, Clause& C);
public:
	Clause();
private:
	vector<Atom> Atoms;
	int father; //�Ӿ伯�е��±ָ꣬����Դ�Ӿ䣬û������-1
	int mother; //�Ӿ伯�е��±ָ꣬����Դ�Ӿ䣬û������-1
	bool valid; //��ĳ���Ӿ����뵱ǰ�����Ӿ���й�ᣬ��õ��Ӿ���Ϊ��Ч
	bool key; //�ж��Ƿ�Ϊ�ؼ�·��
};

class Solution
{
	friend ostream& operator <<(ostream& out, Clause& C);
	friend int main();
private:
	status Find_Prediate_Index(string Predicate_Name, int& index);
	status Find_Identifier_Index(string Identifier_Name, Identifier& Pres_ID);
	status Input_C_and_V();
	status Input_Atom(Clause& Present_Clause, string& Clause_str, int Atom_begin, int Atom_end);
	status Input_Clause();
	status Input_Guess();
	status Input_All();
	status Equals(Atom A1, Atom A2, bool Jump_Variable);
	status Equals_Clause(Clause C, int& index);
	status Child(int clause1, int clause2);
	status Delete_Replace(int clause_father, int clause_mother, int delete_atom);
	status Substitute_All(int clause_father);
	status Resolution();
	void Display_Key(int target_clause);
	void Draw_Process();
};