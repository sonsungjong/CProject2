#include <stdio.h>

int MenReadArray(int* arr, int length)
{
	int i;
	printf("�迭�� ��� �о�� : [ ");
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
	printf("�迭�� ��� �о�� : [ ");
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
	SayHello();			// �ڱ��ڽ� ȣ��(����Լ�)
}

// �Ѿ�� ���ں��� 3����
int SayHello2(int a_count)
{
	printf("Hello\n");

	// count�� 3�̵Ǹ� ���̻� ����Լ��� ȣ������ ����
	if (a_count != 3) {
		SayHello(a_count +1);			// �ڱ��ڽ� ȣ��(����Լ�)
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
	printf("�Լ� ȣ�� �Ϸ� �� Ȯ�� ���, number1: %d, number2 : %d\n", number1, number2);

	SayHello(1);					// ����Լ� : �ڱ��ڽ� ȣ��

	return 0;
}