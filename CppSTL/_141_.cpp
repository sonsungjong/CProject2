#include <iostream>
#include <deque>
using namespace std;

int main141()
{
	deque<int> dq;						// ����� ���� ��� vector�� ������

	dq.push_back(10);
	dq.push_back(20);
	dq.push_back(30);
	dq.push_back(40);
	dq.push_back(50);

	deque<int>::iterator iter = dq.begin();					// ������ġ�� ����Ű�� �ݺ���
	cout << iter[0] << endl;
	cout << iter[1] << endl;
	cout << iter[2] << endl;
	cout << iter[3] << endl;
	cout << iter[4] << endl;
	cout << endl;

	iter += 2;									// +=
	cout << *iter << endl;
	cout << endl;

	deque<int>::iterator iter2 = iter + 2;				// +
	cout << *iter2 << endl;							// �ݺ��� ������

	return 0;
}