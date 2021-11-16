#include"head.h"

status Solution::Find_Prediate_Index(string Predicate_Name, int& index)
{
	if (PREDICATE_NAME.size() == 0)
		return NONE;
	vector<string>::iterator IT = find(PREDICATE_NAME.begin(), PREDICATE_NAME.end(), Predicate_Name);
	if (IT == PREDICATE_NAME.end())
		return NONE; //查找失败，返回NONE
	else
	{
		index = int(IT - PREDICATE_NAME.begin()); //查找成功，赋下标值
		return OK; //查找成功，返回OK
	}
}

status Solution::Find_Identifier_Index(string Identifier_Name, Identifier& Pres_ID)
{
	//查找常量名集
	vector<string>::iterator IT = find(CONSTANT_NAME.begin(), CONSTANT_NAME.end(), Identifier_Name);
	if (IT != CONSTANT_NAME.end()) //查找成功
	{
		Pres_ID.type = IDENTIFIER_TYPE::CONSTANT; //标识符为常量
		Pres_ID.index = int(IT - CONSTANT_NAME.begin()); //赋下标
		return OK; //返回OK
	}
	//查找变量名集
	IT = find(VARIABLE_NAME.begin(), VARIABLE_NAME.end(), Identifier_Name);
	if (IT != VARIABLE_NAME.end()) //查找成功
	{
		Pres_ID.type = IDENTIFIER_TYPE::VARIABLE; //标识符为常量
		Pres_ID.index = int(IT - VARIABLE_NAME.begin()); //赋下标
		return OK; //返回OK
	}
	else
		return NONE;
}

status Solution::Input_C_and_V() //本题不使用该函数
{
	string VC;
	//输入变量名
	cout << "请输入所使用的全部变量名（空格分隔，#结束）：" << endl;
	cout << "本题示例输入：x #" << endl;
	while (true)
	{
		cin >> VC;
		if (VC == "#")
			break;
		VARIABLE_NAME.push_back(VC);
	}
	//输入常量名
	cout << "请输入所使用的全部常量名（空格分隔，#结束）：" << endl;
	cout << "本题示例输入：A B C #" << endl;
	while (true)
	{
		cin >> VC;
		if (VC == "#")
			break;
		CONSTANT_NAME.push_back(VC);
	}

	if (VARIABLE_NAME.size() == 0 || CONSTANT_NAME.size() == 0) //若无输入则返回ERROR
		return ERROR;
	return OK;
}

status Solution::Input_Atom(Clause& Present_Clause, string& Clause_str, int Atom_begin, int Atom_end)
{
	//判断是否为正文字
	Atom Present_Atom;
	if (Clause_str[Atom_begin] == '!')
	{
		Present_Atom.positive = false; //否定式
		Atom_begin++;
	}
	//判断谓词
	string Predicate_str; //谓词字符串
	int Predicate_index; //谓词下标
	int Identifier_begin; //标识符起始位置
	int i;
	for (i = Atom_begin;i < Atom_end;i++)
	{
		if (Clause_str[i] == '(')
		{
			Predicate_str = Clause_str.substr(Atom_begin, i - Atom_begin);
			if (Find_Prediate_Index(Predicate_str, Predicate_index) != NONE) //查找成功
				Present_Atom.index = Predicate_index;
			else //找不到则新加入
			{
				PREDICATE_NAME.push_back(Predicate_str);
				Present_Atom.index = PREDICATE_NAME.size() - 1;
			}
			Identifier_begin = i + 1; //标识符起始位置
			break;
		}
	}
	if (i == Atom_end)
		return ERROR; //格式错误，返回ERROR

	//判断标识符
	for (i = Identifier_begin; i < Atom_end;i++)
	{
		if (Clause_str[i] == ')' || Clause_str[i] == ',')
		{
			Identifier ID;
			string ID_str = Clause_str.substr(Identifier_begin, i - Identifier_begin);
			if (!Find_Identifier_Index(ID_str, ID)) //找不到则新加入
			{
				if (ID_str.size() == 1 && (ID_str[0] >= 65) && (ID_str[0] <= 90)) //大写字母，常量
				{
					CONSTANT_NAME.push_back(ID_str);
					ID.type = IDENTIFIER_TYPE::CONSTANT;
					ID.index = CONSTANT_NAME.size() - 1;
				}
				else if (ID_str.size() == 1 && (ID_str[0] >= 97) && (ID_str[0] <= 122)) //小写字母，变量
				{
					VARIABLE_NAME.push_back(ID_str);
					ID.type = IDENTIFIER_TYPE::VARIABLE;
					ID.index = VARIABLE_NAME.size() - 1;
				}
				else
					return ERROR; //出现了未定义标识符，返回ERROR
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
	cout << "请输入Horn子句(|代表或，!代表非，大写字母表示常量，小写字母表示变量)，以单独一行#作为结尾：" << endl;
	cout << "本题示例输入：" << endl;
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
			break; //输入结束

		int i;
		int Atom_begin = 0;
		int Atom_end = 0;
		for (i = 0;i < Clause_str.size();i++)
		{
			if (Clause_str[i] == '|' || i == Clause_str.size() - 1)
			{
				Atom_end = ((i == Clause_str.size() - 1) ? i + 1 : i);
				if (!Input_Atom(Present_Clause, Clause_str, Atom_begin, Atom_end))
					return ERROR; //输入单子句失败，返回ERROR
				Atom_begin = i + 1;
			}
		}
		CLAUSE_SET.push_back(Present_Clause); //加入子句集
	}
	return OK;
}

status Solution::Input_Guess() //输入目标单子句
{
	cout << "请输入猜测的结果（单子句）：" << endl;
	cout << "本题示例：Murder(A,A)" << endl;

	string Clause_str;
	cin >> Clause_str;

	if (!Input_Atom(TARGET_CLAUSE, Clause_str, 0, Clause_str.size()))
		return ERROR; //输入单子句失败，返回ERROR
	return OK;
}

status Solution::Input_All() //完成所有输入
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