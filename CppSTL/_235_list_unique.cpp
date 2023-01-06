#include <cstdio>
#include <list>

using namespace std;
// unique() : list�� ������ �ߺ��� ����

bool Predicate(int first, int second)
{
	return second - first <= 0;
}

int main235()
{
	list<int> lst;

	lst.push_back(10);
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(30);
	lst.push_back(30);
	lst.push_back(40);
	lst.push_back(50);
	lst.push_back(10);

	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	//lst.unique();								// �ߺ� ���� (������ �ߺ���)
	lst.unique(Predicate);						// ���̸� ��������
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}