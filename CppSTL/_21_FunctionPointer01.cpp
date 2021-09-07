#include <iostream>

using namespace std;

void Print(int n)
{
	cout << "정수 : " << n << endl;
}

int main21()
{
	// void Print(int n)의 함수 포인터 선언
	void (*pf)(int);
	// 함수의 이름은 함수의 시작 주소
	pf = Print;

	Print(10);			// 일반적인 함수 호출
	pf(10);				// 포인터를 이용한 함수 호출
	(*pf)(10);			// 포인터를 이용한 함수 호출2

	// 함수의 시작 주소
	cout << endl;
	cout << Print << endl;
	cout << pf << endl;
	cout << *pf << endl;

	return 0;
}

/*
	 변수 : 값을 저장하는 메모리 공간
	 포인터 : 주소를 저장하는 메모리 공간
	 함수 포인터 : 함수의 시작 주소를 저장하는 포인터
*/