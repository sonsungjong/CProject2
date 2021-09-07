#include <iostream>

using namespace std;

void _Print()
{
	cout << "정적 함수 Print()" << endl;
}

class PrintClass
{
public:
	void Print()
	{
		cout << "멤버 함수 Print()" << endl;
	}
};

int main22()
{
	PrintClass prt;
	PrintClass* p = &prt;

	_Print();			// 정적함수 호출
	prt.Print();			// 객체로 멤버함수 호출
	p->Print();			// 주소로 멤버함수 호출

	return 0;
}