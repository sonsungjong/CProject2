#include <iostream>
using namespace std;

// 오버로딩이 가능한 단항연산자 : !, &, ~, *, +, -, ++, --, 형변환

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const
	{
		cout << x << ',' << y << endl;
	}

	const Point& operator++()					// 전위 ++연산자
	{
		++x;
		++y;
		return *this;
	}

	const Point operator++(int)				// 후위 ++연산자
	{
		Point pt(x, y);
		++x;						// 내부 구현이므로 전위++을 사용해도 무방
		++y;
		return pt;
	}
};

int main021()
{
	Point p1(2, 3), p2(2, 3);
	Point result;

	result = ++p1;				// p1.operator++();
	p1.Print();
	result.Print();

	result = p2++;					// p2.operator++(0);
	p2.Print();
	result.Print();
	
	return 0;
}