#include <iostream>
// 오버로딩이 가능한 이항 연산자 : +, -, *, /, ==, !=, <, <=, >, >= 등...
using namespace std;

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
	bool operator==(const Point& arg) const
	{
		return x == arg.x && y == arg.y ? true : false;
	}

	bool operator!=(const Point& arg) const
	{
		return !(*this == arg);
	}
};

int main027()
{
	Point p1(2, 3), p2(5, 5), p3(2, 3);

	if (p1 != p2)						// p1.operator!=(p2)
	{
		cout << "p1 != p2" << endl;
	}

	if (p1 != p3)						// p1.operator!=(p3)
	{
		cout << "p1 != p3" << endl;
	}

	return 0;
}