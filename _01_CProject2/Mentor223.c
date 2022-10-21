#include <stdio.h>

int number = 0;
int MenAdd(int a, int b);

void MenNumPrint()
{
	printf("전역변수 number는 %d를 저장하고 있다.\n", number);
}

void MenIncreaseNumber()
{
	static int number = 0;
	number++;
	printf("static변수 number : %d \n", number);
}

int main_223()
{
	int result;
	int number = 3;

	result = MenAdd(3, 5);
	printf("함수 반환 값 : %d\n", result);
	printf("지역변수 number는 %d를 저장하고 있다.\n", number);
	MenNumPrint();

	MenIncreaseNumber();
	MenIncreaseNumber();
	MenIncreaseNumber();
	MenIncreaseNumber();
	MenIncreaseNumber();

	return 0;
}

int MenAdd(int a, int b)
{
	return a + b;
}