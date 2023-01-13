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

	// multiset�� �ݺ��� ��(pair) ��ü ����
	pair<multiset<int>::iterator, multiset<int>::iterator> ms_iter_pair;
	ms_iter_pair = ms.equal_range(30);				// 30 ã��

	for (ms_iter = ms_iter_pair.first; ms_iter != ms_iter_pair.second; ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	if (ms_iter_pair.first == ms_iter_pair.second)
	{
		// lower_bound �� upper_bound�� ���ٸ� ã�� ���Ұ� ����
		printf("ã�� ���Ұ� �����ϴ�. \n");
	}
	else if (ms_iter_pair.first != ms_iter_pair.second)
	{
		// first�� second�� �ٸ��ٸ� ���Ұ� ����
		printf("���Ҹ� ã�ҽ��ϴ�! \n");
	}

	return 0;
}