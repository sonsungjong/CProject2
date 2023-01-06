#include <cstdio>
#include <list>

using namespace std;
// sort : ����

int main237()
{
	list<int> lst;

	lst.push_back(20);
	lst.push_back(10);
	lst.push_back(50);
	lst.push_back(30);
	lst.push_back(40);

	// ����
	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// ���� ��������
	lst.sort();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// ���� ��������
	lst.sort(greater<int>());
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}