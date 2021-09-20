#include <iostream>
using namespace std;

void Print()
{
	cout << "전역함수" << endl;
}

struct Functor1
{
	void operator()()
	{
		cout << "함수 객체" << endl;
	}
};

int main3101() {
	Functor1 functor;

	Print();			// 전역함수
	functor();		// 함수객체 == functor.operator()

	return 0;
}