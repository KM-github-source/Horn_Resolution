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
			out << "!"; //������
		out << PREDICATE_NAME[C.Atoms[i].index] << "("; //���ν���Լ�������
		for (int j = 0;j < C.Atoms[i].Idens.size();j++)
		{
			if (C.Atoms[i].Idens[j].type == IDENTIFIER_TYPE::CONSTANT)
				out << CONSTANT_NAME[C.Atoms[i].Idens[j].index];
			else
				out << VARIABLE_NAME[C.Atoms[i].Idens[j].index];
			if (j == C.Atoms[i].Idens.size() - 1)
				out << ")"; //��ʶ�������������������
			else
				out << ",";
		}
		if (i != C.Atoms.size() - 1)
			out << "|"; //�����е��Ӿ�����������
	}
	return out;
}

bool operator < (Identifier a, Identifier b)
{
	return (a.index < b.index);
}

status Solution::Equals(Atom A1, Atom A2, bool Jump_Variable) //�ж��������Ӿ��Ƿ���������ͱ���������
{
	if (A1.index == A2.index && A1.Idens.size() == A2.Idens.size()) //ν����ͬ�ұ�ʶ��������ͬ
	{
		for (int i = 0;i < A1.Idens.size();i++)
		{
			if (A1.Idens[i].type == IDENTIFIER_TYPE::VARIABLE || A2.Idens[i].type == IDENTIFIER_TYPE::VARIABLE)
			{
				if (Jump_Variable) //��������
					continue;
				else //����������
				{
					if (A1.Idens[i].type == IDENTIFIER_TYPE::VARIABLE && A2.Idens[i].type == IDENTIFIER_TYPE::VARIABLE)
					{ //��Ϊ�������жϱ������Ƿ�һ��
						if (VARIABLE_NAME[A1.Idens[i].index] != VARIABLE_NAME[A2.Idens[i].index])
							return FALSE;
					}
					else //��ʶ�����Ͳ�ͬ��ֱ�ӷ���FALSE
						return FALSE;
				}
			}
			else
			{
				if (CONSTANT_NAME[A1.Idens[i].index] != CONSTANT_NAME[A2.Idens[i].index]) //��������ͬ�򷵻ز�ͬ
					return ERROR;
			}
		}
		return OK; //��ͬ�򷵻�OK
	}
	else
		return ERROR; //����ͬ����ERROR
}

status Solution::Equals_Clause(Clause C, int& index) //��ѯ�Ӿ伯���Ƿ�����C����Ӿ�
{
	for (int k = 0;k < CLAUSE_SET.size();k++)
	{
		if (C.Atoms.size() == CLAUSE_SET[k].Atoms.size()) //�������Ӿ�������
		{
			int i = 0;
			for (i = 0;i < C.Atoms.size();i++)
			{
				if (C.Atoms[i].positive == CLAUSE_SET[k].Atoms[i].positive) //���Ӿ����������
				{
					if (Equals(C.Atoms[i], CLAUSE_SET[k].Atoms[i], false)) //���Ӿ����
						continue;
					else
						break;
				}
				else
					break;
			}
			if (i == C.Atoms.size())
			{
				index = k; //������ͬ�Ӿ���±�
				return TRUE;
			}
		}
	}
	return FALSE;
}