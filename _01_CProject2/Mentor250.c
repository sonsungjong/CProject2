#include <stdio.h>

// #1
int GetDiffByValue(int a_number1, int a_number2)
{
	return a_number1 - a_number2;
}

int CallDiffByRef(int* ap_number1, int* ap_number2)
{
	int num = 0;
	if (*ap_number1 < *ap_number2)
	{
		num = *ap_number1;
		*ap_number1 = *ap_number2;
		*ap_number2 = num;
	}
}

// #2
int Swap3(int* a_num1, int* a_num2, int* a_num3)
{
	int temp = 0;
	temp = *a_num3;
	*a_num3 = *a_num2;
	*a_num2 = *a_num1;
	*a_num1 = temp;
}

int RecursiveSum(int n)
{
	static int total = 0;
	if (n != 0) {
		total += n;
		RecursiveSum(n - 1);
	}

	return total;
}

int main_250()
{
	int number1 = 2, number2 = 10;

	// #1
	GetDiffByValue(number1, number2);
	CallDiffByRef(&number1, &number2);

	printf("number1 : %d, number2 : %d \n", number1, number2);

	// #2
	int num1 = 5, num2 = 8, num3 = 10;
	printf("함수 호출 전 num1 num2 num3 : %d %d %d\n", num1, num2, num3);
	Swap3(&num1, &num2, &num3);
	printf("함수 호출 후 num1 num2 num3 : %d %d %d\n", num1, num2, num3);

	// #3
	int n;
	printf("n을 입력하세요 :");
	scanf_s("%d", &n);
	int result = RecursiveSum(n);
	printf("1부터 n까지의 합 : %d\n", result);

	return 0;
}