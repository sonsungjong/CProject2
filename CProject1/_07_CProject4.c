#include <stdio.h>
#include <malloc.h>

int main74() {
	// short p[5][3]
	// 배열을 가리키는 포인터
	short (*p)[3];
	int count = 5;

	// short [count][3] 크기를 사용할 수 있도록 6*count 바이트를 할당한다.
	p = (short (*)[3])malloc(sizeof(short[3]) * count);
	if (p != NULL) {
		// 할당된 메모리에 0값을 대입한다.
		for (int i = 0; i < count; i++) {
			for (int sub_i = 0; sub_i < 3; sub_i++) {
				(*(p + i))[sub_i] = 0;					// p[i][sub_i] = 0; 동일
			}
		}
		free(p);
	}

	return 0;
}