#include <iostream>
#include <vector>
using namespace std;

int main191()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter = v.begin();				// ���ۿ��Ҹ� ����Ų��
	cout << *iter << endl;				// ������

	iter += 2;				// ����ĭ���� 2ĭ �̵�
	cout << *iter << endl;

	iter -= 1;				// ����ĭ���� 1ĭ �̵�
	cout << *iter << endl;

	return 0;
}