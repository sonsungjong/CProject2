#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int num[6] = { 0,0,0,0,0,0 };
	int flag;
	srand((unsigned int)time(NULL));
	printf("�ֻ��� ���� Ƚ�� : ");
	scanf_s("%d", &flag);
	for (int i = 0; i < flag; i++) {
		int dice = rand() % 6 + 1;
		switch (dice) {
		case 1:
			num[0]++;
			break;
		case 2:
			num[1]++;
			break;
		case 3:
			num[2]++;
			break;
		case 4:
			num[3]++;
			break;
		case 5:
			num[4]++;
			break;
		case 6:
			num[5]++;
			break;
		}
	}
	printf("����1�� ���� = %d, Ȯ�� = %f\n", num[0], (float)num[0] / flag);
	printf("����2�� ���� = %d, Ȯ�� = %f\n", num[1], (float)num[1] / flag);
	printf("����3�� ���� = %d, Ȯ�� = %f\n", num[2], (float)num[2] / flag);
	printf("����4�� ���� = %d, Ȯ�� = %f\n", num[3], (float)num[3] / flag);
	printf("����5�� ���� = %d, Ȯ�� = %f\n", num[4], (float)num[4] / flag);
	printf("����6�� ���� = %d, Ȯ�� = %f\n", num[5], (float)num[5] / flag);

	system("pause");
	return 0;
}