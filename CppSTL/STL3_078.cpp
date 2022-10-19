#include <iostream>

using namespace std;

void gPrint3(int a, int b)
{
	cout << "전역함수 : " << a << ',' << b << endl;
}

// 매개변수를 갖는 함수객체
struct Functor3
{
	void operator() (int a, int b)
	{
		cout << "함수객체 : " << a << ',' << b << endl;
	}
};

// 멤버변수를 갖는 함수객체
class Adder
{
	int total;
public:
	explicit Adder(int n = 0) : total(n) 
	{}
	int operator()(int n)
	{
		return total += n;
	}
};

int main078()
{
	Functor3 functor3;
	Adder add(0);

	gPrint3(10, 20);
	functor3(10, 20);				// functor3.operator()(10, 20)

	// 멤버변수를 활용하는 함수객체
	cout << add(10) << endl;				// 10을 누적 => 10
	cout << add(10) << endl;				// 10을 누적 => 20
	cout << add(50) << endl;				// 50을 누적 => 70

	return 0;
}