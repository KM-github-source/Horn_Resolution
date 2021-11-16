#include"head.h"

status Solution::Child(int clause1, int clause2) //判断clause1是否为clause2归结而来
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
	New_clause.mother = clause_mother; //归结得到新子句，保持双亲指针

	map<Identifier, Identifier> M; //用于实现变量到常量的哈希表

	for (int i = 0;i < CLAUSE_SET[clause_father].Atoms[0].Idens.size();i++)
	{
		if (CLAUSE_SET[clause_mother].Atoms[delete_atom].Idens[i].type == IDENTIFIER_TYPE::VARIABLE) //是变量则哈希
			M[CLAUSE_SET[clause_mother].Atoms[delete_atom].Idens[i]] = CLAUSE_SET[clause_father].Atoms[0].Idens[i];
	}

	New_clause.Atoms = CLAUSE_SET[clause_mother].Atoms;
	New_clause.Atoms.erase(vector<Atom>::iterator(New_clause.Atoms.begin() + delete_atom)); //删除指定单子句

	for (int i = 0;i < New_clause.Atoms.size();i++)
	{
		for (int j = 0;j < New_clause.Atoms[i].Idens.size();j++)
		{
			if (New_clause.Atoms[i].Idens[j].type == IDENTIFIER_TYPE::VARIABLE && M.end() != M.find(New_clause.Atoms[i].Idens[j]))
				New_clause.Atoms[i].Idens[j] = M[New_clause.Atoms[i].Idens[j]]; //对其余单子句进行置换
		}
	}
	int index = -1;
	if (!Equals_Clause(New_clause, index)) //新子句不在知识库中
	{
		CLAUSE_SET.push_back(New_clause); //加入子句集
		cout << "C" << setiosflags(ios::left) << setw(int(2)) << (CLAUSE_SET.size() - 1) << "：C" << setw(int(2)) << New_clause.father << "与" << "C" << setw(int(2)) << New_clause.mother << "归结出  " << New_clause << endl;
	}
	else
		cout << "C" << setiosflags(ios::left) << setw(int(2)) << index << "：C" << setw(int(2)) << New_clause.father << "与" << "C" << setw(int(2)) << New_clause.mother << "归结出  " << New_clause << endl;
	

	if (New_clause.Atoms.size() == 0)
	{
		cout << "归结出矛盾！" << endl;
		return CONTRARIETY;
	}
	else if (New_clause.Atoms.size() == 1 && New_clause.Atoms[0].positive == TARGET_CLAUSE.Atoms[0].positive && Equals(New_clause.Atoms[0], TARGET_CLAUSE.Atoms[0], true))
	{
		cout << "归结出目标子句！" << endl;
		return RIGHT;
	}
	else
		return NORMAL; //若既没有归结出矛盾，也没有归结出结论，则正常返回
}

status Solution::Substitute_All(int clause_father)
{
	status state;
	status isChild;
	for (int i = 0;i < CLAUSE_SET.size();i++)
	{
		isChild = Child(clause_father, i);
		if (CLAUSE_SET[i].valid && (isChild == FALSE)) //father有效且不由i归结得来，则进行归结
		{
			for (int j = 0;j < CLAUSE_SET[i].Atoms.size();j++)
			{
				if (CLAUSE_SET[i].Atoms[j].positive != CLAUSE_SET[clause_father].Atoms[0].positive && Equals(CLAUSE_SET[i].Atoms[j], CLAUSE_SET[clause_father].Atoms[0], true))
				{
					state = Delete_Replace(clause_father, i, j);
					if (state != NORMAL) //归结出矛盾，或归结出结论，则返回
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
	cout << "初始知识库中的子句：" << endl;
	for (int i = 0;i < CLAUSE_SET.size();i++)
		cout << "C" << i << "：" << CLAUSE_SET[i] << endl;
	cout << "*************************************" << endl;
	cout << "归结开始：" << endl;
	for (int i = 0;i < CLAUSE_SET.size();i++)
	{
		if (CLAUSE_SET[i].Atoms.size() == 1) //若为单子句
		{
			CLAUSE_SET[i].valid = false; //单子句置为无效
			state = Substitute_All(i);
			if (state != NORMAL) //归结出矛盾，或归结出结论，则返回
				return state;
		}
	}
	return NORMAL;
}