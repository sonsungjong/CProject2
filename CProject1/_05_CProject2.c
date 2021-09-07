#include <stdio.h>

void SwapVoidPointer(void* ap_a, void* ap_b);

int main52() {

	// void* : 배려문법, 타입을 지정하지않은 포인터
	int data;
	short* p;

	// &data : int*
	p = (short*)&data;		// 형변환이 필요
	*p = 2;			// 역참조

	int data2;
	void* p2;

	p2 = &data2;			// 정해져있지 않기때문에 모든 자료형의 주소를 받을 수 있음
	*(short*)p2 = 7;	// 대신 사용할 때 형변환 해줘야함

	// 함수를 만들어서 제공할 경우 매개변수를 void* 로 만들어 사용하는 쪽에서 자료형을 정할 수 있게해줌
	SwapVoidPointer(&data, &data2);
	
	return 0;
}

// 매개변수를 void*로 정의
void SwapVoidPointer(void* ap_a, void* ap_b) {
	int temp;

	temp = *(int*)ap_a;
	*(int*)ap_a = *(int*)ap_b;
	*(int*)ap_b = temp;
}