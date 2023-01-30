#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// find_end() 조건자버전

bool Pred311(int left, int right)
{
	return left <= right;
}

int main311()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(15);
	v1.push_back(20);
	v1.push_back(40);
	v1.push_back(50);
	v1.push_back(60);
	v1.push_back(10);
	v1.push_back(11);
	v1.push_back(12);
	v1.push_back(80);

	v2.push_back(10);
	v2.push_back(15);
	v2.push_back(25);

	vector<int>::iterator iter;
	iter = find_end(v1.begin(), v1.end(), v2.begin(), v2.end(), Pred311);
	if (iter != v1.end()) {
		// 일치하는 마지막 순차열의 첫 원소의 반복자 iter
		printf("iter : %d\n", *iter);
		printf("iter-1 : %d\n", *(iter-1));
		printf("iter+1 : %d\n", *(iter+1));
	}

	return 0;
}