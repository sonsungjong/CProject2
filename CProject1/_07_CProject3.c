#include <stdio.h>
#include <malloc.h>

int main73() {
	// short p[5][3];
	// 포인터를 항목으로 가지는 배열
	short* p[5];
	int count = 3;

	// 다섯 개의 포인터에 2*count 바이트씩 메모리 할당 후 0대입
	for (int i = 0; i < 5; i++) {
		p[i] = (short*)malloc(sizeof(short) * count);
		if (p[i] != NULL) {
			for (int sub_1 = 0; sub_1 < count; sub_1++) {
				*(p[i] + sub_1) = 0;
			}
		}
	}

	// 할당했던 메모리를 해제한다.
	for (int i = 0; i < 5; i++) {
		if (p[i] != NULL) { free(p[i]); }
	}

	return 0;
}
