#include <iostream>
#include <algorithm>
using namespace std;

// 클라이언트
struct Functor4
{
	void operator()(int n)
	{
		cout << n << " ";
	}
};

struct Functor5
{
	void operator()(int n)
	{
		cout << n * n << " ";
	}
};

struct Functor6
{
	void operator()(int n)
	{
		cout << "정수 : " << n << endl;
	}
};

int main80()
{
	int arr[5] = { 10, 20, 30, 40, 50 };
	
	for_each(arr, arr + 5, Functor4());
	// 임시 함수 객체 Functor4()를 만들어 함수로 전달

	cout << endl << endl;
	for_each(arr, arr + 5, Functor5());

	cout << endl << endl;
	for_each(arr, arr + 5, Functor6());

	return 0;
}