#include <stdio.h>
#include <malloc.h>

int main72() {
	// 배열은 스택(1MB)을 사용
	short data[5];

	// 다섯 개의 정숫값을 입력받는다.
	for (int i = 0; i < 5; i++) {
		scanf_s("%hd", data + i);
	}

	// 입력받은 정숫값을 출력한다.
	for (int i = 0; i < 5; i++) {
		printf("%hd, ", data[i]);
	}

	// ==========================
	// 힙(2GB)을 사용
	short* p_data;					// 변수 자체는 stack
	int count = 5;

	// 10바이트 크기의 메모리를 할당
	p_data = (short*)malloc(sizeof(short) * count);			// heap에 10바이트를 할당

	// 메모리 할당에 성공했는지 여부를 체크한다.
	if (p_data != NULL) {
		// count 개의 정숫값을 입력받는다.
		for (int i = 0; i < count; i++) {
			scanf_s("%hd", p_data + i);
		}

		// 입력받은 정숫값을 출력한다.
		for (int i = 0; i < count; i++) {
			printf("%hd, ", *(p_data + i));
		}

		// 할당된 메모리를 해제한다.
		free(p_data);
	}

	// 크기에 대한 융통성과 힙을 사용
	// 짧게 반복하려면 배열을 사용하고 큰 데이터를 일시적으로 사용한다면 메모리할당 이용하는 것이 이득
	return 0;
}