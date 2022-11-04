#include <iostream>
#include <vector>
#include <algorithm>
// 알고리즘 : 원소를 조사, 변경, 관리, 처리할 목적

using namespace std;

int main143()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;
	iter = find(v.begin(), v.end(), 20);						// [begin, end) 에서 20 찾기
	cout << *iter << endl;					// 찾았으면 멈춰서 가리킴

	iter = find(v.begin(), v.end(), 100);						// [begin, end) 에서 100 찾기
	if (iter == v.end())					// 100이 없으면 iter == v.end() 임.
	{
		cout << "100 없음" << endl;
	}

	return 0;
}