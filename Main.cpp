#include"head.h"

vector<string> CONSTANT_NAME; //��������
vector<string> VARIABLE_NAME; //��������
vector<string> PREDICATE_NAME; //ν������
vector<Clause> CLAUSE_SET; //�Ӿ伯
Clause TARGET_CLAUSE; //Ŀ���Ӿ�

int main()
{
	Solution S;
	status state;
	state = S.Input_All();
	if (state == ERROR) //�������
	{
		cout << "�������" << endl;
		return 0;
	}
	cout << "*************************************" << endl;
	state = S.Resolution();
	cout << "*************************************" << endl;
	if (state == CONTRARIETY) //����ì��
	{
		cout << "����ì�ܣ��²ⲻ������" << endl;
		return 0;
	}
	else if (state == NORMAL) //��������δ����ì�ܻ�²��Ӿ�
	{
		cout << "�޷������²��Ӿ䣬�²ⲻ������" << endl;
		return 0;
	}
	else //�����²��Ӿ�
	{
		cout << "�����²��Ӿ䣬�ؼ����·�����£�" << endl;
		cout << "*************************************" << endl;
		S.Display_Key(CLAUSE_SET.size() - 1);
		S.Draw_Process();
		cout << endl;
	}
	return 0;
}