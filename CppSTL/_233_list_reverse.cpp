#include <cstdio>
#include <list>

using namespace std;
// reverse() : list ��� ������ �������� �ݴ�� ������
// unique() : ������ �ߺ����Ҹ� ����

int main233()
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

	// ����
	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// ������
	lst.reverse();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// �ߺ� ����
	lst.unique();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}