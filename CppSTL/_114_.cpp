#include <iostream>
#include <string>
using namespace std;

template<typename IterT, typename Func>
void For_each114(IterT begin, IterT end, Func pf)
{
	while (begin != end)
	{
		pf(*begin++);
	}
}

template<typename T>
void PrintT(T n)
{
	cout << n << " ";
}

template<typename T>
struct PrintFunctor
{
	string sep;

	explicit PrintFunctor(const string& s = " ") : sep(s) {}
	void operator()(T data) const
	{
		cout << data << sep;
	}
};

int main114()
{
	int arr[5] = { 10, 20, 30, 40, 50 };
	For_each114(arr, arr + 5, PrintFunctor<int>());
	cout << endl;

	string sarr[3] = {"abc", "abcde", "Hello!"};
	For_each114(sarr, sarr + 3, PrintFunctor<string>("*\n"));
	cout << endl;

	return 0;
}