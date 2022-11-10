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
	cout << Plus()(10, 20) << endl;				// ����� ���� Plus �Լ���
	cout << Minus()(10, 20) << endl;				// ����� ���� Minus �Լ���
	cout << endl;

	cout << plus<int>()(10, 20) << endl;				// STL�� plus
	cout << minus<int>()(10, 20) << endl;				// STL�� minus

	return 0;
}