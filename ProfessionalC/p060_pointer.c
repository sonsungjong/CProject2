#include <stdio.h>

/*
	������ ������ �������
	�޸𸮴� ���� �� 1������ ����Ʈ �迭�̴�.
	�����͸� ������Ű�� ������ ����, ���ҽ�Ű�� �ڷ� ����
	�����͸� 1 ������Ű�� �޸𸮿��� 1����Ʈ �̻� ������ ���ư���
	�����͸� 1 ������ų �� �����̴� ����Ʈ ���ڸ� ��Ÿ���� ���� �������� C ������ Ÿ���� �����Ѵ�
	�̷��� �������� ������� ������ C �ڷ����� ���� �����ȴ�.

	�������, int* �� char* �� ũ�Ⱑ ������ ������� ������ �ٸ���.
	int*�� 4����Ʈ�� ���� ������ ����, char*�� 1����Ʈ�� ���� ������ ���´�
*/
int main060(int argc, char** argv)
{
	int var = 1;

	int* int_ptr = 0;				// �����͸� NULL�� �����
	int_ptr = &var;

	char* char_ptr = 0;
	char_ptr = (char*)&var;

	printf("Before arithmetic: int_ptr: %u, char_ptr: %u\n", (unsigned int)int_ptr, (unsigned int)char_ptr);

	int_ptr++;				// ������� ���� 4����Ʈ
	char_ptr++;			// ������� ���� 1����Ʈ

	// ���� �������� ���� ������ 4����Ʈ, ���� �������� ���� ������ 1����Ʈ
	printf("After arithmetic: int_ptr: %u, char_ptr: %u\n", (unsigned int)int_ptr, (unsigned int)char_ptr);


	int arr[5] = { 9, 22, 30, 23, 18 };

	// ���� �迭�� ��� ���� ��� (������ �̻��)
	for (int i = 0; i < 5; i++) {
		printf("%d\n", arr[i]);
	}

	// ���� �迭�� ��� ���� ��� (������)
	int* ptr = &arr[0];
	for (;;) {
		printf("%d\n", *ptr);
		if (ptr == &arr[5 - 1]) {
			break;
		}
		ptr++;
	}

	return 0;
}