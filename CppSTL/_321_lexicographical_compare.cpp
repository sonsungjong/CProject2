#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

template<typename T>
class Less321
{
public:
	bool operator() (const T& left, const T& right) const
	{
		return left < right;				// '왼쪽이 작다'로 정의
	}
};

int main321()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);

	v2.push_back(10);
	v2.push_back(25);
	v2.push_back(30);

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), less<int>())) {
		printf("기준 less... v1과 v2의 비교: true \n");
	}
	else {
		printf("기준 less... v1과 v2의 비교: false \n");
	}

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), greater<int>())) {
		printf("기준 greater... v1과 v2의 비교: true \n");
	}
	else {
		printf("기준 greater... v1과 v2의 비교: false \n");
	}

	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), Less321<int>())) {
		printf("Less321 조건자 기준... v1과 v2의 비교: true \n");
	}
	else {
		printf("Less321 조건자 기준... v1과 v2의 비교 : false \n");
	}

	return 0;
}