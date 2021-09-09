#include <iostream>
#include <vector>
using namespace std;

int main522()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;			// 반복자 생성
	for (iter = v.begin(); iter != v.end(); ++iter) {
		cout << *iter << endl;
	}

	return 0;
}