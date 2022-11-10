#include <iostream>
using namespace std;

// 타입 변환 연산자 오버로딩을 이용한 타입 변환 : 자신의 타입을 다른 타입으로 변환

class A
{
};

class B
{
public:
	operator A()
	{
		cout << "operator A() 호출" << endl;
		return A();
	}

	operator int()
	{
		cout << "operator int() 호출" << endl;
		return 10;
	}

	operator double()
	{
		cout << "operator double() 호출" << endl;
		return 5.5;
	}
};

int main051()
{
	A a;
	int n;
	double d;

	B b;
	a = b;				// 암시적 호출 : B타입의 객체를 A타입으로 변환
	n = b;				// 암시적 호출 : 
	d = b;				// 암시적 호출

	cout << endl;
	a = b.operator A();						// 명시적 호출
	n = b.operator int();					// 명시적 호출
	d = b.operator double();				// 명시적 호출

	return 0;
}