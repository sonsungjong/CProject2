#include <cstdio>
#include <set>
using namespace std;
// count : 갯수
// find : 찾기
// lower_bound, upper_bound : 찾기

int main272()
{
	multiset<int> ms;
	ms.insert(50);
	ms.insert(30);
	ms.insert(80);
	ms.insert(80);				// 80 중복
	ms.insert(30);				// 30 중복
	ms.insert(70);
	ms.insert(10);

	multiset<int>::iterator ms_iter;
	for (ms_iter = ms.begin(); ms_iter != ms.end(); ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	/* count */
	printf("30 원소의 갯수: %lld개\n", ms.count(30));				// 30 원소의 갯수
	/* find */
	ms_iter = ms.find(30);				// 30 첫번쨰 원소의 반복자
	printf("ms_iter: %d\n", *ms_iter);

	/* lower_bound, upper_bound */
	multiset<int>::iterator ms_lower_iter;
	multiset<int>::iterator ms_upper_iter;

	ms_lower_iter = ms.lower_bound(30);				// 30 순차열의 시작 반복자
	ms_upper_iter = ms.upper_bound(30);			// 30 순차열의 끝 반복자
	printf("lower_iter: %d, upper_iter: %d\n", *ms_lower_iter, *ms_upper_iter);

	printf("구간 [lower_iter, upper_iter)의 순차열: ");
	for (ms_iter = ms_lower_iter; ms_iter != ms_upper_iter; ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	return 0;
}
