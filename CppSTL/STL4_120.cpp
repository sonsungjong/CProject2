#include <iostream>
#include <functional>
#include <string>
using namespace std;

template<typename T>
struct Less
{
	bool operator()(T a, T b)
	{
		return a < b;
	}
};

template<typename T>
struct Greater
{
	bool operator()(T a, T b)
	{
		return a > b;
	}
};

template<typename RetType, typename ArgType>
class Functor120
{
public:
	RetType operator() (ArgType data)
	{
		cout << data << endl;
		return RetType();
	}
};

template<typename T1, typename T2>
struct Pair120
{
	T1 first;
	T2 second;
	Pair120(const T1& ft, const T2& sd) : first(ft), second(sd) {}
};

int main120()
{
	cout << Less<int>()(10, 20) << endl;
	cout << Less<int>()(20, 10) << endl;
	cout << Greater<int>()(10, 20) << endl;
	cout << Greater<int>()(20, 10) << endl;
	cout << endl;
	cout << less<int>()(10, 20) << endl;
	cout << less<int>()(20, 10) << endl;
	cout << greater<int>()(10, 20) << endl;
	cout << greater<int>()(20, 10) << endl;
	cout << endl;

	Functor120<void, int> functor1;
	functor1(10);
	Functor120<bool, string> functor2;
	functor2("Hello");
	cout << endl;

	Pair120<int, int> p1(10, 20);
	cout << p1.first << ',' << p1.second << endl;
	Pair120<int, string> p2(1, "one");
	cout << p2.first << ',' << p2.second << endl;
	cout << endl;

	pair<int, int> p3(10, 20);
	cout << p3.first << ',' << p3.second << endl;
	pair<int, string> p4(1, "one");
	cout << p4.first << ',' << p4.second << endl;

	return 0;
}