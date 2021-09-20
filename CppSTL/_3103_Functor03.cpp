#include <iostream>
using namespace std;

class Adder
{
protected:
	int total;
public:
	explicit Adder(int n = 0) : total(n) {}
	int operator()(int n)
	{
		return total += n;
	}
};

int main3103() {
	Adder add(0);

	cout << add(10) << endl;			// 10
	cout << add(20) << endl;			// 30
	cout << add(30) << endl;			// 60

	return 0;
}