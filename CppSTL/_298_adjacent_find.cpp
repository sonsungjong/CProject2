#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

int main298()
{
	// 원하는 값을 찾지 못할 때
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
	vector<int>::iterator iter_b = v.begin();
	vector<int>::iterator iter_e = v.begin() + 2;

	iter = adjacent_find(iter_b, iter_e);

	if (iter != iter_e) {
		printf("%d \n", *iter);
	}
	printf("=================\n");

	iter_b = v.begin();
	iter_e = v.end();
	iter = adjacent_find(iter_b, iter_e);

	if (iter != iter_e) {
		printf("%d\n", *iter);
	}

	if (iter != v.end()) {
		printf("%d\n", *iter);
	}


	return 0;
}