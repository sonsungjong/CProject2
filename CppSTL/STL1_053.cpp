#include <iostream>
using namespace std;

class Point
{
	int x;
	int y;
public:
	explicit Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }

	operator int() const
	{
		return x;
	}

};

int main053()
{
	int n = 10;

	Point pt(2, 3);
	n = pt;						// pt.operator int() 암시적 호출
	cout << n << endl;				// 2

	return 0;
}