#include <stdio.h>

// 함수도 메모리 공간에 저장되므로 주소 할당됨
int MyFunc(int num1, int num2)
{
	printf("num1 : %d, num2 : %d\n", num1, num2);
}

int FirstFunc(int a, int b) {}
double SecondFunc(double d) {}
void ThirdFunc(char* c) {}

int main_266()
{
	// 매개변수로 int가 2개
	// int 반환하는 함수의 주소를 저장하는
	// 함수 포인터 fptr
	int (*fptr)(int, int);
	fptr = MyFunc;
	fptr(9, 6);

	int (*first)(int, int);
	double (*second)(double);
	void (*third)(char*);

	first = FirstFunc;
	second = SecondFunc;
	third = ThirdFunc;

	return 0;
}