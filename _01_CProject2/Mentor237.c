#include <stdio.h>

// #1
void MenOper(int a, int b)
{
	printf("%d\n", a + b);
	printf("%d\n", a - b);
	printf("%d\n", a * b);
	printf("%d\n", a / b);
}

// #2
int MenSum(int x, int y)
{
	int total = 0;
	for (int i = x + 1; i < y; i++)
	{
		total += i;
	}
	return total;
}

// #3
int Num7(int num)
{
	int count = 0;
	for (int i = num; i < num + 200; i++) {
		if (i % 7 == 0) {
			count++;
		}
	}

	return count;
}

// #4
float Cm2M(float c)
{
	return c / 100.0f;
}

// #5
void PrintTime(int a_second)
{
	int hour = 0;
	int minute = 0;
	int second = 0;

	hour = a_second / 3600;
	minute = (a_second % 3600) / 60;
	second = a_second % 60;

	printf("%d�ð�/%d��/%d��\n", hour, minute, second);
}

int main_237()
{
	// #1
	MenOper(10, 2);

	// #2
	int result2 = MenSum(5, 9);
	printf("5�� 9 ������ ������ ������ %d\n", result2);

	// #3
	int num3 = 0;
	printf("������ �Է��ϼ��� :");
	scanf_s("%d", &num3);
	int num7 = Num7(num3);
	printf("%d�� %d ���� 7�� ����� %d�� �Դϴ�.\n", num3, num3 + 200, num7);

	// #4
	printf("Ű�� cm������ �Է��ϼ��� :");
	scanf_s("%d", &num3);
	float height = Cm2M(num3);
	printf("��� : %.2f m \n", height);

	// #5
	printf("�� �� : ");
	scanf_s("%d", &num3);
	PrintTime(num3);

	return 0;
}