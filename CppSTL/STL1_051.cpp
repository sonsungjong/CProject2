#include <iostream>
using namespace std;

// Ÿ�� ��ȯ ������ �����ε��� �̿��� Ÿ�� ��ȯ : �ڽ��� Ÿ���� �ٸ� Ÿ������ ��ȯ

class A
{
};

class B
{
public:
	operator A()
	{
		cout << "operator A() ȣ��" << endl;
		return A();
	}

	operator int()
	{
		cout << "operator int() ȣ��" << endl;
		return 10;
	}

	operator double()
	{
		cout << "operator double() ȣ��" << endl;
		return 5.5;
	}
};

int main051()
{
	A a;
	int n;
	double d;

	B b;
	a = b;				// �Ͻ��� ȣ�� : BŸ���� ��ü�� AŸ������ ��ȯ
	n = b;				// �Ͻ��� ȣ�� : 
	d = b;				// �Ͻ��� ȣ��

	cout << endl;
	a = b.operator A();						// ����� ȣ��
	n = b.operator int();					// ����� ȣ��
	d = b.operator double();				// ����� ȣ��

	return 0;
}