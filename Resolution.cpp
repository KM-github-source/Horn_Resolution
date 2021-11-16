#include"head.h"

status Solution::Child(int clause1, int clause2) //�ж�clause1�Ƿ�Ϊclause2������
{
	if (CLAUSE_SET[clause1].father == -1 || CLAUSE_SET[clause1].mother == -1)
		return FALSE;
	else if (CLAUSE_SET[clause1].father == clause2 || CLAUSE_SET[clause1].mother == clause2)
		return TRUE;
	else
		return (Child(CLAUSE_SET[clause1].father, clause2) || Child(CLAUSE_SET[clause1].mother, clause2));
}

status Solution::Delete_Replace(int clause_father, int clause_mother, int delete_atom)
{
	Clause New_clause;
	New_clause.father = clause_father;
	New_clause.mother = clause_mother; //���õ����Ӿ䣬����˫��ָ��

	map<Identifier, Identifier> M; //����ʵ�ֱ����������Ĺ�ϣ��

	for (int i = 0;i < CLAUSE_SET[clause_father].Atoms[0].Idens.size();i++)
	{
		if (CLAUSE_SET[clause_mother].Atoms[delete_atom].Idens[i].type == IDENTIFIER_TYPE::VARIABLE) //�Ǳ������ϣ
			M[CLAUSE_SET[clause_mother].Atoms[delete_atom].Idens[i]] = CLAUSE_SET[clause_father].Atoms[0].Idens[i];
	}

	New_clause.Atoms = CLAUSE_SET[clause_mother].Atoms;
	New_clause.Atoms.erase(vector<Atom>::iterator(New_clause.Atoms.begin() + delete_atom)); //ɾ��ָ�����Ӿ�

	for (int i = 0;i < New_clause.Atoms.size();i++)
	{
		for (int j = 0;j < New_clause.Atoms[i].Idens.size();j++)
		{
			if (New_clause.Atoms[i].Idens[j].type == IDENTIFIER_TYPE::VARIABLE && M.end() != M.find(New_clause.Atoms[i].Idens[j]))
				New_clause.Atoms[i].Idens[j] = M[New_clause.Atoms[i].Idens[j]]; //�����൥�Ӿ�����û�
		}
	}
	int index = -1;
	if (!Equals_Clause(New_clause, index)) //���Ӿ䲻��֪ʶ����
	{
		CLAUSE_SET.push_back(New_clause); //�����Ӿ伯
		cout << "C" << setiosflags(ios::left) << setw(int(2)) << (CLAUSE_SET.size() - 1) << "��C" << setw(int(2)) << New_clause.father << "��" << "C" << setw(int(2)) << New_clause.mother << "����  " << New_clause << endl;
	}
	else
		cout << "C" << setiosflags(ios::left) << setw(int(2)) << index << "��C" << setw(int(2)) << New_clause.father << "��" << "C" << setw(int(2)) << New_clause.mother << "����  " << New_clause << endl;
	

	if (New_clause.Atoms.size() == 0)
	{
		cout << "����ì�ܣ�" << endl;
		return CONTRARIETY;
	}
	else if (New_clause.Atoms.size() == 1 && New_clause.Atoms[0].positive == TARGET_CLAUSE.Atoms[0].positive && Equals(New_clause.Atoms[0], TARGET_CLAUSE.Atoms[0], true))
	{
		cout << "����Ŀ���Ӿ䣡" << endl;
		return RIGHT;
	}
	else
		return NORMAL; //����û�й���ì�ܣ�Ҳû�й������ۣ�����������
}

status Solution::Substitute_All(int clause_father)
{
	status state;
	status isChild;
	for (int i = 0;i < CLAUSE_SET.size();i++)
	{
		isChild = Child(clause_father, i);
		if (CLAUSE_SET[i].valid && (isChild == FALSE)) //father��Ч�Ҳ���i������������й��
		{
			for (int j = 0;j < CLAUSE_SET[i].Atoms.size();j++)
			{
				if (CLAUSE_SET[i].Atoms[j].positive != CLAUSE_SET[clause_father].Atoms[0].positive && Equals(CLAUSE_SET[i].Atoms[j], CLAUSE_SET[clause_father].Atoms[0], true))
				{
					state = Delete_Replace(clause_father, i, j);
					if (state != NORMAL) //����ì�ܣ���������ۣ��򷵻�
						return state;
				}
			}
		}
	}
	return NORMAL;
}

status Solution::Resolution()
{
	status state;
	cout << "��ʼ֪ʶ���е��Ӿ䣺" << endl;
	for (int i = 0;i < CLAUSE_SET.size();i++)
		cout << "C" << i << "��" << CLAUSE_SET[i] << endl;
	cout << "*************************************" << endl;
	cout << "��Ὺʼ��" << endl;
	for (int i = 0;i < CLAUSE_SET.size();i++)
	{
		if (CLAUSE_SET[i].Atoms.size() == 1) //��Ϊ���Ӿ�
		{
			CLAUSE_SET[i].valid = false; //���Ӿ���Ϊ��Ч
			state = Substitute_All(i);
			if (state != NORMAL) //����ì�ܣ���������ۣ��򷵻�
				return state;
		}
	}
	return NORMAL;
}