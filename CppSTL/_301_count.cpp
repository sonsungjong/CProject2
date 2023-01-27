#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

// count : 순차열에서 해당 원소의 갯수를 구한다
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

	// 구간 v.begin() ~ v.end() 에서 30의 갯수를 반환
	printf("30의 개수: %lld", count(v.begin(), v.end(), 30));
	
	return 0;
}