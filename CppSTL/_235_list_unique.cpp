#include <cstdio>
#include <list>

using namespace std;
// unique() : list의 인접한 중복값 제거

bool Predicate(int first, int second)
{
	return second - first <= 0;
}

int main235()
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

	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	//lst.unique();								// 중복 제거 (인접한 중복만)
	lst.unique(Predicate);						// 참이면 원소제거
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}