#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

// count : ���������� �ش� ������ ������ ���Ѵ�
int main301()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(30);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	// ���� v.begin() ~ v.end() ���� 30�� ������ ��ȯ
	printf("30�� ����: %lld", count(v.begin(), v.end(), 30));
	
	return 0;
}