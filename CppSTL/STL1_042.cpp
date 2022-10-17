#include <iostream>
using namespace std;

// 메모리접근, 클래스멤버접근 연산자 오버로딩 (*, ->) : 반복자나 스마트포인터의 객체에 사용됨

// 일반클래스
class Point
{
	int x;
	int y;
public:
	Point(int a_x = 0, int a_y = 0) : x(a_x), y(a_y) {}
	void Print() const { cout << x << ',' << y << endl; }
};

// 스마트포인터 (자동 해제)
class PointPtr
{
	Point *ptr;
public:
	PointPtr(Point *p) : ptr(p) {}
	~PointPtr()
	{
		delete ptr;
	}

	// Point클래스의 멤버함수 사용을 위해 ->연산자 오버로딩을 해줘야함
	Point* operator->() const
	{
		return ptr;
	}

	Point& operator*() const
	{
		return *ptr;
	}
};

int main042()
{
	Point* p1 = new Point(2, 3);				// 일반포인터
	PointPtr p2 = new Point(5, 5);				// 스마트포인터

	p1->Print();
	p2->Print();				// p2.operator->()->Print();
	cout << endl;

	(*p1).Print();
	(*p2).Print();					// p2.operator*().Print();

	delete p1;
	//delete p2;					// 메모리 자동제거

	return 0;
}