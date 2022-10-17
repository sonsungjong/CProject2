#include <iostream>
using namespace std;

// 1. 생성자를 이용한 타입변환
// 2. 타입변환 연산자 오버로딩 사용

class A
{
};

class B
{
public:
	B() { cout << "B() 생성자" << endl; }
	B(A& a_a) { cout << "B(A a_a) 생성자" << endl; }
	B(int a_n) { cout << "B(int a_n) 생성자" << endl; }
	B(double a_d) { cout << "B(double a_d) 생성자" << endl; }
};

int main048()
{
	A a;
	int n = 10;
	double d = 5.5;

	B b;					// 생성자 호출
	b = a;				// b = B(a)
	b = n;				// b = B(n)
	b = d;				// b = B(d)

	return 0;
}