#include <iostream>

using namespace std;

void Print(int n)
{
	cout << "���� : " << n << endl;
}

int main21()
{
	// void Print(int n)�� �Լ� ������ ����
	void (*pf)(int);
	// �Լ��� �̸��� �Լ��� ���� �ּ�
	pf = Print;

	Print(10);			// �Ϲ����� �Լ� ȣ��
	pf(10);				// �����͸� �̿��� �Լ� ȣ��
	(*pf)(10);			// �����͸� �̿��� �Լ� ȣ��2

	// �Լ��� ���� �ּ�
	cout << endl;
	cout << Print << endl;
	cout << pf << endl;
	cout << *pf << endl;

	return 0;
}

/*
	 ���� : ���� �����ϴ� �޸� ����
	 ������ : �ּҸ� �����ϴ� �޸� ����
	 �Լ� ������ : �Լ��� ���� �ּҸ� �����ϴ� ������
*/