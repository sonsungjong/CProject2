#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// find_first_of : 두 순차열을 비교하여 모든 원소 중 Pred가 참인 것을 하나라도 발견되면 발견된 첫 원소의 반복자를 반환

bool Pred315(int left, int right)
{
	return left > right;
}

int main315()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);
	
	v2.push_back(40);
	v2.push_back(80);
	v2.push_back(20);

	printf("v1 : ");
	for (vector<int>::size_type i = 0; i < v1.size(); ++i) {
		printf("%d ", v1[i]);
	}
	printf("\nv2 : ");
	for (vector<int>::size_type i = 0; i < v2.size(); ++i) {
		printf("%d ", v2[i]);
	}
	printf("\n");

	vector<int>::iterator iter;
	iter = find_first_of(v1.begin(), v1.end(), v2.begin(), v2.end(), Pred315);
	if (iter != v1.end()) {
		printf("iter : %d\n", *iter);
	}

	return 0;
}