#include <iostream>
using namespace std;

// 멤버함수 오버로딩 : 일반적인 오버로딩
// 전역함수 오버로딩 : 멤버함수 오버로딩을 사용할 수 없을 때

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
	int GetX() const { return x; }				// x의 getter
	int GetY() const { return y; }					// y의 getter

	// 멤버함수 오버로딩
	const Point operator-(const Point& arg) const
	{
		return Point(x - arg.x, y - arg.y);
	}
};

// 전역함수 오버로딩
const Point operator+(const Point& argL, const Point& argR)
{
	return Point(argL.GetX() + argR.GetX(), argL.GetY() + argR.GetY());
}

int main028()
{
	Point p1(2, 3), p2(5, 5);
	Point p3;
	Point p4;

	p3 = p1 - p2;							// p1.operator-(p2);
	p3.Print();

	p4 = p1 + p2;						// operator+(p1, p2);
	p4.Print();

	return 0;
}