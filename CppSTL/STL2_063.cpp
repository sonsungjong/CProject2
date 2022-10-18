#include <iostream>
using namespace std;

// 정적함수 : 전역함수, namespace내의 전역함수, static멤버함수
// 정적함수 호출 : 시그너처를 통해 사용

void GPrint(int n)
{
	cout << "전역 함수 : " << n << endl;
}

namespace A
{
	void Print(int n)
	{
		cout << "namespace A 전역 함수 : " << n << endl;
	}
}

class Point
{
public:
	static void Print(int n)
	{
		cout << "Point 클래스의 정적 멤버 함수 : " << n << endl;
	}
};

int main063()
{
	void (*pf)(int);				// 정적 함수 포인터 선언

	GPrint(10);				// 1. namespace 없는 전역함수 호출
	A::Print(10);					// 2. namespace A의 전역함수 호출
	Point::Print(10);						// 3. Point 클래스의 정적 멤버함수 호출

	pf = GPrint;					// 1. 함수 포인터로 namespace 없는 전역함수 저장
	pf(10);

	pf = A::Print;						// 2. 함수 포인터로 namespace가 A인 전역함수 저장
	pf(10);

	pf = Point::Print;						// 3. 함수 포인터로 Point클래스의 정적 멤버함수 저장
	pf(10);

	return 0;
}