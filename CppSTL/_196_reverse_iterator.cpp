#include <iostream>
#include <vector>
using namespace std;

int main196()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;						// 정방향 반복자
	vector<int>::reverse_iterator riter;			// 역방향 반복자

	// 정방향 조회
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	// 역방향 조회
	for (riter = v.rbegin(); riter != v.rend(); ++riter)
	{
		cout << *riter << " ";
	}
	cout << endl;

	// rbegin() : 끝 원소
	// rend() : 첫 원소

	return 0;
}