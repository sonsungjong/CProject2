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
	++lst_iter;						// 30 원소 위치를 가리킴

	lst1.splice(lst_iter, lst2);						// iter가 가리키는 위치에 lst2의 모든 원소를 잘라붙임 (앞쪽에 삽입)

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