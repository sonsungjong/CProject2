#include <iostream>

using namespace std;

void _Print()
{
	cout << "���� �Լ� Print()" << endl;
}

class PrintClass
{
public:
	void Print()
	{
		cout << "��� �Լ� Print()" << endl;
	}
};

int main22()
{
	PrintClass prt;
	PrintClass* p = &prt;

	_Print();			// �����Լ� ȣ��
	prt.Print();			// ��ü�� ����Լ� ȣ��
	p->Print();			// �ּҷ� ����Լ� ȣ��

	return 0;
}