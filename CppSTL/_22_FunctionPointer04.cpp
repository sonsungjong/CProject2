#include <iostream>

using namespace std;

void _Print(int n)
{
	cout << "전역 함수: " << n << endl;
}

namespace PrintA
{
	void _Print(int n)
	{
		cout << "namespace A 전역함수: " << n << endl;
	}
}

class PrintClassA
{
public:
	static void _Print(int n)
	{
		cout << "Point 클래스의 정적 멤버 함수: " << n << endl;
	}
};

int main224()
{
	_Print(10);							// 전역함수 호출
	PrintA::_Print(10);				// namespace 전역함수 호출
	PrintClassA::_Print(10);		// class 의 정적멤버함수 호출

	cout << "\n===========함수포인터 사용===========" << endl;
	void (*pf)(int);			// 정적 함수포인터 선언

	pf = _Print;					// 전역함수의 시작주소를 함수포인터에 할당
	pf(10);
	pf = PrintA::_Print;		// namespace
	pf(10);
	pf = PrintClassA::_Print;		// class
	pf(10);

	return 0;
}