#include "pch.h"

void main()
{
	char str1[50];
	char str2[50];
	printf("���ڿ�1 �Է�>>");
	scanf("%[^\n]s", str1);
	rewind(stdin);
	printf("���ڿ�2 �Է�>>");
	scanf("%[^\n]s", str2);

	// ���ڿ��� ������ ��
	if (strcmp(str1, str2) == 0) {
		printf("�����ϴ�.");
	}
	else {
		printf("�ٸ��ϴ�.");
	}
	
	// �� �� ���ڿ� �˻�
	if (strlen(str1) > strlen(str2)) {
		printf("%s\n", str1);
		printf("%s\n", str2);
	}
	else {
		printf("%s\n", str2);
		printf("%s\n", str1);
	}

	// ���ڿ� �� ���ڸ� �����Ͽ� ���ϱ�
	printf("���ڿ�3 �Է�>>");
	char str3[50] = { 0, };
	scanf("%s", str3);
	int sum = 0;
	for (int i = 0; i < 50; i++) {
		if (str3[i] > 47 && str3[i] < 58) {
			sum += str3[i] - 48;
		}
	}
	printf("%d\n", sum);
}
