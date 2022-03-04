#include <stdio.h>

void PrintValue(int a_value) {
	printf("Value = %d\n", a_value);
}

int main91() {
	// 데이터 포인터와 함수 포인터
	// 대상은 정하지 않고 행위를 정할 수 있는 문법 (그가 밥을 먹는다)
	// 미래의 발생할 어떤 것을 표현할 수 있음.

	// 데이터 포인터 : 변수의 주소를 가리킴, 대상을 미룸
	// 함수 포인터 : 함수의 주소를 가리킴, 행위를 미룸

	// 데이터 포인터
	int data = 0;
	int* p;

	data = 5;
	p = &data;
	*p = 6;

	// int -> int*
	// void Print(int a_value) -> void(*)(int)
	void (*p_func)(int);				// 정수값 인자 한개를 갖는 함수의 함수포인터
	p_func = &PrintValue;						// &생략가능

	(*p_func)(5);						// p_func(5);

	return 0;
}