#include <cstdio>
#include <set>
using namespace std;

int main257()
{
	set<int> s;				// 기본 정렬 기준 less
	set<int, greater<int>> s2;					// 정렬 기준 greater
	pair<set<int>::iterator, bool> pr;

	s.insert(50);
	s.insert(30);
	s.insert(80);
	s.insert(40);
	s.insert(10);
	s.insert(70);
	pr = s.insert(90);

	set<int>::iterator set_iter;
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	s.insert(pr.first, 85);				// pr.first 위치에 85 삽입
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	printf("정렬 기준 greater<int> 부여\n");
	s2.insert(50);
	s2.insert(30);
	s2.insert(80);
	s2.insert(40);
	s2.insert(10);
	s2.insert(70);
	s2.insert(80);
	set<int, greater<int>>::iterator set_greater_iter;
	for (set_greater_iter = s2.begin(); set_greater_iter != s2.end(); ++set_greater_iter)
	{
		printf("%d ", *set_greater_iter);
	}
	printf("\n");

	return 0;
}