#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

bool Pred300(int a, int b)
{
	return abs(b - a) > 10;
}

int main300()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(50);
	v.push_back(90);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		printf("%d ", v[i]);
	}
	printf("\n");

	vector<int>::iterator iter;
	iter = adjacent_find(v.begin(), v.end(), Pred300);

	if (iter != v.end()) {
		printf("%d \n", *iter);
	}

	for (; iter != v.end(); ++iter) {
		printf("%d ", *iter);
	}
	printf("\n");

	return 0;
}