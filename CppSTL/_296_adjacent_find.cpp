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
	// 구간 [v.begin(), v.end())에서 현재 원소와 다음 원소가 같아지는 첫 원소 반복자를 반환
	iter = adjacent_find(v.begin(), v.end());

	if (iter != v.end()) {
		// 같은 원소가 없다면 구간의 끝 반복자 반환
		printf("%d \n", *iter);
	}

	for (; iter != v.end(); ++iter)
	{
		printf("%d ", *iter);
	}
	printf("\n");

	return 0;
}