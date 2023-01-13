#include <cstdio>
#include <set>
using namespace std;

int main264()
{
	set<int, less<int>> s;

	// 30�� 50 ��
	int result = !s.key_comp()(30, 50) && !s.key_comp()(50, 30);		// 30�� 50���� �ռ����� �ʰ�, 50�� 30���� �ռ����� ������? false
	printf("%d\n", result);

	// 30�� 30 ��
	result = !s.key_comp()(30, 30) && !s.key_comp()(30, 30);			// 30�� 30���� �ռ����� �ʰ� 30�� 30���� �ռ����� ������? true
	printf("%d\n", result);

	return 0;
}