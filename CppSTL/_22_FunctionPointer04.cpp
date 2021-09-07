#include <iostream>

using namespace std;

void _Print(int n)
{
	cout << "���� �Լ�: " << n << endl;
}

namespace PrintA
{
	void _Print(int n)
	{
		cout << "namespace A �����Լ�: " << n << endl;
	}
}

class PrintClassA
{
public:
	static void _Print(int n)
	{
		cout << "Point Ŭ������ ���� ��� �Լ�: " << n << endl;
	}
};

int main224()
{
	_Print(10);							// �����Լ� ȣ��
	PrintA::_Print(10);				// namespace �����Լ� ȣ��
	PrintClassA::_Print(10);		// class �� ��������Լ� ȣ��

	cout << "\n===========�Լ������� ���===========" << endl;
	void (*pf)(int);			// ���� �Լ������� ����

	pf = _Print;					// �����Լ��� �����ּҸ� �Լ������Ϳ� �Ҵ�
	pf(10);
	pf = PrintA::_Print;		// namespace
	pf(10);
	pf = PrintClassA::_Print;		// class
	pf(10);

	return 0;
}