#include <iostream>

using namespace std;

class secPoint 
{
protected:
	int x, y;

public:
	secPoint(int x=0, int y=0) { this->x = x; this->y = y; }
	secPoint operator++()				// 전위++
	{
		++x;
		++y;
		return secPoint(x,y);
	}
	secPoint operator++(int)			// 후위++
	{
		secPoint pt(x, y);
		++x;
		++y;
		return pt;
	}
	void Print()
	{
		cout << x << "," << y << endl;
	}
};

int main12() {
	secPoint p1(2, 3), p2(2, 3);
	secPoint result;

	result = ++p1;
	p1.Print();
	result.Print();

	result = p2++;
	p2.Print();
	result.Print();

	return 0;
}