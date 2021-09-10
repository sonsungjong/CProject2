#include <iostream>
#include <functional>
using namespace std;

int main5213() {
	cout << less<int>()(10, 20) << endl;
	cout << less<int>()(20, 20) << endl;
	cout << less<int>()(20, 10) << endl;
	cout << "==============" << endl;
	cout << not2(less<int>())(10, 20) << endl;
	cout << not2(less<int>())(20, 20) << endl;
	cout << not2(less<int>())(20, 10) << endl;
	cout << endl;
	cout << endl;

	less<int> le;
	cout << le(10, 20) << endl;
	cout << le(20, 20) << endl;
	cout << le(20, 10) << endl;
	cout << "==============" << endl;
	cout << not2(le)(10, 20) << endl;
	cout << not2(le)(20, 20) << endl;
	cout << not2(le)(20, 10) << endl;

	return 0;
}