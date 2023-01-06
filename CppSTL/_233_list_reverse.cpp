#include <cstdio>
#include <list>

using namespace std;
// reverse() : list 모든 원소의 순차열을 반대로 뒤집음
// unique() : 인접한 중복원소를 제거

int main233()
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

	// 원본
	list<int>::iterator lst_iter;
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// 뒤집음
	lst.reverse();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	// 중복 제거
	lst.unique();
	for (lst_iter = lst.begin(); lst_iter != lst.end(); ++lst_iter)
	{
		printf("%d ", *lst_iter);
	}
	printf("\n");

	return 0;
}