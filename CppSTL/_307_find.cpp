#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

bool Pred307(int n)
{
	return n > 35;
}

int main307()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	vector<int>::iterator iter;
	// find 로 구간에서 20 찾기
	iter = find(v.begin(), v.end(), 20);
	if (iter != v.end()) {
		printf("%d 을 찾았다 \n", *iter);
	}

	// find_if 로 구간에서 35보다 큰 첫 원소 찾기
	iter = find_if(v.begin(), v.end(), Pred307);
	if (iter != v.end()) {
		printf("순차열에서 35보다 큰 첫번째 원소: %d \n", *iter);
	}

	return 0;
}