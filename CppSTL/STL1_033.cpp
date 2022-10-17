#include <iostream>
using namespace std;

// 함수 호출 연산자 오버로딩 : 객체를 함수처럼 동작시키는 연산자
// 함수 객체 : 함수 호출 연산자인 객체

struct FuncObject
{
public:
	void operator() (int arg) const
	{
		cout << "정수 : " << arg << endl;
	}
};

void Print1(int arg)
{
	cout << "정수 : " << arg << endl;
}

int main033()
{
	void (*Print2)(int) = Print1;
	FuncObject Print3;

	Print1(10);						// 함수사용
	Print2(10);							// 함수포인터 사용
	Print3(10);							// 함수객체 사용
	//Print3.operator(10);

	return 0;
}