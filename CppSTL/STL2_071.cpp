#include <iostream>
#include <algorithm>
using namespace std;

// 클라이언트1
void _MyPrint1(int n)
{
	cout << n << ' ';
}

void _MyPrint2(int n)
{
	cout << n * n << " ";
}

void _MyPrint3(int n)
{
	cout << "정수 : " << n << endl;
}

int main071()
{
	int arr[5] = { 10, 20, 30, 40, 50 };

	for_each(arr, arr + 5, _MyPrint1);
	cout << endl << endl;

	for_each(arr, arr + 5, _MyPrint2);
	cout << endl << endl;
	for_each(arr, arr + 5, _MyPrint3);				// 함수의 주소를 매개변수로 전달

	return 0;
}