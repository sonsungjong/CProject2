#include <stdio.h>

int main_148()
{
	// ���� ������ ���Ե� �迭�� ���� �� �ʱ�ȭ
	int numbers[3] = { 1,2,3 };

	// ���� ������ ���Ե��� ���� �迭�� ���� �� �ʱ�ȭ
	char characters[] = { 'a','b','c','d','e' };

	int i;
	int odd[3];
	int even[5] = { 2,4,6,8,10 };

	odd[0] = 1;
	odd[1] = 3;
	odd[2] = 5;

	printf("%d %d %d \n", odd[0], odd[1], odd[2]);

	for (i = 0; i < 5; i++)
	{
		printf("%d ", even[i]);
	}
	printf("\n");

	return 0;
}