#include <stdio.h>
#include <malloc.h>

int main73() {
	// short p[5][3];
	// �����͸� �׸����� ������ �迭
	short* p[5];
	int count = 3;

	// �ټ� ���� �����Ϳ� 2*count ����Ʈ�� �޸� �Ҵ� �� 0����
	for (int i = 0; i < 5; i++) {
		p[i] = (short*)malloc(sizeof(short) * count);
		if (p[i] != NULL) {
			for (int sub_1 = 0; sub_1 < count; sub_1++) {
				*(p[i] + sub_1) = 0;
			}
		}
	}

	// �Ҵ��ߴ� �޸𸮸� �����Ѵ�.
	for (int i = 0; i < 5; i++) {
		if (p[i] != NULL) { free(p[i]); }
	}

	return 0;
}
