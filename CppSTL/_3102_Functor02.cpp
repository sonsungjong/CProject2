#include <iostream>
using namespace std;

void Print2(int a, int b)
{
	cout << "�����Լ�: " << a << ',' << b << endl;
}

struct Functor2
{
	void operator()(int a, int b)
	{
		cout << "�Լ���ü: " << a << ',' << b << endl;
	}
};

int main3102() {
	Functor2 functor;

	Print2(10, 20);			// �����Լ�
	functor(10, 20);		// �Լ���ü == functor.operator()(10, 20)

	return 0;
}