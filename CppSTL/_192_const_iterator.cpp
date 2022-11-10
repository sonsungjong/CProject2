#include <iostream>
#include <vector>
using namespace std;

int main192()
{
	// 반복자가 가리키는 값이 변경되지 않는다면 상수반복자를 사용한다.
	// iterator ----> const_iterator
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter = v.begin();
	vector<int>::const_iterator citer = v.begin();

	cout << *iter << endl;
	cout << *citer << endl;
	cout << endl;

	cout << *++iter << endl;
	cout << *++citer << endl;

	*iter = 100;					// 반복자는 값을 수정할 수 있고 상수반복자는 값을 수정할 수 없음

	return 0;
}