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

	cout << v[0] << ", " << v.front() << endl;					// front() : ù��° ����
	cout << v[4] << ", " << v.back() << endl;					// back() : ������ ����

	cout << endl;
	v.front() = 100;							// front�� ó�� ���� ����
	v.back() = 500;							// back���� ������ ���� ����
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}