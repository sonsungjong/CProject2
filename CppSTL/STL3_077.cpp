#include <iostream>
using namespace std;

// 함수 객체 : 함수처럼 동작하는 객체, ()연산자를 오버로딩하여 제작

void gPrint1()
{
	cout << "전역 함수" << endl;
}

struct Functor1
{
	// 함수객체는 ()연산자를 오버로딩한 클래스를 말함
	void operator() ()
	{
		cout << "함수 객체! " << endl;
	}
};

int main077()
{
	Functor1 functor1;

	gPrint1();				// 전역함수
	functor1();				// 함수자(함수객체)

	return 0;
}