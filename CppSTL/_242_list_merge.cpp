#include <cstdio>
#include <list>

using namespace std;
// merge : ����Ʈ�� ��ħ

int main242()
{
	list<int> lst1;
	list<int> lst2;

	// ��������
	lst1.push_back(50);
	lst1.push_back(40);
	lst1.push_back(30);
	lst1.push_back(20);
	lst1.push_back(10);

	// �������� - ���Ĺ���� �޶� �պ� �Ұ���
	lst2.push_back(25);
	lst2.push_back(35);
	lst2.push_back(60);

	// ������������ ����
	lst2.sort(greater<int>());

	// ����
	list<int>::iterator lst_iter;
	printf("lst1: ");
	for (lst_iter = lst1.begin(); lst_iter != lst1.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	printf("lst2: ");
	for (lst_iter = lst2.begin(); lst_iter != lst2.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// lst1�� lst2 �պ�
	// �� ����Ʈ�� ���ı����� ����
	lst1.merge(lst2, greater<int>());

	printf("lst1: ");
	for (lst_iter = lst1.begin(); lst_iter != lst1.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	printf("lst2: ");
	for (lst_iter = lst2.begin(); lst_iter != lst2.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}