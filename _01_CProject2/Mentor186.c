#include <stdio.h>

int main_186()
{
	int i, j;
	int arr[2][3] = {
		{1,2,3},
		{4,5,6}
	};

	// �迭�� ��� ��� ���
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}

	// �迭�� ��ڰ��� ��� 10���� �����ϱ�
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			arr[i][j] = 10;
		}
	}

	// �ٽ� �迭�� ��� ��� ����ϱ�
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}

	return 0;
}