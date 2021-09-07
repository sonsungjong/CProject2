#include <iostream>

using namespace std;

class Point
{
protected:
	int x;
	int y;
public:
	Point() { x = 0; y = 0; };
	Point(int x, int y) 
	{
		this->x = x;
		this->y = y;
	}
	void Print()
	{
		cout << x << ", " << y << endl;
	}
	const Point operator+(Point arg)
	{
		Point pt;
		pt.x = this->x + arg.x;
		pt.y = this->y + arg.y;

		return pt;
	}
};

int main11() {
	Point p1, p2;
	Point p3;
	p1 = Point(2, 3);
	p2 = Point(5, 5);
	//p3 = p1.operator+(&p2);
	p3 = p1 + p2;

	p3.Print();
	
	return 0;
}