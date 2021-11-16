#include"head.h"

status Solution::Find_Prediate_Index(string Predicate_Name, int& index)
{
	if (PREDICATE_NAME.size() == 0)
		return NONE;
	vector<string>::iterator IT = find(PREDICATE_NAME.begin(), PREDICATE_NAME.end(), Predicate_Name);
	if (IT == PREDICATE_NAME.end())
		return NONE; //����ʧ�ܣ�����NONE
	else
	{
		index = int(IT - PREDICATE_NAME.begin()); //���ҳɹ������±�ֵ
		return OK; //���ҳɹ�������OK
	}
}

status Solution::Find_Identifier_Index(string Identifier_Name, Identifier& Pres_ID)
{
	//���ҳ�������
	vector<string>::iterator IT = find(CONSTANT_NAME.begin(), CONSTANT_NAME.end(), Identifier_Name);
	if (IT != CONSTANT_NAME.end()) //���ҳɹ�
	{
		Pres_ID.type = IDENTIFIER_TYPE::CONSTANT; //��ʶ��Ϊ����
		Pres_ID.index = int(IT - CONSTANT_NAME.begin()); //���±�
		return OK; //����OK
	}
	//���ұ�������
	IT = find(VARIABLE_NAME.begin(), VARIABLE_NAME.end(), Identifier_Name);
	if (IT != VARIABLE_NAME.end()) //���ҳɹ�
	{
		Pres_ID.type = IDENTIFIER_TYPE::VARIABLE; //��ʶ��Ϊ����
		Pres_ID.index = int(IT - VARIABLE_NAME.begin()); //���±�
		return OK; //����OK
	}
	else
		return NONE;
}

status Solution::Input_C_and_V() //���ⲻʹ�øú���
{
	string VC;
	//���������
	cout << "��������ʹ�õ�ȫ�����������ո�ָ���#��������" << endl;
	cout << "����ʾ�����룺x #" << endl;
	while (true)
	{
		cin >> VC;
		if (VC == "#")
			break;
		VARIABLE_NAME.push_back(VC);
	}
	//���볣����
	cout << "��������ʹ�õ�ȫ�����������ո�ָ���#��������" << endl;
	cout << "����ʾ�����룺A B C #" << endl;
	while (true)
	{
		cin >> VC;
		if (VC == "#")
			break;
		CONSTANT_NAME.push_back(VC);
	}

	if (VARIABLE_NAME.size() == 0 || CONSTANT_NAME.size() == 0) //���������򷵻�ERROR
		return ERROR;
	return OK;
}

status Solution::Input_Atom(Clause& Present_Clause, string& Clause_str, int Atom_begin, int Atom_end)
{
	//�ж��Ƿ�Ϊ������
	Atom Present_Atom;
	if (Clause_str[Atom_begin] == '!')
	{
		Present_Atom.positive = false; //��ʽ
		Atom_begin++;
	}
	//�ж�ν��
	string Predicate_str; //ν���ַ���
	int Predicate_index; //ν���±�
	int Identifier_begin; //��ʶ����ʼλ��
	int i;
	for (i = Atom_begin;i < Atom_end;i++)
	{
		if (Clause_str[i] == '(')
		{
			Predicate_str = Clause_str.substr(Atom_begin, i - Atom_begin);
			if (Find_Prediate_Index(Predicate_str, Predicate_index) != NONE) //���ҳɹ�
				Present_Atom.index = Predicate_index;
			else //�Ҳ������¼���
			{
				PREDICATE_NAME.push_back(Predicate_str);
				Present_Atom.index = PREDICATE_NAME.size() - 1;
			}
			Identifier_begin = i + 1; //��ʶ����ʼλ��
			break;
		}
	}
	if (i == Atom_end)
		return ERROR; //��ʽ���󣬷���ERROR

	//�жϱ�ʶ��
	for (i = Identifier_begin; i < Atom_end;i++)
	{
		if (Clause_str[i] == ')' || Clause_str[i] == ',')
		{
			Identifier ID;
			string ID_str = Clause_str.substr(Identifier_begin, i - Identifier_begin);
			if (!Find_Identifier_Index(ID_str, ID)) //�Ҳ������¼���
			{
				if (ID_str.size() == 1 && (ID_str[0] >= 65) && (ID_str[0] <= 90)) //��д��ĸ������
				{
					CONSTANT_NAME.push_back(ID_str);
					ID.type = IDENTIFIER_TYPE::CONSTANT;
					ID.index = CONSTANT_NAME.size() - 1;
				}
				else if (ID_str.size() == 1 && (ID_str[0] >= 97) && (ID_str[0] <= 122)) //Сд��ĸ������
				{
					VARIABLE_NAME.push_back(ID_str);
					ID.type = IDENTIFIER_TYPE::VARIABLE;
					ID.index = VARIABLE_NAME.size() - 1;
				}
				else
					return ERROR; //������δ�����ʶ��������ERROR
			}
			Present_Atom.Idens.push_back(ID);
			Identifier_begin = i + 1;
		}
	}
	Present_Clause.Atoms.push_back(Present_Atom);
	return OK;
}

