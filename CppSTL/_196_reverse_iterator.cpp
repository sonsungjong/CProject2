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

	vector<int>::iterator iter;						// ������ �ݺ���
	vector<int>::reverse_iterator riter;			// ������ �ݺ���

	// ������ ��ȸ
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	// ������ ��ȸ
	for (riter = v.rbegin(); riter != v.rend(); ++riter)
	{
		cout << *riter << " ";
	}
	cout << endl;

	// rbegin() : �� ����
	// rend() : ù ����

	return 0;
}