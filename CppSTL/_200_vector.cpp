#include <iostream>
#include <vector>
using namespace std;

int main200()
{
	// erase()
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	vector<int>::iterator iter;
	vector<int>::iterator iter2;
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	iter = v.begin() + 2;
	// iter�� ����Ű�� ��ġ�� ���Ҹ� ����
	// iter2�� ���� ���� 40�� ����Ŵ
	iter2 = v.erase(iter);
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	// [v.begin() +1, v.end()) ������ ���Ҹ� ����
	iter2 = v.erase(v.begin() + 1, v.end());			// iter2�� ���� ���� v.end()�� ����Ŵ
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << " ";
	}
	cout << endl;

	return 0;
}