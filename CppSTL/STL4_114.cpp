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

void PrintInt(int n)
{
	cout << n << " ";
}

void PrintString(string data)
{
	cout << data << " ";
}

int main()
{
	int arr[5] = { 10, 20, 30, 40, 50 };
	For_each114(arr, arr + 5, PrintInt);
	cout << endl;

	string sarr[3] = {"abc", "abcde", "Hello!"};
	For_each114(sarr, sarr + 3, PrintString);
	cout << endl;

	return 0;
}