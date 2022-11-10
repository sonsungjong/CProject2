#include <iostream>
using namespace std;

// 1. 정적 함수 호출 (정적 함수) -- 전역함수, namespace내의 전역함수, static멤버함수
// 2. 객체로 멤버 함수 호출 (멤버 함수)
// 3. 객체의 주소로 멤버 함수 호출 (멤버 함수)

// 함수 포인터의 선언과 호출은 위 세가지 함수 호출 방식에 따라 약간의 차이가 있음.

void Print()
{
	cout << "정적 함수 Print()" << endl;
}

class Point
{
public:
	void Print()
	{
		cout << "멤버 함수 Print()" << endl;
	}
};

int main061()
{
	Point pt;
	Point *p = &pt;

	Print();						// 정적함수 호출
	pt.Print();					// 객체를 통한 멤버함수 호출
	p->Print();					// 주소를 통한 멤버함수 호출

	return 0;
}