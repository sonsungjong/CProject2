#include <iostream>
using namespace std;

class Point
{
	int x;
	int y;
public:
	explicit Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() { cout << x << ',' << y << endl; }
	void PrintInt(int n) { cout << "테스트 정수 : " << n << endl; }
};

int main065()
{
	Point pt(2, 3);
	Point *p = &pt;

	void (Point::*pf1)();				// 멤버 함수 포인터 선언
	pf1 = &Point::Print;

	void (Point::*pf2) (int);				// 멤버 함수 포인터 선언
	pf2 = &Point::PrintInt;

	pt.Print();
	pt.PrintInt(10);
	cout << endl;

	(pt.*pf1)();						// 객체로 멤버 함수 포인터를 이용한 호출
	(pt.*pf2)(10);						// 객체로 멤버 함수 포인터를 이용한 호출
	cout << endl;

	(p->*pf1)();							// 주소로 멤버 함수 포인터를 이용한 호출
	(p->*pf2)(10);					// 주소로 멤버 함수 포인터를 이용한 호출

	return 0;
}