#include <cstdio>
#include <list>

using namespace std;
// merge : 리스트를 합침

int main242()
{
	list<int> lst1;
	list<int> lst2;

	// 내림차순
	lst1.push_back(50);
	lst1.push_back(40);
	lst1.push_back(30);
	lst1.push_back(20);
	lst1.push_back(10);

	// 오름차순 - 정렬방식이 달라 합병 불가능
	lst2.push_back(25);
	lst2.push_back(35);
	lst2.push_back(60);

	// 내림차순으로 정렬
	lst2.sort(greater<int>());

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

	// lst1과 lst2 합병
	// 두 리스트의 정렬기준을 지정
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