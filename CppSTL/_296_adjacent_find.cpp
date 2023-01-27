#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int main296()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(30);
	v.push_back(40);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	vector<int>::iterator iter;
	// ���� [v.begin(), v.end())���� ���� ���ҿ� ���� ���Ұ� �������� ù ���� �ݺ��ڸ� ��ȯ
	iter = adjacent_find(v.begin(), v.end());

	if (iter != v.end()) {
		// ���� ���Ұ� ���ٸ� ������ �� �ݺ��� ��ȯ
		printf("%d \n", *iter);
	}

	for (; iter != v.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	return 0;
}