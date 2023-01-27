#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

// equal : 두 순차열이 전부 같은지 비교 (첫번째 순차열의 크기만큼만 비교)
int main304()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(30);
	v2.push_back(40);
	v2.push_back(50);

	printf("v1 : ");
	for (vector<int>::size_type i = 0; i < v1.size(); ++i) {
		printf("%d ", v1[i]);
	}
	printf("\n");

	printf("v2 : ");
	for (vector<int>::size_type i = 0; i < v2.size(); ++i) {
		printf("%d ", v2[i]);
	}
	printf("\n");
	
	// 구간 [v1.begin(), v1.end()) 와 구간 [v2.begin(), v2.begin()+3) 을 비교
	if (equal(v1.begin(), v1.end(), v2.begin())) {
		printf("두 순차열은 같습니다.\n");
	}

	return 0;
}