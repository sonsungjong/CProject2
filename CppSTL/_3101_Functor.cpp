#include <iostream>
using namespace std;

void Print()
{
	cout << "�����Լ�" << endl;
}

struct Functor1
{
	void operator()()
	{
		cout << "�Լ� ��ü" << endl;
	}
};

int main3101() {
	Functor1 functor;

	Print();			// �����Լ�
	functor();		// �Լ���ü == functor.operator()

	return 0;
}