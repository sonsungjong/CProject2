#include <iostream>
using namespace std;

void Print2(int a, int b)
{
	cout << "전역함수: " << a << ',' << b << endl;
}

struct Functor2
{
	void operator()(int a, int b)
	{
		cout << "함수객체: " << a << ',' << b << endl;
	}
};

int main3102() {
	Functor2 functor;

	Print2(10, 20);			// 전역함수
	functor(10, 20);		// 함수객체 == functor.operator()(10, 20)

	return 0;
}