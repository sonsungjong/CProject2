#include <iostream>
#include <vector>
using namespace std;

int main179()
{
	vector<int> v1(5);						// 0���� �ʱ�ȭ�� 5ĭ¥�� �����̳�
	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	for (vector<int>::size_type i = 0; i < v1.size(); ++i)
	{
		cout << v1[i] << " ";												// 0 0 0 0 0 10 20 30 40 50
	}
	cout << endl;

	vector<int> v2(5);						// 0���� �ʱ�ȭ�� 5ĭ¥�� �����̳�
	v2[0] = 10;
	v2[1] = 20;
	v2[2] = 30;
	v2[3] = 40;
	v2[4] = 50;

	for (vector<int>::size_type i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << " ";												// 10 20 30 40 50
	}
	cout << endl;

	vector<int> v3(5, 10);						// 10���� �ʱ�ȭ�� 5ĭ¥�� �����̳�
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	v3.resize(3);						// 3ĭ¥�� �����̳ʷ� ũ�⺯��
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	v3.resize(10, 33);							// 10ĭ¥���� Ȯ���ϸ鼭 �߰� �����Ǵ� ĭ�� �ʱⰪ�� 33
	for (vector<int>::size_type i = 0; i < v3.size(); ++i)
	{
		cout << v3[i] << " ";
	}
	cout << endl;
	cout << "size: " << v3.size() << "    capacity: " << v3.capacity() << endl;

	return 0;
}
