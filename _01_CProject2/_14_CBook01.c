#include <stdio.h>

int main1401()
{
	char input_string[10];
	/* ��ȯ���� NULL�� �ƴ϶�� ���ڿ��� �Է¹޾Ƽ� input_string�� ������ */
	if (NULL != gets(input_string)) {						// ��Ʈ�� + C�� �ϸ� �Է����
		printf("input : %s \n", input_string);
	}
	else {
		printf("input -> Canceled \n");
	}

	return 0;
}