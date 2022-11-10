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

	// �̵� �� ���� ����
	vector<int>::iterator iter = v.begin();
	int* p = arr;
	// iter�� pó�� ����
	cout << *iter << ", " << *p << endl;

	// �̵� ����, ���� �Ұ�
	vector<int>::const_iterator citer = v.begin();
	const int* cp = arr;
	// citer�� cpó�� ����
	cout << *citer << ", " << *cp << endl;

	// �̵� �Ұ�, ���� ����
	const vector<int>::iterator iter_const = v.begin();
	int* const p_const = arr;
	// iter_cosnt �� p_const ó�� ����
	cout << *iter_const << ", " << *p_const << endl;

	// �̵� �� ���� �Ұ�
	const vector<int>::const_iterator citer_const = v.begin();
	const int* const cp_const = arr;
	// citer_const �� cp_const ó�� ����
	cout << *citer_const << ", " << *cp_const << endl;

	return 0;
}