#include <iostream>
#include <vector>
using namespace std;

int main194()
{
	vector<int> v;
	int arr[5] = { 10, 20, 30, 40, 50 };

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	// 이동 및 변경 가능
	vector<int>::iterator iter = v.begin();
	int* p = arr;
	// iter는 p처럼 동작
	cout << *iter << ", " << *p << endl;

	// 이동 가능, 변경 불가
	vector<int>::const_iterator citer = v.begin();
	const int* cp = arr;
	// citer는 cp처럼 동작
	cout << *citer << ", " << *cp << endl;

	// 이동 불가, 변경 가능
	const vector<int>::iterator iter_const = v.begin();
	int* const p_const = arr;
	// iter_cosnt 는 p_const 처럼 동작
	cout << *iter_const << ", " << *p_const << endl;

	// 이동 및 변경 불가
	const vector<int>::const_iterator citer_const = v.begin();
	const int* const cp_const = arr;
	// citer_const 는 cp_const 처럼 동작
	cout << *citer_const << ", " << *cp_const << endl;

	return 0;
}