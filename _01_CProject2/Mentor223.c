#include <stdio.h>

int number = 0;
int MenAdd(int a, int b);

void MenNumPrint()
{
	printf("�������� number�� %d�� �����ϰ� �ִ�.\n", number);
}

void MenIncreaseNumber()
{
	static int number = 0;
	number++;
	printf("static���� number : %d \n", number);
}

int main_223()
{
	int result;
	int number = 3;

	result = MenAdd(3, 5);
	printf("�Լ� ��ȯ �� : %d\n", result);
	printf("�������� number�� %d�� �����ϰ� �ִ�.\n", number);
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