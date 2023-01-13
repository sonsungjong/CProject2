#include <cstdio>
#include <set>

using namespace std;
// multiset이 set과 유일하게 다른 점은 '중복'을 허용한다는 것이다.

int main270()
{
	multiset<int> ms;
	multiset<int>::iterator ms_iter;

	ms.insert(50);
	ms.insert(30);
	ms.insert(80);
	ms.insert(80);				// 중복
	ms.insert(30);				// 중복
	ms.insert(70);
	ms_iter = ms.insert(10);				// 삽입 위치를 가리키는 반복자 반환
	
	printf("iter의 원소: %d\n", *ms_iter);
	for (ms_iter = ms.begin(); ms_iter != ms.end(); ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	return 0;
}
