#include <stdio.h>
#include <string.h>

int main1404()
{
	int pos_num = 1;
	int num = 0;
	int i = 0;
	int count = 0;
	char num_string[4] = "123";

	// ���ڿ� ���� ���ϱ� (strlen �� null�� ���ܵ� ���� ��ȯ��)
	count = strlen(num_string);				// 3 (null���� ���ܵ�)

	// �ְ� �ڸ��� ���ϱ�
	for (i = 0; i < count - 1; i++) {
		pos_num = pos_num * 10;
	}

	// ���ڿ��� ���ڷ� ��ȯ - '0'
	for (i = 0; i < count; i++) {
		num = num + (num_string[i] - '0') * pos_num;
		pos_num = pos_num / 10;
	}

	printf(" %s -> %d \n", num_string, num);

	return 0;
}