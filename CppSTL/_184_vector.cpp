#include <iostream>
#include <vector>
using namespace std;

int main184()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	cout << v[0] << ", " << v.front() << endl;					// front() : 첫번째 원소
	cout << v[4] << ", " << v.back() << endl;					// back() : 마지막 원소

	cout << endl;
	v.front() = 100;							// front로 처음 원소 수정
	v.back() = 500;							// back으로 마지막 원소 수정
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}