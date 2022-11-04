#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

int main144()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	list<int> lst;
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(40);
	lst.push_back(50);

	sort(v.begin(), v.end());							// 시퀀스 자료구조는 정렬 가능
	// sort(lst.begin(), lst.end());						// 연관 자료구조는 정렬 불가

	vector<int>::iterator iter = v.begin();
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;
	}

	return 0;
}