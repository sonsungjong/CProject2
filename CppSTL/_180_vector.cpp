#include <iostream>
#include <vector>
using namespace std;

int main180()
{
	vector<int> v(5);				// 5ĭ¥�� �̸� ����

	v[0] = 10;
	v[1] = 20;
	v[2] = 30;
	v[3] = 40;
	v[4] = 50;
	for (uint64_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	v.clear();				// ���͸� ����
	cout << "size: " << v.size() << "    capacity: " << v.capacity() << endl;

	if (v.empty())						// ����ִ��� Ȯ���Ѵ�
	{
		cout << "v�� ���Ұ� �����ϴ�." << endl;
	}

	return 0;
}