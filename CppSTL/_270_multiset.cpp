#include <cstdio>
#include <set>

using namespace std;
// multiset�� set�� �����ϰ� �ٸ� ���� '�ߺ�'�� ����Ѵٴ� ���̴�.

int main270()
{
	multiset<int> ms;
	multiset<int>::iterator ms_iter;

	ms.insert(50);
	ms.insert(30);
	ms.insert(80);
	ms.insert(80);				// �ߺ�
	ms.insert(30);				// �ߺ�
	ms.insert(70);
	ms_iter = ms.insert(10);				// ���� ��ġ�� ����Ű�� �ݺ��� ��ȯ
	
	printf("iter�� ����: %d\n", *ms_iter);
	for (ms_iter = ms.begin(); ms_iter != ms.end(); ++ms_iter)
	{
		printf("%d ", *ms_iter);
	}
	printf("\n");

	return 0;
}
