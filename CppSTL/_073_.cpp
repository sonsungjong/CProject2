#include <iostream>
using namespace std;

void gPrint(int a_data) 
{
	cout << "gPrint �۷ι� �Լ�" << endl;
}

class MyClass
{
public:
	static void sPrint(int a_data) 
	{
		cout << "sPrint ������ƽ ����Լ�" << endl;
	}

	void mPrint(int a_data) 
	{
		cout << "mPrint ��� �Լ�" << endl;
	}
};

int main073()
{
	// �����Լ��� �����Լ���� �Լ�������
	void (*pf1)(int) = gPrint;
	void (*pf2)(int) = MyClass::sPrint;

	// ����Լ���� �Լ�������
	void (MyClass::*pf3)(int) = &MyClass::mPrint;

	// �����Լ� �Լ������� ȣ��
	pf1(10);

	MyClass mc;
	(mc.*pf3)(10);				// ����Լ� �Լ������� ȣ��

	return 0;
}