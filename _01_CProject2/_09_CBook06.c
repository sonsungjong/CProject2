#include <stdio.h>

int main0906() {
	int i, step;
	printf("����� �ܼ� ���� : ");
	scanf_s("%d", &step);

	for (i = 1; i <= 9; i++) {
		printf("%d*%d = %d\n", step, i, step * i);
	}

	return 0;
}