#include <stdio.h>
#include <stdlib.h>

int main_276()
{
	int* ptr;
	int count = 0;
	int maxSize = 3;
	int num;

	ptr = (int*)calloc(maxSize, sizeof(int));
	while (1)
	{
		printf("������ �Է��ϼ���(-1 �Է½� ����): ");
		scanf_s("%d", &num);
		if (num == -1) {
			break;
		}

		// �Է��� ������ ������ �޸� ũ�⸸ŭ á�� ���
		if (count == maxSize)
		{
			maxSize += maxSize;
			// ���Ҵ��� ���� �޸𸮿��� Ȯ��
			ptr = (int*)realloc(ptr, maxSize * sizeof(int));
		}

		ptr[count++] = num;
	}

	for (int i = 0; i < count; i++) {
		printf("%d ", ptr[i]);
	}
	printf("\n");

	if (ptr != NULL) {
		free(ptr);
	}

	return 0;
}