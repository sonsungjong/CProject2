#include <iostream>
using namespace std;

// 1. �����ڸ� �̿��� Ÿ�Ժ�ȯ
// 2. Ÿ�Ժ�ȯ ������ �����ε� ���

class A
{
};

class B
{
public:
	B() { cout << "B() ������" << endl; }
	B(A& a_a) { cout << "B(A a_a) ������" << endl; }
	B(int a_n) { cout << "B(int a_n) ������" << endl; }
	B(double a_d) { cout << "B(double a_d) ������" << endl; }
};

int main048()
{
	A a;
	int n = 10;
	double d = 5.5;

	B b;					// ������ ȣ��
	b = a;				// b = B(a)
	b = n;				// b = B(n)
	b = d;				// b = B(d)

	return 0;
}