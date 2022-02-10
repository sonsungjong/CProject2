#include <stdio.h>
#include <malloc.h>

int main75() {
	short **pp;						// 4byte stack(1MB)
	int group_count = 5, sub_count = 3;

	// group_count 개의 포인터를 사용할 수 있도록 4*group_count 바이트 메모리를 할당
	pp = (short**)malloc(sizeof(short*) * group_count);				// heap(2GB)
	if (pp == NULL) { return -1; }

	// group_count 개의 포인터에 2*sub_count 바이트씩 메모리를 할당하고
	// 할당된 메모리에 0값을 대입한다.
	for (int i = 0; i < group_count; i++) {
		*(pp + i) = (short*)malloc(sizeof(short) * sub_count);
		if (*(pp + i) != NULL) {
			for (int sub_i = 0; sub_i < sub_count; sub_i++) {
				*(*(pp + i) + sub_i) = 0;
			}
		}
	}

	// 할당한 메모리 해제
	for (int i = 0; i < group_count; i++) {
		if (*(pp + i) != NULL) { free(*(pp + i)); }
	}
	free(pp);

	return 0;
}