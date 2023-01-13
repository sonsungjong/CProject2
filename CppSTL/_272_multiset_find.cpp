#include <cstdio>
#include <set>
using namespace std;
// count : ����
// find : ã��
// lower_bound, upper_bound : ã��

int main272()
{
	multiset<int> ms;
	ms.insert(50);
	ms.insert(30);
	ms.insert(80);
	ms.insert(80);				// 80 �ߺ�
	ms.insert(30);				// 30 �ߺ�
	ms.insert(70);
	ms.insert(10);

	multiset<int>::iterator ms_iter;
	for (ms_iter = ms.begin(); ms_iter != ms.end(); ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	/* count */
	printf("30 ������ ����: %lld��\n", ms.count(30));				// 30 ������ ����
	/* find */
	ms_iter = ms.find(30);				// 30 ù���� ������ �ݺ���
	printf("ms_iter: %d\n", *ms_iter);

	/* lower_bound, upper_bound */
	multiset<int>::iterator ms_lower_iter;
	multiset<int>::iterator ms_upper_iter;

	ms_lower_iter = ms.lower_bound(30);				// 30 �������� ���� �ݺ���
	ms_upper_iter = ms.upper_bound(30);			// 30 �������� �� �ݺ���
	printf("lower_iter: %d, upper_iter: %d\n", *ms_lower_iter, *ms_upper_iter);

	printf("���� [lower_iter, upper_iter)�� ������: ");
	for (ms_iter = ms_lower_iter; ms_iter != ms_upper_iter; ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	return 0;
}
