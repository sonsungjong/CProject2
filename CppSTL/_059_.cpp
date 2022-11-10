#include <iostream>
using namespace std;

// 함수포인터 : 함수의 주소를 저장하는 변수, 어댑터에 주로 사용
// 함수의 이름은 함수가 시작하는 시작주소
// 함수 시그너처(반환 타입과 매개변수 리스트)와 동일하게 자료형을 선언해줘야함

void Print(int n)
{
	cout << "정수: " << n << endl;
}

// 함수포인터
int main059()
{
	// 포인터 변수
	int n = 10;
	int *pn = &n;

	// 함수포인터 선언
	void (*pf)(int);						// void Print(int n){} 함수의 주소를 저장할 포인터변수

	// 함수의 이름은 함수의 시작주소
	pf = Print;							// 함수의 이름을 주소로 넘겨줌
	
	Print(10);				// 함수 호출
	pf(10);						// 포인터를 이용한 함수호출1
	(*pf)(10);						// 포인터를 이용한 함수호출2

	cout << endl;
	cout << Print << endl;					// 함수주소
	cout << pf << endl;						// 함수주소
	cout << *pf << endl;						// 함수주소

	return 0;
}
