#include <iostream>
using namespace std;

// 1. ���� �Լ� ȣ�� (���� �Լ�) -- �����Լ�, namespace���� �����Լ�, static����Լ�
// 2. ��ü�� ��� �Լ� ȣ�� (��� �Լ�)
// 3. ��ü�� �ּҷ� ��� �Լ� ȣ�� (��� �Լ�)

// �Լ� �������� ����� ȣ���� �� ������ �Լ� ȣ�� ��Ŀ� ���� �ణ�� ���̰� ����.

void Print()
{
	cout << "���� �Լ� Print()" << endl;
}

class Point
{
public:
	void Print()
	{
		cout << "��� �Լ� Print()" << endl;
	}
};

int main061()
{
	Point pt;
	Point *p = &pt;

	Print();						// �����Լ� ȣ��
	pt.Print();					// ��ü�� ���� ����Լ� ȣ��
	p->Print();					// �ּҸ� ���� ����Լ� ȣ��

	return 0;
}