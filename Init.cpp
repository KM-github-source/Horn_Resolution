#include"head.h"

Atom::Atom()
{
	this->index = -1;
	this->positive = true;
}

Clause::Clause()
{
	this->father = -1;
	this->mother = -1;
	this->key = false;
	this->valid = true;
}

ostream& operator << (ostream& out, Clause& C)
{
	for (int i = 0;i < C.Atoms.size();i++)
	{
		if (!C.Atoms[i].positive)
			out << "!"; //负文字
		out << PREDICATE_NAME[C.Atoms[i].index] << "("; //输出谓词以及左括号
		for (int j = 0;j < C.Atoms[i].Idens.size();j++)
		{
			if (C.Atoms[i].Idens[j].type == IDENTIFIER_TYPE::CONSTANT)
				out << CONSTANT_NAME[C.Atoms[i].Idens[j].index];
			else
				out << VARIABLE_NAME[C.Atoms[i].Idens[j].index];
			if (j == C.Atoms[i].Idens.size() - 1)
				out << ")"; //标识符输出完毕则输出右括号
			else
				out << ",";
		}
		if (i != C.Atoms.size() - 1)
			out << "|"; //若还有单子句则输入或符号
	}
	return out;
}

bool operator < (Identifier a, Identifier b)
{
	return (a.index < b.index);
}

status Solution::Equals(Atom A1, Atom A2, bool Jump_Variable) //判断两个单子句是否除了正负和变量外均相等
{
	if (A1.index == A2.index && A1.Idens.size() == A2.Idens.size()) //谓词相同且标识符个数相同
	{
		for (int i = 0;i < A1.Idens.size();i++)
		{
			if (A1.Idens[i].type == IDENTIFIER_TYPE::VARIABLE || A2.Idens[i].type == IDENTIFIER_TYPE::VARIABLE)
			{
				if (Jump_Variable) //跳过变量
					continue;
				else //不跳过变量
				{
					if (A1.Idens[i].type == IDENTIFIER_TYPE::VARIABLE && A2.Idens[i].type == IDENTIFIER_TYPE::VARIABLE)
					{ //均为变量则判断变量名是否一致
						if (VARIABLE_NAME[A1.Idens[i].index] != VARIABLE_NAME[A2.Idens[i].index])
							return FALSE;
					}
					else //标识符类型不同，直接返回FALSE
						return FALSE;
				}
			}
			else
			{
				if (CONSTANT_NAME[A1.Idens[i].index] != CONSTANT_NAME[A2.Idens[i].index]) //常量名不同则返回不同
					return ERROR;
			}
		}
		return OK; //相同则返回OK
	}
	else
		return ERROR; //不相同返回ERROR
}

status Solution::Equals_Clause(Clause C, int& index) //查询子句集中是否已有C这个子句
{
	for (int k = 0;k < CLAUSE_SET.size();k++)
	{
		if (C.Atoms.size() == CLAUSE_SET[k].Atoms.size()) //包含单子句个数相等
		{
			int i = 0;
			for (i = 0;i < C.Atoms.size();i++)
			{
				if (C.Atoms[i].positive == CLAUSE_SET[k].Atoms[i].positive) //单子句正负性相等
				{
					if (Equals(C.Atoms[i], CLAUSE_SET[k].Atoms[i], false)) //单子句相等
						continue;
					else
						break;
				}
				else
					break;
			}
			if (i == C.Atoms.size())
			{
				index = k; //返回相同子句的下标
				return TRUE;
			}
		}
	}
	return FALSE;
}