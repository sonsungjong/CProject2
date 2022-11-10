#include <iostream>

// friend

// 1. 함수 friend
// 2. 클래스 friend
// 클래스 멤버를 접근 제한과 상관없이 모두 사용할 수 있게됨

// 함수 프렌드
class A
{
private:
	int x;
	friend void Func();
};

void Func()
{
	// class A의 모든 멤버를 접근 제한 없이 사용할 수 있음
	A a;
	a.x = 0;
}

// 클래스 프렌드
class B
{
private:
	int y;
	friend class C;
};

class C
{
	// class B의 모든 멤버를 접근제한 상관없이 모두 사용할 수 있음
	void SetB_y()
	{
		B b;
		b.y = 0;
	}
};
