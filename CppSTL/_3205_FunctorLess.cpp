#include <iostream>
using namespace std;

bool Pred_less(int a, int b) {
	return a < b;
}

struct _Less
{
	bool operator()(int a, int b)
	{
		return a < b;
	}
};

int main3205() {
	_Less less;

	cout << Pred_less(10, 20) << endl;
	cout << Pred_less(20, 10) << endl;
	cout << endl;
	cout << less(10, 20) << endl;
	cout << less(20, 10) << endl;
	cout << _Less()(10, 20) << endl;
	cout << _Less()(20, 10) << endl;
	cout << endl;
	cout << less.operator()(10, 20) << endl;
	cout << _Less().operator()(10, 20) << endl;

	return 0;
}