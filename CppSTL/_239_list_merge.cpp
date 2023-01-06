#include <cstdio>
#include <list>

using namespace std;
// merge : 정렬된 두 개의 list를 합병

int main239()
{
	list<int> lst1;
	list<int> lst2;

	lst1.push_back(10);
	lst1.push_back(20);
	lst1.push_back(30);
	lst1.push_back(40);
	lst1.push_back(50);

	lst2.push_back(25);
	lst2.push_back(35);
	lst2.push_back(60);

	// 원본
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

	// 두 리스트 합병
	lst1.merge(lst2);
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