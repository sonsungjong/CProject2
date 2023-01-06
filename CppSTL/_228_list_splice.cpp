#include <cstdio>
#include <list>

using namespace std;

int main228()
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
	printf("\n=============\n");

	lst_iter = lst1.begin();
	++lst_iter;
	++lst_iter;						// 30 ���� ��ġ�� ����Ŵ

	lst1.splice(lst_iter, lst2);						// iter�� ����Ű�� ��ġ�� lst2�� ��� ���Ҹ� �߶���� (���ʿ� ����)

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