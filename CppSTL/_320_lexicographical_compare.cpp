#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// lexicographical_compare(v1, v1, v2, v2) : v1�� ������ ���� (true, false)

int main320()
{
	vector<int> v1;
	vector<int> v2;
	vector<int> v3;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(50);

	v3.push_back(10);
	v3.push_back(20);
	v3.push_back(30);

	// ó���� �۳�? (v1)
	if (lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end())) {
		printf("v1 < v2 (true)\n");				// v1�� �۴�
	}
	else {
		printf("v1 >= v2 (false)\n");				// v1�� �����ʴ�
	}

	// ó���� �۳�? (v1 �� �۳�)
	if (lexicographical_compare(v1.begin(), v1.end(), v3.begin(), v3.end())) {
		printf("v1 < v3 (true)\n");				// v1�� �۴�
	}
	else {
		printf("v1 >= v3 (false)\n");				// v1�� �����ʴ�
	}

	return 0;
}