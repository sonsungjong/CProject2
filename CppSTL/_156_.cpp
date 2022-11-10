#include <iostream>
#include <functional>				// not2
using namespace std;

// not2 : 조건자 함수 객체를 반대 의미의 조건자 함수 객체로 변경하는 어댑터

// less를 NOT(반대) 시키는 예제
int main156()
{
	cout << less<int>()(10, 20) << endl;
	cout << less<int>()(20, 20) << endl;
	cout << less<int>()(20, 10) << endl;
	cout << "=============" << endl;

	cout << not2(less<int>())(10, 20) << endl;
	cout << not2(less<int>())(20, 20) << endl;
	cout << not2(less<int>())(20, 10) << endl;
	cout << endl;

	less<int> l;
	cout << l(10, 20) << endl;
	cout << l(20, 20) << endl;
	cout << l(10, 10) << endl;
	cout << "=============" << endl;
	cout << not2(l)(10, 20) << endl;
	cout << not2(l)(20, 20) << endl;
	cout << not2(l)(20, 10) << endl;

	return 0;
}