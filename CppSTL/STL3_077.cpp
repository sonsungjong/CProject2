#include <iostream>
using namespace std;

// �Լ� ��ü : �Լ�ó�� �����ϴ� ��ü, ()�����ڸ� �����ε��Ͽ� ����

void gPrint1()
{
	cout << "���� �Լ�" << endl;
}

struct Functor1
{
	// �Լ���ü�� ()�����ڸ� �����ε��� Ŭ������ ����
	void operator() ()
	{
		cout << "�Լ� ��ü! " << endl;
	}
};

int main077()
{
	Functor1 functor1;

	gPrint1();				// �����Լ�
	functor1();				// �Լ���(�Լ���ü)

	return 0;
}