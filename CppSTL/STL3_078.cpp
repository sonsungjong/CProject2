#include <iostream>

using namespace std;

void gPrint3(int a, int b)
{
	cout << "�����Լ� : " << a << ',' << b << endl;
}

// �Ű������� ���� �Լ���ü
struct Functor3
{
	void operator() (int a, int b)
	{
		cout << "�Լ���ü : " << a << ',' << b << endl;
	}
};

// ��������� ���� �Լ���ü
class Adder
{
	int total;
public:
	explicit Adder(int n = 0) : total(n) 
	{}
	int operator()(int n)
	{
		return total += n;
	}
};

int main078()
{
	Functor3 functor3;
	Adder add(0);

	gPrint3(10, 20);
	functor3(10, 20);				// functor3.operator()(10, 20)

	// ��������� Ȱ���ϴ� �Լ���ü
	cout << add(10) << endl;				// 10�� ���� => 10
	cout << add(10) << endl;				// 10�� ���� => 20
	cout << add(50) << endl;				// 50�� ���� => 70

	return 0;
}