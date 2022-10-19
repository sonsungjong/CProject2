#include <iostream>
#include <functional>
using namespace std;

struct Plus
{
	int operator()(int a, int b)
	{
		return a + b;
	}
};

struct Minus
{
	int operator()(int a, int b)
	{
		return a - b;
	}
};

int main86()
{
	cout << Plus()(10, 20) << endl;				// 사용자 정의 Plus 함수자
	cout << Minus()(10, 20) << endl;				// 사용자 정의 Minus 함수자
	cout << endl;

	cout << plus<int>()(10, 20) << endl;				// STL의 plus
	cout << minus<int>()(10, 20) << endl;				// STL의 minus

	return 0;
}