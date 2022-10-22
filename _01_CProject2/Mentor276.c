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
		printf("정수를 입력하세요(-1 입력시 종료): ");
		scanf_s("%d", &num);
		if (num == -1) {
			break;
		}

		// 입력한 정수의 개수가 메모리 크기만큼 찼을 경우
		if (count == maxSize)
		{
			maxSize += maxSize;
			// 재할당을 통한 메모리영역 확장
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