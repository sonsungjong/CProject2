#include <cstdio>
#include <list>

using namespace std;

int main230()
{
	list<int> lst1;
	list<int> lst2;

	lst1.push_back(10);
	lst1.push_back(20);
	lst1.push_back(30);
	lst1.push_back(40);
	lst1.push_back(50);

	lst2.push_back(100);
	lst2.push_back(200);
	lst2.push_back(300);
	lst2.push_back(400);
	lst2.push_back(500);

	list<int>::iterator lst_iter1;
	list<int>::iterator lst_iter2;

	// ����
	printf("lst1: ");
	for (lst_iter1 = lst1.begin(); lst_iter1 != lst1.end(); ++lst_iter1)
	{
		printf("%d ", *lst_iter1);
	}
	printf("\n");

	printf("lst2: ");
	for (lst_iter2 = lst2.begin(); lst_iter2 != lst2.end(); ++lst_iter2)
	{
		printf("%d ", *lst_iter2);
	}
	printf("\n============\n");

	// splice
	lst_iter1 = lst1.begin();
	lst_iter2 = lst2.begin();
	++lst_iter1;
	++lst_iter1;					// 30 ��ġ
	++lst_iter2;					// 200 ��ġ

	// lst_iter1�� ����Ű�� ��ġ�� lst_iter2�� ����Ű�� ��ġ�� lst2�� ���Ҹ� �߶� ����
	lst1.splice(lst_iter1, lst2, lst_iter2);

	printf("lst1: ");
	for (lst_iter1 = lst1.begin(); lst_iter1 != lst1.end(); ++lst_iter1)
	{
		printf("%d ", *lst_iter1);
	}
	printf("\n");

	printf("lst2: ");
	for (lst_iter2 = lst2.begin(); lst_iter2 != lst2.end(); ++lst_iter2)
	{
		printf("%d ", *lst_iter2);
	}
	printf("\n===========\n");

	// lst1.end() �� ����Ű�� ��ġ�� [lst2.begin(), lst2.end()] �� �߶����
	lst1.splice(lst1.end(), lst2, lst2.begin(), lst2.end());

	printf("lst1: ");
	for (lst_iter1 = lst1.begin(); lst_iter1 != lst1.end(); ++lst_iter1)
	{
		printf("%d ", *lst_iter1);
	}
	printf("\n");

	printf("lst2: ");
	for (lst_iter2 = lst2.begin(); lst_iter2 != lst2.end(); ++lst_iter2)
	{
		printf("%d ", *lst_iter2);
	}
	printf("\n");

	return 0;
}