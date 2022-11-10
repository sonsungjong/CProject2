#include <iostream>
using namespace std;

// 서버가 클라이언트의 함수를 미리 알고있는 것은 불가능함
// 콜백 메커니즘은 클라이언트가 서버를 호출할 때 서버에 클라이언트의 정보를 제공하도록 설계
// 함수 포인터의 매개변수를 이용해 콜백 함수의 주소를 전달함

// 서버
// 배열의 모든 원소에 반복적인 작업을 수행하게 추상화됨 (구체적인 작업은 없음)
void For_each(int* begin, int* end, void (*pf)(int))
{
	while (begin != end)
	{
		pf(*begin++);				// 클라이언트 함수 호출 (콜백)
	}
}

// 클라이언트1
void PrintClient1(int n)
{
	cout << n << ' ';				// 공백을 이용해 원소출력
}

// 클라이언트2
void PrintClient2(int n)			// 각 원소를 제곱해 출력
{
	cout << n * n << " ";
}

// 클라이언트3
void PrintClient3(int n)
{
	cout << "정수 : " << n << endl;				// 문자열과 줄바꿈을 이용해 원소 출력
}

int main069()
{
	int arr[5] = { 10, 20, 30, 40, 50 };

	For_each(arr, arr + 5, PrintClient1);				// 매개변수로 Print1() 의 주소를 전달
	cout << endl << endl;
	For_each(arr, arr + 5, PrintClient2);				// 매개변수로 Print2() 함수의 주소를 전달
	cout << endl << endl;
	For_each(arr, arr + 5, PrintClient3);				// 매개변수로 Print3() 함수의 주소를 전달

	return 0;
}