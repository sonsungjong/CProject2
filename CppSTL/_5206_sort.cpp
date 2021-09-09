#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

int main526()
{
	vector<int> v;
	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	list<int> lt;
	lt.push_back(10);
	lt.push_back(20);
	lt.push_back(30);
	lt.push_back(40);
	lt.push_back(50);

	sort(v.begin(), v.end());				// ���� ���� (vector�� deque �� ����), �⺻�� ��������(10, 20, 30, 40, 50)
	//sort(v.begin(), v.end(), less<int>());			// �����ͺ��� �������� ���� (10, 20, 30, 40, 50)
	//sort(lt.begin(), lt.end());				// ���� �Ұ�

	vector<int>::iterator iter;
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		cout << *iter << endl;
	}
	cout << endl;

	return 0;
}