#include <iostream>
#include <vector>
using namespace std;

int main203()
{
	vector<int> v1;
	vector<int> v2;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	v2.push_back(10);
	v2.push_back(20);
	v2.push_back(50);

	if (v1 == v2)						// ��� ���Ұ� ������
	{
		cout << "v1 == v2" << endl;
	}

	if (v1 != v2)						// �ϳ��� �ٸ� ���Ұ� �ִ°�
	{
		cout << "v1 != v2" << endl;
	}

	if (v1 < v2)						// �������� ���Ҹ� �ϳ��� ���Ͽ� v1�� ���Ұ� �۴ٸ� ��, �ƴϸ� ���� (���ڿ�ó�� ���Ҹ� �ϳ��� ���Ѵ�.)
	{
		cout << "v1 < v2" << endl;
	}

	return 0;
}