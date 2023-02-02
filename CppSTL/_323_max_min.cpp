#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// max() : ū ��
// min() : ���� ��

class Point323
{
private:
	int x;
	int y;
public:
	explicit Point323(int _x = 0, int _y = 0) : x(_x), y(_y)
	{
	}

	int GetX() const { return x; }
	int GetY() const { return y; }
	void Print() const { printf("(%d,%d)\n", x, y); }
};

bool XCompare(const Point323& left, const Point323& right)
{
	return left.GetX() < right.GetX();
}

bool YCompare(const Point323& left, const Point323& right)
{
	return left.GetY() < right.GetY();
}

int main323()
{
	int a = 10;
	int b = 20;

	int r = max(a,b);				// �� ���� �� �� ū ��
	printf("max: %d \n", r);

	r = min(a, b);					// �� ���� �� �� ���� ��
	printf("min: %d \n", r);

	Point323 pt1(5, 8);
	Point323 pt2(3, 9);
	Point323 pt3;

	pt3 = max(pt1, pt2, XCompare);
	printf("x��ǥ max: ");
	pt3.Print();

	pt3 = max(pt1, pt2, YCompare);
	printf("y��ǥ max: ");
	pt3.Print();

	return 0;
}