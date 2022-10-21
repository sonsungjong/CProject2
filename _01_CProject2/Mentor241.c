#include <stdio.h>

int MenReadArray(int* arr, int length)
{
	int i;
	printf("배열의 요소 읽어보기 : [ ");
	for (i = 0; i < length; i++)
	{
		// arr[i] == (*arr+i)
		printf("%d", arr[i]);
		if (i + 1 < length)
		{
			printf(", ");
		}
		else {
			printf(" ");
		}
	}

	printf("]\n");
}

int MenReadArray2(int arr[], int length)
{
	int i;
	printf("배열의 요소 읽어보기 : [ ");
	for (i = 0; i < length; i++)
	{
		// arr[i] == (*arr+i)
		printf("%d", arr[i]);
		if (i + 1 < length)
		{
			printf(", ");
		}
		else {
			printf(" ");
		}
	}

	printf("]\n");
}

void MenSwap(int* a_number1, int* a_number2)
{
	int backup = *a_number1;
	*a_number1 = *a_number2;
	*a_number2 = backup;
}

void SayHello()
{
	printf("Hello\n");
	SayHello();			// 자기자신 호출(재귀함수)
}

// 넘어온 숫자부터 3까지
int SayHello2(int a_count)
{
	printf("Hello\n");

	// count가 3이되면 더이상 재귀함수를 호출하지 않음
	if (a_count != 3) {
		SayHello(a_count +1);			// 자기자신 호출(재귀함수)
	}

	return 0;
}

int main_241()
{
	int arr[3] = { 3,6,9 };
	MenReadArray(arr, 3);
	MenReadArray2(arr, 3);

	int number1 = 33, number2 = 99;
	MenSwap(&number1, &number2);
	printf("함수 호출 완료 후 확인 결과, number1: %d, number2 : %d\n", number1, number2);

	SayHello(1);					// 재귀함수 : 자기자신 호출

	return 0;
}