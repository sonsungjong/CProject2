#include <cstdio>
#include <set>

using namespace std;
// count() : 해당 원소의 갯수를 반환
// find() : 해당 원소를 검색하여 그 위치를 가리키는 반복자를 반환

int main262()
{
	set<int> s;

	s.insert(50);
	s.insert(30);
	s.insert(80);
	s.insert(40);
	s.insert(10);
	s.insert(70);
	s.insert(90);

	set<int>::iterator set_iter;

	// count : 갯수
	for (set_iter = s.begin(); set_iter != s.end(); ++set_iter)
	{
		printf("%d ", *set_iter);
	}
	printf("\n");

	// count : 그 원소의 갯수를 반환
	printf("원소 50의 갯수: %lld\n", s.count(50));
	printf("원소 100의 갯수: %lld\n", s.count(100));

	// find : 찾기
	set_iter = s.find(30);					// 30의 반복자를 반환 (반복자를 역참조하여 사용)
	if (set_iter != s.end())
	{
		printf("%d가 s에 있다.\n", *set_iter);
	}
	else {
		printf("해당 값이 s에 없다.\n");
	}

	return 0;
}