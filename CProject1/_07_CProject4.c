#include <stdio.h>
#include <malloc.h>

int main74() {
	// short p[5][3]
	// �迭�� ����Ű�� ������
	short (*p)[3];
	int count = 5;

	// short [count][3] ũ�⸦ ����� �� �ֵ��� 6*count ����Ʈ�� �Ҵ��Ѵ�.
	p = (short (*)[3])malloc(sizeof(short[3]) * count);
	if (p != NULL) {
		// �Ҵ�� �޸𸮿� 0���� �����Ѵ�.
		for (int i = 0; i < count; i++) {
			for (int sub_i = 0; sub_i < 3; sub_i++) {
				(*(p + i))[sub_i] = 0;					// p[i][sub_i] = 0; ����
			}
		}
		free(p);
	}

	return 0;
}