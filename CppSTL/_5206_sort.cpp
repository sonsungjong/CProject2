#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

int main526()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	list<int> lt;
	lt.push_back(10);
	lt.push_back(20);
	lt.push_back(30);
	lt.push_back(40);
	lt.push_back(50);

	sort(v.begin(), v.end());				// 정렬 가능 (vector와 deque 만 가능), 기본은 오름차순(10, 20, 30, 40, 50)
	//sort(v.begin(), v.end(), less<int>());			// 작은것부터 오름차순 정렬 (10, 20, 30, 40, 50)
	//sort(lt.begin(), lt.end());				// 정렬 불가

	vector<int>::iterator iter;
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;
	}
	cout << endl;

	return 0;
}