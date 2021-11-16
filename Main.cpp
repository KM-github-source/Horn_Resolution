#include"head.h"

vector<string> CONSTANT_NAME; //常量名集
vector<string> VARIABLE_NAME; //变量名集
vector<string> PREDICATE_NAME; //谓词名集
vector<Clause> CLAUSE_SET; //子句集
Clause TARGET_CLAUSE; //目标子句

int main()
{
	Solution S;
	status state;
	state = S.Input_All();
	if (state == ERROR) //输入错误
	{
		cout << "输入错误！" << endl;
		return 0;
	}
	cout << "*************************************" << endl;
	state = S.Resolution();
	cout << "*************************************" << endl;
	if (state == CONTRARIETY) //归结出矛盾
	{
		cout << "归结出矛盾，猜测不成立！" << endl;
		return 0;
	}
	else if (state == NORMAL) //归结结束，未归结出矛盾或猜测子句
	{
		cout << "无法归结出猜测子句，猜测不成立！" << endl;
		return 0;
	}
	else //归结出猜测子句
	{
		cout << "归结出猜测子句，关键归结路径如下：" << endl;
		cout << "*************************************" << endl;
		S.Display_Key(CLAUSE_SET.size() - 1);
		S.Draw_Process();
		cout << endl;
	}
	return 0;
}