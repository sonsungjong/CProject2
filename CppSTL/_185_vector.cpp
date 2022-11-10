#include <iostream>
#include <vector>
using namespace std;

// at()
int main185()
{
	// v[i] : ������ üũ���� �ʾ� ����
	// v.at(i) : �������� ������ üũ�� ������
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

	v[0] = 100;				// ���� ���� ���� 0 index ����
	v[4] = 500;				// ���� ���� ���� 4 index ����
	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	v.at(0) = 1000;				// ���� ���� �Ŀ� 0 index ����
	v.at(4) = 5000;				// ���� ���� �Ŀ� 4 index ����
	for (vector<int>::size_type i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	// ����� ���� ������ üũ�ϳ� ���ϳ� ����
	cout << "=======" << endl;
	try
	{
		cout << v.at(0) << endl;
		cout << v.at(3) << endl;
		cout << v.at(6) << endl;						// throw : out_of_range ����
	}
	catch (out_of_range& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}