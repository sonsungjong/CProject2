#include <iostream>
// using namespace : 특별한 namespace가 없으면 std::를 쓰겠다는 뜻
using namespace std;

// 함수 오버로딩
/*
int add(int a, int b) 
{
	return a + b;
}
*/

int add(int a, int b, int c)
{
	return a + b + c;
}

double add(double a, double b)
{
	return a + b;
}

// 오버로딩 피해서 C언어에 적용
extern "C" {
	int add(int a, int b)
	{
		return a + b;
	}
}

// namespace (ssj_add같이 변수명 앞에 이름붙이면 허접해보여서 만듦)
namespace ssj
{
	// 이 안의 변수명, 함수명은 앞에 ssj가 붙은 꼴이 됨
	int add(int a, int b)
	{
		return a + b;
	}
}

// using namespace들끼리 변수,함수명이 곂치면 안됨
// using namespace ssj;

int main함수오버로딩() {
	int s = add(2, 3);
	double temp = add(2.1, 3.0);
	int myName = ssj::add(3, 3);

	cout << "정수: " << s << endl;
	cout << "실수: " << temp << endl;
	cout << "namespace: " << myName << endl;

	return 0;
}

// 오버로딩하면 C와 호환이 되지않음
// extern "C"{} 안에 넣어 호환