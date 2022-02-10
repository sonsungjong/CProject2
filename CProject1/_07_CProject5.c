#include <stdio.h>
#include <malloc.h>

int main75() {
	short **pp;						// 4byte stack(1MB)
	int group_count = 5, sub_count = 3;

	// group_count ���� �����͸� ����� �� �ֵ��� 4*group_count ����Ʈ �޸𸮸� �Ҵ�
	pp = (short**)malloc(sizeof(short*) * group_count);				// heap(2GB)
	if (pp == NULL) { return -1; }

	// group_count ���� �����Ϳ� 2*sub_count ����Ʈ�� �޸𸮸� �Ҵ��ϰ�
	// �Ҵ�� �޸𸮿� 0���� �����Ѵ�.
	for (int i = 0; i < group_count; i++) {
		*(pp + i) = (short*)malloc(sizeof(short) * sub_count);
		if (*(pp + i) != NULL) {
			for (int sub_i = 0; sub_i < sub_count; sub_i++) {
				*(*(pp + i) + sub_i) = 0;
			}
		}
	}

	// �Ҵ��� �޸� ����
	for (int i = 0; i < group_count; i++) {
		if (*(pp + i) != NULL) { free(*(pp + i)); }
	}
	free(pp);

	return 0;
}