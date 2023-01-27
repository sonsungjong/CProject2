#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

bool Pred302(int n)
{
	return n > 25;
}

// count_if : 해당 범위에서 리턴 조건에 부합하는 원소의 갯수를 반환
int main302()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i) {
		printf("%d ", v[i]);
	}
	printf("\n");

	// v.begin() ~ v.end() 에서 25보다 큰 원소의 갯수를 반환
	printf("25보다 큰 원소의 개수 : %lld", count_if(v.begin(), v.end(), Pred302));

	return 0;
}