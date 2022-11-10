#include <iostream>
using namespace std;


// 두개의 매개변수를 갖는 템플릿
template<typename T1, typename T2>
void gPrint6(T1 a, T2 b)
{
	cout << a << ", " << b << endl;
}

template<typename T>
void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

int main95()
{
	gPrint6(10, 1.5);
	gPrint6("Hello", 60);
	gPrint6(1.5, "Hello!");

	cout << "==============" << endl;

	int n1 = 10, n2 = 20;
	float d1 = 1.1f, d2 = 2.2;

	cout << n1 << ", " << n2 << endl;
	Swap(n1, n2);
	cout << n1 << ", " << n2 << endl;
	cout << endl;

	cout << d1 << ", " << d2 << endl;
	Swap(d1, d2);
	cout << d1 << ", " << d2 << endl;
	cout << endl;
	return 0;
}