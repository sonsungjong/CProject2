#include <iostream>

class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { std::cout << x << ',' << y << std::endl; }
	void operator+(Point arg)
	{
		std::cout << "operator+() 함수 호출" << std::endl;
	}
};

int main() {
	Point p1(2, 3), p2(5, 5);
	p1 + p2;
	//p1* p2;
	//p1 = p2;
	//p1 == p2;
	//p1 += p2;

	return 0;
}