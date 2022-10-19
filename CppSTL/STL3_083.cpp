#include <iostream>
#include <functional>
using namespace std;

// 타입정의 (이름이 길어서)
typedef less<int> Less;
typedef greater<int> Greater;

int main83()
{
	Less o_less;

	cout << o_less(10, 20) << endl;				// 객체로 함수자 호출
	cout << o_less(20, 10) << endl;
	cout << o_less.operator()(10, 20) << endl;
	cout << endl;

	cout << Less()(10, 20) << endl;				// 임시객체로 함수자 호출
	cout << Less()(20, 10) << endl;
	cout << Less().operator()(10, 20) << endl;

	cout << Greater()(10, 20) << endl;				// 임시객체로 함수자 호출
	cout << Greater()(20, 10) << endl;

	cout << endl;
	// STL functional의 less와 greater사용
	cout << less<int>()(10, 20) << endl;
	cout << less<int>()(20, 10) << endl;
	cout << greater<int>()(10, 20) << endl;
	cout << greater<int>()(20, 10) << endl;

	return 0;
}