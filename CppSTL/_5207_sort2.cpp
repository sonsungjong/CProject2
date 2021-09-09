#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

int main527()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	sort(v.begin(), v.end(), less<int>());			// 작은것부터 오름차순 정렬 (10, 20, 30, 40, 50)
	vector<int>::iterator iter;

	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;
	}
	cout << endl;

	sort(v.begin(), v.end(), greater<int>());			// 큰것부터 내림차순 정렬 (50, 40, 30, 20, 10)
	vector<int>::iterator iter2;
	for (iter2 = v.begin(); iter2 != v.end(); ++iter2)
	{
		cout << *iter2 << endl;
	}
	cout << endl;

	return 0;
}