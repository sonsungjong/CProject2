#include <iostream>
#include <vector>
using namespace std;

int main192()
{
	// �ݺ��ڰ� ����Ű�� ���� ������� �ʴ´ٸ� ����ݺ��ڸ� ����Ѵ�.
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

	*iter = 100;					// �ݺ��ڴ� ���� ������ �� �ְ� ����ݺ��ڴ� ���� ������ �� ����

	return 0;
}