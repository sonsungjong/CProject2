#include <iostream>
using namespace std;

class Point
{
	int x;
	int y;
public:
	explicit Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
};

// 일반화 함수 템플릿
template<typename T>
void gPrint6(T a)
{
	cout << a << endl;
}

// 특수화 함수 템플릿
template<>
void gPrint6(Point a)
{
	cout << "Print 특수화 버전 :";
	a.Print();
}

int main100()
{
	int n = 10;
	double d = 2.5;
	Point pt(2, 3);

	gPrint6(n);				// gPrint6<int>(n)					일반화 버전 호출
	gPrint6(d);				// gPrint6<double>(n)				일반화 버전 호출
	gPrint6(pt);				// gPrint6<Point>(n)			특수화 버전 호출

	return 0;
}