#include <iostream>
// 오버로딩과 네임스페이스
extern "C"
{
	int add(int a, int b)
	{
		return a + b;
	}
}

namespace SSJ
{
	int add(int a, int b)
	{
		return a + b;
	}

	double add(double a, double b)
	{
		return a + b;
	}

	int add(int a, int b, int c)
	{
		return a + b + c;
	}
}

namespace myname
{
	int add(int a, int b)
	{
		return a + b;
	}

	double add(double a, double b)
	{
		return a + b;
	}

	int add(int a, int b, int c)
	{
		return a + b + c;
	}
}

using namespace myname;

int main31() {
	// namespace는 스코프 연산자와 함께 사용
	int sum = SSJ::add(2, 3);
	// namespace를 매번 적는 것이 불편하다면 using namespace를 사용
	int myname_sum = add(3.1, 4.1);

	return 0;
}

/*
	overloading : 함수 이름을 짓는 스트레스를 줄임
	개발자들간 함수명이 곂치는 문제를 방지
	가독성이 떨어지기 때문에 사용을 추천하지 않음
	C언어에서는 안됨
	오버로딩의 효율이 떨어져서 namespace를 만들었음

	namespace : C언어에서 함수명이 곂치면 안되기 때문에 각 함수앞에 name을 붙였었는데 너무 없어보여서 namespace라는 문법을 만듦
	namespace는 스코프(::) 연산자와 함께 사용
	namespace를 매번 적는 것이 불편하다면 using namespace를 사용

	extern "C"{} 안에 함수를 한개 넣어서 C언어와 호환시킬 수 있음
	이 경우 extern "C"{} 안에 있는 함수가 기본 함수가 됨
*/