#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

class Point326
{
private:
	int x;
	int y;
public:
	explicit Point326(int a_x = 0, int a_y = 0) : x(a_x), y(a_y)
	{
	}
	int GetX() const { return x; }
	int GetY() const { return y; }
	void Print() const { printf("(%d,%d)\n", x, y); }
};

bool Compare326(const Point326& left, const Point326& right)
{
	if (left.GetX() < right.GetX()) { return true; }
	else if (left.GetX() > right.GetX()) { return false; }
	else { return left.GetY() < right.GetY(); }
}

int main326()
{
	vector<Point326> v;

	v.push_back(Point326(3, 2));
	v.push_back(Point326(2, 5));
	v.push_back(Point326(1, 5));
	v.push_back(Point326(3, 3));
	v.push_back(Point326(3, 2));

	vector<Point326>::iterator iter;
	iter = max_element(v.begin(), v.end(), Compare326);
	printf("max: ");
	iter->Print();

	return 0;
}