status Solution::Input_Clause()
{
	string Clause_str;
	cout << "������Horn�Ӿ�(|�����!����ǣ���д��ĸ��ʾ������Сд��ĸ��ʾ����)���Ե���һ��#��Ϊ��β��" << endl;
	cout << "����ʾ�����룺" << endl;
	cout << "Murder(C,A)|Murder(B,A)|Murder(A,A)" << endl;
	cout << "Hate(x,A)|!Murder(x,A)" << endl;
	cout << "!Hate(C,x)|!Hate(A,x)" << endl;
	cout << "Hate(A,A)" << endl;
	cout << "Hate(A,C)" << endl;
	cout << "Richer(x,A)|Hate(B,x)" << endl;
	cout << "!Richer(x,A)|!Hate(B,x)" << endl;
	cout << "Hate(A,x)|!Hate(B,x)" << endl;
	cout << "!Hate(A,x)|Hate(B,x)" << endl;
	cout << "!Hate(A,A)|!Hate(A,B)|!Hate(A,C)" << endl;
	cout << "!Hate(B,A)|!Hate(B,B)|!Hate(B,C)" << endl;
	cout << "!Hate(C,A)|!Hate(C,B)|!Hate(C,C)" << endl;
	cout << "!Richer(x,A)|!Murder(x,A)" << endl;
	cout << "#" << endl;

	while (true)
	{
		Clause Present_Clause;
		string Clause_str;
		cin >> Clause_str;
		if (Clause_str == "#")
			break; //�������

		int i;
		int Atom_begin = 0;
		int Atom_end = 0;
		for (i = 0;i < Clause_str.size();i++)
		{
			if (Clause_str[i] == '|' || i == Clause_str.size() - 1)
			{
				Atom_end = ((i == Clause_str.size() - 1) ? i + 1 : i);
				if (!Input_Atom(Present_Clause, Clause_str, Atom_begin, Atom_end))
					return ERROR; //���뵥�Ӿ�ʧ�ܣ�����ERROR
				Atom_begin = i + 1;
			}
		}
		CLAUSE_SET.push_back(Present_Clause); //�����Ӿ伯
	}
	return OK;
}

status Solution::Input_Guess() //����Ŀ�굥�Ӿ�
{
	cout << "������²�Ľ�������Ӿ䣩��" << endl;
	cout << "����ʾ����Murder(A,A)" << endl;

	string Clause_str;
	cin >> Clause_str;

	if (!Input_Atom(TARGET_CLAUSE, Clause_str, 0, Clause_str.size()))
		return ERROR; //���뵥�Ӿ�ʧ�ܣ�����ERROR
	return OK;
}

status Solution::Input_All() //�����������
{
	//if (!Input_C_and_V())
		//return ERROR;
	//cout << "*************************************" << endl;
	if (!Input_Clause())
		return ERROR;
	cout << "*************************************" << endl;
	if (!Input_Guess())
		return ERROR;
	cout << "*************************************" << endl;
}