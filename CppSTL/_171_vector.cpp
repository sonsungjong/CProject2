#include <iostream>
#include <vector>
using namespace std;

int main171()
{
	vector<int> v;

	v.push_back(10);
	v.push_back(20);
	v.push_back(30);
	v.push_back(40);
	v.push_back(50);

	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	// size() : ������ ����
	cout << v.size() << endl;

	// capacity() : �Ҵ�� �޸� ũ��
	cout << v.capacity() << endl;									// vector�� ȿ���� ���� �߰� ���� �̸� ������ Ȯ���س���

	// max_size() : �ִ� ���� ������ ������ ����
	cout << v.max_size() << endl;

	cout << "=============" << endl;

	// capacity �޸� ���Ҵ�
	vector<int> v2;

	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(10);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(20);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(30);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(40);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(50);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(60);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(70);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(80);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	v2.push_back(90);
	cout << "size: " << v2.size() << "    capacity: " << v2.capacity() << endl;
	
	for (unsigned __int64 i = 0; i < v2.size(); ++i)
	{
		cout << v2[i] << " ";
	}
	cout << endl;

	return 0;
}
