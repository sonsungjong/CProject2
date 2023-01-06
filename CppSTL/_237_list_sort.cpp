#include <cstdio>
#include <list>

using namespace std;
// sort : 정렬

int main237()
{
	list<int> lst;

	lst.push_back(20);
	lst.push_back(10);
	lst.push_back(50);
	lst.push_back(30);
	lst.push_back(40);

	// 원본
	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// 정렬 오름차순
	lst.sort();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// 정렬 내림차순
	lst.sort(greater<int>());
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}