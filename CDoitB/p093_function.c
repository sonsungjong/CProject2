#include <stdio.h>

// # : 전처리기 (코드 실행 전에 미리 처리할 것)
#define NUMBER1 3

int sum93(int value1, int value2)
{
	int result = value1 + value2;
	return result;
}

int main093() 
{
	int s;
	s = sum93(NUMBER1, 5);
	printf("더하기 작업의 결과는 %d \n", s);

	return 0;
}