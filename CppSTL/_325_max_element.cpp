#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// max_element : 최대값을 가리키는 iterator
// min_element : 최소값을 가리키는 iterator

int main325()
{
	vector<int> v;
	v.push_back(30);
	v.push_back(10);
	v.push_back(20);
	v.push_back(50);
	v.push_back(40);

	vector<int>::iterator iter;

	iter = max_element(v.begin(), v.end());
	printf("%d\n", *iter);					// 50

	iter = min_element(v.begin(), v.end());
	printf("%d\n", *iter);					// 10

	return 0;
}