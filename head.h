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

extern vector<string> CONSTANT_NAME; //常量名集
extern vector<string> VARIABLE_NAME; //变量名集
extern vector<string> PREDICATE_NAME; //谓词名集
extern vector<Clause> CLAUSE_SET; //子句集
extern Clause TARGET_CLAUSE; //目标子句

enum class IDENTIFIER_TYPE {CONSTANT, VARIABLE};
class Identifier //标识符
{
	friend bool operator < (Identifier a, Identifier b);
public:
	bool operator > (Identifier b) { return this->index > b.index; }
	bool operator < (Identifier b) { return this->index < b.index; }
	IDENTIFIER_TYPE type; //标识符类型
	int index; //在常量或变量名集中的下标
};

class Atom //单子句
{
public:
	Atom();
	vector<Identifier> Idens; //标识符
	bool positive;
	int index; //在谓词名集中的下标
};

class Clause //子句
{
	friend class Solution;
	friend ostream& operator <<(ostream& out, Clause& C);
public:
	Clause();
private:
	vector<Atom> Atoms;
	int father; //子句集中的下标，指向归结源子句，没有则置-1
	int mother; //子句集中的下标，指向归结源子句，没有则置-1
	bool valid; //若某单子句已与当前所有子句进行归结，则该单子句置为无效
	bool key; //判断是否为关键路径
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