#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

bool Pred306(int left, int right)
{
	return abs(right - left) < 5;
}

int main306()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(21);
	v1.push_back(30);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(33);

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

	// v1.begin() ~ v1.end() 와 v2.begin() ~ v2.begin()+3 을 비교
	if (equal(v1.begin(), v1.end(), v2.begin(), Pred306)) {
		printf("두 순차열은 같습니다.\n");
	}

	return 0;
}