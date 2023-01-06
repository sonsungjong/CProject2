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

	// 원본
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
	++lst_iter1;					// 30 위치
	++lst_iter2;					// 200 위치

	// lst_iter1이 가리키는 위치에 lst_iter2가 가리키는 위치의 lst2의 원소를 잘라 붙임
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

	// lst1.end() 가 가리키는 위치에 [lst2.begin(), lst2.end()] 를 잘라붙임
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