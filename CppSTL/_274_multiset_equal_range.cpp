#include <cstdio>
#include <set>
using namespace std;

int main274()
{
	multiset<int> ms;
	ms.insert(50);
	ms.insert(30);
	ms.insert(80);
	ms.insert(80);
	ms.insert(30);
	ms.insert(70);
	ms.insert(10);
	
	multiset<int>::iterator ms_iter;
	for (ms_iter = ms.begin(); ms_iter != ms.end(); ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	// multiset의 반복자 쌍(pair) 객체 생성
	pair<multiset<int>::iterator, multiset<int>::iterator> ms_iter_pair;
	ms_iter_pair = ms.equal_range(30);				// 30 찾기

	for (ms_iter = ms_iter_pair.first; ms_iter != ms_iter_pair.second; ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	if (ms_iter_pair.first == ms_iter_pair.second)
	{
		// lower_bound 와 upper_bound가 같다면 찾는 원소가 없음
		printf("찾는 원소가 없습니다. \n");
	}
	else if (ms_iter_pair.first != ms_iter_pair.second)
	{
		// first와 second가 다르다면 원소가 있음
		printf("원소를 찾았습니다! \n");
	}

	return 0;
}