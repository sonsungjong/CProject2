#include <iostream>
using namespace std;

// ����Լ� �����ε� : �Ϲ����� �����ε�
// �����Լ� �����ε� : ����Լ� �����ε��� ����� �� ���� ��

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
	int GetX() const { return x; }				// x�� getter
	int GetY() const { return y; }					// y�� getter

	// ����Լ� �����ε�
	const Point operator-(const Point& arg) const
	{
		return Point(x - arg.x, y - arg.y);
	}
};

// �����Լ� �����ε�
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