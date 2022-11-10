#include <iostream>
using namespace std;

// 템플릿 : 함수 템플릿, 클래스 템플릿
// 1. 함수 템플릿 : 함수를 만들어내는 틀
// 2. 클래스 템플릿 : 클래스를 만들어내는 틀

// 자료형을 사용하는 클라이언트에서 지정하게끔 설정
template<typename T>
void gPrint5(T a, T b)
{
	cout << a << ", " << b << endl;
}

int main91()
{
	// 암시적 타입지정
	gPrint5(10, 20);
	gPrint5(0.123f, 1.123f);
	gPrint5("ABC", "abcde");

	// 명시적 타입지정
	gPrint5<int>(10, 20);
	gPrint5<float>(0.123f, 1.123f);
	gPrint5<const char*>("ABC", "abcde");

	return 0;
}