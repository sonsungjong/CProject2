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

// �Ϲ�ȭ �Լ� ���ø�
template<typename T>
void gPrint6(T a)
{
	cout << a << endl;
}

// Ư��ȭ �Լ� ���ø�
template<>
void gPrint6(Point a)
{
	cout << "Print Ư��ȭ ���� :";
	a.Print();
}

int main100()
{
	int n = 10;
	double d = 2.5;
	Point pt(2, 3);

	gPrint6(n);				// gPrint6<int>(n)					�Ϲ�ȭ ���� ȣ��
	gPrint6(d);				// gPrint6<double>(n)				�Ϲ�ȭ ���� ȣ��
	gPrint6(pt);				// gPrint6<Point>(n)			Ư��ȭ ���� ȣ��

	return 0;
}