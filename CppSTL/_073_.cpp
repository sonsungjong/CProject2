#include <iostream>
using namespace std;

void gPrint(int a_data) 
{
	cout << "gPrint 글로벌 함수" << endl;
}

class MyClass
{
public:
	static void sPrint(int a_data) 
	{
		cout << "sPrint 스태이틱 멤버함수" << endl;
	}

	void mPrint(int a_data) 
	{
		cout << "mPrint 멤버 함수" << endl;
	}
};

int main073()
{
	// 전역함수나 정적함수라면 함수포인터
	void (*pf1)(int) = gPrint;
	void (*pf2)(int) = MyClass::sPrint;

	// 멤버함수라면 함수포인터
	void (MyClass::*pf3)(int) = &MyClass::mPrint;

	// 전역함수 함수포인터 호출
	pf1(10);

	MyClass mc;
	(mc.*pf3)(10);				// 멤버함수 함수포인터 호출

	return 0;
}