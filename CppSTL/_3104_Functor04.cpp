#include <iostream>
#include <algorithm>
using namespace std;

struct Functor01 {			// 출력
	void operator()(int n)
	{
		cout << n << " ";
	}
};

struct Functor02 {			// 제곱
	void operator()(int n)
	{
		cout << n*n << " ";
	}
};

struct Functor03 {				// 문자열과 endl
	void operator()(int n)
	{
		cout << "정수 : " << n << endl;
	}
};

int main3104() {
	int arr[5] = { 10, 20, 30, 40, 50 };

	for_each(arr, arr + 5, Functor01());

	cout << endl << endl;
	for_each(arr, arr + 5, Functor02());

	cout << endl << endl;
	for_each(arr, arr + 5, Functor03());

	return 0;
}