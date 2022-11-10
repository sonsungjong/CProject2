// const 멤버 함수와 비 const 멤버 함수
#include <iostream>
using namespace std;

class Point2
{
	int x;
	int y;
public:
	Point2(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	// 멤버 변수를 바꾸지 않으므로 const함수
	int GetX() const
	{
		return x;
	}

	int GetY() const					// const 함수
	{
		return y;
	}

	void SetX(int a_x)				// 비 const 함수 : 멤버 변경
	{
		x = a_x;
	}

	void SetY(int a_y)				// 비 const 함수 : 멤버 변경
	{
		y = a_y;
	}

	void Print() const
	{
		cout << x << ',' << y << endl;
	}
};

int main020()
{
	const Point2 p1(0, 0);				// const 객체 : 모든 멤버 변경 불가 (const 함수만 호출가능)
	Point2 p2(2, 3);				// 비 const 객체

	p1.Print();
	p2.Print();

	cout << "p1: " << p1.GetX() << ',' << p1.GetY() << endl;
	cout << "p2: " << p2.GetX() << ',' << p2.GetY() << endl;

	// p1.SetX(5);						// 컴파일러 오류 -> const 객체는 const 멤버함수만 사용 가능
	// p1.SetY(5);						// 컴파일러 오류 -> const 객체는 const 멤버함수만 사용 가능
	p2.SetX(5);
	p2.SetY(5);

	return 0;
}
