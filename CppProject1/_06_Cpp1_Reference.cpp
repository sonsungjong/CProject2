#include <iostream>

using namespace std;

void SwapPointer(int* ap_a, int* ap_b)
{
	int temp;
	temp = *ap_a;
	*ap_a = *ap_b;
	*ap_b = temp;
}

void SwapReference(int& ap_a, int& ap_b)
{
	// 프로그램 언어학자들이 봤을때 포인터쓰다 역참조 빼먹을 수 있으니 레퍼런스ㄱㄱ
	int temp;
	temp = ap_a;
	ap_a = ap_b;
	ap_b = temp;
	// 포인터 기호를 숨겨버림
}

int mainReference() {
	int data = 5;
	int& r = data;		// 레퍼런스 : 포인터 편하게 쓰라고 만든 문법, 안쓰는 것 권장
	int* p = &data;	// 포인터

	cout << data << endl;
	r = 7;		// data = 7;
	cout << data << endl;
	*p = 8;		//	data = 8;
	cout << data << endl;

	int num1 = 3, num2 = 6;
	SwapPointer(&num1, &num2);
	SwapReference(num1, num2);
	cout << num1 << endl;
	cout << num2 << endl;

	// 기계어로는 완전히 동일함 (보기만 더 편하게 했을뿐임)
	// 단, 반환값을 레퍼런스로 하면 타이밍(레이스컨디션) 문제가 발생함
	return 0;
}