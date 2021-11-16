#include"head.h"

void Solution::Display_Key(int target_clause) //递归输出关键归结路径
{
	CLAUSE_SET[target_clause].key = true;
	if (CLAUSE_SET[target_clause].father == -1 || CLAUSE_SET[target_clause].mother == -1)
		return;
	Display_Key(CLAUSE_SET[target_clause].father); //输出前面的归结过程
	Display_Key(CLAUSE_SET[target_clause].mother); //输出前面的归结过程
	//输出最后一步
	cout << CLAUSE_SET[CLAUSE_SET[target_clause].father] << " 与    " << CLAUSE_SET[CLAUSE_SET[target_clause].mother] << " 归结，得到：" << CLAUSE_SET[target_clause] << endl;
	return;
}

void Solution::Draw_Process()
{
	fstream out;
	out.open("graph.dot", ios::out);
	out << "digraph G{ " << endl << "rankdir=LR;" << endl;
	for (int i = 0; i < CLAUSE_SET.size(); i++) 
	{
		out << "node" << i << "[label=\"" << i << ":" << CLAUSE_SET[i] << "\"";
		if (CLAUSE_SET[i].key)
			out << ",color=red";
		out << "];" << endl;
		if (CLAUSE_SET[i].father != -1 && CLAUSE_SET[i].mother != -1) {
			out << "node" << CLAUSE_SET[i].father << " -> node" << i;
			if (CLAUSE_SET[CLAUSE_SET[i].father].key && CLAUSE_SET[i].key)
				out << "[color=red]";
			out << ";" << endl;
			out << "node" << CLAUSE_SET[i].mother << " -> node" << i;
			if (CLAUSE_SET[CLAUSE_SET[i].mother].key && CLAUSE_SET[i].key)
				out << "[color=red]";
			out << ";" << endl;
		}
	}
	out << "}" << endl;

	system("dot -T png graph.dot -o tree.png");
	system("tree.png");

	return;
}