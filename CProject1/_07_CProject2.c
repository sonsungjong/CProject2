#include <stdio.h>
#include <malloc.h>

int main72() {
	// �迭�� ����(1MB)�� ���
	short data[5];

	// �ټ� ���� �������� �Է¹޴´�.
	for (int i = 0; i < 5; i++) {
		scanf_s("%hd", data + i);
	}

	// �Է¹��� �������� ����Ѵ�.
	for (int i = 0; i < 5; i++) {
		printf("%hd, ", data[i]);
	}

	// ==========================
	// ��(2GB)�� ���
	short* p_data;					// ���� ��ü�� stack
	int count = 5;

	// 10����Ʈ ũ���� �޸𸮸� �Ҵ�
	p_data = (short*)malloc(sizeof(short) * count);			// heap�� 10����Ʈ�� �Ҵ�

	// �޸� �Ҵ翡 �����ߴ��� ���θ� üũ�Ѵ�.
	if (p_data != NULL) {
		// count ���� �������� �Է¹޴´�.
		for (int i = 0; i < count; i++) {
			scanf_s("%hd", p_data + i);
		}

		// �Է¹��� �������� ����Ѵ�.
		for (int i = 0; i < count; i++) {
			printf("%hd, ", *(p_data + i));
		}

		// �Ҵ�� �޸𸮸� �����Ѵ�.
		free(p_data);
	}

	// ũ�⿡ ���� ���뼺�� ���� ���
	// ª�� �ݺ��Ϸ��� �迭�� ����ϰ� ū �����͸� �Ͻ������� ����Ѵٸ� �޸��Ҵ� �̿��ϴ� ���� �̵�
	return 0;
}