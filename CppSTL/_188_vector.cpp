#include <iostream>
#include <vector>
using namespace std;

int main188()
{
	// assign(n, x)
	vector<int> v(5, 1);					// 5ĭ¥�� ���͸� 1�� �ʱ�ȭ
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	v.assign(5, 2);				// 5���� ���Ұ��� 2�� �Ҵ�
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}