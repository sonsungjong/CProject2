#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main332()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(30);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;
	iter = search_n(v.begin(), v.end(), 3, 30);			// 3번째 일치하는 부분을 찾음

	if (iter != v.end())
	{
		cout << "iter : " << *iter << endl;
		cout << "iter-1 : " << *(iter-1) << endl;
		cout << "iter+1 : " << *(iter+1) << endl;
	}

	return 0;
}